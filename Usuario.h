#ifndef USUARIO_H
#define USUARIO_H

#include "Texto.h"
#include "Fecha.h"
#include "ListaDinamica.h"
#include "Album.h"
#include "Mensaje.h"

// los albumes van por valor; de los mensajes solo se guardan punteros porque
// el mismo mensaje le sirve al emisor y al receptor
class Usuario {
private:
    Texto nombre;
    Texto clave;
    Fecha nacimiento;
    Texto correo;
    Texto ciudad;
    Texto pais;
    Fecha inscripcion;
    ListaDinamica<Album> albumes;
    ListaDinamica<Mensaje*> recibidos;
    ListaDinamica<Mensaje*> enviados;
    int primerNoLeido;

public:
    Usuario();
    Usuario(const Texto& nom, const Texto& cl, const Fecha& nac,
            const Texto& cor, const Texto& ciu, const Texto& pa,
            const Fecha& ins);

    const Texto& getNombre() const;
    const Texto& getClave() const;
    const Fecha& getNacimiento() const;
    const Texto& getCorreo() const;
    const Texto& getCiudad() const;
    const Texto& getPais() const;
    const Fecha& getInscripcion() const;

    void setCorreo(const Texto& c);
    void setCiudad(const Texto& c);
    void setPais(const Texto& p);

    bool claveCorrecta(const Texto& intento) const;

    int cantidadAlbumes() const;
    Album& getAlbum(int i);
    const Album& getAlbum(int i) const;
    void agregarAlbum(const Album& a);
    int buscarAlbum(const Texto& nom) const;

    void recibir(Mensaje* m);
    void enviar(Mensaje* m);
    int cantidadRecibidos() const;
    int cantidadEnviados() const;
    Mensaje* getRecibido(int i) const;
    Mensaje* getEnviado(int i) const;
    const ListaDinamica<Mensaje*>& getRecibidos() const;
    const ListaDinamica<Mensaje*>& getEnviados() const;

    int getPrimerNoLeido() const;
    void setPrimerNoLeido(int posicion);
    // los no leidos siempre quedan al final, entonces no hay que recorrer nada
    int cantidadNoLeidos() const;
    void marcarTodoLeido();

    long memoriaDinamica() const;
};

#endif
