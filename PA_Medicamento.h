//
// Created by Pablo Montoro on 24/09/2025.
//

#ifndef P1_PA_MEDICAMENTO_H
#define P1_PA_MEDICAMENTO_H

#include "string"

#include "Laboratorio.h"

class PA_Medicamento {

private:
    int _idNumero;
    std::string _idAlpha;
    std::string _nombre;
    Laboratorio *_serve;

public:
    Laboratorio* servidoPor();
    void setIdNum(int idNum);

    bool operator==(const PA_Medicamento &rhs) const;
    int getIdNum() const;

    PA_Medicamento(const PA_Medicamento &_unaCopia);
    bool operator<(const PA_Medicamento &_unaComparacion) const;

    void setNombre(const std::string &nombre);
    PA_Medicamento(int idNum, const std::string &ifAlpha, const std::string &nombre);

    void servidoPor(Laboratorio *unLaboratorio);

    PA_Medicamento();
    const std::string &_getIdAlpha() const;

    void _setIdAlpha(const std::string &ifAlpha);
    const std::string &getNombre() const;




};


#endif //P1_PA_MEDICAMENTO_H
