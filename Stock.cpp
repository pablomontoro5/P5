//
// Created by Lázaro Ruiz on 10/11/2025.
//

#include "Stock.h"

/**
 * @brief Decrementa la cantidad almacenada de un medicamento.
 * @param n Número de unidades a restar del stock actual.
 * @post El valor interno num_stock se reduce en n (sin comprobación de límites).
 */
void Stock::decrementa(int n) {
    num_stock -= n;
}

/**
 * @brief Incrementa la cantidad almacenada de un medicamento.
 * @param n Número de unidades a sumar al stock actual.
 * @post El valor interno num_stock se incrementa en n.
 */
void Stock::incrementa(int n) {
    num_stock += n;
}

/**
 * @brief Obtiene el identificador numérico del principio activo asociado al stock.
 * @return Identificador del PA.
 */
int Stock::getIdPaMed() const {
    return id_PaMed;
}

/**
 * @brief Establece el identificador numérico del principio activo.
 * @param idPaMed Nuevo identificador.
 */
void Stock::setIdPaMed(int idPaMed) {
    id_PaMed = idPaMed;
}

/**
 * @brief Obtiene la cantidad actual de unidades en stock.
 * @return Número de unidades almacenadas.
 */
unsigned int Stock::getNumStock() const {
    return num_stock;
}

/**
 * @brief Establece la cantidad de unidades en stock.
 * @param numStock Nueva cantidad de unidades.
 */
void Stock::setNumStock(unsigned int numStock) {
    num_stock = numStock;
}

/**
 * @brief Constructor por defecto de Stock.
 * Inicializa el ID del PA a 0, el stock a 0 y el puntero al medicamento a nullptr.
 */
Stock::Stock() : id_PaMed(0), num_stock(0), number(nullptr) {}

/**
 * @brief Destructor de la clase Stock.
 * No libera memoria externa; el puntero number no se destruye aquí.
 */
Stock::~Stock() {}

/**
 * @brief Constructor parametrizado de Stock.
 * @param idPaMed Identificador del principio activo.
 * @param numStock Cantidad inicial en stock.
 * @param number Puntero al medicamento asociado.
 */
Stock::Stock(int idPaMed, unsigned int numStock, PA_Medicamento *number)
        : id_PaMed(idPaMed), num_stock(numStock), number(number) {}

/**
 * @brief Constructor de copia de Stock.
 * Copia ID, cantidad y puntero al medicamento.
 * @param _unaCopia Objeto Stock a copiar.
 */
Stock::Stock(const Stock &_unaCopia)
        : id_PaMed(_unaCopia.id_PaMed),
          num_stock(_unaCopia.num_stock),
          number(_unaCopia.number) {}

/**
 * @brief Operador de comparación estricta para ordenar objetos Stock.
 *
 * El contenedor std::set<Stock> requiere un criterio de ordenación estricta.
 * En este caso, los objetos se ordenan por el identificador del principio activo.
 *
 * @param rhs Objeto Stock con el que se compara.
 * @return true si este Stock tiene un ID menor que rhs; false en caso contrario.
 */
bool Stock::operator<(const Stock &rhs) const {
    return id_PaMed < rhs.id_PaMed;
}

/**
 * @brief Operador de comparación mayor.
 * @param rhs Objeto comparado.
 * @return true si este objeto es mayor que rhs.
 */
bool Stock::operator>(const Stock &rhs) const {
    return rhs < *this;
}

/**
 * @brief Operador menor o igual.
 * @param rhs Objeto comparado.
 * @return true si este objeto es menor o igual que rhs.
 */
bool Stock::operator<=(const Stock &rhs) const {
    return !(rhs < *this);
}

/**
 * @brief Operador mayor o igual.
 * @param rhs Objeto comparado.
 * @return true si este objeto es mayor o igual que rhs.
 */
bool Stock::operator>=(const Stock &rhs) const {
    return !(*this < rhs);
}

/**
 * @brief Obtiene el puntero al medicamento asociado al stock.
 * @return Puntero a PA_Medicamento.
 */
PA_Medicamento *Stock::getNumber() const {
    return number;
}

/**
 * @brief Establece el puntero al medicamento asociado.
 * @param number Nuevo puntero a PA_Medicamento.
 */
void Stock::setNumber(PA_Medicamento *number) {
    Stock::number = number;
}
