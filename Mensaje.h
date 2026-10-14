#ifndef MENSAJE_H
#define MENSAJE_H

#include "Texto.h"
#include "Fecha.h"

// un mensaje guarda los indices del emisor y del receptor, no los punteros,
// asi ocupa menos y sirve igual para buscar al usuario en la red
class Mensaje {
private:
    int emisor;
    int receptor;
    Fecha fecha;
    Texto contenido;
    bool leido;

public:
    Mensaje();
    Mensaje(int quienEnvia, int quienRecibe, const Fecha& f, const Texto& texto);

    int getEmisor() const;
    int getReceptor() const;
    const Fecha& getFecha() const;
    const Texto& getContenido() const;
    bool getLeido() const;

    void setLeido(bool valor);

    long memoriaDinamica() const;
};

#endif
