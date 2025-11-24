//
// Created by pablomontoro5 on 24/11/2025.
//

#ifndef P5_ENTRADA_H
#define P5_ENTRADA_H
#include "PA_Medicamento.h"
enum Estado{LIBRE,OCUPADA,DISPONIBLE};

class Entrada {
private:
    unsigned long clave;
    Estado marca;
    PA_Medicamento dato;

public:
    Entrada() : clave(0), marca(LIBRE), dato() {}
    ~Entrada() {}

    // --- GETTERS ---
    unsigned long getClave() const { return clave; }
    Estado getMarca() const { return marca; }
    const PA_Medicamento& getDato() const { return dato; }

    // --- SETTERS ---
    void setClave(unsigned long c) { clave = c; }
    void setMarca(Estado m) { marca = m; }
    void setDato(const PA_Medicamento& d) { dato = d; }
};


#endif //P5_ENTRADA_H
