/*
 * CachManager.cpp
 *
 *  Created on: 10 may 2026
 *      Author: danielavalentina.s
 */

#include "CacheManager.h"
#include <sstream>
#include <iostream>

// parsear "id,titulo,fecha,tipo,plazas"
static Actividad parsearActividad(const std::string &s) {
    Actividad a;
    std::stringstream ss(s);
    std::string campo;

    std::getline(ss, campo, ','); a.id     = std::stoi(campo);
    std::getline(ss, campo, ','); a.titulo = campo;
    std::getline(ss, campo, ','); a.fecha  = campo;
    std::getline(ss, campo, ','); a.tipo   = campo;
    std::getline(ss, campo, ','); a.plazas = std::stoi(campo);

    return a;
}

const std::vector<Actividad>& CacheManager::getActividades(ClientComms &c) {
    if (!acts_cargadas) {
        std::cout << "[CACHE] Pidiendo actividades al servidor...\n";
        std::string resp = c.enviar("GET_ACTS");

        // parsear "OK|act1|act2|act3..."
        if (resp.substr(0, 2) == "OK") {
            std::stringstream ss(resp.substr(3)); // saltar "OK|"
            std::string item;
            while (std::getline(ss, item, '|')) {
                if (!item.empty())
                    acts_cache.push_back(parsearActividad(item));
            }
            acts_cargadas = true;
        }
    } else {
        std::cout << "[CACHE] Usando actividades en memoria (sin peticion red)\n";
    }
    return acts_cache;
}

const std::vector<std::string>& CacheManager::getNoticias(ClientComms &c) {
    if (!noticias_cargadas) {
        std::cout << "[CACHE] Pidiendo noticias al servidor...\n";
        std::string resp = c.enviar("GET_NOTICIAS");

        if (resp.substr(0, 2) == "OK") {
            std::stringstream ss(resp.substr(3));
            std::string item;
            while (std::getline(ss, item, '|')) {
                if (!item.empty())
                    noticias_cache.push_back(item);
            }
            noticias_cargadas = true;
        }
    } else {
        std::cout << "[CACHE] Usando noticias en memoria (sin peticion red)\n";
    }
    return noticias_cache;
}

void CacheManager::invalidarActividades() {
    acts_cargadas = false;
    acts_cache.clear();
}
