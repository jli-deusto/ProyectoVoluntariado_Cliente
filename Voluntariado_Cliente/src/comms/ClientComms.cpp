/*
 * ClientComms.cpp
 *
 *  Created on: 10 may 2026
 *      Author: danielavalentina.s
 */





#include "ClientComms.h"
#include <iostream>
#include <winsock2.h>

#define MAX_BUFFER 4096

ClientComms::ClientComms() : sock(INVALID_SOCKET), conectado(false) {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
}

ClientComms::~ClientComms() {
    desconectar();
    WSACleanup();
}

bool ClientComms::conectar(const std::string &ip, int puerto) {
	// crear socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) return false;

    struct sockaddr_in server;
    // direccion completa del servidor al cual se conecta el cliente:
    server.sin_family      = AF_INET; // Protocolo IPv4
    server.sin_port        = htons(puerto); // Puerto en formato de red
    server.sin_addr.s_addr = inet_addr(ip.c_str()); // IP en formato numerico

    // conectar con el servidor
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "No se pudo conectar al servidor\n";
        return false;
    }

    conectado = true;
    std::cout << "Conectado al servidor " << ip << ":" << puerto << "\n";
    return true;
}

void ClientComms::desconectar() {
    if (conectado) {
        closesocket(sock);
        conectado = false;
    }
}

bool ClientComms::estaConectado() const {
    return conectado;
}

std::string ClientComms::enviar(const std::string &comando) {
    if (!conectado) return "ERROR|No conectado";

    // enviar comando con salto de línea
    std::string msg = comando + "\n";
    send(sock, msg.c_str(), msg.size(), 0);

    // recibir respuesta
    char buffer[MAX_BUFFER] = {0};
    int bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0) {
        conectado = false;
        return "ERROR|Conexion perdida";
    }

    // quitar '\n' del final
    std::string respuesta(buffer);
    if (!respuesta.empty() && respuesta.back() == '\n')
        respuesta.pop_back();

    return respuesta;
}
