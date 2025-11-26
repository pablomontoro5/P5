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
    unsigned tamaf;                 // tamaño de la tabla (primo mayor)
    unsigned tamal;                 // número de elementos insertados
    unsigned primoMenor;            // primo menor del tamaño
    unsigned maxCol;                // colisión máxima de una inserción
    unsigned max10;                 // cuántas veces se superan 10 colisiones
    unsigned long sumaColisiones;   // para el promedio
    unsigned redisp;                // número de redispersiones realizadas

    std::vector<Entrada> tabla;     // la tabla hash en sí

    // ----------- FUNCIONES PRIVADAS -------------
    bool esprimo(unsigned n);
    int primo_menor(unsigned numero);
    int primo_mayor(unsigned numero);

    // Tres funciones hash según práctica
    unsigned hash(unsigned long clave, int intento);
    unsigned hash2(unsigned long clave, int i);
    unsigned hash3(unsigned long clave, int i);

public:

    // ----------- CONSTRUCTORES -------------
    THashMedicam(unsigned long maxElementos, double lamda = 0.7);
    THashMedicam(const THashMedicam &otro);       // constructor copia

    // ----------- OPERADOR ASIGNACIÓN -------------
    THashMedicam& operator=(const THashMedicam &otro);

    // ----------- DESTRUCTOR -------------
    ~THashMedicam();

    // ----------- OPERACIONES PRINCIPALES -------------
    bool insertar(unsigned long clave, PA_Medicamento &pa);
    PA_Medicamento* buscar(unsigned long clave);
    bool borrar(unsigned long clave);

    // ----------- INFORMACIÓN INTERNA -------------
    unsigned int numElementos() const { return tamal; }
    unsigned int tamTabla() const { return tamaf; }
    unsigned int maxColisiones() const { return maxCol; }
    unsigned int numMax10() const { return max10; }
    float factorCarga() const{
        if(tamaf == 0){
            return 0.0;
        }
        return static_cast<float>(tamal) / static_cast<float>(tamaf);
    }
    float promedioColisiones() const { return tamal == 0 ? 0 : (float)sumaColisiones / tamal; }
    unsigned int numRedispersiones() const { return redisp; }

    // (PARA LA PARTE EN PAREJAS)
    void redispersar(unsigned nuevoTam);
};
#endif //P5_THASHMEDICAM_H
