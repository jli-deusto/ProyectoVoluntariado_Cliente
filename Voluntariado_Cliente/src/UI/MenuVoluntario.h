/*
 * MenuVoluntario.h
 *
 *  Created on: 10 may 2026
 *      Author: danielavalentina.s
 */


#ifndef MENUVOLUNTARIO_H_
#define MENUVOLUNTARIO_H_

#include "MenuBase.h"
#include "../cache/CacheManager.h"

class MenuVoluntario : public MenuBase {
private:
    CacheManager &cache;
    int usuario_id;

public:
    MenuVoluntario(ClientComms &c, CacheManager &cache, int id)
        : MenuBase(c), cache(cache), usuario_id(id) {}

    void mostrar() override;
    void ejecutar() override;

private:
    void verActividades();
    void reservarActividad();
    void verNoticias();
    void verPerfil();
};

#endif

