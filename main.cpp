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
        // Lambdas a probar
        std::vector<float> lambdas = {0.65, 0.68};

        // Las 3 funciones hash
        std::vector<int> funciones = {1, 2, 3};
        std::vector<std::string> nombres = {
                "Hash Cuadrática",
                "Hash Doble A",
                "Hash Doble B"
        };

        // Rutas ficheros
        std::string fP = "../pa_medicamentos.csv";
        std::string fL = "../lab2.csv";
        std::string fF = "../farmacias.csv";

        // Necesitamos conocer cuántos medicamentos hay
        unsigned long totalPA = MediExpress::contarMedicamentos(fP);

        // -----------------------------
        // SOLO DOS MEDIEXPRESS:
        // uno por cada lambda
        // -----------------------------
        for (float lambda : lambdas) {

            std::cout << "\n=========================================\n";
            std::cout << "        Probando λ = " << lambda << "\n";
            std::cout << "=========================================\n";

            // Tamaño inicial: luego THashMedicam usa el siguiente primo mayor
            unsigned long tamBase = totalPA / lambda;

            // Crear UN SOLO MediExpress para esta λ
            MediExpress ME(fP, fL, fF, tamBase, lambda);

            // Probar las 3 funciones hash SOBRE ESTE MISMO OBJETO
            for (int i = 0; i < funciones.size(); ++i) {
                std::cout << "\n>> Probando: " << nombres[i]
                          << "  (función hash = " << funciones[i] << ")\n";

                ME.mostrarEstadoTabla();
            }
        }

        std::cout << "\n=========================================\n";
        std::cout << "          FIN DE PRUEBAS AJUSTE\n";
        std::cout << "=========================================\n";

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

