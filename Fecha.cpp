#include "Fecha.h"

Fecha::Fecha() {
    dia = 0;
    mes = 0;
    anio = 0;
}

Fecha::Fecha(int d, int m, int a) {
    dia = d;
    mes = m;
    anio = a;
}

int Fecha::getDia() const {
    return dia;
}

int Fecha::getMes() const {
    return mes;
}

int Fecha::getAnio() const {
    return anio;
}

bool Fecha::esValida() const {
    if (anio < 1 || mes < 1 || mes > 12 || dia < 1) {
        return false;
    }
    int diasDelMes = 31;
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        diasDelMes = 30;
    } else if (mes == 2) {
        bool bisiesto = (anio % 4 == 0 && anio % 100 != 0) || anio % 400 == 0;
        diasDelMes = bisiesto ? 29 : 28;
    }
    return dia <= diasDelMes;
}

long Fecha::clave() const {
    return (long)anio * 10000 + mes * 100 + dia;
}

bool Fecha::operator==(const Fecha& otra) const {
    return clave() == otra.clave();
}

bool Fecha::operator<(const Fecha& otra) const {
    return clave() < otra.clave();
}

bool Fecha::operator<=(const Fecha& otra) const {
    return clave() <= otra.clave();
}

std::ostream& operator<<(std::ostream& salida, const Fecha& f) {
    if (f.dia < 10) {
        salida << '0';
    }
    salida << f.dia << '/';
    if (f.mes < 10) {
        salida << '0';
    }
    salida << f.mes << '/' << f.anio;
    return salida;
}
