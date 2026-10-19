#include "Archivos.h"
#include "Medicion.h"
#include <fstream>

Archivos::Archivos(const Texto& c) {
    carpeta = c;
}

const Texto& Archivos::getCarpeta() const {
    return carpeta;
}

int Archivos::siguienteCampo(const char* linea, int desde, char* destino, int maximo) {
    int k = 0;
    while (linea[desde] != '\0' && linea[desde] != '|') {
        if (k < maximo - 1) {
            destino[k] = linea[desde];
            k++;
        }
        desde++;
    }
    destino[k] = '\0';
    if (linea[desde] == '|') {
        desde++;
    }
    return desde;
}

int Archivos::aEntero(const char* texto) {
    int valor = 0;
    int i = 0;
    bool negativo = false;
    if (texto[0] == '-') {
        negativo = true;
        i = 1;
    }
    while (texto[i] >= '0' && texto[i] <= '9') {
        valor = valor * 10 + (texto[i] - '0');
        i++;
    }
    return negativo ? -valor : valor;
}

Fecha Archivos::leerFecha(const char* texto) {
    // viene como dd/mm/aaaa
    int partes[3];
    partes[0] = 0;
    partes[1] = 0;
    partes[2] = 0;
    int cual = 0;
    for (int i = 0; texto[i] != '\0' && cual < 3; i++) {
        if (texto[i] == '/') {
            cual++;
        } else if (texto[i] >= '0' && texto[i] <= '9') {
            partes[cual] = partes[cual] * 10 + (texto[i] - '0');
        }
    }
    return Fecha(partes[0], partes[1], partes[2]);
}

RedSocial* Archivos::cargar() const {
    Texto ruta = carpeta + "/usuarios.txt";
    std::ifstream archivo(ruta.cadena());
    if (!archivo.is_open()) {
        return 0;
    }

    char linea[1024];
    char campo[256];
    archivo.getline(linea, 1024);
    int cuantos = aEntero(linea);
    if (cuantos <= 0) {
        archivo.close();
        return 0;
    }

    RedSocial* red = new RedSocial(cuantos, carpeta);
    int leidos = 0;
    while (leidos < cuantos && archivo.getline(linea, 1024)) {
        Medicion::contar();
        int pos = 0;
        pos = siguienteCampo(linea, pos, campo, 256);
        Texto nombre(campo);
        pos = siguienteCampo(linea, pos, campo, 256);
        Texto clave(campo);
        pos = siguienteCampo(linea, pos, campo, 256);
        Fecha nacimiento = leerFecha(campo);
        pos = siguienteCampo(linea, pos, campo, 256);
        Texto correo(campo);
        pos = siguienteCampo(linea, pos, campo, 256);
        Texto ciudad(campo);
        pos = siguienteCampo(linea, pos, campo, 256);
        Texto pais(campo);
        pos = siguienteCampo(linea, pos, campo, 256);
        Fecha inscripcion = leerFecha(campo);

        red->agregarUsuario(new Usuario(nombre, clave, nacimiento, correo,
                                        ciudad, pais, inscripcion));
        leidos++;
    }
    archivo.close();

    // el indice por nombre se arma una sola vez, aqui
    red->construirIndice();

    cargarAmistades(red);
    cargarSolicitudes(red);
    cargarAlbumes(red);
    cargarFotos(red);
    cargarMensajes(red);
    return red;
}

bool Archivos::cargarAmistades(RedSocial* red) const {
    Texto ruta = carpeta + "/amistades.txt";
    std::ifstream archivo(ruta.cadena());
    if (!archivo.is_open()) {
        return false;
    }
    int a;
    int b;
    while (archivo >> a >> b) {
        Medicion::contar();
        if (a >= 0 && b >= 0 && a < red->cantidadUsuarios() && b < red->cantidadUsuarios()) {
            red->hacerAmigos(a, b);
        }
    }
    archivo.close();
    return true;
}

bool Archivos::cargarSolicitudes(RedSocial* red) const {
    Texto ruta = carpeta + "/solicitudes.txt";
    std::ifstream archivo(ruta.cadena());
    if (!archivo.is_open()) {
        return false;
    }
    int de;
    int para;
    while (archivo >> de >> para) {
        Medicion::contar();
        if (de >= 0 && para >= 0 && de < red->cantidadUsuarios()
            && para < red->cantidadUsuarios()) {
            red->registrarSolicitud(de, para);
        }
    }
    archivo.close();
    return true;
}

bool Archivos::cargarAlbumes(RedSocial* red) const {
    Texto ruta = carpeta + "/albumes.txt";
    std::ifstream archivo(ruta.cadena());
    if (!archivo.is_open()) {
        return false;
    }
    char linea[1024];
    char campo[256];
    while (archivo.getline(linea, 1024)) {
        Medicion::contar();
        int pos = 0;
        pos = siguienteCampo(linea, pos, campo, 256);
        int dueno = aEntero(campo);
        pos = siguienteCampo(linea, pos, campo, 256);
        Texto nombre(campo);
        pos = siguienteCampo(linea, pos, campo, 256);
        int privacidad = aEntero(campo);
        if (dueno >= 0 && dueno < red->cantidadUsuarios()) {
            red->getUsuario(dueno)->agregarAlbum(Album(nombre, privacidad));
        }
    }
    archivo.close();
    return true;
}

bool Archivos::cargarFotos(RedSocial* red) const {
    Texto ruta = carpeta + "/fotos.txt";
    std::ifstream archivo(ruta.cadena());
    if (!archivo.is_open()) {
        return false;
    }
    char linea[1024];
    char campo[256];
    while (archivo.getline(linea, 1024)) {
        Medicion::contar();
        int pos = 0;
        pos = siguienteCampo(linea, pos, campo, 256);
        int dueno = aEntero(campo);
        pos = siguienteCampo(linea, pos, campo, 256);
        Texto album(campo);
        pos = siguienteCampo(linea, pos, campo, 256);
        Texto nombre(campo);
        pos = siguienteCampo(linea, pos, campo, 256);
        Fecha captura = leerFecha(campo);
        pos = siguienteCampo(linea, pos, campo, 256);
        int tamano = aEntero(campo);

        if (dueno < 0 || dueno >= red->cantidadUsuarios()) {
            continue;
        }
        int cual = red->getUsuario(dueno)->buscarAlbum(album);
        if (cual < 0) {
            continue;
        }
        Foto foto(nombre, captura, tamano);
        // los me gusta vienen al final separados por coma
        pos = siguienteCampo(linea, pos, campo, 256);
        int valor = 0;
        bool hayNumero = false;
        for (int i = 0; ; i++) {
            if (campo[i] >= '0' && campo[i] <= '9') {
                valor = valor * 10 + (campo[i] - '0');
                hayNumero = true;
            } else {
                if (hayNumero) {
                    foto.cambiarLike(valor);
                }
                valor = 0;
                hayNumero = false;
                if (campo[i] == '\0') {
                    break;
                }
            }
        }
        red->getUsuario(dueno)->getAlbum(cual).agregarFoto(foto);
    }
    archivo.close();
    return true;
}

bool Archivos::cargarMensajes(RedSocial* red) const {
    Texto ruta = carpeta + "/mensajes.txt";
    std::ifstream archivo(ruta.cadena());
    if (!archivo.is_open()) {
        return false;
    }
    char linea[1024];
    char campo[256];
    while (archivo.getline(linea, 1024)) {
        Medicion::contar();
        int pos = 0;
        pos = siguienteCampo(linea, pos, campo, 256);
        int emisor = aEntero(campo);
        pos = siguienteCampo(linea, pos, campo, 256);
        int receptor = aEntero(campo);
        pos = siguienteCampo(linea, pos, campo, 256);
        Fecha fecha = leerFecha(campo);
        pos = siguienteCampo(linea, pos, campo, 256);
        bool leido = aEntero(campo) == 1;
        // el contenido es lo ultimo que queda de la linea
        Texto contenido(linea + pos);

        if (emisor >= 0 && receptor >= 0 && emisor < red->cantidadUsuarios()
            && receptor < red->cantidadUsuarios() && emisor != receptor) {
            red->agregarMensaje(emisor, receptor, fecha, contenido, leido);
        }
    }
    archivo.close();
    return true;
}

bool Archivos::guardar(const RedSocial* red) const {
    bool bien = guardarUsuarios(red);
    bien = guardarAmistades(red) && bien;
    bien = guardarSolicitudes(red) && bien;
    bien = guardarAlbumes(red) && bien;
    bien = guardarFotos(red) && bien;
    bien = guardarMensajes(red) && bien;
    return bien;
}

bool Archivos::guardarUsuarios(const RedSocial* red) const {
    Texto ruta = carpeta + "/usuarios.txt";
    std::ofstream archivo(ruta.cadena());
    if (!archivo.is_open()) {
        return false;
    }
    archivo << red->cantidadUsuarios() << '\n';
    for (int i = 0; i < red->cantidadUsuarios(); i++) {
        Medicion::contar();
        Usuario* u = red->getUsuario(i);
        archivo << u->getNombre() << '|' << u->getClave() << '|'
                << u->getNacimiento() << '|' << u->getCorreo() << '|'
                << u->getCiudad() << '|' << u->getPais() << '|'
                << u->getInscripcion() << '\n';
    }
    archivo.close();
    return true;
}

bool Archivos::guardarAmistades(const RedSocial* red) const {
    Texto ruta = carpeta + "/amistades.txt";
    std::ofstream archivo(ruta.cadena());
    if (!archivo.is_open()) {
        return false;
    }
    const MatrizBits& amistad = red->getAmistad();
    // solo la mitad de arriba, la pareja se guarda una sola vez
    for (int i = 0; i < red->cantidadUsuarios(); i++) {
        for (int j = i + 1; j < red->cantidadUsuarios(); j++) {
            Medicion::contar();
            if (amistad.obtener(i, j)) {
                archivo << i << ' ' << j << '\n';
            }
        }
    }
    archivo.close();
    return true;
}

bool Archivos::guardarSolicitudes(const RedSocial* red) const {
    Texto ruta = carpeta + "/solicitudes.txt";
    std::ofstream archivo(ruta.cadena());
    if (!archivo.is_open()) {
        return false;
    }
    const MatrizBits& pendientes = red->getSolicitudes();
    for (int i = 0; i < red->cantidadUsuarios(); i++) {
        for (int j = 0; j < red->cantidadUsuarios(); j++) {
            Medicion::contar();
            if (pendientes.obtener(i, j)) {
                archivo << i << ' ' << j << '\n';
            }
        }
    }
    archivo.close();
    return true;
}

bool Archivos::guardarAlbumes(const RedSocial* red) const {
    Texto ruta = carpeta + "/albumes.txt";
    std::ofstream archivo(ruta.cadena());
    if (!archivo.is_open()) {
        return false;
    }
    for (int i = 0; i < red->cantidadUsuarios(); i++) {
        Usuario* u = red->getUsuario(i);
        for (int a = 0; a < u->cantidadAlbumes(); a++) {
            Medicion::contar();
            archivo << i << '|' << u->getAlbum(a).getNombre() << '|'
                    << u->getAlbum(a).getPrivacidad() << '\n';
        }
    }
    archivo.close();
    return true;
}

bool Archivos::guardarFotos(const RedSocial* red) const {
    Texto ruta = carpeta + "/fotos.txt";
    std::ofstream archivo(ruta.cadena());
    if (!archivo.is_open()) {
        return false;
    }
    for (int i = 0; i < red->cantidadUsuarios(); i++) {
        Usuario* u = red->getUsuario(i);
        for (int a = 0; a < u->cantidadAlbumes(); a++) {
            const Album& album = u->getAlbum(a);
            for (int f = 0; f < album.cantidadFotos(); f++) {
                Medicion::contar();
                const Foto& foto = album.getFoto(f);
                archivo << i << '|' << album.getNombre() << '|'
                        << foto.getNombre() << '|' << foto.getCaptura() << '|'
                        << foto.getTamano() << '|';
                for (int k = 0; k < foto.cantidadLikes(); k++) {
                    if (k > 0) {
                        archivo << ',';
                    }
                    archivo << foto.getLike(k);
                }
                archivo << '\n';
            }
        }
    }
    archivo.close();
    return true;
}

bool Archivos::guardarMensajes(const RedSocial* red) const {
    Texto ruta = carpeta + "/mensajes.txt";
    std::ofstream archivo(ruta.cadena());
    if (!archivo.is_open()) {
        return false;
    }
    // se guardan en el mismo orden en que se crearon, que es el cronologico
    for (int i = 0; i < red->cantidadMensajes(); i++) {
        Medicion::contar();
        Mensaje* m = red->getMensaje(i);
        archivo << m->getEmisor() << '|' << m->getReceptor() << '|'
                << m->getFecha() << '|' << (m->getLeido() ? 1 : 0) << '|'
                << m->getContenido() << '\n';
    }
    archivo.close();
    return true;
}
