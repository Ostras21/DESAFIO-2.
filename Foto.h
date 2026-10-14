#ifndef FOTO_H
#define FOTO_H

#include "Texto.h"
#include "Fecha.h"
#include "ListaDinamica.h"

// la foto no guarda la ruta, esa se arma cuando se necesita.
// en likes se guardan los indices de los usuarios que dieron me gusta,
// asi se puede quitar el like y no solo contarlos
class Foto {
private:
    Texto nombre;
    Fecha captura;
    int tamano;
    ListaDinamica<int> likes;

public:
    Foto();
    Foto(const Texto& nom, const Fecha& f, int kilobytes);

    const Texto& getNombre() const;
    const Fecha& getCaptura() const;
    int getTamano() const;
    int cantidadLikes() const;
    int getLike(int i) const;

    bool tieneLike(int usuario) const;
    // pone el like si no estaba y lo quita si ya estaba, en un solo recorrido.
    // devuelve true si quedo con like
    bool cambiarLike(int usuario);

    long memoriaDinamica() const;
};

#endif
