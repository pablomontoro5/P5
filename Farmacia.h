//
// Created by Pablo Montoro on 26/10/2025.
//

#ifndef P3_FARMACIA_H
#define P3_FARMACIA_H
#include <iostream>
#include <stdexcept>
#include <string>
#include <set>
#include <list>
#include <vector>

#include "PA_Medicamento.h"
#include "Stock.h"
class MediExpress;
class Farmacia {
public:
    Farmacia(const std::string &cif, const std::string &provincia, const std::string &localidad,
             const std::string &nombre, const std::string &direccion, const std::string &codPostal,
             MediExpress *linkMed);

private:
    std::string _Cif,_Provincia,_Localidad,_Nombre,_Direccion,_CodPostal;
    std::set<Stock> _order;
    MediExpress *linkMed;

    int buscaMedicamID(int _id_num);
    void pedidoMedicam(int _id_Num, int n);


public:
    void setDireccion(const std::string &direccion);
    int contienePaMed(int id_num);

    const std::string &getCodPostal() const;
    Farmacia();

    const std::string &getCif() const;
    void nuevoStock(PA_Medicamento *pa, int n);



    const std::string &getProvincia() const;
    std::vector<PA_Medicamento*> buscarMedicamNombre(const std::string &nombreMedicam);


    void setProvincia(const std::string &provincia);

    const std::string &getLocalidad() const;

    void setLocalidad(const std::string &localidad);

    const std::string &getNombre() const;


    void setNombre(const std::string &nombre);
    void setLinkMed(MediExpress *linkMed);

    const std::string &getDireccion() const;
    MediExpress *getLinkMed() const;


    void setCodPostal(const std::string &codPostal);
    int comprarMedicam(int _idNum, int numAComprar, PA_Medicamento* &result);


    void setCif(const std::string &cif);
    bool eliminarStock(int _idNum);


};


#endif //P3_FARMACIA_H
