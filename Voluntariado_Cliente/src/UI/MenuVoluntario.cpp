/*
 * MenuVoluntario.cpp
 *
 *  Created on: 10 may 2026
 *      Author: danielavalentina.s
 */




#include "MenuVoluntario.h"
#include <iostream>
#include <sstream>



struct Reserva {
    int id_reserva;
    int id_actividad;
    std::string titulo;
    std::string fecha;
    int estado;
};


static Reserva parsearReserva(const std::string &s) {
    Reserva r;
    std::stringstream ss(s);
    std::string campo;
    std::getline(ss, campo, ','); r.id_reserva   = std::stoi(campo);
    std::getline(ss, campo, ','); r.id_actividad = std::stoi(campo);
    std::getline(ss, campo, ','); r.titulo       = campo;
    std::getline(ss, campo, ','); r.fecha        = campo;
    std::getline(ss, campo, ','); r.estado       = std::stoi(campo);
    return r;
}



void MenuVoluntario::mostrar() {
    printf("\n==================================================\n");
    printf("         MENU VOLUNTARIO\n");
    printf("==================================================\n");
    printf("1. Ver actividades\n");
    printf("2. Reservar actividad\n");
    printf("3. Mis reservas\n");
    printf("4. Cancelar reserva\n");
    printf("5. Ver noticias\n");
    printf("6. Reportar incidencia medioambiental\n");
    printf("7. Mi perfil\n");
    printf("8. Cerrar sesion\n");
}

void MenuVoluntario::ejecutar() {
    int op = -1;
    while (op != 7) {
        mostrar();
        op = pedirOpcion(7);
        switch (op) {
			case 1: verActividades();    break;
			case 2: reservarActividad(); break;
			case 3: verMisReservas();    break;
			case 4: cancelarReserva();   break;
			case 5: verNoticias();       break;
			case 6: verPerfil();         break;
			case 7: verPerfil();            break;
			case 8:
				comms.enviar("LOGOUT");
				printf("Sesion cerrada. Hasta pronto!\n");
				break;
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





void MenuVoluntario::verMisReservas() {
    printf("\n===== MIS RESERVAS =====\n");

    std::string resp = comms.enviar(
        "GET_MIS_RESERVAS|" + std::to_string(usuario_id));

    if (resp.substr(0, 2) != "OK") {
        printf("Error al obtener reservas.\n");
        return;
    }

    std::stringstream ss(resp.substr(3));
    std::string item;
    bool hay = false;

    while (std::getline(ss, item, '|')) {
        if (item.empty()) continue;
        Reserva r = parsearReserva(item);
        printf("ID reserva: %d | Actividad: %s | Fecha: %s | Estado: %s\n",
               r.id_reserva,
               r.titulo.c_str(),
               r.fecha.c_str(),
               r.estado ? "Aprobada" : "Cancelada");
        hay = true;
    }

    if (!hay) printf("No tienes reservas activas.\n");
}





void MenuVoluntario::cancelarReserva() {
    printf("\n===== CANCELAR RESERVA =====\n");

    // mostrar reservas activas primero
    std::string resp = comms.enviar(
        "GET_MIS_RESERVAS|" + std::to_string(usuario_id));

    if (resp.substr(0, 2) != "OK") {
        printf("Error al obtener reservas.\n");
        return;
    }

    std::stringstream ss(resp.substr(3));
    std::string item;
    bool hay = false;

    while (std::getline(ss, item, '|')) {
        if (item.empty()) continue;
        Reserva r = parsearReserva(item);
        if (r.estado == 1) {
            printf("ID reserva: %d | %s | %s\n",
                   r.id_reserva, r.titulo.c_str(), r.fecha.c_str());
            hay = true;
        }
    }

    if (!hay) {
        printf("No tienes reservas activas que cancelar.\n");
        return;
    }

    printf("\nID de la reserva a cancelar: ");
    char buf[10];
    fgets(buf, sizeof(buf), stdin);
    int id_reserva = atoi(buf);

    char cmd[64];
    snprintf(cmd, sizeof(cmd), "CANCELAR|%d", id_reserva);
    std::string respCancel = comms.enviar(cmd);

    if (respCancel.substr(0, 2) == "OK") {
        printf("Reserva cancelada correctamente.\n");
    } else {
        printf("Error: %s\n", respCancel.substr(6).c_str());
    }
}





void MenuVoluntario::verPerfil() {
    printf("\n===== MI PERFIL =====\n");
    printf("ID de usuario: %d\n", usuario_id);

    std::string resp = comms.enviar(
        "GET_MIS_RESERVAS|" + std::to_string(usuario_id));

    int total = 0, activas = 0;

    if (resp.substr(0, 2) == "OK") {
        std::stringstream ss(resp.substr(3));
        std::string item;
        while (std::getline(ss, item, '|')) {
            if (item.empty()) continue;
            Reserva r = parsearReserva(item);
            total++;
            if (r.estado == 1) activas++;
        }
    }

    printf("Total reservas:      %d\n", total);
    printf("Reservas activas:    %d\n", activas);
    printf("Reservas canceladas: %d\n", total - activas);
}






void MenuVoluntario::reportarIncidencia() {
    printf("\n===== REPORTAR INCIDENCIA MEDIOAMBIENTAL =====\n");

    char descripcion[500];
    char ubicacion[100];

    printf("Descripcion del incidente: ");
    fflush(stdout);
    fgets(descripcion, sizeof(descripcion), stdin);
    descripcion[strcspn(descripcion, "\n")] = 0;

    printf("Ubicacion: ");
    fflush(stdout);
    fgets(ubicacion, sizeof(ubicacion), stdin);
    ubicacion[strcspn(ubicacion, "\n")] = 0;

    std::string cmd = "INCIDENCIA|" + std::string(descripcion)
                      + "|" + std::string(ubicacion);

    std::string resp = comms.enviar(cmd);

    if (resp.substr(0, 2) == "OK") {
        printf("Incidencia registrada correctamente. Gracias por reportarla.\n");
    } else {
        printf("Error: %s\n", resp.substr(6).c_str());
    }
}
