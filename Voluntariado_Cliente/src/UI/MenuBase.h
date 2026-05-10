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
        char buf[10];
        printf("Seleccione una opcion: ");
        fflush(stdout);
        fgets(buf, sizeof(buf), stdin);
        sscanf(buf, "%d", &op);
        if (op < 1 || op > max) return -1;
        return op;
    }
};

#endif /* UI_MENUBASE_H_ */
