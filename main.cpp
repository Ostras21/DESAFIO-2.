#include <iostream>
#include <cstdlib>
#include "Texto.h"
#include "Medicion.h"
#include "Archivos.h"
#include "RedSocial.h"
#include "Menu.h"

using namespace std;

int main() {
    // semilla fija para que el sorteo de las fotos se pueda repetir en las pruebas
    srand(20262);

    // qt creator no corre el programa desde la carpeta del proyecto sino desde la
    // de compilacion, asi que se prueban varias rutas hasta dar con la carpeta
    const char* rutas[5] = {
        "datos",
        "../datos",
        "../../datos",
        "../../../datos",
        "../../../../datos"
    };

    Texto carpeta;
    RedSocial* red = 0;
    Medicion::reiniciar();
    for (int i = 0; i < 5 && red == 0; i++) {
        carpeta = Texto(rutas[i]);
        Archivos buscador(carpeta);
        red = buscador.cargar();
    }

    if (red == 0) {
        cout << "no se encontro la carpeta datos" << endl;
        cout << "debe estar al lado del archivo UdeABook.pro" << endl;
        return 1;
    }

    Archivos archivos(carpeta);
    cout << "datos leidos de la carpeta " << carpeta << endl;
    cout << "usuarios cargados: " << red->cantidadUsuarios() << endl;
    cout << "mensajes cargados: " << red->cantidadMensajes() << endl;
    cout << "memoria usada: " << red->memoriaTotal() << " bytes" << endl;
    cout << "iteraciones de la carga: " << Medicion::total() << endl;

    Menu menu(red);
    menu.ejecutar();

    if (archivos.guardar(red)) {
        cout << "datos guardados" << endl;
    } else {
        cout << "no se pudieron guardar los datos" << endl;
    }

    delete red;
    return 0;
}
