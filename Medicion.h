#ifndef MEDICION_H
#define MEDICION_H

// lleva la cuenta de las iteraciones que hace el programa
class Medicion {
private:
    static unsigned long iteraciones;

public:
    static void reiniciar();
    static void contar(unsigned long cuantas = 1);
    static unsigned long total();
};

#endif
