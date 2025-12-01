//
// Created by pablomontoro5 on 24/11/2025.
//

#ifndef P5_THASHMEDICAM_H
#define P5_THASHMEDICAM_H

#include "PA_Medicamento.h"
#include "Entrada.h"
#include <vector>
#include <stdexcept>

class THashMedicam {

private:
    // ----------- ATRIBUTOS INTERNOS -------------
    unsigned _tamanioFisicoHash;                 ///< tamaño de la _tablaHash (primo mayor)
    unsigned _tamanioLogicoHash;                 ///< número de elementos insertados
    unsigned primoMenor;            ///< primo menor del tamaño
    unsigned _maximoColisiones;                ///< colisión máxima de una inserción
    unsigned _masDiezColisiones;                 ///< cuántas veces se superan 10 colisiones
    unsigned long sumaColisiones;   ///< para el promedio
    unsigned _Redispersion;               ///< número de redispersiones realizadas

    std::vector<Entrada> _tablaHash;

    // ----------- FUNCIONES PRIVADAS -------------
    bool esNumeroPrimo(unsigned int n);
    unsigned int getPrimoMenor(unsigned int numero);
    unsigned int getPrimoMayor(unsigned int numero);

    // Tres funciones _funcionHashNumeroUno según práctica
    unsigned _funcionHashNumeroUno(unsigned long clave, int intento);
    unsigned _funcionHashNumeroDos(unsigned long clave, int intento);
    unsigned _funcionHashNumero3(unsigned long clave, int intento);

public:

    // ----------- CONSTRUCTORES -------------
    THashMedicam(unsigned long maxElementos, double lamda = 0.7);


    // ----------- DESTRUCTOR -------------
    ~THashMedicam();

    // ----------- INFORMACIÓN INTERNA -------------
    unsigned int numElementos() const { return _tamanioLogicoHash; }
    unsigned int tamTabla() const { return _tamanioFisicoHash; }
    float factorCarga() const { return (float)_tamanioLogicoHash / _tamanioFisicoHash; }

    unsigned int maxColisiones() const { return _maximoColisiones; }
    unsigned int numMax10() const { return _masDiezColisiones; }
    float promedioColisiones() const { return _tamanioLogicoHash == 0 ? 0 : (float)sumaColisiones / _tamanioLogicoHash; }
    unsigned int numRedispersiones() const { return _Redispersion; }

    // ----------- OPERACIONES PRINCIPALES -------------
    bool insertarEnTabla(unsigned long clave, PA_Medicamento &dato);
    PA_Medicamento* buscarEnTabla(unsigned long clave);
    bool borrarEnTabla(unsigned long clave);



};
#endif //P5_THASHMEDICAM_H
