#ifndef TEXTO_H
#define TEXTO_H

#include <iostream>

// cadena de caracteres con memoria dinamica
class Texto {
private:
    char* datos;
    int longitud;

public:
    Texto();
    Texto(const char* origen);
    Texto(const Texto& otro);
    ~Texto();

    Texto& operator=(const Texto& otro);
    bool operator==(const Texto& otro) const;
    bool operator<(const Texto& otro) const;
    Texto operator+(const Texto& otro) const;

    const char* cadena() const;
    int tam() const;
    long memoriaDinamica() const;

    friend std::ostream& operator<<(std::ostream& salida, const Texto& t);
};

#endif
