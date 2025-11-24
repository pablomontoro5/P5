//
// Created by Lazaro Ruiz on 07/10/2025.
//

#include "Laboratorio.h"
/**
 * @brief Establece la dirección del laboratorio.
 * @param direccion Nueva dirección a asignar.
 */
void Laboratorio::setDireccion(const std::string &direccion) {
    _direccion = direccion;
}
/**
 * @brief Constructor de copia de Laboratorio.
 * @param orig Objeto Laboratorio a copiar.
 */
Laboratorio::Laboratorio(const Laboratorio &orig) {
    _ID = orig._ID;
    _codigoPostal = orig._codigoPostal;
    _direccion = orig._direccion;
    _nombreLab = orig._nombreLab;
    _localidad = orig._localidad;
}
/**
 * @brief Operador de comparación por ID.
 * @param rhs Otro laboratorio con el que comparar.
 * @return true si el ID de este laboratorio es menor que el del comparado; false en caso contrario.
 */
bool Laboratorio::operator<(const Laboratorio &rhs) const {
    return _ID < rhs._ID;
}

/**
 * @brief Establece el identificador único del laboratorio.
 * @param id Nuevo valor del ID.
 */
void Laboratorio::setId(int id) {
    _ID = id;
}
/**
 * @brief Devuelve el nombre del laboratorio.
 * @return Referencia constante al nombre del laboratorio.
 */
const std::string &Laboratorio::getNombreLab() const {
    return _nombreLab;
}
/**
 * @brief Establece el código postal del laboratorio.
 * @param codigoPostal Nuevo código postal.
 */
void Laboratorio::setCodigoPostal(const std::string &codigoPostal) {
    _codigoPostal = codigoPostal;
}
/**
 * @brief Devuelve la dirección del laboratorio.
 * @return Referencia constante a la dirección.
 */
const std::string &Laboratorio::getDireccion() const {
    return _direccion;
}



/**
 * @brief Devuelve la localidad del laboratorio.
 * @return Referencia constante a la localidad.
 */
const std::string &Laboratorio::getLocalidad() const {
    return _localidad;
}

/**
 * @brief Destructor de Laboratorio.
 */
Laboratorio::~Laboratorio() {

}
/**
 * @brief Establece la localidad del laboratorio.
 * @param localidad Nueva localidad.
 */
void Laboratorio::setLocalidad(const std::string &localidad) {
    _localidad = localidad;
}
/**
 * @brief Constructor de Laboratorio con parámetros.
 * @param id Identificador numérico del laboratorio.
 * @param nombreLab Nombre del laboratorio.
 * @param direccion Dirección postal del laboratorio.
 * @param codigoPostal Código postal asociado.
 * @param localidad Localidad o ciudad.
 */
Laboratorio::Laboratorio(int id, const std::string &nombreLab, const std::string &direccion,
                         const std::string &codigoPostal, const std::string &localidad) : _ID(id),
                                                                                          _nombreLab(nombreLab),
                                                                                          _direccion(direccion),
                                                                                          _codigoPostal(codigoPostal),
                                                                                          _localidad(localidad) {

}

/**
 * @brief Constructor por defecto. Inicializa un laboratorio vacío.
 * @note Usa valores de marcador ("---") para los campos de texto y 0 para el ID.
 */
Laboratorio::Laboratorio(): _ID(0),_nombreLab("---"),_direccion("---"),_codigoPostal("---"),_localidad("---") {}

/**
 * @brief Obtiene el identificador único del laboratorio.
 * @return Entero con el ID del laboratorio.
 */
int Laboratorio::getId() const {
    return _ID;
}
/**
 * @brief Establece el nombre del laboratorio.
 * @param nombreLab Nuevo nombre a asignar.
 */
void Laboratorio::setNombreLab(const std::string &nombreLab) {
    _nombreLab = nombreLab;
}
/**
 * @brief Devuelve el código postal del laboratorio.
 * @return Referencia constante al código postal.
 */
const std::string &Laboratorio::getCodigoPostal() const {
    return _codigoPostal;
}