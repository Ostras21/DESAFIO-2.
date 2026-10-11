#include "Texto.h"
#include "Medicion.h"

Texto::Texto() {
    longitud = 0;
    datos = new char[1];
    datos[0] = '\0';
}

Texto::Texto(const char* origen) {
    longitud = 0;
    while (origen[longitud] != '\0') {
        Medicion::contar();
        longitud++;
    }
    datos = new char[longitud + 1];
    for (int i = 0; i <= longitud; i++) {
        Medicion::contar();
        datos[i] = origen[i];
    }
}

Texto::Texto(const Texto& otro) {
    longitud = otro.longitud;
    datos = new char[longitud + 1];
    for (int i = 0; i <= longitud; i++) {
        Medicion::contar();
        datos[i] = otro.datos[i];
    }
}

Texto::~Texto() {
    delete[] datos;
}

Texto& Texto::operator=(const Texto& otro) {
    if (this == &otro) {
        return *this;
    }
    delete[] datos;
    longitud = otro.longitud;
    datos = new char[longitud + 1];
    for (int i = 0; i <= longitud; i++) {
        Medicion::contar();
        datos[i] = otro.datos[i];
    }
    return *this;
}

bool Texto::operator==(const Texto& otro) const {
    // si el largo es distinto ni se recorre
    if (longitud != otro.longitud) {
        return false;
    }
    for (int i = 0; i < longitud; i++) {
        Medicion::contar();
        if (datos[i] != otro.datos[i]) {
            return false;
        }
    }
    return true;
}

bool Texto::operator<(const Texto& otro) const {
    int i = 0;
    while (datos[i] != '\0' && otro.datos[i] != '\0') {
        Medicion::contar();
        if (datos[i] != otro.datos[i]) {
            return datos[i] < otro.datos[i];
        }
        i++;
    }
    // si uno es prefijo del otro, el mas corto va primero
    return datos[i] == '\0' && otro.datos[i] != '\0';
}

Texto Texto::operator+(const Texto& otro) const {
    Texto resultado;
    delete[] resultado.datos;
    resultado.longitud = longitud + otro.longitud;
    resultado.datos = new char[resultado.longitud + 1];
    for (int i = 0; i < longitud; i++) {
        Medicion::contar();
        resultado.datos[i] = datos[i];
    }
    for (int i = 0; i <= otro.longitud; i++) {
        Medicion::contar();
        resultado.datos[longitud + i] = otro.datos[i];
    }
    return resultado;
}

const char* Texto::cadena() const {
    return datos;
}

int Texto::tam() const {
    return longitud;
}

long Texto::memoriaDinamica() const {
    return longitud + 1;
}

std::ostream& operator<<(std::ostream& salida, const Texto& t) {
    salida << t.datos;
    return salida;
}
