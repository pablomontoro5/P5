//
// Created by Lazaro Ruiz on 08/10/2025.
//

#ifndef P2_MEDIEXPRESS_H
#define P2_MEDIEXPRESS_H

#include <vector>
#include <list>
#include <map>

#include "PA_Medicamento.h"
#include "Laboratorio.h"
#include "Farmacia.h"

class MediExpress {
private:
    std::vector<Farmacia> _pharmacy;
    std::list<Laboratorio> _labs;
    std::map<int,PA_Medicamento> _medicamentos;

public:
    std::vector<Laboratorio*> buscarLabs(const std::string &nombrePa);
    void suministrarFarmacia(Farmacia *f, int id_num, int n);

    void _cargarMedicamentosDesdeFichero(const std::string& _ficheroMedicamentos);
    void asignarParesYVerificar();

    void _cargarFarmaciasDesdeFichero(const std::string& _ficheroFarmacias);
    MediExpress(const std::string &_ficheroMedicamentos, const std::string &_ficheroLaboratorios, const std::string &_ficheroFarmacias);


    std::vector<PA_Medicamento*> getMedicamentosSinLab();
    Laboratorio* buscarLab(const std::string &nombreLab);

    std::vector<Farmacia*> buscarFarmacias(const std::string &provincia);
    bool eliminarMedicamento(int id_num);

    void suministrarMed(PA_Medicamento *pa, Laboratorio *l);
    PA_Medicamento* buscarCompuesto(int _idDelNumero);

    std::vector<Laboratorio*> buscarLabCiudad(const std::string &nombreCiudad);
    Farmacia* buscarFarmacia(const std::string &cif);

    std::vector<PA_Medicamento*> buscarCompuesto(const std::string &nombrePA);


    void asignarLabsMadridAMedicamentosSinAsignar();
    void _cargarLaboratoriosDesdeFichero(const std::string& _ficheroLaboratorios);

    };


#endif //P2_MEDIEXPRESS_H
