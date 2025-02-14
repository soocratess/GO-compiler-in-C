#ifndef TABLA_SIMBOLOS_H
#define TABLA_SIMBOLOS_H

#include <stdio.h>

#include "../HashTable/hashTable.h"

// --------------------- INTERACCIÓN CON LA TABLA SÍMBOLOS --------------------- //

// Crear la tabla de símbolos
int inicializarTS(hashTable *tabla);

// Destruir la tabla de símbolos
int destruirTS(hashTable *tabla);

// Imprimir la tabla de símbolos
void imprimirTS(hashTable tabla);

// --------------------- INTERACCIÓN CON ELEMENTOS DE LA TABLA --------------------- //

// Insertar elemento en la tabla
int insertarElemento(token t, hashTable *tabla);

// Modificar elemento en la tabla
int modificarElemento(token t, hashTable tabla);

// Buscar elemento en la tabla
int buscarElemento(char *lexema, hashTable tabla);

// Borrar elemento en la tabla
int borrarElemento(char *lexema, hashTable *tabla);


#endif //TABLA_SIMBOLOS_H

