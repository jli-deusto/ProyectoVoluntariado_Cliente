//============================================================================
// Name        : Voluntariado_Cliente.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <cstring>
#include "comms/ClientComms.h"
#include "cache/CacheManager.h"
#include "ui/MenuBase.h"
#include "ui/MenuVoluntario.h"
#include "UI/MenuAdmin.h"




static std::string hashear(const std::string &pw) {
    unsigned long h = 5381;
    for (unsigned char c : pw)
        h = ((h << 5) + h) + c;
    char buf[32];
    snprintf(buf, sizeof(buf), "%016lx", h);
    return std::string(buf);
}


static void mostrarBienvenida() {
    std::cout << "\n==================================================\n";
    std::cout << "     ~ Bienvenido a B2H: Back to Heaven ~\n";
    std::cout << "==================================================\n";
    std::cout << "1. Iniciar sesion\n";
    std::cout << "2. Registrarse\n";
    std::cout << "3. Salir\n";
}


static bool flujoLogin(ClientComms &comms, int &id_out, int &rol_out) {
    std::string usuario, clave;

    std::cout << "\n===== Iniciar Sesion =====\n";
    std::cout << "Usuario: ";
    std::getline(std::cin, usuario);
    std::cout << "Clave: ";
    std::getline(std::cin, clave);

    std::string hash = hashear(clave);
    std::string resp = comms.enviar("LOGIN|" + usuario + "|" + hash);

    if (resp.substr(0, 2) != "OK") {
        std::cout << "Error: " << resp.substr(6) << "\n";
        return false;
    }

    size_t p1 = resp.find('|');
    size_t p2 = resp.find('|', p1 + 1);

    id_out = std::stoi(resp.substr(p1 + 1, p2 - p1 - 1));
    rol_out = std::stoi(resp.substr(p2 + 1));

    std::cout << "Sesion iniciada correctamente.\n";
    return true;
}



static bool flujoRegistro(ClientComms &comms) {
    std::string nombre, mail, clave, confirmar, tlf;

    std::cout << "\n===== Registro de Nuevo Usuario =====\n";
    std::cout << "Nombre: ";
    std::getline(std::cin, nombre);
    std::cout << "Email: ";
    std::getline(std::cin, mail);
    std::cout << "Telefono: ";
    std::getline(std::cin, tlf);

    while (true) {
        std::cout << "Clave: ";
        std::getline(std::cin, clave);
        std::cout << "Confirmar clave: ";
        std::getline(std::cin, confirmar);
        if (clave == confirmar) break;
        std::cout << "Las claves no coinciden. Intentalo de nuevo.\n";
    }

    std::string hash = hashear(clave);
    std::string resp = comms.enviar("REGISTRO|" + nombre + "|" + mail + "|" + hash + "|" + tlf);

    if (resp.substr(0, 2) != "OK") {
        std::cout << "Error: " << resp.substr(6) << "\n";
        return false;
    }

    std::cout << "Registro completado. Ya puedes iniciar sesion.\n";
    return true;
}












int main() {
    ClientComms comms;

    if (!comms.conectar("127.0.0.1", 8080)) {
        std::cerr << "No se pudo conectar al servidor\n";
        return 1;
    }

    CacheManager cache;


    while (true) {
    	mostrarBienvenida();

    	std::string entrada;
    	std::cout << "Seleccione una opcion: ";
    	std::getline(std::cin, entrada);
    	int op = entrada.empty() ? -1 : std::stoi(entrada);

    	if (op == 1) {
    		int id = -1, rol = -1;
    		if (!flujoLogin(comms, id, rol)) continue;

    		MenuBase *menu = nullptr;

    		if (rol == 1) {
    			menu = new MenuAdmin(comms, cache, id);
    		} else {
    			menu = new MenuVoluntario(comms, cache, id);
    		}

    		menu->ejecutar();
    		delete menu;

    	} else if (op == 2) {
    		flujoRegistro(comms);

    	} else if (op == 3) {
    		std::cout << "\nSaliendo del programa...\n";
    		break;

    	} else {
    		std::cout << "Opcion invalida.\n";
    	}
    }


//    // login
//    std::string resp = comms.enviar("LOGIN|carlos@gmail.com|000000009bdf7f72");
//    if (resp.substr(0, 2) != "OK") {
//        std::cerr << "Login fallido: " << resp << "\n";
//        return 1;
//    }
//
//    // extraer id y rol de "OK|id|rol"
//    int id  = std::stoi(resp.substr(3, resp.find('|', 3) - 3));
//    int rol = std::stoi(resp.substr(resp.rfind('|') + 1));
//
//    CacheManager cache;
//
//    // polimorfismo — MenuBase decide qué menú mostrar
//    MenuBase *menu = new MenuVoluntario(comms, cache, id);
//    menu->ejecutar();
//    delete menu;

    comms.desconectar();
    return 0;
}
