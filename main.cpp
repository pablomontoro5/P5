#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "PA_Medicamento.h"
#include "Laboratorio.h"
#include "MediExpress.h"
#include "Farmacia.h"
/**  @author Pablo Javier Montoro Bermúdez pjmb0003@red.ujaen.es
      @author Lázaro Ruiz Fernández lrf00011@red.ujaen.es
*/


int main(int argc, const char * argv[]) {
    std::cout << "*** Programa de prueba 1: Ajuste de la tabla ***" << std::endl;
    try{
        //Ejecutar con las 3 tablas _funcionHashNumeroUno
        std::cout << "***Probando la tabla Hash con lambda 0.65 ***" << std::endl;
        MediExpress prueba("../pa_medicamentos.csv","../lab2.csv",
                           "../farmacias.csv",3310,0.65);
        prueba.mostrarEstadoTabla();

        std::cout << "***Probando la tabla Hash con lambda 0.68 ***" << std::endl;
        MediExpress prueba1("../pa_medicamentos.csv","../lab2.csv",
                            "../farmacias.csv",3310,0.68);
        prueba1.mostrarEstadoTabla();

        std::cout << "***EJERCICIO 1 - Buscamos Medicamentos ***:" << std::endl;
        std::string _compABuscar[]={"MAGNESIO CLORURO HEXAHIDRATO", "CLORURO", "ANHIDRO CALCIO CLORURO",
                                    "LIDOCAINA HIDROCLORURO", "MENTA PIPERITA", "VIRUS GRIPE"};

        int i=0;
        while (i < 6){
            int j=0;
            std::vector<PA_Medicamento *> _vectorDeCompuestos = prueba.buscarCompuesto(_compABuscar[i]);
            std::cout << "***El principio activo de Medicamento : ***" << _compABuscar[i] << "*** aparece : ****" << _vectorDeCompuestos.size() << "*** veces ***" << std::endl;
            while(j < _vectorDeCompuestos.size()){
                std::cout << "***Id del Principio Activo: ***" << _vectorDeCompuestos[j]->getIdNum() << "***Nombre del Principio Activo: ***" << _vectorDeCompuestos[j]->getNombre() << std::endl;
                ++j;
            }
            ++i;
        }

        std::cout << "***EJERCICIO 2 - Compras de MAGNESIO en Farmacias de Sevilla***:" << std::endl;
        std::vector<Farmacia*> _vectorDeFarmaciasEnSevilla = prueba.buscarFarmacias("SEVILLA");
        PA_Medicamento *_pAux;
        int i2=0;
        while(i2 < _vectorDeFarmaciasEnSevilla.size()){
            std::vector<PA_Medicamento*> _vectorDeVentas;
            int j2=0;
            while(j2 < 12){
                _vectorDeVentas = _vectorDeFarmaciasEnSevilla[i2]->buscarMedicamNombre("MAGNESIO");
                if(_vectorDeVentas.size() != 0){
                    bool enc=false;
                    int k = 0;
                    while (k < _vectorDeVentas.size() && !enc) {
                        if (_vectorDeFarmaciasEnSevilla[i2]->buscaMedicamID(_vectorDeVentas[k]->getIdNum())) {
                            enc=true;
                            int final= _vectorDeFarmaciasEnSevilla[i2]->comprarMedicam(_vectorDeVentas[k]->getIdNum(), 1, _pAux);
                            std::cout << "***Se han hallado Medicamentos con Magnesio en : ***" << _vectorDeFarmaciasEnSevilla[i2]->getNombre() << std::endl;
                            std::cout << "***Comprando el medicamento con Id :***" << _vectorDeVentas[k]->getIdNum() << "***. Presenta estas unidades inicialmente :***" << final << std::endl;
                        }
                        k++;
                    }
                    if(!enc){
                        std::cout << "***Se han agotado las existencias de Magnesio en :***" << _vectorDeFarmaciasEnSevilla[i2]->getNombre()
                                  << "***Se procede a pedir ***" << std::endl;
                        prueba.suministrarFarmacia(_vectorDeFarmaciasEnSevilla[i2], 3640, 10);
                    }
                }else{
                    std::cout << "***No se han hallado Medicamentos con Magnesio en : ***" << _vectorDeFarmaciasEnSevilla[i2]->getNombre()
                              << "***Se procede a pedir ***" << std::endl;
                    prueba.suministrarFarmacia(_vectorDeFarmaciasEnSevilla[i2], 3640, 10);
                }
                ++j2;
            }
            ++i2;
        }

        std::cout << "***EJERCICIO 3 - Alerta Sanitaria en Ubeda***:" << std::endl;
        std::vector<Farmacia*> _vJaen = prueba.buscarFarmacias("JAEN");
        std::vector<PA_Medicamento*> _vAntigenos= prueba.buscarCompuesto("ANTIGENO OLIGOSACARIDO");
        std::vector<Farmacia*> res;
        int i3=0;
        while(i3 < _vJaen.size()){
            bool encontrado2 = false;
            int j3=0;
            while(j3 < _vAntigenos.size() && !encontrado2){
                if (_vJaen[i]->buscaMedicamID(_vAntigenos[j3]->getIdNum())) {
                    encontrado2 = true;
                    res.push_back(_vJaen[i3]);
                }
                _vJaen[i]->nuevoStock(_vAntigenos[j3],10);
                std::cout << "***Nuevas unidades del Antigeno :***" << _vAntigenos[j3]->getNombre() << "*** en Jaen : ***"
                          << _vJaen[i3]->buscaMedicamID(_vAntigenos[j3]->getIdNum())
                          << "*** en la Farmacia cuyo nombre es : ***" << _vJaen[i3]->getNombre() << std::endl;
            }
            ++i3;
        }
        std::cout << "***Numero de Farmacias en Jaen que vendian algun Principio Activo con Antigeno Oligosacarido : ***" << res.size() << std::endl;

        int i4=0;
        while(i4 < res.size()){
            std::cout << "***Nombre de la Farmacia :***" << res[i4]->getNombre()
                      << "***Localidad donde se encuentra la Farmacia :***" << res[i4]->getLocalidad() << std::endl;
            ++i4;
        }

        std::cout << "***EJERCICIO 4 - Buscar y eliminar el CIANURO de todas las farmacias***:" << std::endl;
        std::vector<PA_Medicamento*> _vectorDeCianuro= prueba.buscarCompuesto("CIANURO");
        if (_vectorDeCianuro.size() == 0) {
            std::cout << "***Palabra Cianuro no hallada***" << std::endl;
        }
        int i5=0;
        while(i5 < _vectorDeCianuro.size()){
            if (prueba.eliminarMedicamento(_vectorDeCianuro[i5]->getIdNum())){
                std::cout << "***Compuesto: Cianuro eliminado con exito ***" << _vectorDeCianuro[i5]->getIdNum()  << std::endl;
            }else{
                std::cout << "***Compuesto: Cianuro no hallado*** " << _vectorDeCianuro[i5]->getIdNum() << std::endl;
            }
            if (!prueba.buscarCompuesto(_vectorDeCianuro[i]->getIdNum())){
                std::cout << "***Compuesto: Cianuro no existente***" << _vectorDeCianuro[i]->getIdNum() << std::endl;
            }else{
                std::cout << "***Cianuro existe ***" << _vectorDeCianuro[i]->getIdNum() << std::endl;
            }
            ++i5;
        }

        std::cout << "***EJERCICIO 4.2 - Buscar y eliminar el BISMUTO de todas las farmacias***:" << std::endl;
        std::vector<PA_Medicamento*> _vectorDeBismuto= prueba.buscarCompuesto("BISMUTO");
        if (_vectorDeBismuto.size() == 0) {
            std::cout << "***Palabra Bismuto no hallada***" << std::endl;
        }
        int i6=0;
        while(i6 < _vectorDeBismuto.size() ){
            if (prueba.eliminarMedicamento(_vectorDeBismuto[i6]->getIdNum())) {
                std::cout << "***Compuesto: Bismuto eliminado con exito ***" << _vectorDeBismuto[i6]->getIdNum() <<  std::endl;
            } else {
                std::cout << "***Compuesto: Bismuto no hallado***" << _vectorDeBismuto[i6]->getIdNum() << std::endl;
            }
            if (!prueba.buscarCompuesto(_vectorDeBismuto[i6]->getIdNum())){
                std::cout << "***Compuesto: Bismuto no existente***" << _vectorDeBismuto[i6]->getIdNum() << std::endl;
            }else{
                std::cout << "***Bismuto existe ***" << _vectorDeBismuto[i6]->getIdNum() << std::endl;
            }
            ++i6;
        }

    }catch(std::runtime_error &rte){
        std::cerr << rte.what() << std::endl;
    }catch(std::out_of_range &oor){
        std::cerr << oor.what() << std::endl;
    }catch(std::bad_alloc &ba){
        std::cerr << ba.what() << std::endl;
    }catch(std::invalid_argument &inarg){
        std::cerr << inarg.what() << std::endl;
    }

    return 0;
}

