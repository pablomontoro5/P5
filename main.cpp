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
        std::cout << "Hash-0.65: " << std::endl;
        MediExpress prueba("../pa_medicamentos.csv","../lab2.csv",
                           "../farmacias.csv",3310,0.65);
        prueba.mostrarEstadoTabla();

        std::cout << "Hash-0.68: " << std::endl;
        MediExpress prueba1("../pa_medicamentos.csv","../lab2.csv",
                            "../farmacias.csv",3310,0.68);
        prueba1.mostrarEstadoTabla();

        std::cout << "EJERCICIO 1 - Buscamos Medicamentos:" << std::endl;
        std::string compuestos[]={"MAGNESIO CLORURO HEXAHIDRATO", "CLORURO", "ANHIDRO CALCIO CLORURO",
                                  "LIDOCAINA HIDROCLORURO", "MENTA PIPERITA", "VIRUS GRIPE"};

        for(int i=0; i<6; ++i){
            std::vector<PA_Medicamento *> vComp = prueba.buscarCompuesto(compuestos[i]);
            std::cout << "PaMedicamento: " << compuestos[i] << " aparece " << vComp.size() << "." << std::endl;
            for(int j=0; j < vComp.size(); ++j){
                std::cout << "ID: " << vComp[j]->getIdNum() << " Nombre: " << vComp[j]->getNombre() << std::endl;
            }
        }

        std::cout << "EJERCICIO 2 - Compras de MAGNESIO en Farmacias de Sevilla:" << std::endl;
        std::vector<Farmacia*> vSevilla = prueba.buscarFarmacias("SEVILLA");
        PA_Medicamento *pMedi;
        for(int i=0; i<vSevilla.size(); ++i){
            std::cout << " Farmacia " << i << ": " << std::endl;
            std::vector<PA_Medicamento*> ventas;

            for(int j=0; j<12; ++j){
                ventas = vSevilla[i]->buscarMedicamNombre("MAGNESIO");
                std::cout << " Cliente " << j+1 << ": " << std::endl;
                if(ventas.size() != 0){
                    bool enc=false;
                    for (int k=0; k<ventas.size() && !enc; ++k) {
                        if (vSevilla[i]->buscaMedicamID(ventas[k]->getIdNum())) {
                            enc=true;
                            int final= vSevilla[i]->comprarMedicam(ventas[k]->getIdNum(), 1, pMedi);
                            std::cout << "Hay Medicamentos con MAGNESIO en: " << vSevilla[i]->getNombre() << std::endl;
                            std::cout << "Comprado Med. id= " << ventas[k]->getIdNum() << ". Unidades iniciales: " << final << std::endl;
                        }
                    }
                    if(!enc){
                        std::cout << "AGOTADAS existencias de MAGNESIO en : " << vSevilla[i]->getNombre()
                             << " SE SOLICITA" << std::endl;
                        prueba.suministrarFarmacia(vSevilla[i], 3640, 10);
                    }
                }else{
                    std::cout << "NO HAY Medicamentos con MAGNESIO en : " << vSevilla[i]->getNombre()
                         << " SE SOLICITA" << std::endl;
                    prueba.suministrarFarmacia(vSevilla[i], 3640, 10);
                }
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

