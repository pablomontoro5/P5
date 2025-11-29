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
unsigned THashMedicam::_funcionHashNumeroUno(unsigned long clave, int i) {
    unsigned long posicionfinal;
    // peor FUNCION DE DISPERSION 1
    unsigned long parteClave = clave;
    unsigned long parteCuadrado = pow(i, 2);
    unsigned long parteTamanioFisico = tamaf;
    posicionfinal= (parteClave+(parteCuadrado)) % parteTamanioFisico;
    return posicionfinal;
}

//  la funcion de dispersion es doble
unsigned THashMedicam::_funcionHashNumeroDos(unsigned long clave, int i) {
    unsigned long posicion,posicionfinal;

    posicion = clave % tamaf;   //  FUNCION DE DISPERSION 2
    posicionfinal = (posicion + (i* (primoMenor-(clave % (primoMenor))))) % tamaf;
    return posicionfinal;
}
//  la funcion de dispersion es doble
unsigned THashMedicam::_funcionHashNumero3(unsigned long clave, int i) {
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

bool THashMedicam::insertar(unsigned long clave, PA_Medicamento &dato) {
    unsigned i=0,y;
    bool encontrado = false;

    while (!encontrado ) {
        //y= _funcionHashNumeroUno(clave, i);
        //y= _funcionHashNumeroDos(clave, i);
        y= _funcionHashNumero3(clave, i);

        if (tabla[y].getMarca()==LIBRE || tabla[y].getMarca()==DISPONIBLE ) {
            tamal++;
            tabla[y].setDato(dato);
            tabla[y].setMarca(OCUPADA);
            tabla[y].setClave(clave);
            encontrado = true;   //Encontre un sitio libre
        }else{
            if (tabla[y].getDato().getIdNum()==clave)
                return false;  //EL medicamento ya esta en la tabla
            else
                ++i;   //No he dado aun con una posicion libre
        }
    }

    if (i>maxCol){
        maxCol=i;
    }
    if (i>10) {
        max10++;
    }
    sumaColisiones+=i;



    return encontrado;
}

PA_Medicamento *THashMedicam::buscar(unsigned long clave) {
    unsigned i=0,y=0;
    bool enc = false;
    while (!enc ) {
        //y= _funcionHashNumeroUno(clave, i);
        //y= _funcionHashNumeroDos(clave, i);
        y= _funcionHashNumero3(clave, i);

        if (tabla[y].getMarca()==OCUPADA && tabla[y].getClave()==clave){

            return (&tabla[y].getDato());
            //enc=true;
        }else{
            if (tabla[y].getMarca()==LIBRE){
                return 0;
            }else
                ++i;   //No he dado aun con su posicion
        }
    }
    return 0;
}

bool THashMedicam::borrar(unsigned long clave) {
    unsigned i=0,y=0;
    bool fin = false;
    while (!fin) {
        //y= _funcionHashNumeroUno(clave, i);
        //y= _funcionHashNumeroDos(clave, i);
        y= _funcionHashNumero3(clave, i);
        if (tabla[y].getMarca()==OCUPADA && tabla[y].getMarca()==clave){

            tabla[y].setMarca(DISPONIBLE); //lo encontre lo borro y salgo del bucle
            fin=true;
            //tabla[y].dato;
            tamal--;
        }else{
            if (tabla[y].getMarca()==LIBRE)
                break;
            else
                ++i;   //No he dado aun con su posicion
        }
    }
    return fin;
}

THashMedicam::~THashMedicam() {

}