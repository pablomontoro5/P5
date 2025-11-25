cmake_minimum_required(VERSION 3.31)
project(P5)

set(CMAKE_CXX_STANDARD 26)

add_executable(P5 main.cpp Farmacia.cpp Farmacia.h Laboratorio.cpp Laboratorio.h MediExpress.cpp MediExpress.h PA_Medicamento.cpp PA_Medicamento.h Stock.cpp Stock.h
        THashMedicam.cpp
        THashMedicam.h
        Entrada.h)
