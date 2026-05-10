//============================================================================
// Name        : Voluntariado_Cliente.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "comms/ClientComms.h"
#include "cache/CacheManager.h"
#include "ui/MenuBase.h"
#include "ui/MenuVoluntario.h"

int main() {
    ClientComms comms;

    if (!comms.conectar("127.0.0.1", 8080)) {
        std::cerr << "No se pudo conectar al servidor\n";
        return 1;
    }

    // login
    std::string resp = comms.enviar("LOGIN|carlos@gmail.com|000000009bdf7f72");
    if (resp.substr(0, 2) != "OK") {
        std::cerr << "Login fallido: " << resp << "\n";
        return 1;
    }

    // extraer id y rol de "OK|id|rol"
    int id  = std::stoi(resp.substr(3, resp.find('|', 3) - 3));
    int rol = std::stoi(resp.substr(resp.rfind('|') + 1));

    CacheManager cache;

    // polimorfismo — MenuBase decide qué menú mostrar
    MenuBase *menu = new MenuVoluntario(comms, cache, id);
    menu->ejecutar();
    delete menu;

    comms.desconectar();
    return 0;
}
