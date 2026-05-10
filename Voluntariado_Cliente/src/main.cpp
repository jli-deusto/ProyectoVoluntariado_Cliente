//============================================================================
// Name        : Voluntariado_Cliente.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "comms/ClientComms.h"

int main() {
    ClientComms comms;

    if (!comms.conectar("127.0.0.1", 8080)) {
        std::cerr << "No se pudo conectar\n";
        return 1;
    }

    // probar login
    std::string resp = comms.enviar("LOGIN|carlos@gmail.com|000000009bdf7f72");
    std::cout << "Respuesta LOGIN: " << resp << "\n";

    // probar GET_ACTS
    resp = comms.enviar("GET_ACTS");
    std::cout << "Respuesta GET_ACTS: " << resp << "\n";

    comms.enviar("LOGOUT");
    comms.desconectar();

    return 0;
}
