#ifndef LISTADINAMICA_H
#define LISTADINAMICA_H

#include "Medicion.h"

// arreglo dinamico que duplica su capacidad cuando se llena,
// asi agregar al final cuesta O(1) en promedio
template <typename T>
class ListaDinamica {
private:
    T* datos;
    int cantidad;
    int capacidad;

    void crecer() {
        int nuevaCapacidad = capacidad * 2;
        T* nuevos = new T[nuevaCapacidad];
        for (int i = 0; i < cantidad; i++) {
            Medicion::contar();
            nuevos[i] = datos[i];
        }
        delete[] datos;
        datos = nuevos;
        capacidad = nuevaCapacidad;
    }

public:
    ListaDinamica() {
        cantidad = 0;
        capacidad = 4;
        datos = new T[capacidad];
    }

    ListaDinamica(const ListaDinamica& otra) {
        cantidad = otra.cantidad;
        capacidad = otra.capacidad;
        datos = new T[capacidad];
        for (int i = 0; i < cantidad; i++) {
            Medicion::contar();
            datos[i] = otra.datos[i];
        }
    }

    ~ListaDinamica() {
        delete[] datos;
    }

    ListaDinamica& operator=(const ListaDinamica& otra) {
        if (this == &otra) {
            return *this;
        }
        delete[] datos;
        cantidad = otra.cantidad;
        capacidad = otra.capacidad;
        datos = new T[capacidad];
        for (int i = 0; i < cantidad; i++) {
            Medicion::contar();
            datos[i] = otra.datos[i];
        }
        return *this;
    }

    void agregar(const T& elemento) {
        if (cantidad == capacidad) {
            crecer();
        }
        datos[cantidad] = elemento;
        cantidad++;
    }

    // saca el elemento i poniendo el ultimo en su lugar, asi no hay que correr todo
    void eliminar(int i) {
        datos[i] = datos[cantidad - 1];
        cantidad--;
    }

    T& operator[](int i) {
        return datos[i];
    }

    const T& operator[](int i) const {
        return datos[i];
    }

    int tam() const {
        return cantidad;
    }

    // solo la memoria del arreglo; si guarda punteros, lo apuntado lo suma quien lo creo
    long memoriaDinamica() const {
        return (long)capacidad * (long)sizeof(T);
    }
};

#endif
