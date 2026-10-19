#include "Menu.h"
#include "Medicion.h"
#include <iostream>

using namespace std;

Menu::Menu(RedSocial* r) {
    red = r;
}

int Menu::leerEntero(const char* pregunta) {
    char entrada[64];
    cout << pregunta;
    cin.getline(entrada, 64);
    int valor = 0;
    int i = 0;
    bool negativo = false;
    if (entrada[0] == '-') {
        negativo = true;
        i = 1;
    }
    while (entrada[i] >= '0' && entrada[i] <= '9') {
        valor = valor * 10 + (entrada[i] - '0');
        i++;
    }
    return negativo ? -valor : valor;
}

void Menu::leerTexto(const char* pregunta, char* destino, int maximo) {
    cout << pregunta;
    cin.getline(destino, maximo);
}

Fecha Menu::leerFecha(const char* pregunta) {
    cout << pregunta << endl;
    int dia = leerEntero("  dia: ");
    int mes = leerEntero("  mes: ");
    int anio = leerEntero("  anio: ");
    return Fecha(dia, mes, anio);
}

void Menu::mostrarRecursos() const {
    long memoria = red->memoriaTotal() + sizeof(Menu) + grupoFotos.memoriaDinamica();
    cout << "---------------------------------------------" << endl;
    cout << "memoria usada: " << memoria << " bytes" << endl;
    cout << "iteraciones de esta operacion: " << Medicion::total() << endl;
    cout << "---------------------------------------------" << endl;
}

void Menu::iniciarSesion() {
    char nombre[128];
    char clave[128];
    leerTexto("nombre de usuario: ", nombre, 128);
    leerTexto("clave: ", clave, 128);

    Medicion::reiniciar();
    bool entro = red->iniciarSesion(Texto(nombre), Texto(clave));
    if (entro) {
        int yo = red->usuarioActual();
        cout << "bienvenido " << red->getUsuario(yo)->getNombre() << endl;
        cout << "amigos: " << red->cantidadAmigos(yo)
             << "   mensajes sin leer: " << red->getUsuario(yo)->cantidadNoLeidos()
             << endl;
        // al entrar se limpia el grupo anterior de fotos
        while (grupoFotos.tam() > 0) {
            grupoFotos.eliminar(grupoFotos.tam() - 1);
        }
    } else {
        cout << "usuario o clave incorrectos" << endl;
    }
    mostrarRecursos();
}

void Menu::mostrarFotosPerfil() {
    int yo = red->usuarioActual();
    int cuantas = leerEntero("cuantas fotos de perfil quiere ver: ");
    if (cuantas <= 0) {
        return;
    }

    Medicion::reiniciar();
    ListaDinamica<int> nuevos;
    red->fotosDePerfil(yo, cuantas, nuevos, grupoFotos);

    cout << "fotos de perfil sugeridas:" << endl;
    for (int i = 0; i < nuevos.tam(); i++) {
        int quien = nuevos[i];
        cout << "  " << red->getUsuario(quien)->getNombre()
             << "  (amigos en comun: " << red->amigosEnComun(yo, quien) << ")" << endl;
        cout << "    " << red->rutaFotoPerfil(quien) << endl;
    }
    if (nuevos.tam() == 0) {
        cout << "  no quedan usuarios nuevos por mostrar" << endl;
    }

    // se guarda el grupo mostrado para excluirlo la proxima vez
    while (grupoFotos.tam() > 0) {
        grupoFotos.eliminar(grupoFotos.tam() - 1);
    }
    for (int i = 0; i < nuevos.tam(); i++) {
        grupoFotos.agregar(nuevos[i]);
    }
    mostrarRecursos();
}

void Menu::administrarAmistades() {
    int yo = red->usuarioActual();
    cout << endl;
    cout << "1. ver mis amigos" << endl;
    cout << "2. enviar solicitud" << endl;
    cout << "3. ver solicitudes recibidas" << endl;
    cout << "4. aceptar solicitud" << endl;
    cout << "5. rechazar solicitud" << endl;
    int opcion = leerEntero("opcion: ");

    if (opcion == 1) {
        Medicion::reiniciar();
        ListaDinamica<int> amigos;
        red->listaAmigos(yo, amigos);
        cout << "tiene " << amigos.tam() << " amigos" << endl;
        int limite = amigos.tam() < 20 ? amigos.tam() : 20;
        for (int i = 0; i < limite; i++) {
            cout << "  " << red->getUsuario(amigos[i])->getNombre() << endl;
        }
        if (amigos.tam() > limite) {
            cout << "  ... y " << (amigos.tam() - limite) << " mas" << endl;
        }
        mostrarRecursos();
    } else if (opcion == 2) {
        char nombre[128];
        leerTexto("a quien: ", nombre, 128);
        Medicion::reiniciar();
        int otro = red->buscarUsuario(Texto(nombre));
        if (otro < 0) {
            cout << "ese usuario no existe" << endl;
        } else if (red->enviarSolicitud(yo, otro)) {
            cout << "solicitud enviada" << endl;
        } else {
            cout << "no se pudo enviar (ya son amigos o ya hay una solicitud)" << endl;
        }
        mostrarRecursos();
    } else if (opcion == 3) {
        Medicion::reiniciar();
        ListaDinamica<int> pendientes;
        red->solicitudesRecibidas(yo, pendientes);
        cout << "tiene " << pendientes.tam() << " solicitudes" << endl;
        for (int i = 0; i < pendientes.tam(); i++) {
            cout << "  " << red->getUsuario(pendientes[i])->getNombre()
                 << "  (amigos en comun: " << red->amigosEnComun(yo, pendientes[i])
                 << ")" << endl;
        }
        mostrarRecursos();
    } else if (opcion == 4 || opcion == 5) {
        char nombre[128];
        leerTexto("nombre del solicitante: ", nombre, 128);
        Medicion::reiniciar();
        int otro = red->buscarUsuario(Texto(nombre));
        if (otro < 0) {
            cout << "ese usuario no existe" << endl;
        } else if (opcion == 4 && red->aceptarSolicitud(yo, otro)) {
            cout << "ahora son amigos" << endl;
        } else if (opcion == 5 && red->rechazarSolicitud(yo, otro)) {
            cout << "solicitud rechazada" << endl;
        } else {
            cout << "no hay una solicitud de esa persona" << endl;
        }
        mostrarRecursos();
    }
}

void Menu::visitarPerfil() {
    int yo = red->usuarioActual();
    char nombre[128];
    leerTexto("nombre del perfil: ", nombre, 128);

    Medicion::reiniciar();
    int otro = red->buscarUsuario(Texto(nombre));
    if (otro < 0) {
        cout << "ese usuario no existe" << endl;
        mostrarRecursos();
        return;
    }

    Usuario* perfil = red->getUsuario(otro);
    cout << "nombre: " << perfil->getNombre() << endl;
    cout << "ciudad: " << perfil->getCiudad() << ", " << perfil->getPais() << endl;
    cout << "nacimiento: " << perfil->getNacimiento() << endl;
    cout << "inscrito desde: " << perfil->getInscripcion() << endl;
    cout << "amigos: " << red->cantidadAmigos(otro) << endl;
    if (otro != yo) {
        cout << "amigos en comun: " << red->amigosEnComun(yo, otro) << endl;
        cout << (red->sonAmigos(yo, otro) ? "son amigos" : "no son amigos") << endl;
    }

    ListaDinamica<int> visibles;
    red->albumesVisibles(otro, yo, visibles);
    cout << "albumes que puede ver: " << visibles.tam() << endl;
    for (int i = 0; i < visibles.tam(); i++) {
        const Album& album = perfil->getAlbum(visibles[i]);
        cout << "  album: " << album.getNombre()
             << "  (fotos: " << album.cantidadFotos() << ")" << endl;
        for (int f = 0; f < album.cantidadFotos(); f++) {
            const Foto& foto = album.getFoto(f);
            cout << "    " << foto.getNombre() << "  " << foto.getCaptura()
                 << "  " << foto.getTamano() << " kb  me gusta: "
                 << foto.cantidadLikes();
            if (foto.tieneLike(yo)) {
                cout << "  (usted ya dio me gusta)";
            }
            cout << endl;
        }
    }
    mostrarRecursos();

    int poner = leerEntero("desea dar o quitar un me gusta? 1 si, 0 no: ");
    if (poner != 1) {
        return;
    }
    char nombreAlbum[128];
    char nombreFoto[128];
    leerTexto("nombre del album: ", nombreAlbum, 128);
    leerTexto("nombre de la foto: ", nombreFoto, 128);

    Medicion::reiniciar();
    int resultado = red->darQuitarLike(yo, otro, Texto(nombreAlbum), Texto(nombreFoto));
    if (resultado < 0) {
        cout << "no se encontro esa foto" << endl;
    } else if (resultado == 1) {
        cout << "me gusta puesto" << endl;
    } else {
        cout << "me gusta quitado" << endl;
    }
    mostrarRecursos();
}

void Menu::verSugerencias() {
    int yo = red->usuarioActual();
    int cuantos = leerEntero("cuantas sugerencias quiere (maximo 20): ");
    if (cuantos <= 0) {
        return;
    }
    if (cuantos > 20) {
        cuantos = 20;
    }

    Medicion::reiniciar();
    ListaDinamica<int> sugeridos;
    ListaDinamica<int> comunes;
    red->sugerirAmigos(yo, cuantos, sugeridos, comunes);

    cout << "amigos de sus amigos:" << endl;
    for (int i = 0; i < sugeridos.tam(); i++) {
        cout << "  " << red->getUsuario(sugeridos[i])->getNombre()
             << "  (amigos en comun: " << comunes[i] << ")" << endl;
    }
    if (sugeridos.tam() == 0) {
        cout << "  no hay sugerencias" << endl;
    }
    mostrarRecursos();
}

void Menu::administrarMensajes() {
    int yo = red->usuarioActual();
    cout << endl;
    cout << "1. enviar mensaje" << endl;
    cout << "2. leer los nuevos" << endl;
    cout << "3. ver conversacion entre dos fechas" << endl;
    int opcion = leerEntero("opcion: ");

    if (opcion == 1) {
        char nombre[128];
        char contenido[512];
        leerTexto("para quien: ", nombre, 128);
        Fecha fecha = leerFecha("fecha del mensaje:");
        leerTexto("mensaje: ", contenido, 512);

        Medicion::reiniciar();
        int otro = red->buscarUsuario(Texto(nombre));
        if (otro < 0) {
            cout << "ese usuario no existe" << endl;
        } else if (red->enviarMensaje(yo, otro, fecha, Texto(contenido))) {
            cout << "mensaje enviado" << endl;
        } else {
            cout << "no se pudo enviar: solo se escribe a los amigos y la fecha"
                 << " no puede ser anterior al ultimo mensaje" << endl;
        }
        mostrarRecursos();
    } else if (opcion == 2) {
        Medicion::reiniciar();
        ListaDinamica<Mensaje*> nuevos;
        red->leerNuevos(yo, nuevos);
        cout << "mensajes nuevos: " << nuevos.tam() << endl;
        for (int i = 0; i < nuevos.tam(); i++) {
            cout << "  de " << red->getUsuario(nuevos[i]->getEmisor())->getNombre()
                 << "  " << nuevos[i]->getFecha() << endl;
            cout << "    " << nuevos[i]->getContenido() << endl;
        }
        mostrarRecursos();
    } else if (opcion == 3) {
        char nombre[128];
        leerTexto("con quien: ", nombre, 128);
        Fecha desde = leerFecha("desde:");
        Fecha hasta = leerFecha("hasta:");

        Medicion::reiniciar();
        int otro = red->buscarUsuario(Texto(nombre));
        if (otro < 0) {
            cout << "ese usuario no existe" << endl;
            mostrarRecursos();
            return;
        }
        ListaDinamica<Mensaje*> conversacion;
        red->mensajesEntre(yo, otro, desde, hasta, conversacion);
        cout << "mensajes encontrados: " << conversacion.tam() << endl;
        for (int i = 0; i < conversacion.tam(); i++) {
            Mensaje* m = conversacion[i];
            cout << "  " << m->getFecha() << "  "
                 << (m->getEmisor() == yo ? "yo" : "el otro") << ": "
                 << m->getContenido() << endl;
        }
        mostrarRecursos();
    }
}

void Menu::ejecutar() {
    int opcion = -1;
    while (opcion != 0) {
        int yo = red->usuarioActual();
        cout << endl;
        cout << "=============== UdeABook ===============" << endl;
        if (yo < 0) {
            cout << "sin sesion" << endl;
            cout << "1. iniciar sesion" << endl;
            cout << "0. salir" << endl;
        } else {
            cout << "sesion de " << red->getUsuario(yo)->getNombre() << endl;
            cout << "1. fotos de perfil" << endl;
            cout << "2. amistades" << endl;
            cout << "3. visitar un perfil" << endl;
            cout << "4. sugerencias de amigos" << endl;
            cout << "5. mensajes" << endl;
            cout << "6. cerrar sesion" << endl;
            cout << "0. salir" << endl;
        }
        opcion = leerEntero("opcion: ");

        if (yo < 0) {
            if (opcion == 1) {
                iniciarSesion();
            }
        } else if (opcion == 1) {
            mostrarFotosPerfil();
        } else if (opcion == 2) {
            administrarAmistades();
        } else if (opcion == 3) {
            visitarPerfil();
        } else if (opcion == 4) {
            verSugerencias();
        } else if (opcion == 5) {
            administrarMensajes();
        } else if (opcion == 6) {
            red->cerrarSesion();
            cout << "sesion cerrada" << endl;
        }
    }
}
