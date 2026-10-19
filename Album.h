#ifndef ALBUM_H
#define ALBUM_H

#include "Texto.h"
#include "ListaDinamica.h"
#include "Foto.h"

// el album es el dueño de sus fotos, por eso van por valor
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
