/*
 * ClientComms.h
 *
 *  Created on: 10 may 2026
 *      Author: danielavalentina.s
 */

#ifndef COMMS_CLIENTCOMMS_H_
#define COMMS_CLIENTCOMMS_H_



#include <winsock2.h>
#include <string>

class ClientComms {
private:
    SOCKET sock;
    bool conectado;

public:

    // constructor
    ClientComms();

    // destructor
    ~ClientComms();

    // funciones de conexion con el servidor
    bool conectar(const std::string &ip, int puerto);
    void desconectar();
    bool estaConectado() const;

    std::string enviar(const std::string &comando);
};


#endif /* COMMS_CLIENTCOMMS_H_ */
