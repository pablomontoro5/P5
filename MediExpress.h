//
// Created by Lazaro Ruiz on 08/10/2025.
//

#ifndef P2_MEDIEXPRESS_H
#define P2_MEDIEXPRESS_H

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include "PA_Medicamento.h"
#include "Laboratorio.h"
#include "Farmacia.h"
#include "THashMedicam.h"

class MediExpress {
private:
    std::multimap<std::string,Farmacia> _pharmacy;
    std::list<Laboratorio> _labs;
    //P5
    THashMedicam idMedication;
    std::vector<int> _vMedi;
    std::multimap<std::string,PA_Medicamento*> _nombMedication;
    std::list<PA_Medicamento> listaPaMed;

public:
    std::vector<Laboratorio*> buscarLabs(const std::string &nombrePa);
    void suministrarFarmacia(Farmacia *f, int id_num, int n);

    void _cargarMedicamentosDesdeFichero(const std::string& _ficheroMedicamentos);
    void asignarParesYVerificar();

    void _cargarFarmaciasDesdeFichero(const std::string& _ficheroFarmacias);
    MediExpress(const std::string &nomFichPaMed, const std::string &nomFichLab,
                const std::string &nomFichFar, unsigned long tam, float lamda);

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
    static unsigned long contarMedicamentos(const std::string &nomFichPaMed);



    //P5

    void mostrarEstadoTabla();
    MediExpress();
    void _postprocesarCargas(const std::string &fichFarmacias);


};


#endif //P2_MEDIEXPRESS_H
