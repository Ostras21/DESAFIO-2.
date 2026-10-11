#include "Medicion.h"

unsigned long Medicion::iteraciones = 0;

void Medicion::reiniciar() {
    iteraciones = 0;
}

void Medicion::contar(unsigned long cuantas) {
    iteraciones += cuantas;
}

unsigned long Medicion::total() {
    return iteraciones;
}
