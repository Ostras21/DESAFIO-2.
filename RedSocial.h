#ifndef REDSOCIAL_H
#define REDSOCIAL_H

#include "Texto.h"
#include "Fecha.h"
#include "ListaDinamica.h"
#include "MatrizBits.h"
#include "Usuario.h"
#include "Mensaje.h"

// esta clase es la dueña de los usuarios y de los mensajes: los crea con new
// y los borra en el destructor. la amistad y las solicitudes se guardan en dos
// matrices de bits para poder preguntar si dos personas son amigas en un paso
class RedSocial {
private:
    ListaDinamica<Usuario*> usuarios;
    ListaDinamica<Mensaje*> mensajes;
    MatrizBits amistad;
    MatrizBits solicitudes;
    int* indicePorNombre;
    int capacidadIndice;
    int tamIndice;
    bool indiceListo;
    Texto rutaBase;
    int sesion;

    void ordenarIndice(int* arreglo, int inicio, int fin, int* apoyo);
    // devuelve la primera posicion de la lista con fecha mayor o igual a la
    // buscada; la lista esta ordenada porque los mensajes se agregan en orden
    static int primeroDesde(const ListaDinamica<Mensaje*>& lista, const Fecha& desde);
    static int pesoPorComunes(int comunes);

public:
    RedSocial(int cantidadUsuarios, const Texto& ruta);
    ~RedSocial();

    int cantidadUsuarios() const;
    Usuario* getUsuario(int i) const;
    const Texto& getRutaBase() const;
    const MatrizBits& getAmistad() const;
    const MatrizBits& getSolicitudes() const;
    int cantidadMensajes() const;
    Mensaje* getMensaje(int i) const;

    // ---- carga inicial ----
    void agregarUsuario(Usuario* u);
    // se llama una sola vez cuando ya estan todos los usuarios cargados
    void construirIndice();
    void hacerAmigos(int a, int b);
    void registrarSolicitud(int de, int para);
    Mensaje* agregarMensaje(int de, int para, const Fecha& f, const Texto& texto, bool leido);

    // ---- sesion ----
    int buscarUsuario(const Texto& nombre);
    bool iniciarSesion(const Texto& nombre, const Texto& clave);
    void cerrarSesion();
    int usuarioActual() const;

    // ---- amistades ----
    bool sonAmigos(int a, int b) const;
    int cantidadAmigos(int u) const;
    int amigosEnComun(int a, int b) const;
    void listaAmigos(int u, ListaDinamica<int>& destino) const;
    bool enviarSolicitud(int de, int para);
    void solicitudesRecibidas(int u, ListaDinamica<int>& destino) const;
    bool aceptarSolicitud(int quien, int solicitante);
    bool rechazarSolicitud(int quien, int solicitante);
    // amigos de mis amigos ordenados por cuantos amigos tenemos en comun
    void sugerirAmigos(int u, int k, ListaDinamica<int>& destino,
                       ListaDinamica<int>& comunes) const;

    // ---- fotos de perfil de la pantalla principal ----
    void fotosDePerfil(int u, int cantidad, ListaDinamica<int>& grupo,
                       const ListaDinamica<int>& excluir) const;

    // ---- mensajes ----
    bool enviarMensaje(int de, int para, const Fecha& f, const Texto& texto);
    void leerNuevos(int u, ListaDinamica<Mensaje*>& destino);
    void mensajesEntre(int a, int b, const Fecha& desde, const Fecha& hasta,
                       ListaDinamica<Mensaje*>& destino) const;

    // ---- albumes y fotos ----
    void albumesVisibles(int dueno, int quienMira, ListaDinamica<int>& destino) const;
    int darQuitarLike(int quien, int dueno, const Texto& album, const Texto& foto);
    Texto rutaFotoPerfil(int u) const;
    Texto rutaFoto(int u, const Texto& album, const Texto& foto) const;

    long memoriaTotal() const;
};

#endif
