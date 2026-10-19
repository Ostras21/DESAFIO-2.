#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include "Texto.h"
#include "Fecha.h"
#include "RedSocial.h"

// se encarga de leer y escribir los archivos de texto.
// los campos van separados por | y cada registro en una linea
class Archivos {
private:
    Texto carpeta;

    // copia el campo que empieza en desde y devuelve donde arranca el siguiente
    static int siguienteCampo(const char* linea, int desde, char* destino, int maximo);
    static int aEntero(const char* texto);
    static Fecha leerFecha(const char* texto);

    bool cargarAmistades(RedSocial* red) const;
    bool cargarSolicitudes(RedSocial* red) const;
    bool cargarMensajes(RedSocial* red) const;
    bool cargarAlbumes(RedSocial* red) const;
    bool cargarFotos(RedSocial* red) const;

    bool guardarUsuarios(const RedSocial* red) const;
    bool guardarAmistades(const RedSocial* red) const;
    bool guardarSolicitudes(const RedSocial* red) const;
    bool guardarMensajes(const RedSocial* red) const;
    bool guardarAlbumes(const RedSocial* red) const;
    bool guardarFotos(const RedSocial* red) const;

public:
    Archivos(const Texto& c);

    const Texto& getCarpeta() const;

    // crea la red con los datos de los archivos; devuelve 0 si algo falla
    RedSocial* cargar() const;
    bool guardar(const RedSocial* red) const;
};

#endif
