#include <iostream>
#include "Medicion.h"
#include "Texto.h"
#include "Fecha.h"
#include "ListaDinamica.h"
#include "MatrizBits.h"

using namespace std;

// por ahora el main solo prueba las clases de apoyo
int main() {
    Texto nombre("Juan");
    Texto apellido("Gomez");
    Texto completo = nombre + Texto(" ") + apellido;
    cout << "texto: " << completo << "  (largo " << completo.tam() << ")" << endl;
    cout << "ordenando: " << (nombre < apellido ? "Juan va primero" : "Gomez va primero")
         << endl;

    Fecha hoy(11, 10, 2026);
    Fecha antes(1, 3, 2026);
    Fecha mala(31, 2, 2026);
    cout << "fecha " << hoy << " valida: " << (hoy.esValida() ? "si" : "no") << endl;
    cout << "fecha " << mala << " valida: " << (mala.esValida() ? "si" : "no") << endl;
    cout << (antes < hoy ? "marzo es antes que octubre" : "algo quedo mal") << endl;

    ListaDinamica<int> numeros;
    for (int i = 0; i < 10; i++) {
        numeros.agregar(i * i);
    }
    cout << "lista con " << numeros.tam() << " numeros, el quinto es "
         << numeros[4] << endl;
    numeros.eliminar(0);
    cout << "despues de eliminar uno quedan " << numeros.tam() << endl;
    cout << "memoria de la lista: " << numeros.memoriaDinamica() << " bytes" << endl;

    // prueba de la matriz de bits con unas amistades de ejemplo:
    // 0 y 1 son amigos de 5 y de 7, entonces tienen 2 amigos en comun
    Medicion::reiniciar();
    MatrizBits amistad(1000);
    amistad.poner(0, 5);
    amistad.poner(5, 0);
    amistad.poner(0, 7);
    amistad.poner(7, 0);
    amistad.poner(1, 5);
    amistad.poner(5, 1);
    amistad.poner(1, 7);
    amistad.poner(7, 1);

    cout << "0 y 5 son amigos: " << (amistad.obtener(0, 5) ? "si" : "no") << endl;
    cout << "0 y 1 son amigos: " << (amistad.obtener(0, 1) ? "si" : "no") << endl;
    cout << "amigos de 0: " << amistad.contarFila(0) << endl;
    cout << "amigos en comun entre 0 y 1: " << amistad.contarComunes(0, 1) << endl;
    cout << "memoria de la matriz de 1000 x 1000: " << amistad.memoriaDinamica()
         << " bytes" << endl;
    cout << "iteraciones de la prueba de la matriz: " << Medicion::total() << endl;

    return 0;
}
