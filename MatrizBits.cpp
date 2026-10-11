#include "MatrizBits.h"
#include "Medicion.h"

MatrizBits::MatrizBits(int tamano) {
    n = tamano;
    palabrasPorFila = (n + 63) / 64;
    total = n * palabrasPorFila;
    palabras = new unsigned long long[total];
    for (int i = 0; i < total; i++) {
        Medicion::contar();
        palabras[i] = 0;
    }
}

MatrizBits::MatrizBits(const MatrizBits& otra) {
    n = otra.n;
    palabrasPorFila = otra.palabrasPorFila;
    total = otra.total;
    palabras = new unsigned long long[total];
    for (int i = 0; i < total; i++) {
        Medicion::contar();
        palabras[i] = otra.palabras[i];
    }
}

MatrizBits::~MatrizBits() {
    delete[] palabras;
}

MatrizBits& MatrizBits::operator=(const MatrizBits& otra) {
    if (this == &otra) {
        return *this;
    }
    delete[] palabras;
    n = otra.n;
    palabrasPorFila = otra.palabrasPorFila;
    total = otra.total;
    palabras = new unsigned long long[total];
    for (int i = 0; i < total; i++) {
        Medicion::contar();
        palabras[i] = otra.palabras[i];
    }
    return *this;
}

int MatrizBits::contarUnos(unsigned long long x) {
    // x & (x-1) apaga el bit encendido mas bajo, asi que
    // se repite una vez por cada uno que haya
    int cuenta = 0;
    while (x != 0) {
        Medicion::contar();
        x = x & (x - 1);
        cuenta++;
    }
    return cuenta;
}

void MatrizBits::poner(int i, int j) {
    palabras[i * palabrasPorFila + j / 64] |= (1ULL << (j % 64));
}

void MatrizBits::quitar(int i, int j) {
    palabras[i * palabrasPorFila + j / 64] &= ~(1ULL << (j % 64));
}

bool MatrizBits::obtener(int i, int j) const {
    return (palabras[i * palabrasPorFila + j / 64] >> (j % 64)) & 1ULL;
}

int MatrizBits::contarFila(int i) const {
    int cuenta = 0;
    for (int k = 0; k < palabrasPorFila; k++) {
        Medicion::contar();
        cuenta += contarUnos(palabras[i * palabrasPorFila + k]);
    }
    return cuenta;
}

int MatrizBits::contarComunes(int a, int b) const {
    // se comparan 64 casillas por cada vuelta
    int cuenta = 0;
    for (int k = 0; k < palabrasPorFila; k++) {
        Medicion::contar();
        cuenta += contarUnos(palabras[a * palabrasPorFila + k] & palabras[b * palabrasPorFila + k]);
    }
    return cuenta;
}

int MatrizBits::tam() const {
    return n;
}

long MatrizBits::memoriaDinamica() const {
    return (long)total * (long)sizeof(unsigned long long);
}
