//
// Created by pablomontoro5 on 24/11/2025.
//

#include "THashMedicam.h"
#include <cmath>

/**
 * @brief Inserta un medicamento en la tabla hash mediante direccionamiento abierto.
 * @param clave Identificador del medicamento.
 * @param dato Referencia al medicamento a insertar.
 * @return true si la inserción se completó con éxito;
 *         false si ya existía un elemento con esa clave.
 *
 * Actualiza estadísticas:
 *  - Máximo de colisiones
 *  - Número de colisiones > 10
 *  - Suma de colisiones para promedio
 */
bool THashMedicam::insertarEnTabla(unsigned long clave, PA_Medicamento &dato) {
    unsigned _intento=0,y;
    bool encontrado = false;

    while (!encontrado ) {
        //y= _funcionHashNumeroUno(clave, _intento);
        //y= _funcionHashNumeroDos(clave, _intento);
        y= _funcionHashNumero3(clave, _intento);

        if (_tablaHash[y].getMarca() == LIBRE || _tablaHash[y].getMarca() == DISPONIBLE ) {
            _tamanioLogicoHash++;
            _tablaHash[y].setDato(dato);
            _tablaHash[y].setMarca(OCUPADA);
            _tablaHash[y].setClave(clave);
            encontrado = true;
        }else{
            if (_tablaHash[y].getDato().getIdNum() == clave) {
                return false;
            }else {
                ++_intento;
            }
        }
    }

    if (_intento > _maximoColisiones){
        _maximoColisiones=_intento;
    }
    if (_intento > 10) {
        _masDiezColisiones++;
    }
    sumaColisiones+=_intento;

    return encontrado;
}

/**
 * @brief Destructor de la tabla hash.
 *
 * No libera memoria manualmente ya que la tabla utiliza contenedores STL.
 */
THashMedicam::~THashMedicam() {

}
/**
 * @brief Función de dispersión cuadrática.
 * @param clave Clave numérica (ID del medicamento).
 * @param i Número de intento en la secuencia de exploración.
 * @return Posición calculada dentro de la tabla hash.
 *
 * Calcula:
 *      (clave + i^2) % tamaño_físico
 */
unsigned THashMedicam::_funcionHashNumeroUno(unsigned long clave, int i) {
    unsigned long posicionfinal;
    unsigned long parteClave = clave;
    unsigned long parteCuadrado = pow(i, 2);
    unsigned long parteTamanioFisico = _tamanioFisicoHash;
    posicionfinal= (parteClave+(parteCuadrado)) % parteTamanioFisico;
    return posicionfinal;
}
/**
 * @brief Construye una tabla hash para un número máximo de elementos y factor de carga dado.
 * @param maxElementos Número máximo estimado de elementos a insertar.
 * @param lamda Factor de carga deseado (0 < λ < 1).
 *
 * El constructor:
 *  - Calcula el tamaño físico como el primo mayor a (maxElementos / lamda)
 *  - Inicializa la tabla con entradas libres
 *  - Inicializa métricas internas
 */
THashMedicam::THashMedicam(unsigned long maxElementos, double lamda): _tamanioFisicoHash(getPrimoMayor(maxElementos / lamda)), _tablaHash(_tamanioFisicoHash, Entrada()),
                                                                      _tamanioLogicoHash(0), _maximoColisiones(0), _masDiezColisiones(0), sumaColisiones(0), _Redispersion(0)
{
    primoMenor= getPrimoMenor(_tamanioFisicoHash);
};
/**
 * @brief Determina si un número es primo.
 * @param n Número a evaluar.
 * @return true si n es primo; false en caso contrario.
 */
bool THashMedicam::esNumeroPrimo(unsigned int n) {
    for(unsigned int i=2; i<=n/2; ++i) {
        if (n%i == 0) {
            return false;
        }
    }
    return true;
}
/**
 * @brief Función de doble dispersión (variante 1).
 * @param clave Clave hash.
 * @param intento Número de intento en la secuencia de exploración.
 * @return Índice calculado en la tabla hash.
 *
 * Fórmula:
 *     h(k,i) = (k % m) + i * (primoMenor - (k % primoMenor))   mod m
 */
unsigned THashMedicam::_funcionHashNumeroDos(unsigned long clave, int intento) {
    unsigned long posicion,posicionfinal;

    posicion = clave % _tamanioFisicoHash;   //  FUNCION DE DISPERSION 2
    posicionfinal = (posicion + (intento * (primoMenor - (clave % (primoMenor))))) % _tamanioFisicoHash;
    return posicionfinal;
}
/**
 * @brief Obtiene el menor número primo mayor o igual al dado.
 * @param numero Límite inferior.
 * @return Número primo superior más cercano.
 */
unsigned int THashMedicam::getPrimoMenor(unsigned int numero) {
    unsigned int i = numero-1;
    while(!esNumeroPrimo(i)) {
        --i;
    }
    return i;
}
/**
 * @brief Función de doble dispersión (variante 2).
 * @param clave Clave hash.
 * @param intento Número del intento.
 * @return Índice dentro de la tabla hash.
 *
 * Fórmula:
 *      h(k,i) = (k % m) + i * (1 + (k % primoMenor))   mod m
 */
unsigned THashMedicam::_funcionHashNumero3(unsigned long clave, int intento) {
    unsigned long posicion,posicionfinal;

    posicion = clave % _tamanioFisicoHash;
    posicionfinal = (posicion + (intento * (1 + (clave % (primoMenor))))) % _tamanioFisicoHash;
    return posicionfinal;
}
/**
 * @brief Obtiene el mayor número primo estrictamente menor que el dado.
 * @param numero Límite superior.
 * @return Número primo inferior más cercano.
 */
unsigned int THashMedicam::getPrimoMayor(unsigned int numero) {
    unsigned int i = numero;
    while(!esNumeroPrimo(i)) {
        ++i;
    }
    return i;
}

/**
 * @brief Busca un medicamento en la tabla hash mediante exploración abierta.
 * @param clave Clave del medicamento.
 * @return Puntero al medicamento si se encuentra; nullptr en caso contrario.
 *
 * La búsqueda se detiene cuando:
 *  - Se encuentra la clave
 *  - Se alcanza una casilla LIBRE
 */

PA_Medicamento *THashMedicam::buscarEnTabla(unsigned long clave) {
    unsigned _intento=0,y=0;
    bool _encontrado = false;
    while (!_encontrado ) {
        //y= _funcionHashNumeroUno(clave, _intento);
        //y= _funcionHashNumeroDos(clave, _intento);
        y= _funcionHashNumero3(clave, _intento);

        if (_tablaHash[y].getMarca() == OCUPADA && _tablaHash[y].getClave() == clave){
            return (&_tablaHash[y].getDato());
        }else{
            if (_tablaHash[y].getMarca() == LIBRE){
                return 0;
            }else
                ++_intento;
        }
    }
    return 0;
}
/**
 * @brief Elimina un elemento de la tabla hash marcándolo como DISPONIBLE.
 * @param clave Identificador del medicamento.
 * @return true si el elemento existía y se eliminó; false si no se encontró.
 *
 * El borrado nunca deja casillas LIBRES, sino DISPONIBLES, para mantener la secuencia de exploración.
 */
bool THashMedicam::borrarEnTabla(unsigned long clave) {
    unsigned i=0,y=0;
    bool fin = false;
    while (!fin) {
        //y= _funcionHashNumeroUno(clave, i);
        //y= _funcionHashNumeroDos(clave, i);
        y= _funcionHashNumero3(clave, i);
        if (_tablaHash[y].getMarca() == OCUPADA && _tablaHash[y].getClave() == clave){
            fin=true;
            _tablaHash[y].setMarca(DISPONIBLE); //lo encontre lo borro y salgo del bucle
            _tamanioLogicoHash--;
        }else{
            if (_tablaHash[y].getMarca() == LIBRE) {
                break;
            }else {
                ++i;
            }
        }
    }
    return fin;
}

