/*
 * MenuVoluntario.cpp
 *
 *  Created on: 10 may 2026
 *      Author: danielavalentina.s
 */




#include "MenuVoluntario.h"
#include <iostream>
#include <sstream>

void MenuVoluntario::mostrar() {
    printf("\n==================================================\n");
    printf("         MENU VOLUNTARIO\n");
    printf("==================================================\n");
    printf("1. Ver actividades\n");
    printf("2. Reservar actividad\n");
    printf("3. Ver noticias\n");
    printf("4. Cerrar sesion\n");
}

void MenuVoluntario::ejecutar() {
    int op = -1;
    while (op != 4) {
        mostrar();
        op = pedirOpcion(4);
        switch (op) {
            case 1: verActividades();   break;
            case 2: reservarActividad(); break;
            case 3: verNoticias();      break;
            case 4: comms.enviar("LOGOUT"); break;
            default: printf("Opcion invalida\n");
        }
    }
}

void MenuVoluntario::verActividades() {
    const std::vector<Actividad> &acts = cache.getActividades(comms);
    printf("\n===== ACTIVIDADES DISPONIBLES =====\n");
    for (const Actividad &a : acts) {
        printf("ID: %d | %s | %s | %s | Plazas: %d\n",
            a.id, a.titulo.c_str(), a.fecha.c_str(),
            a.tipo.c_str(), a.plazas);
    }
}

void MenuVoluntario::reservarActividad() {
    verActividades();
    printf("\nID de la actividad a reservar: ");
    char buf[10];
    fgets(buf, sizeof(buf), stdin);
    int id = atoi(buf);

    char cmd[64];
    snprintf(cmd, sizeof(cmd), "RESERVAR|%d", id);
    std::string resp = comms.enviar(cmd);
    printf("Respuesta: %s\n", resp.c_str());

    // invalidar cache porque cambió el número de plazas
    cache.invalidarActividades();
}

void MenuVoluntario::verNoticias() {
    const std::vector<std::string> &noticias = cache.getNoticias(comms);
    printf("\n===== NOTICIAS =====\n");
    for (const std::string &n : noticias) {
        printf("%s\n", n.c_str());
    }
}
