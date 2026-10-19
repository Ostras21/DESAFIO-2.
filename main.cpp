#include <iostream>
#include <cstdlib>
#include "Medicion.h"
#include "Texto.h"
#include "Fecha.h"
#include "RedSocial.h"

using namespace std;

// arma un usuario rapido para la prueba
Usuario* crearUsuario(const char* nombre, const char* clave, const char* ciudad) {
    return new Usuario(Texto(nombre), Texto(clave), Fecha(4, 7, 2001),
                       Texto("correo@correo.com"), Texto(ciudad), Texto("Colombia"),
                       Fecha(15, 2, 2022));
}

// todavia no se leen los archivos, la red se arma aqui mismo para probarla
int main() {
    srand(20262);

    RedSocial red(6, Texto("datos"));
    red.agregarUsuario(crearUsuario("ana", "clave1", "Medellin"));
    red.agregarUsuario(crearUsuario("bruno", "clave2", "Bogota"));
    red.agregarUsuario(crearUsuario("carla", "clave3", "Cali"));
    red.agregarUsuario(crearUsuario("diego", "clave4", "Pereira"));
    red.agregarUsuario(crearUsuario("elena", "clave5", "Manizales"));
    red.agregarUsuario(crearUsuario("felipe", "clave6", "Envigado"));
    red.construirIndice();

    // ana(0) bruno(1) carla(2) diego(3) elena(4) felipe(5)
    red.hacerAmigos(0, 1);
    red.hacerAmigos(0, 2);
    red.hacerAmigos(1, 3);
    red.hacerAmigos(2, 3);
    red.hacerAmigos(3, 4);
    red.hacerAmigos(1, 5);

    // un album publico y uno privado para probar la privacidad
    Album paseo(Texto("paseo"), 0);
    paseo.agregarFoto(Foto(Texto("foto1"), Fecha(3, 5, 2026), 1200));
    red.getUsuario(2)->agregarAlbum(paseo);
    red.getUsuario(2)->agregarAlbum(Album(Texto("personal"), 2));

    // ----- entrar con el nombre y la clave -----
    Medicion::reiniciar();
    bool entro = red.iniciarSesion(Texto("ana"), Texto("clave1"));
    cout << "inicio de sesion de ana: " << (entro ? "si" : "no") << endl;
    cout << "  iteraciones de la busqueda binaria: " << Medicion::total() << endl;
    int yo = red.usuarioActual();

    cout << "login con clave mala: "
         << (red.iniciarSesion(Texto("bruno"), Texto("equivocada")) ? "si" : "no") << endl;

    // ----- amistades -----
    cout << endl << "ana tiene " << red.cantidadAmigos(yo) << " amigos" << endl;
    ListaDinamica<int> amigos;
    red.listaAmigos(yo, amigos);
    for (int i = 0; i < amigos.tam(); i++) {
        cout << "  " << red.getUsuario(amigos[i])->getNombre() << endl;
    }
    cout << "ana y diego son amigos: " << (red.sonAmigos(0, 3) ? "si" : "no") << endl;
    cout << "amigos en comun entre ana y diego: " << red.amigosEnComun(0, 3) << endl;

    // ----- solicitudes -----
    cout << endl << "ana le manda solicitud a diego: "
         << (red.enviarSolicitud(0, 3) ? "enviada" : "no se pudo") << endl;
    cout << "ana se la manda otra vez: "
         << (red.enviarSolicitud(0, 3) ? "enviada" : "no se pudo") << endl;
    ListaDinamica<int> pendientes;
    red.solicitudesRecibidas(3, pendientes);
    cout << "diego tiene " << pendientes.tam() << " solicitudes" << endl;
    cout << "diego la acepta: " << (red.aceptarSolicitud(3, 0) ? "si" : "no") << endl;
    cout << "ahora ana y diego son amigos: " << (red.sonAmigos(0, 3) ? "si" : "no") << endl;

    // ----- sugerencias -----
    cout << endl << "amigos de los amigos de bruno:" << endl;
    ListaDinamica<int> sugeridos;
    ListaDinamica<int> comunes;
    red.sugerirAmigos(1, 5, sugeridos, comunes);
    for (int i = 0; i < sugeridos.tam(); i++) {
        cout << "  " << red.getUsuario(sugeridos[i])->getNombre()
             << " (amigos en comun: " << comunes[i] << ")" << endl;
    }

    // ----- mensajes -----
    cout << endl;
    cout << "ana le escribe a bruno: "
         << (red.enviarMensaje(0, 1, Fecha(2, 3, 2026), Texto("hola bruno")) ? "si" : "no")
         << endl;
    red.enviarMensaje(1, 0, Fecha(4, 3, 2026), Texto("hola ana, todo bien"));
    red.enviarMensaje(0, 1, Fecha(20, 6, 2026), Texto("nos vemos el lunes"));
    cout << "ana le escribe a elena, que no es su amiga: "
         << (red.enviarMensaje(0, 4, Fecha(5, 3, 2026), Texto("hola")) ? "si" : "no")
         << endl;

    cout << "bruno tiene " << red.getUsuario(1)->cantidadNoLeidos()
         << " mensajes sin leer" << endl;
    ListaDinamica<Mensaje*> nuevos;
    red.leerNuevos(1, nuevos);
    for (int i = 0; i < nuevos.tam(); i++) {
        cout << "  " << nuevos[i]->getFecha() << ": " << nuevos[i]->getContenido() << endl;
    }
    cout << "despues de leerlos le quedan " << red.getUsuario(1)->cantidadNoLeidos() << endl;

    // la conversacion de un rango de fechas no recorre todos los mensajes,
    // busca el primero del rango y de ahi arranca
    Medicion::reiniciar();
    ListaDinamica<Mensaje*> conversacion;
    red.mensajesEntre(0, 1, Fecha(1, 1, 2026), Fecha(31, 5, 2026), conversacion);
    cout << endl << "mensajes entre ana y bruno de enero a mayo: "
         << conversacion.tam() << endl;
    for (int i = 0; i < conversacion.tam(); i++) {
        cout << "  " << conversacion[i]->getFecha() << ": "
             << conversacion[i]->getContenido() << endl;
    }
    cout << "  iteraciones: " << Medicion::total() << endl;

    // ----- albumes y me gusta -----
    cout << endl << "albumes de carla que puede ver ana:" << endl;
    ListaDinamica<int> visibles;
    red.albumesVisibles(2, 0, visibles);
    for (int i = 0; i < visibles.tam(); i++) {
        cout << "  " << red.getUsuario(2)->getAlbum(visibles[i]).getNombre() << endl;
    }
    cout << "ana le da me gusta a la foto1: "
         << red.darQuitarLike(0, 2, Texto("paseo"), Texto("foto1")) << endl;
    cout << "ana se lo quita: "
         << red.darQuitarLike(0, 2, Texto("paseo"), Texto("foto1")) << endl;

    // ----- fotos de perfil de la pantalla principal -----
    cout << endl << "fotos de perfil sugeridas para ana:" << endl;
    ListaDinamica<int> grupo;
    ListaDinamica<int> ninguno;
    red.fotosDePerfil(0, 3, grupo, ninguno);
    for (int i = 0; i < grupo.tam(); i++) {
        cout << "  " << red.rutaFotoPerfil(grupo[i]) << endl;
    }

    cout << endl << "memoria de toda la red: " << red.memoriaTotal() << " bytes" << endl;
    return 0;
}
