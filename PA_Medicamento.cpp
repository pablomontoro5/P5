//
// Created by Pablo Montoro on 24/09/2025.
//

#include <stdexcept>
#include "PA_Medicamento.h"

/**
 * @file PA_Medicamento.cpp
 * @brief Implementación de la clase PA_Medicamento que representa un medicamento con ID numérico, ID alfabético y nombre.
 * @author Pablo Montoro
 * @date 24/09/2025
 */
/**
* @brief Asigna un nuevo identificador alfabético al medicamento.
* @param ifAlpha Cadena con el nuevo ID alfabético.
*/
void PA_Medicamento::_setIdAlpha(const std::string &ifAlpha) {
    _idAlpha = ifAlpha;
}
/**
 * @brief Devuelve el laboratorio que sirve actualmente este medicamento.
 *
 * @return Puntero al laboratorio que sirve el medicamento, o nullptr si no tiene ninguno asignado.
 */
Laboratorio *PA_Medicamento::servidoPor() {
    return _serve;
}
/**
 * @brief Operador de igualdad. Compara dos medicamentos por su ID numérico.
 * @param rhs Otro objeto PA_Medicamento con el que comparar.
 * @return `true` si ambos medicamentos tienen el mismo ID numérico, `false` en caso contrario.
 */
bool PA_Medicamento::operator==(const PA_Medicamento &rhs) const {
    return _idNumero == rhs._idNumero;
}
#include "PA_Medicamento.h"
/**
 * @brief Obtiene el identificador numérico del medicamento.
 * @return Entero que representa el ID numérico.
 */
int PA_Medicamento::getIdNum() const {
    return _idNumero;
}

/**
 * @brief Constructor que inicializa un medicamento con valores dados.
 * @param idNum Identificador numérico del medicamento.
 * @param ifAlpha Identificador alfabético del medicamento.
 * @param nombre Nombre del medicamento.
 */
PA_Medicamento::PA_Medicamento(int idNum, const std::string &ifAlpha, const std::string &nombre)
        : _idNumero(idNum), _idAlpha(ifAlpha), _nombre(nombre),_serve(nullptr) {}



/**
 * @brief Asigna un nuevo identificador numérico al medicamento.
 * @param idNum Nuevo ID numérico.
 */
void PA_Medicamento::setIdNum(int idNum) {
    _idNumero = idNum;
}
/**
 * @brief Constructor de copia.
 * @param _unaCopia Objeto PA_Medicamento a copiar.
 */
PA_Medicamento::PA_Medicamento(const PA_Medicamento &_unaCopia)
        : _idNumero(_unaCopia._idNumero), _idAlpha(_unaCopia._idAlpha), _nombre(_unaCopia._nombre) {}




/**
 * @brief Obtiene el nombre del medicamento.
 * @return Referencia constante a una cadena con el nombre.
 */
const std::string &PA_Medicamento::getNombre() const {
    return _nombre;
}
/**
 * @brief Asigna un laboratorio que servirá este medicamento.
 *
 * @param unLaboratorio Puntero al laboratorio que servirá el medicamento.
 *                      Si es nullptr, se desasigna (el medicamento queda sin laboratorio).
 * @post El atributo interno `_serve` pasa a apuntar a @p unLaboratorio (o a nullptr si se desasigna).
 * @note Actualmente no se lanza excepción si el puntero es nulo, aunque podría añadirse.
 */
void PA_Medicamento::servidoPor(Laboratorio *unLaboratorio) {
    if(unLaboratorio== nullptr){
    }
    _serve = unLaboratorio;
}

/**
 * @brief Asigna un nuevo nombre al medicamento.
 * @param nombre Cadena con el nuevo nombre.
 */
void PA_Medicamento::setNombre(const std::string &nombre) {
    _nombre = nombre;
}





/**
 * @brief Constructor por defecto de la clase PA_Medicamento.
 *
 * Inicializa el identificador numérico en 0 y los campos alfabético y nombre con "***".
 */
PA_Medicamento::PA_Medicamento() : _idNumero(0), _idAlpha("***"), _nombre("***"),_serve(0) {}

/**
 * @brief Obtiene el identificador alfabético del medicamento.
 * @return Referencia constante a una cadena con el ID alfabético.
 */
const std::string &PA_Medicamento::_getIdAlpha() const {
    return _idAlpha;
}
/**
 * @brief Operador de comparación menor. Permite ordenar medicamentos por su ID numérico.
 * @param _unaComparacion Otro objeto PA_Medicamento con el que comparar.
 * @return `true` si el ID numérico del objeto actual es menor que el del objeto comparado.
 */
bool PA_Medicamento::operator<(const PA_Medicamento &_unaComparacion) const {
    return _idNumero < _unaComparacion._idNumero;
}

