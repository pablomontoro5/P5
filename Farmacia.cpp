//
// Created by Pablo Montoro on 26/10/2025.
//

#include "Farmacia.h"
#include "MediExpress.h"
#include "Stock.h"
/**
 * @brief Devuelve el stock actual de un medicamento en la farmacia.
 * @param id_num Identificador numérico del medicamento.
 * @return Cantidad de unidades en stock del medicamento con identificador @p id_num,
 *         o 0 si no existe stock para ese medicamento.
 * @note La búsqueda se realiza en el contenedor interno std::set<Stock> usando un objeto
 *       Stock auxiliar con el mismo id de principio activo.
 */
int Farmacia::contienePaMed(int id_num) {
    int toRet = 0;
    Stock _auxiliarNumero4;
    _auxiliarNumero4.setIdPaMed(id_num);

    std::set<Stock>::iterator _it12= _order.find(_auxiliarNumero4);
    if(_it12 != _order.end()){
        toRet = _it12->getNumStock();
    }
    return toRet;
}
/**
 * @brief Busca medicamentos por nombre (parcialmente) dentro de la farmacia.
 * @param nombreMedicam Subcadena a buscar dentro del nombre de cada medicamento.
 * @return std::vector dinámico con punteros a medicamentos (PA_Medicamento) cuyo nombre
 *         contiene @p nombreMedicam. Puede estar vacío.
 * @note La búsqueda recorre el contenedor interno std::set<Stock>, accediendo al
 *       PA_Medicamento asociado a cada elemento Stock. La coincidencia usa std::string::find
 *       y es sensible a mayúsculas/minúsculas.
 */
std::vector<PA_Medicamento *> Farmacia::buscarMedicamNombre(const std::string &nombreMedicam) {
   std::vector<PA_Medicamento*> toRet;
   std::set<Stock>::iterator _it14 = _order.begin();
   while(_it14 != _order.end()){
       if(_it14->getNumber()->getNombre().find(nombreMedicam) != std::string::npos){
           toRet.push_back(_it14->getNumber());
       }
       _it14++;
   }
   return toRet;
}

/**
 * @brief Obtiene el código CIF de la farmacia.
 * @return Referencia constante al CIF.
 */
const std::string &Farmacia::getCif() const {
    return _Cif;
}


/**
 * @brief Obtiene la provincia donde se encuentra la farmacia.
 * @return Referencia constante a la provincia.
 */
const std::string &Farmacia::getProvincia() const {
    return _Provincia;
}
/**
 * @brief Establece la provincia de la farmacia.
 * @param provincia Nueva provincia.
 */
void Farmacia::setProvincia(const std::string &provincia) {
    _Provincia = provincia;
}

/**
 * @brief Obtiene la localidad de la farmacia.
 * @return Referencia constante a la localidad.
 */
const std::string &Farmacia::getLocalidad() const {
    return _Localidad;
}
/**
 * @brief Establece la localidad de la farmacia.
 * @param localidad Nueva localidad.
 */
void Farmacia::setLocalidad(const std::string &localidad) {
    _Localidad = localidad;
}
/**
 * @brief Obtiene el nombre de la farmacia.
 * @return Referencia constante al nombre.
 */
const std::string &Farmacia::getNombre() const {
    return _Nombre;
}

/**
 * @brief Establece el nombre de la farmacia.
 * @param nombre Nuevo nombre.
 */
void Farmacia::setNombre(const std::string &nombre) {
    _Nombre = nombre;
}
/**
 * @brief Realiza la compra de un medicamento en la farmacia.
 *
 * Si hay stock suficiente del medicamento indicado, se descuenta del std::set<Stock> interno
 * y se devuelve el stock restante. En ese caso, se devuelve también, por referencia, un puntero
 * al medicamento comprado. Si no hay stock suficiente, se realiza un pedido al sistema MediExpress
 * y el puntero resultado se establece a nullptr.
 *
 * @param _idNum Identificador numérico del medicamento a comprar.
 * @param numAComprar Número de unidades que se desea adquirir.
 * @param result Referencia a un puntero donde se devuelve el medicamento comprado si la operación
 *               se realiza con éxito; se deja en nullptr si se lanza un pedido a MediExpress.
 * @return Cantidad de unidades restantes en stock del medicamento tras la operación
 *         (puede ser 0 si se agotó).
 */
int Farmacia::comprarMedicam(int _idNum, int numAComprar, PA_Medicamento* &result) {
    if(buscaMedicamID(_idNum)>=numAComprar){
        Stock _auxiliarNumero5;
        _auxiliarNumero5.setIdPaMed(_idNum);
        std::set<Stock>::iterator _it13 = _order.find(_auxiliarNumero5); //Hacemos esto porque a los set se le ha de pasar un objeto si o si
        Stock _auxiliarNumero6= (*_it13);
        _order.erase(_it13);
        _auxiliarNumero6.decrementa(numAComprar);
        _order.insert(_auxiliarNumero6);
        result = _order.find(_auxiliarNumero6)->getNumber();
    }else{
        pedidoMedicam(_idNum,10);
        result = nullptr;
    }
    return buscaMedicamID(_idNum);

}
/**
 * @brief Obtiene la dirección de la farmacia.
 * @return Referencia constante a la dirección.
 */
const std::string &Farmacia::getDireccion() const {
    return _Direccion;
}
/**
 * @brief Establece la dirección de la farmacia.
 * @param direccion Nueva dirección.
 */
void Farmacia::setDireccion(const std::string &direccion) {
    _Direccion = direccion;
}
/*
* @brief Obtiene el código postal de la farmacia.
* @return Referencia constante al código postal.
*/
const std::string &Farmacia::getCodPostal() const {
    return _CodPostal;
}
/**
 * @brief Añade o incrementa el stock de un medicamento en la farmacia.
 *
 * Si ya existe una entrada Stock para el medicamento indicado en el std::set<Stock> interno,
 * se incrementa su cantidad en @p n unidades. En caso contrario, se crea un nuevo objeto
 * Stock con el identificador del medicamento, la cantidad inicial @p n y el puntero al
 * PA_Medicamento suministrado.
 *
 * @param pa Puntero al medicamento (PA_Medicamento) cuyo stock se quiere modificar.
 * @param n Número de unidades a añadir al stock (debe ser positivo).
 */
void Farmacia::nuevoStock(PA_Medicamento *pa, int n) {
    Stock _auxiliarNumero1;
    _auxiliarNumero1.setIdPaMed(pa->getIdNum());
    std::set<Stock>::iterator _it10 = _order.find(_auxiliarNumero1);
    if(_it10 != _order.end()){
        Stock _auxiliarNumero2 = *_it10;
        _order.erase(_it10);
        _auxiliarNumero2.incrementa(n);
        _order.insert(_auxiliarNumero2);
    }else{
        Stock _nuevoStockAuxiliar(pa->getIdNum(), n, pa);
        _order.insert(_nuevoStockAuxiliar);
    }
}
/**
 * @brief Establece el código postal de la farmacia.
 * @param codPostal Nuevo código postal.
 */
void Farmacia::setCodPostal(const std::string &codPostal) {
    _CodPostal = codPostal;
}
/**
 * @brief Obtiene el puntero al objeto MediExpress asociado a la farmacia.
 * @return Puntero al objeto MediExpress.
 */
MediExpress *Farmacia::getLinkMed() const {
    return linkMed;
}
/**
 * @brief Establece el puntero al objeto MediExpress asociado a la farmacia.
 * @param linkMed Nuevo puntero a MediExpress.
 */
void Farmacia::setLinkMed(MediExpress *linkMed) {
    Farmacia::linkMed = linkMed;
}
/**
 * @brief Constructor parametrizado de la clase Farmacia.
 * @param cif Código CIF de la farmacia.
 * @param provincia Provincia donde se ubica la farmacia.
 * @param localidad Localidad de la farmacia.
 * @param nombre Nombre comercial de la farmacia.
 * @param direccion Dirección física de la farmacia.
 * @param codPostal Código postal de la farmacia.
 * @param linkMed Puntero al objeto MediExpress asociado.
 * @post El contenedor interno de stock (std::set<Stock>) se inicializa vacío.
 */
Farmacia::Farmacia(const std::string &cif, const std::string &provincia, const std::string &localidad,
                   const std::string &nombre, const std::string &direccion, const std::string &codPostal,
                   MediExpress *linkMed) : _Cif(cif), _Provincia(provincia), _Localidad(localidad), _Nombre(nombre),
                                           _Direccion(direccion), _CodPostal(codPostal), linkMed(linkMed),_order() {

}


/**
 * @brief Busca un medicamento en la farmacia por su identificador numérico y devuelve su stock.
 *
 * Crea un objeto Stock auxiliar con el identificador del principio activo y lo utiliza para
 * localizar la entrada correspondiente en el contenedor interno std::set<Stock>. Si el medicamento
 * existe en la farmacia, devuelve la cantidad de unidades almacenadas.
 *
 * @param _id_num Identificador numérico del medicamento a buscar.
 * @return Número de unidades en stock del medicamento indicado, o 0 si no existe stock en la farmacia.
 */
int Farmacia::buscaMedicamID(int _id_num) {
    Stock _auxiliarNumero6;
    _auxiliarNumero6.setIdPaMed(_id_num);
    std::set<Stock>::iterator _it13 = _order.find(_auxiliarNumero6);
    if(_it13 != _order.end()){
        return _it13->getNumStock();
    }
    return 0;
}
/**
 * @brief Solicita un medicamento al sistema MediExpress. Llama al método MediExpress::suministrarFarmacia() para que el sistema central suministre @p n unidades del medicamento identificado por @_id_Num a esta farmacia.

 * @param _id_Num Identificador numérico del medicamento solicitado.
 * @param n Número de unidades que se solicitan al sistema MediExpress.
 * @pre La farmacia debe tener un puntero válido a MediExpress (`linkMed != nullptr`).
 */
void Farmacia::pedidoMedicam(int _id_Num, int n) {
    if(linkMed){
        linkMed->suministrarFarmacia(this,_id_Num,n);
    }
}

/**
 * @brief Constructor por defecto de la clase Farmacia.
 *
 * Inicializa los campos de texto con el valor "---", el puntero a MediExpress a nullptr
 * y el contenedor interno de stock (std::set<Stock>) vacío.
 */
Farmacia::Farmacia() :_Cif("---"),_Provincia("---"),_Localidad("---"),_Nombre("---"),_Direccion("---"),_CodPostal("---"),linkMed(
        nullptr),_order()
{

}

/**
 * @brief Elimina el stock asociado a un medicamento de la farmacia.
 * @param _idNum Identificador numérico del medicamento cuyo stock se desea eliminar.
 * @return true si existía una entrada en el std::set<Stock> interno y se ha eliminado;
 *         false en caso contrario.
 */
bool Farmacia::eliminarStock(int _idNum) {
    Stock _auxiliarNumero3;
    _auxiliarNumero3.setIdPaMed(_idNum);
    std::set<Stock>::iterator _it11 = _order.find(_auxiliarNumero3);
    if(_it11 != _order.end()){
        _order.erase(_it11);
        return true;
    }
    return false;
}
/**
 * @brief Establece el código CIF de la farmacia.
 * @param cif Nuevo código CIF.
 */
void Farmacia::setCif(const std::string &cif) {
    _Cif = cif;
}
