//
// Created by Lazaro Ruiz on 08/10/2025.
//

#include "MediExpress.h"

#include <fstream>
#include <sstream>
#include <chrono>

/**
 * @brief Obtiene todas las farmacias cuya provincia contiene la cadena indicada.
 * @param provincia Subcadena a buscar dentro del campo provincia de cada farmacia.
 * @return std::vector con punteros a farmacias cuya provincia coincide parcial o totalmente.
 * @note La búsqueda se realiza recorriendo el contenedor interno std::vector<Farmacia>.
 *       La coincidencia usa std::string::find y es sensible a mayúsculas/minúsculas.
 */
std::vector<Farmacia*> MediExpress::buscarFarmacias(const std::string &provincia) {

    std::vector<Farmacia*> toRet;

    for (auto it = _pharmacy.begin(); it != _pharmacy.end(); ++it) {

        // it->first  = provincia (clave del multimap)
        // it->second = Farmacia (valor)

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

    // 3. Eliminar el medicamento de la tabla _funcionHashNumeroUno de IDs (idMedication)
    if (idMedication.borrar(id_num)) {
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
        paMedicamentoAux= idMedication.buscar(_vMedi[i]);
        if(paMedicamentoAux && !paMedicamentoAux->servidoPor()){
            toRet.push_back(paMedicamentoAux);
        }
        ++i;
    }

    return toRet;
}

/**
 * @brief Asigna laboratorios de Madrid a los medicamentos que no tienen laboratorio asignado.
 *
 * Obtiene primero los laboratorios cuya localidad contiene "Madrid" mediante buscarLabCiudad()
 * (recorriendo la std::list<Laboratorio> interna) y los medicamentos sin laboratorio mediante
 * getMedicamentosSinLab() (recorriendo el std::map<int, PA_Medicamento> interno). Después, asigna
 * laboratorio y medicamento por pares en orden.
 *
 * @post Cada medicamento sin laboratorio pasa a estar servido por un laboratorio de Madrid, siempre que
 *       haya suficientes laboratorios disponibles. Se muestran por consola las asignaciones realizadas.
 * @note La asignación se realiza por orden de aparición en los vectores temporales obtenidos.
 */
void MediExpress::asignarLabsMadridAMedicamentosSinAsignar() {
    std::vector<Laboratorio *> _Madrid = buscarLabCiudad("Madrid");
    std::vector<PA_Medicamento *> _sin = getMedicamentosSinLab();
    for (int i = 0; i < _Madrid.size() && i < _sin.size(); ++i) {
        suministrarMed(_sin[i], _Madrid[i]);
        std::cout << " *** ID : *** " << _sin[i]->getIdNum() << " *** , Id del laboratorio :  *** "
                  << _sin[i]->servidoPor()->getId() << std::endl;

    }
}

/**
 * @brief Busca un medicamento por su identificador numérico exacto.
 * @param _idDelNumero Identificador numérico del medicamento.
 * @return Puntero al objeto PA_Medicamento si existe; nullptr en caso contrario.
 * @note La búsqueda se realiza mediante el contenedor interno std::map<int, PA_Medicamento>.
 */
PA_Medicamento* MediExpress::buscarCompuesto(int id_num) {
    PA_Medicamento* encontrado = idMedication.buscar(id_num);
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
        idMedication.insertar(idnum, med);

        // Insertar en lista para rendimiento
        listaPaMed.push_back(med);

        // Insertar ID en vector auxiliar
        _vMedi.push_back(idnum);
    }

    is.close();
}


/**
 * @brief Devuelve los laboratorios cuya localidad contenga el texto dado.
 * @param nombreCiudad Subcadena a buscar dentro del campo localidad de cada laboratorio.
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
 * @param nombreLab Subcadena a buscar dentro del nombre del laboratorio.
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
 * @param nombrePA Subcadena a buscar dentro del nombre de cada medicamento (principio activo).
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
 * @param cif Identificador CIF de la farmacia a buscar.
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
 * @brief Busca los laboratorios que suministran medicamentos cuyo nombre contenga una subcadena dada.
 * @param nombrePa Subcadena a buscar dentro del nombre de cada medicamento.
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

    std::cout << "Tamanio de la tabla         : " << idMedication.tamTabla() << "\n";
    std::cout << "Numero de elementos        : " << idMedication.numElementos() << "\n";
    std::cout << "Factor de carga            : " << idMedication.factorCarga() << "\n";
    std::cout << "Max. colisiones insertando : " << idMedication.maxColisiones() << "\n";
    std::cout << "Veces colisiones >10       : " << idMedication.numMax10() << "\n";
    std::cout << "Promedio de colisiones     : " << idMedication.promedioColisiones() << "\n";
    std::cout << "Numero de redisersiones    : " << idMedication.numRedispersiones() << "\n";
    std::cout << "----------------------------------------------\n";


}

MediExpress::MediExpress(): idMedication(3310, 0.7),
                            _nombMedication(), _labs(), _pharmacy(), _vMedi(), listaPaMed() {

}
void MediExpress::_postprocesarCargas(const std::string &fichFarmacias){
    // ============================================================
    // 1. Construir estructura _nombMedication (nombre → puntero PA)
    // ============================================================

    for (int i=0; i<_vMedi.size(); i++){
        PA_Medicamento* p = idMedication.buscar(_vMedi[i]);

        std::string nombre = p->getNombre();
        std::stringstream separar;
        std::string cad;

        separar.str(nombre);

        while (getline(separar, cad, ' ')){
            _nombMedication.insert(make_pair(cad,p));
        }
    }

    // ============================================================
    // 2. Asociar cada 2 medicamentos con un laboratorio
    // ============================================================

    std::list<Laboratorio>::iterator itl = _labs.begin();
    std::vector<int>::iterator itm = _vMedi.begin();
    PA_Medicamento* p;

    while (itm != _vMedi.end() && itl != _labs.end()) {
        p = idMedication.buscar(*itm);
        suministrarMed(p, &(*itl));
        itm++;

        if (itm == _vMedi.end()) break;
        p = idMedication.buscar(*itm);
        suministrarMed(p, &(*itl));

        itl++;
        itm++;
    }

    // ============================================================
    // 3. Asociar medicamentos sin laboratorio a labs de Madrid
    // ============================================================

    std::vector<Laboratorio*> madrid = buscarLabCiudad("Madrid");
    std::vector<PA_Medicamento*> sin = getMedicamentosSinLab();

    for (int i = 0; i < madrid.size() && i < sin.size(); i++) {
        suministrarMed(sin[i], madrid[i]);
    }

    // ============================================================
    // 4. Segunda lectura del fichero de farmacias para obtener CIFs
    // ============================================================

    std::vector<std::string> vCIF;
    std::ifstream is(fichFarmacias);

    if (!is.good()) {
        std::cerr << "Error abriendo archivo de farmacias (segunda lectura)\n";
        return;
    }

    std::string fila, cif;
    std::stringstream columnas;

    while (getline(is, fila)) {
        if (!fila.empty()) {
            columnas.str(fila);
            getline(columnas, cif, ';');
            columnas.clear();
            vCIF.push_back(cif);
        }
    }

    is.close();

    // ============================================================
    // 5. Reparto de medicamentos a farmacias
    // ============================================================

    std::vector<int>::iterator itM = _vMedi.begin();
    for (const std::string &c : vCIF) {

        Farmacia* f = buscarFarmacia(c);
        if (!f) continue;

        int rep = 0;
        while (rep < 100) {

            suministrarFarmacia(f, *itM, 10);

            if (itM == --_vMedi.end())
                itM = _vMedi.begin();
            else
                ++itM;

            rep++;
        }
    }
}

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
    std::cout << " 2.- Prueba de RENDIMIENTO: " << std::endl;

    std::chrono::high_resolution_clock ::time_point start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<_vMedi.size(); i++){
        idMedication.buscar(_vMedi[i]);
    }
    std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Tiempo busca Hash: " << duration.count() << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<_vMedi.size(); i++){
        std::list<PA_Medicamento>::iterator it = listaPaMed.begin();
        while (it!=listaPaMed.end()){
            if (it->getIdNum() == _vMedi[i])
                break;
            it++;
        }
    }
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Tiempo busca list: " << duration.count() << " ms" << std::endl;

    // ======================================================
    // 3. PROCESAMIENTO ADICIONAL (SUSTITUYE PARTE DEL CONSTRUCTOR)
    // ======================================================
    _postprocesarCargas(nomFichFar);

}


unsigned long MediExpress::contarMedicamentos(const std::string &nomFichPaMed) {
    std::ifstream f(nomFichPaMed);
    if (!f) throw std::runtime_error("Error abriendo pa_medicamentos.csv");

    unsigned long count = 0;
    std::string linea;
    while (getline(f, linea))
        count++;

    return count;
}

