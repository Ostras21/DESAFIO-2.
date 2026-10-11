#ifndef MATRIZBITS_H
#define MATRIZBITS_H

// matriz cuadrada de n x n donde cada casilla es un solo bit
// (se usa para la amistad y las solicitudes pendientes)
class MatrizBits {
private:
    unsigned long long* palabras;
    int n;
    int palabrasPorFila;
    int total;

    static int contarUnos(unsigned long long x);

public:
    MatrizBits(int tamano);
    MatrizBits(const MatrizBits& otra);
    ~MatrizBits();

    MatrizBits& operator=(const MatrizBits& otra);

    void poner(int i, int j);
    void quitar(int i, int j);
    bool obtener(int i, int j) const;

    // cuantos unos tiene la fila i (por ejemplo, cuantos amigos tiene)
    int contarFila(int i) const;
    // cuantos unos tienen en comun las filas a y b
    int contarComunes(int a, int b) const;

    int tam() const;
    long memoriaDinamica() const;
};

#endif
