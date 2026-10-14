#include "Album.h"
#include "Medicion.h"

Album::Album() {
    privacidad = 0;
}

Album::Album(const Texto& nom, int priv) {
    nombre = nom;
    privacidad = priv;
}

const Texto& Album::getNombre() const {
    return nombre;
}

int Album::getPrivacidad() const {
    return privacidad;
}

void Album::setPrivacidad(int priv) {
    privacidad = priv;
}

int Album::cantidadFotos() const {
    return fotos.tam();
}

Foto& Album::getFoto(int i) {
    return fotos[i];
}

const Foto& Album::getFoto(int i) const {
    return fotos[i];
}

void Album::agregarFoto(const Foto& f) {
    fotos.agregar(f);
}

int Album::buscarFoto(const Texto& nom) const {
    for (int i = 0; i < fotos.tam(); i++) {
        Medicion::contar();
        if (fotos[i].getNombre() == nom) {
            return i;
        }
    }
    return -1;
}

long Album::memoriaDinamica() const {
    long total = nombre.memoriaDinamica() + fotos.memoriaDinamica();
    for (int i = 0; i < fotos.tam(); i++) {
        total += fotos[i].memoriaDinamica();
    }
    return total;
}
