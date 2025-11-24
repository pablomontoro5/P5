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
    std::cout << "*** Proyecto de prueba: Gestion edificiente de medicamentos Express***" << std::endl;
    try{
        std::cout << "*** Apartado 0. Comenzamos leyendo los ficheros y cargando los datos ***" << std::endl;
        MediExpress _mediExpressPruebas("../pa_medicamentos.csv", "../lab2.csv", "../farmacias.csv");

        std::cout << "*** Apartado 1. Hacemos las compras en las farmacias de Sevilla ***" << std::endl;
        std::vector<Farmacia*> _vectorDeSevilla = _mediExpressPruebas.buscarFarmacias("SEVILLA");
        PA_Medicamento *_punteroAuxiliarPAM;
        for(int i=0; i< _vectorDeSevilla.size(); ++i){
            for(int j=0; j < 12; ++j){
                int stockSevilla = _vectorDeSevilla[2]->comprarMedicam(3640, 1, _punteroAuxiliarPAM);
                if(_punteroAuxiliarPAM != nullptr){
                    std::cout << "*** Stock inicialmente disponible : " << stockSevilla << std::endl;
                    std::cout << "*** Nombre del medicamento *** : " << _punteroAuxiliarPAM->getNombre() << std::endl;
                }
                if(stockSevilla){
                    std::cout << "*** Se ha comprado el medicamento con ID 3640 en el CIF : ***" << _vectorDeSevilla[i]->getCif() << std::endl;
                }else{
                    stockSevilla= _vectorDeSevilla[i]->comprarMedicam(3632, 1, _punteroAuxiliarPAM);
                    if(_punteroAuxiliarPAM != nullptr){
                        std::cout << "*** Stock inicialmente disponible : " << stockSevilla << std::endl;
                        std::cout << "*** Nombre del medicamento *** : " << _punteroAuxiliarPAM->getNombre() << std::endl;
                    }
                    if(stockSevilla){
                        std::cout << "*** Se ha comprado el medicamento con ID 3632 en el CIF : ***" << _vectorDeSevilla[i]->getCif() << std::endl;
                    }else{
                        stockSevilla= _vectorDeSevilla[i]->comprarMedicam(3633, 1, _punteroAuxiliarPAM);
                        if(_punteroAuxiliarPAM != nullptr){
                            std::cout << "*** Stock inicialmente disponible : " << stockSevilla << std::endl;
                            std::cout << "*** Nombre del medicamento *** :" << _punteroAuxiliarPAM->getNombre() << std::endl;
                        }
                        if(stockSevilla){
                            std::cout << "*** Se ha comprado el medicamento con ID 3633 en el CIF *** :" << _vectorDeSevilla[i]->getCif() << std::endl;
                        }
                    }
                }
            }
        }
        std::cout << "*** Apartado 2. Localizamos las farmacias en Madrid que tengan algun medicamento con VIRUS ***" << std::endl;
        std::vector<Farmacia*> _vAux;
        std::vector<Farmacia*> _vectorDeMadrid = _mediExpressPruebas.buscarFarmacias("MADRID");
        std::vector<PA_Medicamento*> _vectorDeVirus = _mediExpressPruebas.buscarCompuesto("VIRUS");
        for (int i=0; i<_vectorDeMadrid.size();i++){
            _vectorDeVirus=_vectorDeMadrid[i]->buscarMedicamNombre("VIRUS");
            if (_vectorDeVirus.size()!=0){
                _vAux.push_back(_vectorDeMadrid[i]);
            }
        }
        std::cout << "***Este es el numero total de farmacias en Madrid que contienen algun medicamento con VIRUS : ***" << _vAux.size() << std::endl;
        for (int i=0; i<_vAux.size();i++){
            /*std::cout << "***Cif de la Farmacia ***: " << _vAux[i]->getCif() << std::endl;
            std::cout << "***Localiad donde se encuentra ***: " << _vAux[i]->getLocalidad() << std::endl;
             */
        }

        std::cout << "*** Apartado 3. Eliminamos el cinauro de todas las Farmacias y de MediExpress ***" << std::endl;
        if(_mediExpressPruebas.eliminarMedicamento(9355)){
            std::cout << "***Cianuro 9355 ha sido eliminado ***" << std::endl;
        }else{
            std::cout << "***No se ha encontrado Cianuro 9355 ***" << std::endl;
        }
        if(!_mediExpressPruebas.buscarCompuesto(9355)){
            std::cout << "***NO EXISTE el cianuro 9355 ***" << std::endl;
        }else{
            std::cout << "***EXISTE el cianuro 9355 ***" << std::endl;
        }
        if(_mediExpressPruebas.eliminarMedicamento(3244)){
            std::cout << "***Cianuro 3244 ha sido eliminado ***" << std::endl;
        }else{
            std::cout << "***No se ha encontrado Cianuro 3244  ***" << std::endl;
        }
        std::cout << "*** Apartado 4. Metodo de parejas ***" << std::endl;
        PA_Medicamento *pAux = _mediExpressPruebas.buscarCompuesto(997);
        for(int i=0; i < _vectorDeMadrid.size(); ++i){
            _vectorDeMadrid[i]->nuevoStock(pAux,20);
        }
        for(int i=0; i < _vectorDeMadrid.size(); ++i){
            int _cantidadQueContiene = _vectorDeMadrid[i]->contienePaMed(997);
            if(_cantidadQueContiene==30){
                std::cout << "***Cif de la Farmacia *** : " << _vectorDeMadrid[i]->getCif() << std::endl;
                std::cout << "***Unidades de Gripe  *** : " << _cantidadQueContiene << std::endl;
            }
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

