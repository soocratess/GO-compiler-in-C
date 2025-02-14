#ifndef hashTable_H
#define hashTable_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// --------------------- ESTRUCTURAS --------------------- //

// Estructura para almacenar tokens
typedef struct token{
    int identificador;  // Código numérico del token
    char *lexema;       // Representación textual del token
    struct token *next; // Lista enlazada para gestionar colisiones
}token;

// Cada posición en la tabla apunta a la cabeza de una lista enlazada de tokens (doble puntero)
typedef token **hashTable;

// --------------------- FUNCIONES --------------------- //

// Inicialización de la tabla de hash con tamaño dinámico.
int inicializarHashTable(hashTable *tabla, int tamanho);

// Libera los recursos asociados con la tabla de hash.
int eliminarHashTable(hashTable *tabla);

// Función hash para calcular el índice de un lexema en la tabla.
int ajustarTamanhoHashTable(hashTable *tabla, int tamanho);

int insertarToken(hashTable *tabla, char *lexema, int identificador);

int eliminarToken(hashTable *tabla, char *lexema);

int buscarToken(hashTable tabla, char *lexema);

int modificarToken(hashTable *tabla, char *lexema, int identificador);

void imprimirHashTable(hashTable tabla);

int cantidadElementosHashTable(hashTable tabla);

int tamanhoHashTable(hashTable tabla);

#endif // hashTable_H