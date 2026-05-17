/*
 * CaheManager.h
 *
 *  Created on: 10 may 2026
 *      Author: danielavalentina.s
 */

#ifndef CACHE_CACHEMANAGER_H_
#define CACHE_CACHEMANAGER_H_


#include "../comms/ClientComms.h"
#include <vector>
#include <string>

struct Actividad {
    int id;
    std::string titulo;
    std::string fecha;
    std::string tipo;
    int plazas;
};

class CacheManager {
private:
    std::vector<Actividad> acts_cache;
    bool acts_cargadas = false;

    std::vector<std::string> noticias_cache;
    bool noticias_cargadas = false;

public:
    const std::vector<Actividad>& getActividades(ClientComms &c);
    const std::vector<std::string>& getNoticias(ClientComms &c);
    void invalidarActividades();
    void invalidarNoticias();
};

#endif /* CACHE_CACHEMANAGER_H_ */
