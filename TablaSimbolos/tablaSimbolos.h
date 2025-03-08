#ifndef TABLA_SIMBOLOS_H
#define TABLA_SIMBOLOS_H

#include <stdio.h>

#include "../HashTable/hashTable.h"

// --------------------- INTERACCIÓN CON LA TABLA SÍMBOLOS --------------------- //

// Crear la tabla de símbolos
int crearTablaSimbolos();

// Destruir la tabla de símbolos
int destruirTablaSimbolos();

// Imprimir la tabla de símbolos
void imprimirTablaSimbolos();

// --------------------- INTERACCIÓN CON ELEMENTOS DE LA TABLA --------------------- //

// Buscar elemento en la tabla
int buscarElemento(char *lexema);


#endif //TABLA_SIMBOLOS_H

