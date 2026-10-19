# UdeABook

Desafío II de Informática II (Universidad de Antioquia, 2026-2).

Capa lógica de una red social básica hecha con programación orientada a objetos en C++:
usuarios, amigos, solicitudes, mensajes, álbumes y fotos. Todo con estructuras propias
y memoria dinámica, sin contenedores de la STL y sin herencia.

## Clases

Clases de apoyo:

- `Medicion`: contador estático de iteraciones.
- `Texto`: cadena de caracteres propia con memoria dinámica.
- `Fecha`: fecha con validación y comparaciones.
- `ListaDinamica<T>`: plantilla de arreglo dinámico que duplica su capacidad.
- `MatrizBits`: matriz de n x n donde cada casilla es un solo bit.

Clases del dominio:

- `Mensaje`, `Foto`, `Album`, `Usuario`: los datos de la red.
- `RedSocial`: dueña de los usuarios y los mensajes, maneja amistades y búsquedas.
- `Archivos`: lee y escribe los archivos de texto.
- `Menu`: la interfaz por consola.

## Compilar

Con Qt Creator: abrir `UdeABook.pro` y ejecutar.

Desde la terminal:

    g++ -std=c++17 -Wall -o udeabook *.cpp

## Datos

Los datos quedan en archivos de texto dentro de la carpeta `datos`, que tiene que estar
al lado del ejecutable. En Qt Creator se puede dejar el proyecto como directorio de
trabajo (Projects > Run > Working directory) para que encuentre la carpeta.

Formato: un registro por línea y los campos separados por `|`.

- `usuarios.txt`: la primera línea es la cantidad de usuarios y después
  `nombre|clave|nacimiento|correo|ciudad|pais|inscripcion`. El número de la línea es
  el índice del usuario.
- `amistades.txt`: `indiceA indiceB`, una sola vez por pareja.
- `solicitudes.txt`: `indiceEmisor indiceReceptor` de las solicitudes pendientes.
- `mensajes.txt`: `emisor|receptor|fecha|leido|contenido`, en orden cronológico.
- `albumes.txt`: `indiceUsuario|nombreAlbum|privacidad` (0 público, 1 amigos, 2 privado).
- `fotos.txt`: `indiceUsuario|nombreAlbum|nombreFoto|fecha|tamano|likes` donde los likes
  son los índices de quienes dieron me gusta separados por coma.

Las rutas de las fotos no se guardan, se arman cuando se necesitan con el nombre del
usuario, el álbum y la foto.

## Generador de datos

En la carpeta `generador` hay un programa aparte que llena la carpeta `datos` con 1200
usuarios, sus amistades, solicitudes, mensajes, álbumes y fotos:

    g++ -std=c++17 -o generar generador/generar.cpp
    ./generar

La carpeta `datos` tiene que existir antes de correrlo.
