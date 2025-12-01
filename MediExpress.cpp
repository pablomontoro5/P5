//
// Created by Lazaro Ruiz on 08/10/2025.
//

#include "MediExpress.h"

#include <fstream>
#include <sstream>
#include <chrono>
/**
 * @brief Cuenta cuántas líneas (medicamentos) contiene un fichero CSV.
 * @param nomFichPaMed Ruta del fichero CSV/semicolon de medicamentos.
 * @return Número total de medicamentos presentes en el fichero.
 * @throws std::runtime_error Si el fichero no puede abrirse.
 */


unsigned long MediExpress::contarMedicamentos(const std::string &nomFichPaMed) {
    std::ifstream f(nomFichPaMed);
    if (!f) throw std::runtime_error("***Error abriendo pa_medicamentos.csv***");

    unsigned long count = 0;
    std::string linea;
    while (getline(f, linea)) {
        count++;
    }

    return count;
}
/**
 * @brief Realiza el procesado posterior a la carga inicial de datos.
 *
 * Este método:
 * 1) Construye la estructura `_nombMedication` que asocia palabras del nombre → medicamento.
 * 2) Asigna cada dos medicamentos a cada laboratorio por orden.
 * 3) Asigna medicamentos sin laboratorio a laboratorios de Madrid.
 * 4) Realiza una segunda lectura del fichero de farmacias para obtener sus CIF.
 * 5) Distribuye 100 medicamentos distintos a cada farmacia del sistema.
 *
 * @param fichFarmacias Ruta del fichero CSV de farmacias (para la segunda lectura de CIF).
 *
 * @post Tras la ejecución, la red interna queda completamente configurada:
 *       - _nombMedication queda poblado
 *       - Los medicamentos quedan asignados a laboratorios
 *       - Las farmacias reciben su stock inicial
 */
void MediExpress::_postprocesarCargas(const std::string &fichFarmacias){
    // ============================================================
    // 1. Construir estructura _nombMedication (nombre → puntero PA)
    // ============================================================

    for (int i=0; i<_vMedi.size(); i++){
        PA_Medicamento* _pMedicamentoAux = idMedication.buscarEnTabla(_vMedi[i]);

        std::string nombre = _pMedicamentoAux->getNombre();
        std::stringstream separar;
        std::string cad;

        separar.str(nombre);

        while (getline(separar, cad, ' ')){
            _nombMedication.insert(make_pair(cad, _pMedicamentoAux));
        }
    }

    // ============================================================
    // 2. Asociar cada 2 medicamentos con un laboratorio
    // ============================================================

    std::list<Laboratorio>::iterator _iteradorDeLalIsta = _labs.begin();
    std::vector<int>::iterator _iteradorDeMedicamentos = _vMedi.begin();
    PA_Medicamento* _pMedAux2;

    while (_iteradorDeMedicamentos != _vMedi.end() && _iteradorDeLalIsta != _labs.end()) {
        _pMedAux2 = idMedication.buscarEnTabla(*_iteradorDeMedicamentos);
        suministrarMed(_pMedAux2, &(*_iteradorDeLalIsta));
        _iteradorDeMedicamentos++;

        if (_iteradorDeMedicamentos == _vMedi.end()) {
            break;
        }
        _pMedAux2 = idMedication.buscarEnTabla(*_iteradorDeMedicamentos);
        suministrarMed(_pMedAux2, &(*_iteradorDeLalIsta));

        _iteradorDeLalIsta++;
        _iteradorDeMedicamentos++;
    }

    // ============================================================
    // 3. Asociar medicamentos _vectorDeMedicamentosSinLaboratorio laboratorio a labs de Madrid
    // ============================================================

    std::vector<Laboratorio*> _vectorDeMadrid = buscarLabCiudad("Madrid");
    std::vector<PA_Medicamento*> _vectorDeMedicamentosSinLaboratorio = getMedicamentosSinLab();

    for (int i = 0; i < _vectorDeMadrid.size() && i < _vectorDeMedicamentosSinLaboratorio.size(); i++) {
        suministrarMed(_vectorDeMedicamentosSinLaboratorio[i], _vectorDeMadrid[i]);
    }

    // ============================================================
    // 4. Segunda lectura del fichero de farmacias para obtener CIFs
    // ============================================================

    std::vector<std::string> _vectorDeCifs;
    std::ifstream is(fichFarmacias);

    if (!is.good()) {
        std::cerr << "***Error abriendo archivo de farmacias (segunda lectura)***";
        return;
    }

    std::string fila, cif;
    std::stringstream columnas;

    while (getline(is, fila)) {
        if (!fila.empty()) {
            columnas.str(fila);
            getline(columnas, cif, ';');
            columnas.clear();
            _vectorDeCifs.push_back(cif);
        }
    }

    is.close();

    // ============================================================
    // 5. Reparto de medicamentos a farmacias
    // ============================================================

    std::vector<int>::iterator _itMed2 = _vMedi.begin();
    for (const std::string &c : _vectorDeCifs) {

        Farmacia* _fAux = buscarFarmacia(c);
        if (!_fAux) continue;

        int _repetidos = 0;
        while (_repetidos < 100) {

            suministrarFarmacia(_fAux, *_itMed2, 10);

            if (_itMed2 == --_vMedi.end()) {
                _itMed2 = _vMedi.begin();
            }else {
                ++_itMed2;
            }
            _repetidos++;
        }
    }
}
/**
 * @brief Obtiene todas las farmacias cuya provincia contiene la cadena indicada.
 * @param provincia Subcadena a buscarEnTabla dentro del campo provincia de cada farmacia.
 * @return std::vector con punteros a farmacias cuya provincia coincide parcial o totalmente.
 * @note La búsqueda se realiza recorriendo el contenedor interno std::vector<Farmacia>.
 *       La coincidencia usa std::string::find y es sensible a mayúsculas/minúsculas.
 */
std::vector<Farmacia*> MediExpress::buscarFarmacias(const std::string &provincia) {

    std::vector<Farmacia*> toRet;

    for (auto it = _pharmacy.begin(); it != _pharmacy.end(); ++it) {

        if (it->first.find(provincia) != std::string::npos) {
            toRet.push_back(&(it->second));
        }
    }
    return toRet;
}


/**
 * @brief Elimina un medicamento del sistema y borra su stock en todas las farmacias.
 * @param id_num Identificador numérico del medicamento a eliminar.
 * @return true si el medicamento existía y se ha eliminado del contenedor principal; false en caso contrario.
 * @post Tras la llamada, todas las farmacias (almacenadas en un std::vector<Farmacia>) habrán eliminado
 *       el stock asociado a @p id_num y, si estaba presente, el medicamento dejará de existir en el
 *       mapa interno std::map<int, PA_Medicamento> de MediExpress.
 */
bool MediExpress::eliminarMedicamento(int id_num) {
    // 1. Eliminar el medicamento del stock de todas las farmacias que lo tengan
    std::multimap<std::string, Farmacia>::iterator it = _pharmacy.begin();
    while (it != _pharmacy.end()) {
        it->second.eliminarStock(id_num);
        it++;
    }

    // 2. Eliminar el medicamento del índice de nombres (nombMedication)
    std::multimap<std::string, PA_Medicamento*>::iterator aux, it2 = _nombMedication.begin();
    while (it2 != _nombMedication.end()) {
        if (it2->second->getIdNum() == id_num) {
            aux = it2;
            aux++;
            _nombMedication.erase(it2);
            it2 = aux;
        } else {
            it2++;
        }
    }

    // 3. Eliminar el medicamento de la _tablaHash _funcionHashNumeroUno de IDs (idMedication)
    if (idMedication.borrarEnTabla(id_num)) {
        return true;
    } else {
        return false;
    }

}




/**
 * @brief Obtiene todos los medicamentos que actualmente no están servidos por ningún laboratorio.
 * @return std::vector dinámico con punteros a medicamentos sin laboratorio asociado (posiblemente vacío).
 * @note La búsqueda se realiza recorriendo el contenedor interno std::map<int, PA_Medicamento> y
 *       comprobando si cada medicamento tiene o no laboratorio asignado.
 */
std::vector<PA_Medicamento *> MediExpress::getMedicamentosSinLab() {
    std::vector<PA_Medicamento*> toRet;
    PA_Medicamento *paMedicamentoAux;
    int i=0;
    while (i < _vMedi.size()){
        paMedicamentoAux= idMedication.buscarEnTabla(_vMedi[i]);
        if(paMedicamentoAux && !paMedicamentoAux->servidoPor()){
            toRet.push_back(paMedicamentoAux);
        }
        ++i;
    }

    return toRet;
}

/**
 * @brief Asigna laboratorios de Madrid a medicamentos sin laboratorio asignado.
 *
 * Obtiene los laboratorios cuya localidad contiene "Madrid" y los medicamentos sin laboratorio.
 * Después asigna cada medicamento a un laboratorio disponible, emparejándolos por índice.
 *
 * @post Los medicamentos sin laboratorio reciben uno nuevo, si hay suficiente número de laboratorios.
 * @note La asignación se realiza por orden de aparición en los vectores temporales.
 */
void MediExpress::asignarLabsMadridAMedicamentosSinAsignar() {
    std::vector<Laboratorio *> _Madrid = buscarLabCiudad("Madrid");
    std::vector<PA_Medicamento *> _sin = getMedicamentosSinLab();
    int i = 0;

    while (i < _Madrid.size() && i < _sin.size()) {
        suministrarMed(_sin[i], _Madrid[i]);
        std::cout << " *** ID : *** " << _sin[i]->getIdNum()
                  << " *** , Id del laboratorio :  *** "
                  << _sin[i]->servidoPor()->getId() << std::endl;

        i++;
    }
}


/**
 * @brief Busca un medicamento por su identificador numérico exacto.
 * @param _idDelNumero Identificador numérico del medicamento.
 * @return Puntero al objeto PA_Medicamento si existe; nullptr en caso contrario.
 * @note La búsqueda se realiza mediante el contenedor interno std::map<int, PA_Medicamento>.
 */
PA_Medicamento* MediExpress::buscarCompuesto(int id_num) {
    PA_Medicamento* encontrado = idMedication.buscarEnTabla(id_num);
    if (encontrado != nullptr){
        return encontrado;
    }
    return nullptr;
}

/**
 * @brief Carga la información de los medicamentos desde un fichero CSV.
 * @param _ficheroMedicamentos Ruta del fichero CSV/semicolon con medicamentos.
 * @post El mapa interno de medicamentos (std::map<int, PA_Medicamento>) queda poblado,
 *       usando como clave el identificador numérico de cada medicamento.
 */
void MediExpress::_cargarMedicamentosDesdeFichero(const std::string &fich) {

    std::ifstream is(fich);
    if (!is.good()) {
        std::cerr << "Error abriendo archivo de medicamentos\n";
        return;
    }

    std::string fila;
    std::stringstream columnas;
    std::string s_idnum, idAlpha, nombre;
    int idnum;

    while (getline(is, fila)) {

        if (fila.empty()) continue;

        columnas.str(fila);
        getline(columnas, s_idnum, ';');
        getline(columnas, idAlpha, ';');
        getline(columnas, nombre, ';');
        columnas.clear();

        idnum = stoi(s_idnum);

        // Crear medicamento
        PA_Medicamento med(idnum, idAlpha, nombre);

        // Insertar en _funcionHashNumeroUno
        idMedication.insertarEnTabla(idnum, med);

        // Insertar en lista para rendimiento
        listaPaMed.push_back(med);

        // Insertar ID en vector auxiliar
        _vMedi.push_back(idnum);
    }

    is.close();
}


/**
 * @brief Devuelve los laboratorios cuya localidad contenga el texto dado.
 * @param nombreCiudad Subcadena a buscarEnTabla dentro del campo localidad de cada laboratorio.
 * @return std::vector dinámico con punteros a los laboratorios que coinciden (puede estar vacío).
 * @note La búsqueda recorre la std::list<Laboratorio> interna y usa std::string::find;
 *       es sensible a mayúsculas/minúsculas.
 */
std::vector<Laboratorio *> MediExpress::buscarLabCiudad(const std::string &nombreCiudad) {
    std::vector<Laboratorio*> toRet;
    std::list<Laboratorio>::iterator it5 = _labs.begin();
    while(it5!=_labs.end()){
        if((*it5).getLocalidad().find(nombreCiudad)!=std::string::npos){
            toRet.push_back((&(*it5)));
        }
        ++it5;
    }
    return toRet;
}
/**
 * @brief Asigna un laboratorio a un principio activo (medicamento).
 * @param pa Puntero al medicamento (principio activo) a asignar.
 * @param l Puntero al laboratorio que suministra el medicamento.
 * @pre Ambos punteros deben ser válidos (no nulos) para que la asignación se aplique.
 * @post El medicamento, almacenado en el std::map<int, PA_Medicamento> interno, queda marcado
 *       como servido por el laboratorio indicado.
 */
void MediExpress::suministrarMed(PA_Medicamento *pa, Laboratorio *l) {
    if(l && pa){
        pa->servidoPor(l);
    }
}
/**
 * @brief Construye un objeto MediExpress cargando medicamentos, laboratorios y farmacias desde fichero
 *        y realizando las asignaciones iniciales.
 * @param _ficheroMedicamentos Ruta del fichero CSV/semicolon con medicamentos.
 * @param _ficheroLaboratorios Ruta del fichero CSV/semicolon con laboratorios.
 * @param _ficheroFarmacias Ruta del fichero CSV/semicolon con farmacias.
 * @post Los contenedores internos (std::map<int, PA_Medicamento> para medicamentos,
 *       std::list<Laboratorio> para laboratorios y std::vector<Farmacia> para farmacias) quedan cargados.
 *       Además, se realizan las asignaciones iniciales entre medicamentos y laboratorios, y se distribuye
 *       stock inicial a las farmacias.
 */

/**
 * @brief Busca un laboratorio cuyo nombre contenga el texto dado.
 * @param nombreLab Subcadena a buscarEnTabla dentro del nombre del laboratorio.
 * @return Puntero al primer laboratorio cuyo nombre contenga @p nombreLab, o nullptr si no se encuentra.
 * @note La búsqueda recorre la std::list<Laboratorio> interna y usa std::string::find;
 *       la comparación es sensible a mayúsculas/minúsculas.
 */
Laboratorio *MediExpress::buscarLab(const std::string &nombreLab) {
    std::list<Laboratorio>::iterator it4 = _labs.begin();
    while(it4!=_labs.end()){
        if((*it4).getNombreLab().find(nombreLab)!=std::string::npos){
            return (&(*it4));
        }
        ++it4;
    }
    return nullptr;
}


/**
 * @brief Busca medicamentos cuyo nombre contenga la subcadena indicada.
 * @param nombrePA Subcadena a buscarEnTabla dentro del nombre de cada medicamento (principio activo).
 * @return std::vector dinámico con punteros a los medicamentos cuyo nombre contiene @p nombrePA.
 * @note La búsqueda se realiza recorriendo el std::map<int, PA_Medicamento> interno y utilizando
 *       std::string::find; la comparación es sensible a mayúsculas/minúsculas.
 */
std::vector<PA_Medicamento*> MediExpress::buscarCompuesto(const std::string &nombrePA) {
    std::vector<PA_Medicamento*> toRet;
    std::vector<std::set<PA_Medicamento*>> _vectorDeSet;
    std::stringstream _auxSeparar;
    std::string _cadenaAxuiliar;
    _auxSeparar.str(nombrePA);
    int i=0;
    while(getline(_auxSeparar,_cadenaAxuiliar,' ')){
        std::set<PA_Medicamento*> _sAux;
        _vectorDeSet.push_back(_sAux);
        auto pair_auto= _nombMedication.equal_range(_cadenaAxuiliar);
        for (auto it=pair_auto.first; it!=pair_auto.second; ++it){
            _vectorDeSet[i].insert(it->second);
        }
        ++i;
    }
    std::set<PA_Medicamento*> c,d;
    c=_vectorDeSet[0];
    for (int i = 1; i < _vectorDeSet.size(); i++) {
        set_intersection(_vectorDeSet[i].begin(), _vectorDeSet[i].end(), c.begin(), c.end(),
                         inserter(d, d.begin()));
        c=d;
        d.clear();
    }
    toRet.insert(toRet.begin(),c.begin(),c.end());

    return toRet;

    return toRet;
}


/**
 * @brief Suministra unidades de un medicamento a una farmacia, según su identificador.
 * @param f Puntero a la farmacia receptora.
 * @param id_num Identificador numérico del medicamento a suministrar.
 * @param n Número de unidades a añadir al stock de la farmacia.
 * @post Si el medicamento existe en el std::map<int, PA_Medicamento> interno, se llama a
 *       Farmacia::nuevoStock(pa, n) para incrementar o crear la entrada correspondiente
 *       en el std::set<Stock> de la farmacia. Si no existe, no se modifica el stock.
 */
void MediExpress::suministrarFarmacia(Farmacia *f, int id_num, int n) {
    PA_Medicamento* medicamento = buscarCompuesto(id_num);
    if(medicamento){
        f->nuevoStock(medicamento,n);
    }else{
        //

    }
}


/**
 * @brief Busca una farmacia por su CIF.
 * @param cif Identificador CIF de la farmacia a buscarEnTabla.
 * @return Puntero a la farmacia encontrada o nullptr si no existe.
 * @note La búsqueda se realiza recorriendo el contenedor interno std::vector<Farmacia>.
 */
Farmacia* MediExpress::buscarFarmacia(const std::string &cif) {

    for (auto it = _pharmacy.begin(); it != _pharmacy.end(); ++it) {

        // it->second = objeto Farmacia
        if (it->second.getCif() == cif) {
            return &(it->second);
        }
    }

    return nullptr;  // no encontrada
}




/**
 * @brief Carga las farmacias desde un fichero CSV y las almacena en el contenedor interno.
 * @param _ficheroFarmacias Ruta del fichero CSV/semicolon con farmacias.
 * @post El contenedor interno de farmacias (std::vector<Farmacia>) queda poblado con las
 *       farmacias leídas del fichero, asociando cada una al objeto MediExpress actual.
 */

void MediExpress::_cargarFarmaciasDesdeFichero(const std::string &fich) {

    std::ifstream is(fich);
    if (!is.good()) {
        std::cerr << "Error abriendo archivo de farmacias\n";
        return;
    }

    std::string fila;
    std::stringstream columnas;
    std::string cif, provincia, localidad, nombre, direccion, cp;

    while (getline(is, fila)) {

        if (fila.empty()) continue;

        columnas.str(fila);
        getline(columnas, cif, ';');
        getline(columnas, provincia, ';');
        getline(columnas, localidad, ';');
        getline(columnas, nombre, ';');
        getline(columnas, direccion, ';');
        getline(columnas, cp, '\r');
        columnas.clear();

        Farmacia far(cif, provincia, localidad, nombre, direccion, cp, this);

        // Igual que en constructor: usar insert, no push_back
        _pharmacy.insert({provincia, far});
    }

    is.close();
}
/**
 * @brief Constructor por defecto de MediExpress.
 *
 * Inicializa la _tablaHash hash interna con tamaño 3310 y factor de carga 0.7,
 * dejando vacíos el resto de contenedores. No realiza carga desde fichero.
 *
 * @post El objeto queda inicializado pero sin datos. Es necesario llamar a los
 *       métodos de carga para poblar el sistema.
 */
MediExpress::MediExpress(): idMedication(3310, 0.7),
                            _nombMedication(), _labs(), _pharmacy(), _vMedi(), listaPaMed() {

}
/**
 * @brief Busca los laboratorios que suministran medicamentos cuyo nombre contenga una subcadena dada.
 * @param nombrePa Subcadena a buscarEnTabla dentro del nombre de cada medicamento.
 * @return std::vector dinámico con punteros a laboratorios que suministran medicamentos coincidentes.
 * @note La búsqueda recorre el contenedor interno std::map<int, PA_Medicamento>. El vector devuelto puede
 *       contener punteros repetidos (si un mismo laboratorio suministra varios medicamentos coincidentes).
 */
std::vector<Laboratorio *> MediExpress::buscarLabs(const std::string &nombrePa) {
    std::vector<Laboratorio*> toRet;
    std::vector< PA_Medicamento*> _medicamNombre = buscarCompuesto(nombrePa);

    int i=0;
    while(i < _medicamNombre.size()) {
        if(_medicamNombre[i]->servidoPor() != nullptr){
            toRet.push_back(_medicamNombre[i]->servidoPor());
        }
        ++i;
    }
    return toRet;
}
/**
 * @brief Carga la información de los laboratorios desde un fichero CSV.
 * @param _ficheroLaboratorios Ruta del fichero CSV/semicolon con laboratorios.
 * @post La lista interna de laboratorios (std::list<Laboratorio>) queda poblada y, de forma implícita,
 *       ordenada por el identificador numérico tal y como aparecen en el fichero.
 */
void MediExpress::_cargarLaboratoriosDesdeFichero(const std::string &fich) {

    std::ifstream is(fich);
    if (!is.good()) {
        std::cerr << "Error abriendo archivo de laboratorios\n";
        return;
    }

    std::string fila;
    std::stringstream columnas;
    std::string s_id, nombre, direccion, cp, localidad;
    int id;

    while (getline(is, fila)) {

        if (fila.empty()) continue;

        columnas.str(fila);
        getline(columnas, s_id, ';');
        getline(columnas, nombre, ';');
        getline(columnas, direccion, ';');
        getline(columnas, cp, ';');
        getline(columnas, localidad, '\r');
        columnas.clear();

        id = stoi(s_id);

        Laboratorio lab(id, nombre, direccion, cp, localidad);
        _labs.push_back(lab);
    }

    is.close();
}


void MediExpress::mostrarEstadoTabla() {

    std::cout << "----------------------------------------------\n";
    std::cout << "        ESTADO DE LA TABLA HASH\n";
    std::cout << "----------------------------------------------\n";

    std::cout << "Tamanio de la _tablaHash         : " << idMedication.tamTabla() << "\n";
    std::cout << "Numero de elementos        : " << idMedication.numElementos() << "\n";
    std::cout << "Factor de carga            : " << idMedication.factorCarga() << "\n";
    std::cout << "Max. colisiones insertando : " << idMedication.maxColisiones() << "\n";
    std::cout << "Veces colisiones >10       : " << idMedication.numMax10() << "\n";
    std::cout << "Promedio de colisiones     : " << idMedication.promedioColisiones() << "\n";
    std::cout << "----------------------------------------------\n";


}


/**
 * @brief Construye el sistema MediExpress cargando todos los datos desde fichero y
 *        generando las estructuras internas necesarias.
 *
 * Este constructor:
 *   - Carga medicamentos, laboratorios y farmacias desde sus CSV.
 *   - Inserta los medicamentos en la _tablaHash hash.
 *   - Prueba el rendimiento comparando búsqueda en hash vs lista.
 *   - Postprocesa toda la red creando asociaciones entre:
 *        • medicamentos ↔ palabras clave
 *        • medicamentos ↔ laboratorios
 *        • medicamentos ↔ farmacias
 *
 * @param nomFichPaMed Ruta del CSV de medicamentos.
 * @param nomFichLab Ruta del CSV de laboratorios.
 * @param nomFichFar Ruta del CSV de farmacias.
 * @param tam Tamaño base de la _tablaHash hash interna.
 * @param lamda Factor de carga deseado de la _tablaHash hash.
 *
 * @post El objeto queda completamente inicializado, con laboratorios y farmacias enlazados
 *       y medicamentos repartidos en todos los nodos del sistema.
 */
MediExpress::MediExpress(const std::string &nomFichPaMed,
                         const std::string &nomFichLab,
                         const std::string &nomFichFar,
                         unsigned long tam,
                         float lamda)
        : idMedication(tam, lamda),
          _labs(),
          _pharmacy(),
          _nombMedication(),
          _vMedi(),
          listaPaMed()
{
    // ======================================================
    // 1. CARGAS PRINCIPALES (ANTES ESTABAN EN EL CONSTRUCTOR)
    // ======================================================
    _cargarMedicamentosDesdeFichero(nomFichPaMed);
    _cargarLaboratoriosDesdeFichero(nomFichLab);
    _cargarFarmaciasDesdeFichero(nomFichFar);

    // ======================================================
    // 2. PRUEBA DE RENDIMIENTO ENTRE EL HASH Y LA LISTA
    // ======================================================

    std::chrono::high_resolution_clock ::time_point start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<_vMedi.size(); i++){
        idMedication.buscarEnTabla(_vMedi[i]);
    }
    std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "***Tiempo de la busqueda de la _tablaHash Hash en milisegundos : ***" << duration.count() << std::endl;

    start = std::chrono::high_resolution_clock::now();
    int i=0;
    while(i < _vMedi.size()){
        std::list<PA_Medicamento>::iterator it = listaPaMed.begin();
        while (it!=listaPaMed.end()){
            if (it->getIdNum() == _vMedi[i]) {
                break;
            }
            it++;
        }
        ++i;
    }

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "***Tiempo de la busqueda de la lista en milisegundos : ***" << duration.count() <<  std::endl;

    // ======================================================
    // 3. PROCESAMIENTO ADICIONAL (SUSTITUYE PARTE DEL CONSTRUCTOR)
    // ======================================================
    _postprocesarCargas(nomFichFar);
}


