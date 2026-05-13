/*
 * MenuAdmin.h
 *
 *  Created on: 10 may 2026
 *      Author: danielavalentina.s
 */



#ifndef UI_MENUADMIN_H_
#define UI_MENUADMIN_H_

#include "MenuBase.h"
#include "../cache/CacheManager.h"

class MenuAdmin : public MenuBase {
private:
    CacheManager &cache;
    int usuario_id;

public:
    MenuAdmin(ClientComms &c, CacheManager &cache, int id)
        : MenuBase(c), cache(cache), usuario_id(id) {}

    void mostrar()  override;
    void ejecutar() override;

private:
    void verActividades();
    void verUsuarios();
    void verNoticias();
    void verEstadisticas();
};

#endif /* UI_MENUADMIN_H_ */
