#include "RedSocial.h"
#include "Medicion.h"
#include <cstdlib>

RedSocial::RedSocial(int cantidadUsuarios, const Texto& ruta)
    : amistad(cantidadUsuarios), solicitudes(cantidadUsuarios) {
    // el indice se reserva de una vez del tamaño que va a necesitar
    capacidadIndice = cantidadUsuarios > 0 ? cantidadUsuarios : 1;
    indicePorNombre = new int[capacidadIndice];
    tamIndice = 0;
    indiceListo = false;
    rutaBase = ruta;
    sesion = -1;
}

RedSocial::~RedSocial() {
    for (int i = 0; i < usuarios.tam(); i++) {
        delete usuarios[i];
    }
    for (int i = 0; i < mensajes.tam(); i++) {
        delete mensajes[i];
    }
    delete[] indicePorNombre;
}

int RedSocial::cantidadUsuarios() const {
    return usuarios.tam();
}

Usuario* RedSocial::getUsuario(int i) const {
    return usuarios[i];
}

const Texto& RedSocial::getRutaBase() const {
    return rutaBase;
}

const MatrizBits& RedSocial::getAmistad() const {
    return amistad;
}

const MatrizBits& RedSocial::getSolicitudes() const {
    return solicitudes;
}

int RedSocial::cantidadMensajes() const {
    return mensajes.tam();
}

Mensaje* RedSocial::getMensaje(int i) const {
    return mensajes[i];
}

void RedSocial::agregarUsuario(Usuario* u) {
    usuarios.agregar(u);
    indiceListo = false;
}

void RedSocial::hacerAmigos(int a, int b) {
    amistad.poner(a, b);
    amistad.poner(b, a);
}

void RedSocial::registrarSolicitud(int de, int para) {
    solicitudes.poner(de, para);
}

Mensaje* RedSocial::agregarMensaje(int de, int para, const Fecha& f,
                                   const Texto& texto, bool leido) {
    Mensaje* m = new Mensaje(de, para, f, texto);
    m->setLeido(leido);
    mensajes.agregar(m);
    usuarios[de]->enviar(m);
    usuarios[para]->recibir(m);
    if (leido) {
        // si ya venia leido, el primer no leido se corre una posicion
        usuarios[para]->setPrimerNoLeido(usuarios[para]->cantidadRecibidos());
    }
    return m;
}

// ordena el indice con mezcla: parte el arreglo en dos, ordena cada mitad y
// luego las une comparando nombres
void RedSocial::ordenarIndice(int* arreglo, int inicio, int fin, int* apoyo) {
    if (inicio >= fin) {
        return;
    }
    int medio = (inicio + fin) / 2;
    ordenarIndice(arreglo, inicio, medio, apoyo);
    ordenarIndice(arreglo, medio + 1, fin, apoyo);

    int i = inicio;
    int j = medio + 1;
    int k = inicio;
    while (i <= medio && j <= fin) {
        Medicion::contar();
        if (usuarios[arreglo[j]]->getNombre() < usuarios[arreglo[i]]->getNombre()) {
            apoyo[k] = arreglo[j];
            j++;
        } else {
            apoyo[k] = arreglo[i];
            i++;
        }
        k++;
    }
    while (i <= medio) {
        Medicion::contar();
        apoyo[k] = arreglo[i];
        i++;
        k++;
    }
    while (j <= fin) {
        Medicion::contar();
        apoyo[k] = arreglo[j];
        j++;
        k++;
    }
    for (int p = inicio; p <= fin; p++) {
        Medicion::contar();
        arreglo[p] = apoyo[p];
    }
}

void RedSocial::construirIndice() {
    if (usuarios.tam() > capacidadIndice) {
        delete[] indicePorNombre;
        capacidadIndice = usuarios.tam();
        indicePorNombre = new int[capacidadIndice];
    }
    tamIndice = usuarios.tam();
    for (int i = 0; i < tamIndice; i++) {
        Medicion::contar();
        indicePorNombre[i] = i;
    }
    if (tamIndice > 1) {
        int* apoyo = new int[tamIndice];
        ordenarIndice(indicePorNombre, 0, tamIndice - 1, apoyo);
        delete[] apoyo;
    }
    indiceListo = true;
}

int RedSocial::buscarUsuario(const Texto& nombre) {
    if (!indiceListo) {
        construirIndice();
    }
    int inicio = 0;
    int fin = tamIndice - 1;
    while (inicio <= fin) {
        Medicion::contar();
        int medio = (inicio + fin) / 2;
        int cual = indicePorNombre[medio];
        if (usuarios[cual]->getNombre() == nombre) {
            return cual;
        }
        if (usuarios[cual]->getNombre() < nombre) {
            inicio = medio + 1;
        } else {
            fin = medio - 1;
        }
    }
    return -1;
}

bool RedSocial::iniciarSesion(const Texto& nombre, const Texto& clave) {
    int cual = buscarUsuario(nombre);
    if (cual < 0) {
        return false;
    }
    if (!usuarios[cual]->claveCorrecta(clave)) {
        return false;
    }
    sesion = cual;
    return true;
}

void RedSocial::cerrarSesion() {
    sesion = -1;
}

int RedSocial::usuarioActual() const {
    return sesion;
}

bool RedSocial::sonAmigos(int a, int b) const {
    return amistad.obtener(a, b);
}

int RedSocial::cantidadAmigos(int u) const {
    return amistad.contarFila(u);
}

int RedSocial::amigosEnComun(int a, int b) const {
    return amistad.contarComunes(a, b);
}

void RedSocial::listaAmigos(int u, ListaDinamica<int>& destino) const {
    for (int j = 0; j < usuarios.tam(); j++) {
        Medicion::contar();
        if (amistad.obtener(u, j)) {
            destino.agregar(j);
        }
    }
}

bool RedSocial::enviarSolicitud(int de, int para) {
    if (de == para || de < 0 || para < 0) {
        return false;
    }
    if (amistad.obtener(de, para)) {
        return false;
    }
    if (solicitudes.obtener(de, para) || solicitudes.obtener(para, de)) {
        return false;
    }
    solicitudes.poner(de, para);
    return true;
}

void RedSocial::solicitudesRecibidas(int u, ListaDinamica<int>& destino) const {
    for (int j = 0; j < usuarios.tam(); j++) {
        Medicion::contar();
        if (solicitudes.obtener(j, u)) {
            destino.agregar(j);
        }
    }
}

bool RedSocial::aceptarSolicitud(int quien, int solicitante) {
    if (!solicitudes.obtener(solicitante, quien)) {
        return false;
    }
    solicitudes.quitar(solicitante, quien);
    hacerAmigos(quien, solicitante);
    return true;
}

bool RedSocial::rechazarSolicitud(int quien, int solicitante) {
    if (!solicitudes.obtener(solicitante, quien)) {
        return false;
    }
    solicitudes.quitar(solicitante, quien);
    return true;
}

void RedSocial::sugerirAmigos(int u, int k, ListaDinamica<int>& destino,
                              ListaDinamica<int>& comunes) const {
    for (int j = 0; j < usuarios.tam(); j++) {
        Medicion::contar();
        if (j == u || amistad.obtener(u, j)) {
            continue;
        }
        // si tenemos al menos un amigo en comun, entonces es amigo de un amigo
        int c = amistad.contarComunes(u, j);
        if (c == 0) {
            continue;
        }
        if (destino.tam() < k) {
            destino.agregar(j);
            comunes.agregar(c);
        } else if (c > comunes[destino.tam() - 1]) {
            // reemplaza al peor de los que ya estaban
            destino[destino.tam() - 1] = j;
            comunes[destino.tam() - 1] = c;
        } else {
            continue;
        }
        // lo sube hasta su puesto
        int p = destino.tam() - 1;
        while (p > 0 && comunes[p - 1] < comunes[p]) {
            Medicion::contar();
            int guardaIndice = destino[p - 1];
            int guardaComun = comunes[p - 1];
            destino[p - 1] = destino[p];
            comunes[p - 1] = comunes[p];
            destino[p] = guardaIndice;
            comunes[p] = guardaComun;
            p--;
        }
    }
}

int RedSocial::pesoPorComunes(int comunes) {
    // las prioridades 1, 2.5 y 3.5 se dejan en enteros 2, 5 y 7 para no
    // trabajar con decimales en el sorteo
    if (comunes <= 10) {
        return 2;
    }
    if (comunes <= 20) {
        return 5;
    }
    return 7;
}

void RedSocial::fotosDePerfil(int u, int cantidad, ListaDinamica<int>& grupo,
                              const ListaDinamica<int>& excluir) const {
    ListaDinamica<int> candidatos;
    ListaDinamica<int> pesos;
    int suma = 0;

    for (int j = 0; j < usuarios.tam(); j++) {
        Medicion::contar();
        if (j == u) {
            continue;
        }
        bool repetido = false;
        for (int e = 0; e < excluir.tam(); e++) {
            Medicion::contar();
            if (excluir[e] == j) {
                repetido = true;
                break;
            }
        }
        if (repetido) {
            continue;
        }
        int p = pesoPorComunes(amistad.contarComunes(u, j));
        candidatos.agregar(j);
        pesos.agregar(p);
        suma += p;
    }

    // se tira un numero entre 0 y la suma de los pesos y se avanza hasta
    // pasarlo, asi el que mas peso tiene sale mas veces
    for (int t = 0; t < cantidad && suma > 0; t++) {
        int tiro = rand() % suma;
        int acumulado = 0;
        int elegido = -1;
        for (int i = 0; i < candidatos.tam(); i++) {
            Medicion::contar();
            acumulado += pesos[i];
            if (tiro < acumulado) {
                elegido = i;
                break;
            }
        }
        if (elegido < 0) {
            break;
        }
        grupo.agregar(candidatos[elegido]);
        suma -= pesos[elegido];
        pesos[elegido] = 0;
    }
}

bool RedSocial::enviarMensaje(int de, int para, const Fecha& f, const Texto& texto) {
    if (de < 0 || para < 0 || de == para) {
        return false;
    }
    if (!amistad.obtener(de, para)) {
        return false;
    }
    if (!f.esValida()) {
        return false;
    }
    // las listas tienen que quedar en orden para poder buscar por fecha,
    // entonces no se acepta un mensaje con fecha anterior al ultimo
    Usuario* emisor = usuarios[de];
    Usuario* receptor = usuarios[para];
    if (emisor->cantidadEnviados() > 0 &&
        f < emisor->getEnviado(emisor->cantidadEnviados() - 1)->getFecha()) {
        return false;
    }
    if (receptor->cantidadRecibidos() > 0 &&
        f < receptor->getRecibido(receptor->cantidadRecibidos() - 1)->getFecha()) {
        return false;
    }
    agregarMensaje(de, para, f, texto, false);
    return true;
}

void RedSocial::leerNuevos(int u, ListaDinamica<Mensaje*>& destino) {
    Usuario* dueno = usuarios[u];
    for (int i = dueno->getPrimerNoLeido(); i < dueno->cantidadRecibidos(); i++) {
        Medicion::contar();
        destino.agregar(dueno->getRecibido(i));
    }
    dueno->marcarTodoLeido();
}

int RedSocial::primeroDesde(const ListaDinamica<Mensaje*>& lista, const Fecha& desde) {
    int inicio = 0;
    int fin = lista.tam();
    while (inicio < fin) {
        Medicion::contar();
        int medio = (inicio + fin) / 2;
        if (lista[medio]->getFecha() < desde) {
            inicio = medio + 1;
        } else {
            fin = medio;
        }
    }
    return inicio;
}

void RedSocial::mensajesEntre(int a, int b, const Fecha& desde, const Fecha& hasta,
                              ListaDinamica<Mensaje*>& destino) const {
    const ListaDinamica<Mensaje*>& recibidos = usuarios[a]->getRecibidos();
    const ListaDinamica<Mensaje*>& enviados = usuarios[a]->getEnviados();

    // en vez de recorrer todo se busca donde empieza el rango en cada lista
    int i = primeroDesde(recibidos, desde);
    int j = primeroDesde(enviados, desde);

    while (true) {
        while (i < recibidos.tam() && recibidos[i]->getFecha() <= hasta
               && recibidos[i]->getEmisor() != b) {
            Medicion::contar();
            i++;
        }
        while (j < enviados.tam() && enviados[j]->getFecha() <= hasta
               && enviados[j]->getReceptor() != b) {
            Medicion::contar();
            j++;
        }
        bool hayRecibido = i < recibidos.tam() && recibidos[i]->getFecha() <= hasta;
        bool hayEnviado = j < enviados.tam() && enviados[j]->getFecha() <= hasta;
        if (!hayRecibido && !hayEnviado) {
            break;
        }
        // de los dos candidatos se toma el mas viejo para que quede en orden
        if (hayRecibido && (!hayEnviado
            || recibidos[i]->getFecha() <= enviados[j]->getFecha())) {
            destino.agregar(recibidos[i]);
            i++;
        } else {
            destino.agregar(enviados[j]);
            j++;
        }
    }
}

void RedSocial::albumesVisibles(int dueno, int quienMira,
                                ListaDinamica<int>& destino) const {
    bool amigos = (dueno == quienMira) || amistad.obtener(dueno, quienMira);
    Usuario* u = usuarios[dueno];
    for (int i = 0; i < u->cantidadAlbumes(); i++) {
        Medicion::contar();
        int privacidad = u->getAlbum(i).getPrivacidad();
        if (privacidad == 0) {
            destino.agregar(i);
        } else if (privacidad == 1 && amigos) {
            destino.agregar(i);
        } else if (privacidad == 2 && dueno == quienMira) {
            destino.agregar(i);
        }
    }
}

int RedSocial::darQuitarLike(int quien, int dueno, const Texto& album,
                             const Texto& foto) {
    int cualAlbum = usuarios[dueno]->buscarAlbum(album);
    if (cualAlbum < 0) {
        return -1;
    }
    Album& elAlbum = usuarios[dueno]->getAlbum(cualAlbum);
    int cualFoto = elAlbum.buscarFoto(foto);
    if (cualFoto < 0) {
        return -1;
    }
    return elAlbum.getFoto(cualFoto).cambiarLike(quien) ? 1 : 0;
}

Texto RedSocial::rutaFotoPerfil(int u) const {
    // la ruta no se guarda en memoria, se arma con el nombre cuando se pide
    return rutaBase + "/" + usuarios[u]->getNombre() + "/albums/profile.png";
}

Texto RedSocial::rutaFoto(int u, const Texto& album, const Texto& foto) const {
    return rutaBase + "/" + usuarios[u]->getNombre() + "/albums/" + album
           + "/" + foto + ".png";
}

long RedSocial::memoriaTotal() const {
    long total = sizeof(RedSocial);
    total += rutaBase.memoriaDinamica();
    total += amistad.memoriaDinamica() + solicitudes.memoriaDinamica();
    total += (long)tamIndice * (long)sizeof(int);

    total += usuarios.memoriaDinamica();
    for (int i = 0; i < usuarios.tam(); i++) {
        total += sizeof(Usuario) + usuarios[i]->memoriaDinamica();
    }

    total += mensajes.memoriaDinamica();
    for (int i = 0; i < mensajes.tam(); i++) {
        total += sizeof(Mensaje) + mensajes[i]->memoriaDinamica();
    }
    return total;
}
