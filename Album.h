#ifndef ALBUM_H
#define ALBUM_H

#include "Texto.h"
#include "ListaDinamica.h"
#include "Foto.h"

// las fotos se guardan por valor dentro del album porque el album es el dueño
// de ellas y no se comparten con nadie mas
class Album {
private:
    Texto nombre;
    int privacidad;
    ListaDinamica<Foto> fotos;

public:
    Album();
    Album(const Texto& nom, int priv);

    const Texto& getNombre() const;
    int getPrivacidad() const;
    void setPrivacidad(int priv);

    int cantidadFotos() const;
    Foto& getFoto(int i);
    const Foto& getFoto(int i) const;

    void agregarFoto(const Foto& f);
    // devuelve la posicion de la foto o -1 si no esta
    int buscarFoto(const Texto& nom) const;

    long memoriaDinamica() const;
};

#endif
