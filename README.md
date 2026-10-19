# UdeABook

Desafío II de Informática II (Universidad de Antioquia, 2026-2).

Capa lógica de una red social básica hecha con programación orientada a objetos en C++:
usuarios, amigos, solicitudes, mensajes, álbumes y fotos. Todo con estructuras propias
y memoria dinámica, sin contenedores de la STL y sin herencia.

## Compilar

Con Qt Creator: abrir `UdeABook.pro` y ejecutar.

Desde la terminal:

    g++ -std=c++17 -Wall -o udeabook *.cpp

## Clases de apoyo

- `Medicion`: contador estático de iteraciones, para poder medir cada operación.
- `Texto`: cadena de caracteres propia con memoria dinámica. Sobrecarga `==`, `<`, `+`
  y `<<` como función amiga.
- `Fecha`: fecha con validación (tiene en cuenta los años bisiestos) y comparaciones.
  El método `clave()` la vuelve un número `aaaammdd` para poder comparar de una vez.
- `ListaDinamica<T>`: plantilla de arreglo dinámico que duplica su capacidad cuando se
  llena, así agregar al final cuesta O(1) en promedio.
- `MatrizBits`: matriz de n x n donde cada casilla es un solo bit.

## Clases del dominio

- `Mensaje`: guarda los índices del emisor y del receptor en vez de punteros, la fecha,
  el contenido y si ya se leyó.
- `Foto`: nombre, fecha, tamaño y la lista de los usuarios que le dieron me gusta. Se
  guarda quién dio el me gusta y no solo cuántos, para poder quitarlo después.
- `Album`: nombre, privacidad (0 público, 1 solo amigos, 2 privado) y sus fotos
  guardadas por valor, porque el álbum es el dueño de ellas.
- `Usuario`: los datos personales, sus álbumes por valor y las listas de mensajes
  recibidos y enviados como punteros. Guarda `primerNoLeido`, la posición donde empiezan
  los mensajes sin leer.
- `RedSocial`: la dueña de los usuarios y de los mensajes, los crea y los destruye.

## Decisiones de eficiencia

- La amistad y las solicitudes pendientes se guardan en dos `MatrizBits` de n x n. Saber
  si dos personas son amigas es mirar un bit, y los amigos en común salen de un AND de
  dos filas contando los unos de 64 en 64. Con 1000 usuarios la matriz pesa unos 122 KB
  en vez de casi 1 MB con booleanos.
- Al cargar los usuarios se arma una sola vez un índice ordenado por nombre con
  ordenamiento por mezcla. De ahí en adelante buscar a alguien por su nombre es una
  búsqueda binaria.
- Los mensajes se agregan en orden cronológico, así que las listas ya quedan ordenadas.
  Para ver una conversación entre dos fechas se busca con búsqueda binaria dónde empieza
  el rango y se recorre solo hasta pasarse de la fecha final.
- Los no leídos siempre quedan al final de la lista de recibidos, entonces contarlos es
  una resta.
- Las sugerencias solo guardan los mejores K candidatos, no la lista completa.

## Lo que falta

La lectura y escritura de los archivos de texto y el menú de consola.

## Diseño

El diagrama de clases y el pre informe del análisis están en la raíz del repositorio.
