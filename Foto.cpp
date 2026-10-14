#include "Foto.h"
#include "Medicion.h"

Foto::Foto() {
    tamano = 0;
}

Foto::Foto(const Texto& nom, const Fecha& f, int kilobytes) {
    nombre = nom;
    captura = f;
    tamano = kilobytes;
}

const Texto& Foto::getNombre() const {
    return nombre;
}

const Fecha& Foto::getCaptura() const {
    return captura;
}

int Foto::getTamano() const {
    return tamano;
}

int Foto::cantidadLikes() const {
    return likes.tam();
}

int Foto::getLike(int i) const {
    return likes[i];
}

bool Foto::tieneLike(int usuario) const {
    for (int i = 0; i < likes.tam(); i++) {
        Medicion::contar();
        if (likes[i] == usuario) {
            return true;
        }
    }
    return false;
}

bool Foto::cambiarLike(int usuario) {
    for (int i = 0; i < likes.tam(); i++) {
        Medicion::contar();
        if (likes[i] == usuario) {
            likes.eliminar(i);
            return false;
        }
    }
    likes.agregar(usuario);
    return true;
}

long Foto::memoriaDinamica() const {
    return nombre.memoriaDinamica() + likes.memoriaDinamica();
}
