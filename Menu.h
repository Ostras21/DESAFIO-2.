#ifndef MENU_H
#define MENU_H

#include "RedSocial.h"
#include "ListaDinamica.h"

// se encarga de hablar con el usuario por consola.
// en grupoFotos queda el ultimo grupo de fotos de perfil que se mostro, para
// que al actualizar no vuelvan a salir las mismas
class Menu {
private:
    RedSocial* red;
    ListaDinamica<int> grupoFotos;

    static int leerEntero(const char* pregunta);
    static void leerTexto(const char* pregunta, char* destino, int maximo);
    static Fecha leerFecha(const char* pregunta);

    void mostrarRecursos() const;
    void iniciarSesion();
    void mostrarFotosPerfil();
    void administrarAmistades();
    void visitarPerfil();
    void verSugerencias();
    void administrarMensajes();

public:
    Menu(RedSocial* r);

    void ejecutar();
};

#endif
