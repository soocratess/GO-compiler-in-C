// Biblioteca creadas
#include "tablaSimbolos.h"
#include "../definiciones.h"


// Biblioteca estándar
#include <stdio.h>

#define TAMANHO_INICIAL 64 // Añadimos espacio extra para minimizar cantidad de redimensiones

// --------------------- INTERACCIÓN CON LA TABLA SÍMBOLOS --------------------- //

// Crear la tabla de símbolos
int inicializarTS(hashTable *tabla) {

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
    if (inicializarHashTable(tabla, TAMANHO_INICIAL) == -1) {
        printf("Error al inicializar la tabla de hash\n");
        return -1;
    }

    // Insertamos las palabras reservadas en la tabla de hash
    for (int i = 0; keywords[i] != NULL; i++) {
        if (insertarToken(tabla, keywords[i], i + 300) == -1) {
            printf("Error al insertar el token %s en la tabla de hash\n", keywords[i]);
        }
    }
}

// Destruir la tabla de símbolos
int destruirTS(hashTable *tabla){
    eliminarHashTable(tabla);
}

// Imprimir la tabla de símbolos
void imprimirTS(hashTable tabla){
    imprimirHashTable(tabla);
}

// --------------------- INTERACCIÓN CON ELEMENTOS DE LA TABLA --------------------- //

// Insertar elemento en la tabla
int insertarElemento(token t, hashTable *tabla){
    return insertarToken(tabla, t.lexema, t.identificador);
}

// Modificar elemento en la tabla
int modificarElemento(token t, hashTable tabla){
    return modificarToken(&tabla, t.lexema, t.identificador);
}

// Buscar elemento en la tabla
int buscarElemento(char *lexema, hashTable tabla){
    return buscarToken(tabla, lexema);
}

// Borrar elemento en la tabla
int borrarElemento(char *lexema, hashTable *tabla){
    return eliminarToken(tabla, lexema);
}


