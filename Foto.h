#ifndef FOTO_H
#define FOTO_H

#include "Texto.h"
#include "Fecha.h"
#include "ListaDinamica.h"

// la ruta no se guarda, se arma cuando se necesita. en likes quedan los
// indices de los que dieron me gusta, para poder quitarlo despues
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
    // si no estaba lo pone y si estaba lo quita; true si quedo con like
    bool cambiarLike(int usuario);

    long memoriaDinamica() const;
};

#endif
