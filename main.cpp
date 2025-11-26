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

