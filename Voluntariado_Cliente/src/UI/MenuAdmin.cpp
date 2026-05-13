/*
 * MenuAdmin.cpp
 *
 *  Created on: 12 may 2026
 *      Author: j.li
 */


#include "MenuAdmin.h"
#include <iostream>
#include <sstream>

void MenuAdmin::mostrar() {
    std::cout << "\n==================================================\n";
    std::cout << "           MENU ADMINISTRADOR\n";
    std::cout << "==================================================\n";
    std::cout << "1. Ver actividades\n";
    std::cout << "2. Ver noticias\n";
    std::cout << "3. Cerrar sesion\n";
}

void MenuAdmin::ejecutar() {
    while (true) {
        mostrar();
        int op = pedirOpcion(3);

        switch (op) {
            case 1: verActividades(); break;
            case 2: verNoticias();    break;
            case 3:
                comms.enviar("LOGOUT");
                std::cout << "Sesion cerrada.\n";
                return;
            default:
                std::cout << "Opcion invalida.\n";
        }
    }
}

void MenuAdmin::verActividades() {
    std::cout << "\n===== ACTIVIDADES =====\n";

    const auto &acts = cache.getActividades(comms);

    if (acts.empty()) {
        std::cout << "No hay actividades.\n";
        return;
    }

    for (const auto &a : acts) {
        std::cout << "ID: " << a.id
                  << " | " << a.titulo
                  << " | " << a.fecha
                  << " | Tipo: " << a.tipo
                  << " | Plazas: " << a.plazas << "\n";
    }
}

void MenuAdmin::verNoticias() {
    std::cout << "\n===== NOTICIAS =====\n";

    const auto &noticias = cache.getNoticias(comms);

    if (noticias.empty()) {
        std::cout << "No hay noticias.\n";
        return;
    }

    int i = 1;
    for (const auto &n : noticias) {
        std::stringstream ss(n);
        std::string id, titulo, contenido, fecha;
        std::getline(ss, id,',');
        std::getline(ss, titulo,',');
        std::getline(ss, contenido,',');
        std::getline(ss, fecha,',');

        std::cout << "\n[" << i++ << "] " << titulo
                  << " (" << fecha << ")\n"
                  << contenido << "\n";
    }
}

