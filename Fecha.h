#ifndef FECHA_H
#define FECHA_H

#include <iostream>

class Fecha {
private:
    int dia;
    int mes;
    int anio;

public:
    Fecha();
    Fecha(int d, int m, int a);

    int getDia() const;
    int getMes() const;
    int getAnio() const;

    bool esValida() const;
    // aaaammdd, sirve para comparar de una sola vez
    long clave() const;

    bool operator==(const Fecha& otra) const;
    bool operator<(const Fecha& otra) const;
    bool operator<=(const Fecha& otra) const;

    friend std::ostream& operator<<(std::ostream& salida, const Fecha& f);
};

#endif
