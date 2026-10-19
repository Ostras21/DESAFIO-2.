#include "Usuario.h"
#include "Medicion.h"

Usuario::Usuario() {
    primerNoLeido = 0;
}

Usuario::Usuario(const Texto& nom, const Texto& cl, const Fecha& nac,
                 const Texto& cor, const Texto& ciu, const Texto& pa,
                 const Fecha& ins) {
    nombre = nom;
    clave = cl;
    nacimiento = nac;
    correo = cor;
    ciudad = ciu;
    pais = pa;
    inscripcion = ins;
    primerNoLeido = 0;
}

const Texto& Usuario::getNombre() const {
    return nombre;
}

const Texto& Usuario::getClave() const {
    return clave;
}

const Fecha& Usuario::getNacimiento() const {
    return nacimiento;
}

const Texto& Usuario::getCorreo() const {
    return correo;
}

const Texto& Usuario::getCiudad() const {
    return ciudad;
}

const Texto& Usuario::getPais() const {
    return pais;
}

const Fecha& Usuario::getInscripcion() const {
    return inscripcion;
}

void Usuario::setCorreo(const Texto& c) {
    correo = c;
}

void Usuario::setCiudad(const Texto& c) {
    ciudad = c;
}

void Usuario::setPais(const Texto& p) {
    pais = p;
}

bool Usuario::claveCorrecta(const Texto& intento) const {
    return clave == intento;
}

int Usuario::cantidadAlbumes() const {
    return albumes.tam();
}

Album& Usuario::getAlbum(int i) {
    return albumes[i];
}

const Album& Usuario::getAlbum(int i) const {
    return albumes[i];
}

void Usuario::agregarAlbum(const Album& a) {
    albumes.agregar(a);
}

int Usuario::buscarAlbum(const Texto& nom) const {
    for (int i = 0; i < albumes.tam(); i++) {
        Medicion::contar();
        if (albumes[i].getNombre() == nom) {
            return i;
        }
    }
    return -1;
}

void Usuario::recibir(Mensaje* m) {
    recibidos.agregar(m);
}

void Usuario::enviar(Mensaje* m) {
    enviados.agregar(m);
}

int Usuario::cantidadRecibidos() const {
    return recibidos.tam();
}

int Usuario::cantidadEnviados() const {
    return enviados.tam();
}

Mensaje* Usuario::getRecibido(int i) const {
    return recibidos[i];
}

Mensaje* Usuario::getEnviado(int i) const {
    return enviados[i];
}

const ListaDinamica<Mensaje*>& Usuario::getRecibidos() const {
    return recibidos;
}

const ListaDinamica<Mensaje*>& Usuario::getEnviados() const {
    return enviados;
}

int Usuario::getPrimerNoLeido() const {
    return primerNoLeido;
}

void Usuario::setPrimerNoLeido(int posicion) {
    primerNoLeido = posicion;
}

int Usuario::cantidadNoLeidos() const {
    return recibidos.tam() - primerNoLeido;
}

void Usuario::marcarTodoLeido() {
    for (int i = primerNoLeido; i < recibidos.tam(); i++) {
        Medicion::contar();
        recibidos[i]->setLeido(true);
    }
    primerNoLeido = recibidos.tam();
}

long Usuario::memoriaDinamica() const {
    long total = nombre.memoriaDinamica() + clave.memoriaDinamica()
               + correo.memoriaDinamica() + ciudad.memoriaDinamica()
               + pais.memoriaDinamica();
    total += albumes.memoriaDinamica();
    for (int i = 0; i < albumes.tam(); i++) {
        total += albumes[i].memoriaDinamica();
    }
    // solo los arreglos de punteros, los mensajes los cuenta la red social
    total += recibidos.memoriaDinamica() + enviados.memoriaDinamica();
    return total;
}
