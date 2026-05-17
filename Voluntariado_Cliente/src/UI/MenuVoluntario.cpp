/*
 * MenuVoluntario.cpp
 *
 *  Created on: 10 may 2026
 *      Author: danielavalentina.s
 */




#include "MenuVoluntario.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>


struct Reserva {
    int id_reserva;
    int id_actividad;
    std::string titulo;
    std::string fecha;
    int estado;
};


static std::string safeSuffix(const std::string &s, size_t pos) {
    return (s.size() > pos) ? s.substr(pos) : "";
}


static Reserva parsearReserva(const std::string &s) {
    Reserva r;
    std::stringstream ss(s);
    std::string campo;

    std::getline(ss, campo, ',');
    r.id_reserva = std::stoi(campo);

    std::getline(ss, campo, ',');
    r.id_actividad = std::stoi(campo);

    std::getline(ss, campo, ',');
    r.titulo = campo;

    std::getline(ss, campo, ',');
    r.fecha = campo;

    std::getline(ss, campo, ',');
    r.estado = std::stoi(campo);

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
    printf("6. Ver incidencias medioambientales\n");
    printf("7. Reportar incidencia medioambiental\n");
	printf("8. Mi perfil\n");
	printf("9. Cerrar sesion\n");
}

void MenuVoluntario::ejecutar() {
    int op = -1;
    while (op != 9) {
        mostrar();
        op = pedirOpcion(9);
        switch (op) {
			case 1: verActividades();
					break;
			case 2: reservarActividad();
					break;
			case 3: verMisReservas();
					break;
			case 4: cancelarReserva();
					break;
			case 5: verNoticias();
					break;
			case 6: verIncidencias();
					break;
			case 7: reportarIncidencia();
					break;
			case 8: verPerfil();
					break;
			case 9:
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

    if (acts.empty()) {
    	printf("No hay actividades disponibles.\n");
    	return;
    }
	printf("%-5s %-30s %-12s %-15s %s\n", "ID", "Titulo", "Fecha", "Tipo", "Plazas");
	printf("--------------------------------------------------------------------\n");

	for (const Actividad &a : acts) {
		printf("%-5d %-30s %-12s %-15s %d\n",
				a.id, a.titulo.c_str(), a.fecha.c_str(), a.tipo.c_str(), a.plazas);
	}
}

void MenuVoluntario::reservarActividad() {
    verActividades();


    printf("\nIntroduce el ID de la actividad a reservar (0 para cancelar): ");
    std::string buf;
    std::getline(std::cin, buf);
    int id = buf.empty() ? 0 : std::stoi(buf);

    if (id <= 0) {
    	printf("Operacion cancelada.\n");
    	return;
    }

    char cmd[64];
    snprintf(cmd, sizeof(cmd), "RESERVAR|%d", id);
    std::string resp = comms.enviar(cmd);


    if (resp.substr(0, 2) == "OK") {
    	printf("\nReserva realizada correctamente.\n");
    	cache.invalidarActividades();
    } else {
    	printf("\n%s\n", safeSuffix(resp, 6).c_str());
    }
}


void MenuVoluntario::verNoticias() {
    printf("\n===== NOTICIAS =====\n");

    const std::vector<std::string> &noticias = cache.getNoticias(comms);

    if (noticias.empty()) {
        printf("No hay noticias disponibles.\n");
        return;
    }

    int i = 1;
    for (const std::string &n : noticias) {
        std::stringstream ss(n);
        std::string id, titulo, contenido, fecha;
        std::getline(ss, id, ',');
        std::getline(ss, titulo, ',');
        std::getline(ss, contenido, ',');
        std::getline(ss, fecha, ',');
        printf("\n[%d] %s  (%s)\n%s\n", i++, titulo.c_str(), fecha.c_str(), contenido.c_str());
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

    std::stringstream ss(safeSuffix(resp, 3));
    std::string item;
    bool hay = false;

    printf("%-10s %-35s %-12s %s\n", "ID", "Actividad", "Fecha", "Estado");
    printf("------------------------------------------------------------\n");


    while (std::getline(ss, item, '|')) {
        if (item.empty()) continue;
        Reserva r = parsearReserva(item);
        printf("%-10d %-35s %-12s %s\n",
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

    std::stringstream ss(safeSuffix(resp, 3));
    std::string item;
    bool hay = false;

    printf("%-10s %-35s %s\n", "ID", "Actividad", "Fecha");
    printf("--------------------------------------------------\n");


    while (std::getline(ss, item, '|')) {
        if (item.empty()) continue;
        Reserva r = parsearReserva(item);
        if (r.estado == 1) {
        	printf("%-10d %-35s %s\n",
                   r.id_reserva, r.titulo.c_str(), r.fecha.c_str());
            hay = true;
        }
    }

    if (!hay) {
        printf("No tienes reservas activas que cancelar.\n");
        return;
    }


    printf("\nIntroduce el ID de la reserva a cancelar (0 para volver): ");
    std::string buf;
    std::getline(std::cin, buf);
    int id_reserva = buf.empty() ? 0 : std::stoi(buf);
    if (id_reserva <= 0) {
    	printf("Operacion cancelada.\n");
    	return;
    }

    char cmd[64];
    snprintf(cmd, sizeof(cmd), "CANCELAR|%d", id_reserva);
    std::string respCancel = comms.enviar(cmd);

    if (respCancel.substr(0, 2) == "OK") {
        printf("Reserva cancelada correctamente.\n");
    } else {
        printf("Error: %s\n", safeSuffix(respCancel, 6).c_str());
    }
}


void MenuVoluntario::verIncidencias() {
    printf("\n===== INCIDENCIAS MEDIOAMBIENTALES =====\n");

    std::string resp = comms.enviar("GET_INCIDENCIAS");

    if (resp.substr(0, 2) != "OK") {
        printf("Error al obtener incidencias.\n");
        return;
    }

    std::stringstream ss(safeSuffix(resp, 3));
    std::string item;
    bool hay = false;
    int i = 1;

    while (std::getline(ss, item, '|')) {
        if (item.empty()) continue;
        std::stringstream si(item);
        std::string id, desc, ubic, fecha, hora, autor;
        std::getline(si, id, ',');
        std::getline(si, desc, ',');
        std::getline(si, ubic, ',');
        std::getline(si, fecha, ',');
        std::getline(si, hora, ',');
        std::getline(si, autor, ',');
        printf("\n[%d] %s %s | %s | %s\n     Descripcion: %s\n",
               i++, fecha.c_str(), hora.c_str(), ubic.c_str(), autor.c_str(), desc.c_str());
        hay = true;
    }

    if (!hay) printf("No hay incidencias registradas.\n");
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

    std::string cmd = "INCIDENCIA|" + std::string(descripcion) + "|" + std::string(ubicacion);
    std::string resp = comms.enviar(cmd);

    if (resp.substr(0, 2) == "OK") {
        printf("\nIncidencia registrada correctamente. Gracias por reportarla.\n");
    } else {
        printf("\nError: %s\n", safeSuffix(resp, 6).c_str());
    }
}


void MenuVoluntario::verPerfil() {
    printf("\n===== MI PERFIL =====\n");

    std::string resp = comms.enviar("GET_PERFIL|" + std::to_string(usuario_id));

    if (resp.substr(0, 2) == "OK") {
        std::stringstream ss(safeSuffix(resp, 3));
        std::string nombre, mail, tlf;
        std::getline(ss, nombre, '|');
        std::getline(ss, mail, '|');
        std::getline(ss, tlf, '|');
        printf("Nombre:    %s\n", nombre.c_str());
        printf("Email:     %s\n", mail.c_str());
        printf("Telefono:  %s\n", tlf.c_str());
    }

    std::string rr = comms.enviar("GET_MIS_RESERVAS|" + std::to_string(usuario_id));
    int total = 0, activas = 0;
    if (rr.substr(0, 2) == "OK") {
        std::stringstream ss(safeSuffix(rr, 3));
        std::string item;
        while (std::getline(ss, item, '|')) {
            if (item.empty()) continue;
            Reserva r = parsearReserva(item);
            total++;
            if (r.estado == 1) activas++;
        }
    }
    printf("-----------------------------\n");
    printf("Total reservas:      %d\n", total);
    printf("Reservas activas:    %d\n", activas);
    printf("Reservas canceladas: %d\n", total - activas);
    printf("\n1. Editar perfil\n2. Volver\n");

    int op = pedirOpcion(2);
    if (op == 1) editarPerfil();
}



void MenuVoluntario::editarPerfil() {
    printf("\n===== EDITAR PERFIL =====\n");
    printf("(Deja en blanco para no cambiar)\n");

    std::string tlf_actual = "";
    std::string resp_perfil = comms.enviar("GET_PERFIL|" + std::to_string(usuario_id));
    if (resp_perfil.substr(0, 2) == "OK") {
    	std::stringstream sp(safeSuffix(resp_perfil, 3));
    	std::string nombre_act, mail_act;
    	std::getline(sp, nombre_act, '|');
    	std::getline(sp, mail_act, '|');
    	std::getline(sp, tlf_actual, '|');
    	if (!tlf_actual.empty() && tlf_actual.back() == '\n'){
    		tlf_actual.pop_back();
    	}
    }

    char nuevo_tlf[20];
    char nueva_pw[70];
    char confirmar_pw[70];

    printf("Nuevo telefono [%s]: ", tlf_actual.c_str());
    fflush(stdout);
    fgets(nuevo_tlf, sizeof(nuevo_tlf), stdin);
    nuevo_tlf[strcspn(nuevo_tlf, "\n")] = 0;

    std::string tlf_enviar = (strlen(nuevo_tlf) > 0) ? std::string(nuevo_tlf) : tlf_actual;

    while (true) {
        printf("Nueva contraseña (o Enter para no cambiar): ");
        fflush(stdout);
        fgets(nueva_pw, sizeof(nueva_pw), stdin);
        nueva_pw[strcspn(nueva_pw, "\n")] = 0;

        if (strlen(nueva_pw) == 0) break; // no cambiar pw

        printf("Confirmar contraseña: ");
        fflush(stdout);
        fgets(confirmar_pw, sizeof(confirmar_pw), stdin);
        confirmar_pw[strcspn(confirmar_pw, "\n")] = 0;

        if (strcmp(nueva_pw, confirmar_pw) == 0) break;
        printf("Las contraseñas no coinciden. Intentalo de nuevo.\n");
    }

    std::string hash_pw = ""; //hash
    if (strlen(nueva_pw) > 0) {
        unsigned long long h = 5381;
        for (unsigned char c : std::string(nueva_pw))
            h = ((h << 5) + h) + c;
        char buf[32];
        snprintf(buf, sizeof(buf), "%016lx", h);
        hash_pw = std::string(buf);
    }

    std::string cmd = "EDITAR_PERFIL|" + std::to_string(usuario_id)
                      + "|" + tlf_enviar
                      + "|" + hash_pw;

    std::string resp = comms.enviar(cmd);

    if (resp.substr(0, 2) == "OK") {
        printf("\nPerfil actualizado correctamente.\n");
    } else {
        printf("Error: %s\n",safeSuffix(resp, 6).c_str());
    }
}
