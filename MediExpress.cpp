//
// Created by Lazaro Ruiz on 08/10/2025.
//

#include "MediExpress.h"

#include <fstream>
#include <sstream>

/**
 * @brief Obtiene todas las farmacias cuya provincia contiene la cadena indicada.
 * @param provincia Subcadena a buscar dentro del campo provincia de cada farmacia.
 * @return std::vector con punteros a farmacias cuya provincia coincide parcial o totalmente.
 * @note La búsqueda se realiza recorriendo el contenedor interno std::vector<Farmacia>.
 *       La coincidencia usa std::string::find y es sensible a mayúsculas/minúsculas.
 */
std::vector<Farmacia *> MediExpress::buscarFarmacias(const std::string &provincia) {
    std::vector<Farmacia*> toRet;
    for(int i=0; i< _pharmacy.size(); ++i){
        if(_pharmacy[i].getProvincia().find(provincia)!= std::string::npos){
            toRet.push_back(&(_pharmacy[i]));
        }
    }
    return toRet;
}
/**
 * @brief Asigna medicamentos a laboratorios por pares y muestra las asignaciones realizadas.
 *
 * Recorre la lista interna de laboratorios (std::list<Laboratorio>) y el mapa de medicamentos
 * (std::map<int, PA_Medicamento>) asignando a cada laboratorio dos medicamentos consecutivos.
 *
 * @post Cada laboratorio recibe, como máximo, dos medicamentos consecutivos del std::map interno
 *       y se muestran por consola las asignaciones realizadas. También se informa del número de
 *       medicamentos que no han podido ser asignados a ningún laboratorio.
 */
void MediExpress::asignarParesYVerificar() {
    std::list<Laboratorio>::iterator it1=_labs.begin();
    std::map<int,PA_Medicamento>::iterator iteradordeMedicamentos1 = _medicamentos.begin();
    while(iteradordeMedicamentos1!=_medicamentos.end() && it1!=_labs.end()){
        suministrarMed(&(iteradordeMedicamentos1->second),&(*it1));
        ++iteradordeMedicamentos1;
        suministrarMed(&(iteradordeMedicamentos1->second),&(*it1));
        ++iteradordeMedicamentos1;
        ++it1;

    }
    int _contadorAuxiliar = 0;
    std::map<int, PA_Medicamento>::iterator itMed = _medicamentos.begin();
    while (itMed != _medicamentos.end()) {
        PA_Medicamento &_medicamentoAuxiliar = itMed->second;
        if (_medicamentoAuxiliar.servidoPor()) {
            std::cout << "*** Id del principio activo del medicamento *** : "
                      << _medicamentoAuxiliar.getIdNum()
                      << " *** Id del laboratorio *** : "
                      << _medicamentoAuxiliar.servidoPor()->getId()
                      << std::endl;
        } else {
            ++_contadorAuxiliar;
        }
        ++itMed;
    }

    std::cout << " *** Este es el numero de medicamentos que no se han asignado *** : " << _contadorAuxiliar << std:: endl;
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
    bool ret = false;
    for(int i=0; i < _pharmacy.size(); ++i){
        _pharmacy[i].eliminarStock(id_num);
    }
    if(_medicamentos.erase(id_num)!=0){
        ret = true;
    }else{
        ret = false;
    }
    return ret;
}




/**
 * @brief Obtiene todos los medicamentos que actualmente no están servidos por ningún laboratorio.
 * @return std::vector dinámico con punteros a medicamentos sin laboratorio asociado (posiblemente vacío).
 * @note La búsqueda se realiza recorriendo el contenedor interno std::map<int, PA_Medicamento> y
 *       comprobando si cada medicamento tiene o no laboratorio asignado.
 */
std::vector<PA_Medicamento *> MediExpress::getMedicamentosSinLab() {
    std::vector<PA_Medicamento*> toRet;
    std::map<int,PA_Medicamento>::iterator it2 = _medicamentos.begin();
    while(it2!=_medicamentos.end()){
        if(!it2->second.servidoPor()){
            toRet.push_back(&(it2->second));
        }
        ++it2;
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
PA_Medicamento* MediExpress::buscarCompuesto(int _idDelNumero){
    PA_Medicamento *toRet = nullptr;
    std::map<int,PA_Medicamento>::iterator it7=_medicamentos.find(_idDelNumero);
    if (it7 != _medicamentos.end()){
        toRet =  &(it7->second);
    }else{
        return 0;
    }
    return toRet;
}
/**
 * @brief Carga la información de los medicamentos desde un fichero CSV.
 * @param _ficheroMedicamentos Ruta del fichero CSV/semicolon con medicamentos.
 * @post El mapa interno de medicamentos (std::map<int, PA_Medicamento>) queda poblado,
 *       usando como clave el identificador numérico de cada medicamento.
 */
void MediExpress::_cargarMedicamentosDesdeFichero(const std::string &_ficheroMedicamentos) {
    std::ifstream is;
    std::stringstream  columnas;
    std::string fila;
    int contador=0;

    std::string _numeroDeId = "";
    std::string _idAlpha="";
    std::string _nombre="";

    int _idNum=0;


    is.open(_ficheroMedicamentos);
    if ( is.good() ) {


        while ( getline(is, fila ) ) {

            if (fila!="") {

                columnas.str(fila);

                getline(columnas, _numeroDeId, ';');
                getline(columnas, _idAlpha, ';');
                getline(columnas, _nombre, ';');
                _idNum = std::stoi(_numeroDeId);
                fila="";
                columnas.clear();

                //Cambiar la forma de la inserción
                PA_Medicamento _unMedicamento(_idNum, _idAlpha, _nombre);
                _medicamentos[_idNum] = _unMedicamento;

            }
        }
        is.close();

    } else {
        std::cout << "***Ha ocurrido un error de apertura en el archivo de medicamentos ***" <<  std::endl;
    }
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

MediExpress::MediExpress(const std::string &_ficheroMedicamentos, const std::string &_ficheroLaboratorios, const std::string &_ficheroFarmacias) {
    _cargarMedicamentosDesdeFichero(_ficheroMedicamentos);

    _cargarLaboratoriosDesdeFichero(_ficheroLaboratorios);

    _cargarFarmaciasDesdeFichero(_ficheroFarmacias);
    std::cout << " *** Muestro lista de laboratorios :  *** " << std::endl;
    std::list<Laboratorio>::iterator it = _labs.begin();
    while (it!=_labs.end()){
        std::cout << " *** Id del laboratorio : *** " << (*it).getId() << std::endl;
        ++it;
    }
    std::cout << " *** Total de numero de laboratorios : *** " << _labs.size() << std::endl;


    asignarParesYVerificar();

    //Asociar los 152 medicamentos sin laboratorio a laboratorios de Madrid
    asignarLabsMadridAMedicamentosSinAsignar();

    //Leemos la segunda lectura del tercer fichero
    std::vector<std::string> _vectorDeCifsDeFarmacias;
    std::ifstream is;
    std::stringstream  columnas;
    std::string fila;
    int contador =0;
    std::string _nuevoCif = "";
    is.open(_ficheroFarmacias);
    if(is.good()){
        while(getline(is,fila)){
            if(fila != ""){
                columnas.str(fila);
                getline(columnas, _nuevoCif, ';');
                fila = "";
                columnas.clear();
                _vectorDeCifsDeFarmacias.push_back(_nuevoCif);

            }
        }
        is.close();
    }else{
        std::cout << "*** Error de apertura en archivo de medicamentos *** " << std::endl;
    }

    std::map<int,PA_Medicamento>::iterator it3=_medicamentos.begin();
    for (int i=0; i<_vectorDeCifsDeFarmacias.size(); i++){
        Farmacia* f= buscarFarmacia(_vectorDeCifsDeFarmacias[i]);
        int c=0;
        while (c<100){
            suministrarFarmacia(f,it3->second.getIdNum(), 10);

            if (it3==--_medicamentos.end()) {
                it3 = _medicamentos.begin();
            }else {
                it3++;
            }
            c++;
        }
    }

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
    std::map<int,PA_Medicamento>::iterator it6 = _medicamentos.begin();
    for(; it6!=_medicamentos.end(); ++it6){
        if(it6->second.getNombre().find(nombrePA)!= std::string::npos){
            toRet.push_back((&(it6->second)));
        }
    }

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
Farmacia *MediExpress::buscarFarmacia(const std::string &cif) {
    Farmacia *aux = nullptr;
    int i=0;
    while(i < _pharmacy.size()){
        if(_pharmacy[i].getCif()==cif){
            aux = (&(_pharmacy[i]));
        }
        ++i;
    }
    return aux;
}



/**
 * @brief Carga las farmacias desde un fichero CSV y las almacena en el contenedor interno.
 * @param _ficheroFarmacias Ruta del fichero CSV/semicolon con farmacias.
 * @post El contenedor interno de farmacias (std::vector<Farmacia>) queda poblado con las
 *       farmacias leídas del fichero, asociando cada una al objeto MediExpress actual.
 */

void MediExpress::_cargarFarmaciasDesdeFichero(const std::string &_ficheroFarmacias) {
    std::ifstream is;
    std::stringstream  columnas;
    std::string fila;
    int contador=0;
    std::string nuevoCif = "";
    std::string nuevaProvincia = "";
    std::string nuevaLocalidad = "";
    std::string nuevoNombre = "";
    std::string nuevaDireccion = "";
    std::string nuevoCodigoPostal = "";
    is.open(_ficheroFarmacias);
    if(is.good()){
        while(getline(is,fila)){
            if(fila!= ""){
                columnas.str(fila);
                getline(columnas,nuevoCif,';');
                getline(columnas,nuevaProvincia,';');
                getline(columnas,nuevaLocalidad,';');
                getline(columnas,nuevoNombre,';');
                getline(columnas,nuevaDireccion,';');
                getline(columnas,nuevoCodigoPostal,'\r');
                fila = "";
                columnas.clear();
                Farmacia datoFarmaceutico(nuevoCif,nuevaProvincia,nuevaLocalidad,nuevoNombre,nuevaDireccion,nuevoCodigoPostal,this);
                _pharmacy.push_back(datoFarmaceutico);
            }
        }
        is.close();
    }else{
        std::cout << " *** Error de apertura del fichero de farmacias ***" << std::endl;
    }
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
    int i=0;
    while(i < _medicamentos.size()) {
        if (_medicamentos[i].getNombre().find(nombrePa) != std::string::npos) {
            toRet.push_back(_medicamentos[i].servidoPor());
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
void MediExpress::_cargarLaboratoriosDesdeFichero(const std::string &_ficheroLaboratorios) {
    std::ifstream is2;
    std::stringstream  columnas2;
    std::string fila2;
    int contador2=0;

    std::string _numeroDeId = "";
    std::string _nombreLab = "";
    std::string _direccion = "";
    std::string _cp = "";
    std::string _localidad = "";
    int _idNumLab=0;


    is2.open(_ficheroLaboratorios);
    if ( is2.good() ) {
        while ( getline(is2, fila2 ) ) {
            if (fila2!="") {
                columnas2.str(fila2);
                getline(columnas2, _numeroDeId, ';');
                getline(columnas2, _nombreLab, ';');
                getline(columnas2, _direccion, ';');
                getline(columnas2, _cp, ';');
                getline(columnas2, _localidad, '\r');
                _idNumLab = std::stoi(_numeroDeId);
                fila2="";
                columnas2.clear();

                Laboratorio _unLab(_idNumLab,_nombreLab,_direccion,_cp,_localidad);

                _labs.push_back(_unLab);


            }
        }
        is2.close();

    } else {
        std::cout << "***Ha ocurrido un error de apertura en el archivo de laboratorios ***" <<  std::endl;
    }
}