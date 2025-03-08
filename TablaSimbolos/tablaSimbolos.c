// Biblioteca creadas
#include "tablaSimbolos.h"
#include "definiciones.h"


// Biblioteca estándar
#include <stdio.h>

#define TAMANHO_INICIAL 64 // Añadimos espacio extra para minimizar cantidad de redimensiones

hashTable tabla = NULL;
// --------------------- INTERACCIÓN CON LA TABLA SÍMBOLOS --------------------- //

// Crear la tabla de símbolos
int crearTablaSimbolos(){

    // Palabras reservadas (keywords) del lenguaje
    char *keywords[TAMANHO_INICIAL] = {
        "break", "case", "chan", "const", "continue",
        "default", "defer", "else", "fallthrough", "for",
        "func", "go", "goto", "if", "import",
        "interface", "map", "package", "range", "return",
        "select", "struct", "switch", "type", "var",
        NULL
    };

    // Inicializamos la tabla de hash
    if (crearHashTable(&tabla, TAMANHO_INICIAL) == -1) {
        printf("Error al inicializar la tabla de hash\n");
        return -1;
    }

    // Insertamos las palabras reservadas en la tabla de hash
    for (int i = 0; keywords[i] != NULL; i++) {
        if (insertarToken(&tabla, keywords[i], i + 300) == -1) {
            printf("Error al insertar el token %s en la tabla de hash\n", keywords[i]);
        }
    }
    return 0;
}

// Destruir la tabla de símbolos
int destruirTablaSimbolos(){
    return destruirHashTable(&tabla);
}

// Imprimir la tabla de símbolos
void imprimirTablaSimbolos(){
    imprimirHashTable(tabla);
}

// --------------------- INTERACCIÓN CON ELEMENTOS DE LA TABLA --------------------- //

// Buscar elemento en la tabla
int buscarElemento(char *lexema){
    int busqueda = buscarToken(tabla, lexema);

    if (busqueda == -1) {
        insertarToken(&tabla, lexema,ID);
        return ID;
    }
    return busqueda;
}


