//
// Created by pablomontoro5 on 24/11/2025.
//

#include "THashMedicam.h"
#include <cmath>
// Funciones sobre numeros primos
bool THashMedicam::esprimo(unsigned int n) {
    for(unsigned int i=2; i<=n/2; ++i) {
        if (n%i == 0) {
            return false;
        }
    }
    return true;
}

unsigned int THashMedicam::primo_menor(unsigned int numero) {
    unsigned int i = numero-1;
    while(!esprimo(i)) {
        --i;
    }
    return i;
}

unsigned int THashMedicam::primo_mayor(unsigned int numero) {
    unsigned int i = numero;
    while(!esprimo(i)) {
        ++i;
    }
    return i;
}

//  la función de dispersión es cuadratica
unsigned THashMedicam::hash(unsigned long clave, int i) {
    unsigned long posicionfinal;
    // peor FUNCION DE DISPERSION 1
    unsigned long parteClave = clave;
    unsigned long parteCuadrado = pow(i, 2);
    unsigned long parteTamanioFisico = tamaf;
    posicionfinal= (parteClave+(parteCuadrado)) % parteTamanioFisico;
    return posicionfinal;
}

//  FUNCION DE DISPERSION 2
unsigned THashMedicam::hash2(unsigned long clave, int i) {
    unsigned long func1 = clave % tamaf;
    //  Dispersión doble
    unsigned long func2 = primoMenor - (clave % primoMenor);
    unsigned long posicionfinal = (func1 + (i * func2)) % tamaf;
    return posicionfinal;
}

//  FUNCION DE DISPERSION 3
unsigned THashMedicam::hash3(unsigned long clave, int i) {
    unsigned long fun1 = clave % tamaf;
    //  Dispersión doble
    unsigned long fun2 = 1 + (clave % primoMenor);
    unsigned long posicionfinal = (fun1 + (i* fun2)) % tamaf;
    return posicionfinal;
}

//tamTabla*(1+(1-lamda)
THashMedicam::THashMedicam(unsigned long maxElementos, double lamda): tamaf(primo_mayor(maxElementos / lamda)), tabla(tamaf, Entrada()),
                                                                      tamal(0), maxCol(0), max10(0), sumaColisiones(0), redisp(0)
{
    primoMenor=primo_menor(tamaf);
};