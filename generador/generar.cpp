// programa aparte que crea los archivos de prueba con mas de mil usuarios.
// no hace parte del programa principal, solo sirve para llenar la carpeta datos
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

const int CANTIDAD_USUARIOS = 1200;
const int AMIGOS_PROMEDIO = 18;
const int CANTIDAD_MENSAJES = 15000;
const int SOLICITUDES = 900;

const char* NOMBRES[] = {
    "Juan", "Maria", "Carlos", "Laura", "Andres", "Camila", "Santiago", "Valentina",
    "Sebastian", "Daniela", "Mateo", "Sara", "Nicolas", "Isabella", "Samuel",
    "Mariana", "David", "Luciana", "Tomas", "Antonia", "Esteban", "Paula",
    "Felipe", "Natalia", "Miguel", "Catalina", "Alejandro", "Juliana", "Diego",
    "Manuela"
};
const int TOTAL_NOMBRES = 30;

const char* APELLIDOS[] = {
    "Gomez", "Rodriguez", "Martinez", "Lopez", "Garcia", "Perez", "Sanchez",
    "Ramirez", "Torres", "Flores", "Rivera", "Gutierrez", "Ruiz", "Alvarez",
    "Romero", "Suarez", "Castro", "Ortiz", "Silva", "Rojas", "Moreno", "Vargas",
    "Cardona", "Zapata", "Osorio", "Mejia", "Arango", "Restrepo", "Velasquez",
    "Quintero"
};
const int TOTAL_APELLIDOS = 30;

const char* CIUDADES[] = {
    "Medellin", "Bogota", "Cali", "Barranquilla", "Cartagena", "Pereira",
    "Manizales", "Bucaramanga", "Santa Marta", "Envigado"
};
const int TOTAL_CIUDADES = 10;

const char* PAISES[] = {
    "Colombia", "Mexico", "Argentina", "Chile", "Peru", "Ecuador", "España"
};
const int TOTAL_PAISES = 7;

int diasDelMes(int mes, int anio) {
    if (mes == 2) {
        bool bisiesto = (anio % 4 == 0 && anio % 100 != 0) || anio % 400 == 0;
        return bisiesto ? 29 : 28;
    }
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        return 30;
    }
    return 31;
}

void escribirFecha(ofstream& archivo, int dia, int mes, int anio) {
    if (dia < 10) {
        archivo << '0';
    }
    archivo << dia << '/';
    if (mes < 10) {
        archivo << '0';
    }
    archivo << mes << '/' << anio;
}

int main() {
    srand(97531);

    // ---------- usuarios ----------
    ofstream usuarios("datos/usuarios.txt");
    if (!usuarios.is_open()) {
        cout << "cree primero la carpeta datos al lado del ejecutable" << endl;
        return 1;
    }
    usuarios << CANTIDAD_USUARIOS << '\n';
    for (int i = 0; i < CANTIDAD_USUARIOS; i++) {
        const char* nombre = NOMBRES[rand() % TOTAL_NOMBRES];
        const char* apellido = APELLIDOS[rand() % TOTAL_APELLIDOS];
        // se le pega el numero para que ningun nombre quede repetido
        usuarios << nombre << '.' << apellido << i << '|';
        usuarios << "clave" << i << '|';
        int anioNac = 1970 + rand() % 36;
        int mesNac = 1 + rand() % 12;
        escribirFecha(usuarios, 1 + rand() % diasDelMes(mesNac, anioNac), mesNac, anioNac);
        usuarios << '|';
        usuarios << nombre << apellido << i << "@correo.com" << '|';
        usuarios << CIUDADES[rand() % TOTAL_CIUDADES] << '|';
        usuarios << PAISES[rand() % TOTAL_PAISES] << '|';
        int anioIns = 2015 + rand() % 11;
        int mesIns = 1 + rand() % 12;
        escribirFecha(usuarios, 1 + rand() % diasDelMes(mesIns, anioIns), mesIns, anioIns);
        usuarios << '\n';
    }
    usuarios.close();

    // ---------- amistades ----------
    // se usa una matriz de bits propia para no repetir parejas
    int palabrasPorFila = (CANTIDAD_USUARIOS + 63) / 64;
    unsigned long long* amistad =
        new unsigned long long[CANTIDAD_USUARIOS * palabrasPorFila];
    for (int i = 0; i < CANTIDAD_USUARIOS * palabrasPorFila; i++) {
        amistad[i] = 0;
    }

    // tambien se guardan las parejas en un arreglo para despues poder sacar
    // mensajes entre gente que de verdad es amiga
    int maximoParejas = CANTIDAD_USUARIOS * AMIGOS_PROMEDIO * 2;
    int* parejaA = new int[maximoParejas];
    int* parejaB = new int[maximoParejas];

    ofstream amistades("datos/amistades.txt");
    int totalAmistades = 0;
    for (int i = 0; i < CANTIDAD_USUARIOS; i++) {
        int cuantos = AMIGOS_PROMEDIO / 2 + rand() % AMIGOS_PROMEDIO;
        for (int k = 0; k < cuantos; k++) {
            int j = rand() % CANTIDAD_USUARIOS;
            if (j == i) {
                continue;
            }
            bool yaEsta = (amistad[i * palabrasPorFila + j / 64] >> (j % 64)) & 1ULL;
            if (yaEsta || totalAmistades >= maximoParejas) {
                continue;
            }
            amistad[i * palabrasPorFila + j / 64] |= (1ULL << (j % 64));
            amistad[j * palabrasPorFila + i / 64] |= (1ULL << (i % 64));
            amistades << i << ' ' << j << '\n';
            parejaA[totalAmistades] = i;
            parejaB[totalAmistades] = j;
            totalAmistades++;
        }
    }
    amistades.close();

    // ---------- solicitudes pendientes ----------
    ofstream solicitudes("datos/solicitudes.txt");
    int puestas = 0;
    int intentos = 0;
    while (puestas < SOLICITUDES && intentos < SOLICITUDES * 20) {
        intentos++;
        int de = rand() % CANTIDAD_USUARIOS;
        int para = rand() % CANTIDAD_USUARIOS;
        if (de == para) {
            continue;
        }
        bool amigos = (amistad[de * palabrasPorFila + para / 64] >> (para % 64)) & 1ULL;
        if (amigos) {
            continue;
        }
        solicitudes << de << ' ' << para << '\n';
        puestas++;
    }
    solicitudes.close();

    // ---------- mensajes ----------
    // las fechas van subiendo para que el archivo quede en orden cronologico;
    // los primeros quedan leidos y los ultimos sin leer
    ofstream mensajes("datos/mensajes.txt");
    int dia = 1;
    int mes = 1;
    int anio = 2020;
    int leidosHasta = (CANTIDAD_MENSAJES * 7) / 10;
    int escritos = 0;
    while (escritos < CANTIDAD_MENSAJES && totalAmistades > 0) {
        // se toma una pareja de amigos al azar y se decide quien escribe
        int cual = rand() % totalAmistades;
        int de = parejaA[cual];
        int para = parejaB[cual];
        if (rand() % 2 == 0) {
            int cambio = de;
            de = para;
            para = cambio;
        }
        mensajes << de << '|' << para << '|';
        escribirFecha(mensajes, dia, mes, anio);
        mensajes << '|' << (escritos < leidosHasta ? 1 : 0) << '|';
        mensajes << "mensaje numero " << escritos << " de prueba\n";
        escritos++;
        // cada ocho mensajes mas o menos avanza un dia
        if (rand() % 8 == 0) {
            dia++;
            if (dia > diasDelMes(mes, anio)) {
                dia = 1;
                mes++;
                if (mes > 12) {
                    mes = 1;
                    anio++;
                }
            }
        }
    }
    mensajes.close();

    // ---------- albumes y fotos ----------
    ofstream albumes("datos/albumes.txt");
    ofstream fotos("datos/fotos.txt");
    int totalFotos = 0;
    for (int i = 0; i < CANTIDAD_USUARIOS; i++) {
        int cuantosAlbumes = 1 + rand() % 3;
        for (int a = 0; a < cuantosAlbumes; a++) {
            albumes << i << "|album" << a << '|' << (rand() % 3) << '\n';
            int cuantasFotos = 1 + rand() % 5;
            for (int f = 0; f < cuantasFotos; f++) {
                int anioFoto = 2018 + rand() % 8;
                int mesFoto = 1 + rand() % 12;
                fotos << i << "|album" << a << "|foto" << f << '|';
                escribirFecha(fotos, 1 + rand() % diasDelMes(mesFoto, anioFoto),
                              mesFoto, anioFoto);
                fotos << '|' << (100 + rand() % 4000) << '|';
                int cuantosLikes = rand() % 8;
                for (int k = 0; k < cuantosLikes; k++) {
                    if (k > 0) {
                        fotos << ',';
                    }
                    fotos << (rand() % CANTIDAD_USUARIOS);
                }
                fotos << '\n';
                totalFotos++;
            }
        }
    }
    albumes.close();
    fotos.close();

    delete[] amistad;
    delete[] parejaA;
    delete[] parejaB;

    cout << "usuarios: " << CANTIDAD_USUARIOS << endl;
    cout << "amistades: " << totalAmistades << endl;
    cout << "solicitudes: " << puestas << endl;
    cout << "mensajes: " << escritos << endl;
    cout << "fotos: " << totalFotos << endl;
    return 0;
}
