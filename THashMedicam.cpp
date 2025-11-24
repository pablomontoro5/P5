//
// Created by pablomontoro5 on 24/11/2025.
//

#include "THashMedicam.h"
// Funciones sobre numeros primos
bool THashMedicam::esprimo(unsigned n) {
    for (unsigned i = 2; i <= n/2; ++i)
        if (n % i == 0)
            return false;
    return true;
}

int THashMedicam::primo_menor(unsigned numero) {
    int i = numero-1;
    while (!esprimo(i)) {
        --i;
    }
    return i;
}

int THashMedicam::primo_mayor(unsigned numero) {
    int i = numero;
    while (!esprimo(i)) {
        ++i;
    }
    return i;
}

//  la función de dispersión es cuadratica
unsigned THashMedicam::hash(unsigned long clave, int i) {
    unsigned long posicionfinal;
    // peor FUNCION DE DISPERSION 1
    posicionfinal= (clave+(i*i)) % tamaf;
    return posicionfinal;
}

//  la funcion de dispersion es doble
unsigned THashMedicam::hash2(unsigned long clave, int i) {
    unsigned long posicion,posicionfinal;

    posicion = clave % tamaf;   //  FUNCION DE DISPERSION 2
    posicionfinal = (posicion + (i* (primoMenor-(clave % (primoMenor))))) % tamaf;
    return posicionfinal;
}
//  la funcion de dispersion es doble
unsigned THashMedicam::hash3(unsigned long clave, int i) {
    unsigned long posicion,posicionfinal;

    posicion = clave % tamaf;   //  FUNCION DE DISPERSION 3
    posicionfinal = (posicion + (i* (1+(clave % (primoMenor))))) % tamaf;
    return posicionfinal;
}

//tamTabla*(1+(1-lamda)
THashMedicam::THashMedicam(unsigned long maxElementos, double lamda): tamaf(primo_mayor(maxElementos / lamda)), tabla(tamaf, Entrada()),
                                                                      tamal(0), maxCol(0), max10(0), sumaColisiones(0), redisp(0)
{
    primoMenor=primo_menor(tamaf);
};