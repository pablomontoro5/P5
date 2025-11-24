//
// Created by Lázaro Ruiz on 10/11/2025.
//

#ifndef P4_STOCK_H
#define P4_STOCK_H

#include "PA_Medicamento.h"

class Stock {
private:
    int id_PaMed; ///< Debe de coincidir siempre con el _idNumero de PA_Medicamento
    unsigned int num_stock; ///< Numero de medicamentos que hay en la farmacia
    PA_Medicamento* number;

public:
    Stock();
    void decrementa(int n);

    virtual ~Stock();
    Stock(int idPaMed, unsigned int numStock, PA_Medicamento *number);

    void setIdPaMed(int idPaMed);
    Stock(const Stock &_unaCopia);

    unsigned int getNumStock() const;
    PA_Medicamento *getNumber() const;

    bool operator<(const Stock &rhs) const;
    void setNumber(PA_Medicamento *number);

    int getIdPaMed() const;
    void incrementa(int n);


    void setNumStock(unsigned int numStock);
    bool operator<=(const Stock &rhs) const;

    bool operator>(const Stock &rhs) const;
    bool operator>=(const Stock &rhs) const;
};


#endif //P4_STOCK_H
