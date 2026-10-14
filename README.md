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
- `MatrizBits`: matriz de n x n donde cada casilla es un solo bit. Con 1000 usuarios la
  matriz de amistades ocupa unos 122 KB en vez de casi 1 MB usando booleanos, y contar
  los amigos en común se hace comparando 64 casillas por vuelta.

## Clases del dominio

- `Mensaje`: guarda los índices del emisor y del receptor en vez de punteros, la fecha,
  el contenido y si ya se leyó.
- `Foto`: nombre, fecha, tamaño y la lista de los usuarios que le dieron me gusta. Se
  guarda quién dio el me gusta y no solo cuántos, para poder quitarlo después.
- `Album`: nombre, privacidad (0 público, 1 solo amigos, 2 privado) y sus fotos
  guardadas por valor, porque el álbum es el dueño de ellas.

## Lo que falta

Las clases `Usuario` y `RedSocial`, la lectura y escritura de los archivos de texto y
el menú de consola.

## Diseño

El diagrama de clases y el pre informe del análisis están en la raíz del repositorio.
