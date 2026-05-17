/*
 * MenuBase.h
 *
 *  Created on: 10 may 2026
 *      Author: danielavalentina.s
 */

#ifndef UI_MENUBASE_H_
#define UI_MENUBASE_H_

#include "../comms/ClientComms.h"
#include <string>
#include <iostream>

class MenuBase {
protected:
    ClientComms &comms;

public:
    MenuBase(ClientComms &c) : comms(c) {}
    virtual ~MenuBase() {}

    virtual void mostrar() = 0;    // cada menú lo implementa diferente
    virtual void ejecutar() = 0;   //  bucle principal del menú

protected:
    // utilidad compartida por todos los menús
    int pedirOpcion(int max) {
        int op = -1;
        std::string buf;
        printf("Seleccione una opcion: ");
        fflush(stdout);
        std::getline(std::cin, buf);
        if (!buf.empty()) op = std::stoi(buf);
        if (op < 1 || op > max) return -1;
        return op;
    }
};

#endif /* UI_MENUBASE_H_ */
