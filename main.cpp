#include <iostream>
#include "Medicion.h"
#include "Texto.h"
#include "Fecha.h"
#include "ListaDinamica.h"
#include "MatrizBits.h"
#include "Mensaje.h"
#include "Foto.h"
#include "Album.h"

using namespace std;

// el main sigue siendo de prueba, va creciendo con cada clase que se agrega
int main() {
    // ----- clases de apoyo -----
    Texto completo = Texto("Juan") + Texto(" ") + Texto("Gomez");
    cout << "texto: " << completo << "  (largo " << completo.tam() << ")" << endl;

    Fecha hoy(14, 10, 2026);
    cout << "fecha " << hoy << " valida: " << (hoy.esValida() ? "si" : "no") << endl;

    MatrizBits amistad(1000);
    amistad.poner(0, 5);
    amistad.poner(5, 0);
    amistad.poner(1, 5);
    amistad.poner(5, 1);
    cout << "amigos en comun entre 0 y 1: " << amistad.contarComunes(0, 1) << endl;

    // ----- mensajes -----
    // el mensaje guarda los indices del que escribe y del que recibe
    Mensaje saludo(3, 8, Fecha(2, 9, 2026), Texto("hola, como vas"));
    cout << endl << "mensaje del usuario " << saludo.getEmisor()
         << " para el usuario " << saludo.getReceptor() << endl;
    cout << "  " << saludo.getFecha() << ": " << saludo.getContenido() << endl;
    cout << "  leido: " << (saludo.getLeido() ? "si" : "no") << endl;
    saludo.setLeido(true);
    cout << "  despues de abrirlo: " << (saludo.getLeido() ? "si" : "no") << endl;

    // ----- fotos con me gusta -----
    Medicion::reiniciar();
    Foto paisaje(Texto("foto1"), Fecha(10, 1, 2026), 2400);
    paisaje.cambiarLike(4);
    paisaje.cambiarLike(9);
    paisaje.cambiarLike(15);
    cout << endl << "foto " << paisaje.getNombre() << " de " << paisaje.getTamano()
         << " kb con " << paisaje.cantidadLikes() << " me gusta" << endl;
    cout << "  el usuario 9 le dio me gusta: "
         << (paisaje.tieneLike(9) ? "si" : "no") << endl;
    // si el mismo usuario vuelve a dar, se le quita
    paisaje.cambiarLike(9);
    cout << "  despues de que el 9 lo quita quedan " << paisaje.cantidadLikes() << endl;
    cout << "  iteraciones de los me gusta: " << Medicion::total() << endl;

    // ----- albumes -----
    Album vacaciones(Texto("vacaciones"), 1);
    vacaciones.agregarFoto(paisaje);
    vacaciones.agregarFoto(Foto(Texto("foto2"), Fecha(11, 1, 2026), 1800));
    cout << endl << "album " << vacaciones.getNombre() << " con "
         << vacaciones.cantidadFotos() << " fotos" << endl;
    cout << "  privacidad: ";
    if (vacaciones.getPrivacidad() == 0) {
        cout << "publico" << endl;
    } else if (vacaciones.getPrivacidad() == 1) {
        cout << "solo amigos" << endl;
    } else {
        cout << "privado" << endl;
    }
    cout << "  foto2 esta en la posicion " << vacaciones.buscarFoto(Texto("foto2")) << endl;
    cout << "  memoria del album: " << vacaciones.memoriaDinamica() << " bytes" << endl;

    return 0;
}
