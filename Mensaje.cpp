#include "Mensaje.h"

Mensaje::Mensaje() {
    emisor = -1;
    receptor = -1;
    leido = false;
}

Mensaje::Mensaje(int quienEnvia, int quienRecibe, const Fecha& f, const Texto& texto) {
    emisor = quienEnvia;
    receptor = quienRecibe;
    fecha = f;
    contenido = texto;
    leido = false;
}

int Mensaje::getEmisor() const {
    return emisor;
}

int Mensaje::getReceptor() const {
    return receptor;
}

const Fecha& Mensaje::getFecha() const {
    return fecha;
}

const Texto& Mensaje::getContenido() const {
    return contenido;
}

bool Mensaje::getLeido() const {
    return leido;
}

void Mensaje::setLeido(bool valor) {
    leido = valor;
}

long Mensaje::memoriaDinamica() const {
    return contenido.memoriaDinamica();
}
