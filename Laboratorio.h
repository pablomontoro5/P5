//
// Created by Lazaro Ruiz on 07/10/2025.
//

#ifndef LABORATIO_LABORATORIO_H
#define LABORATIO_LABORATORIO_H

#include "string"

class Laboratorio {
private:
    int _ID;
    std::string _nombreLab;
    std::string _direccion;
    std::string _codigoPostal;
    std::string _localidad;

public:


    void setLocalidad(const std::string &localidad);

    int getId() const;
    Laboratorio();

    void setId(int id);
    Laboratorio(const Laboratorio &orig);

    const std::string &getNombreLab() const;

    void setNombreLab(const std::string &nombreLab);
    const std::string &getCodigoPostal() const;

    const std::string &getDireccion() const;
    Laboratorio(int id, const std::string &nombreLab, const std::string &direccion, const std::string &codigoPostal,
                const std::string &localidad);

    void setDireccion(const std::string &direccion);
    virtual ~Laboratorio();

    bool operator<(const Laboratorio &rhs) const;
    void setCodigoPostal(const std::string &codigoPostal);

    const std::string &getLocalidad() const;







};


#endif //LABORATIO_LABORATORIO_H
