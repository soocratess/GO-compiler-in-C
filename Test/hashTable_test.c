#include <stdio.h>
#include <assert.h>
#include "../HashTable/hashTable.h"

#define ERROR_GENERAL -1
#define EXITO 0


void test_inicializarHashTable() {
    hashTable tabla;
    int resultado = crearHashTable(&tabla, 10);
    assert(resultado == EXITO);  // Verifica que la inicialización fue exitosa
    assert(tamanhoHashTable(tabla) == 10);
    destruirHashTable(&tabla);
}

void test_insertarToken() {
    hashTable tabla;
    crearHashTable(&tabla, 10);

    int resultado = insertarToken(&tabla, "test", 100);
    assert(resultado == EXITO);
    assert(buscarToken(tabla, "test") == 100);

    destruirHashTable(&tabla);
}

void test_buscarToken() {
    hashTable tabla;
    crearHashTable(&tabla, 10);

    insertarToken(&tabla, "clave1", 200);
    insertarToken(&tabla, "clave2", 300);

    assert(buscarToken(tabla, "clave1") == 200);
    assert(buscarToken(tabla, "clave2") == 300);
    assert(buscarToken(tabla, "no_existe") == ERROR_GENERAL);  // Debe devolver 0 si no está

    destruirHashTable(&tabla);
}

void test_modificarToken() {
    hashTable tabla;
    crearHashTable(&tabla, 10);

    insertarToken(&tabla, "modificar", 400);
    assert(buscarToken(tabla, "modificar") == 400);

    modificarToken(&tabla, "modificar", 500);
    assert(buscarToken(tabla, "modificar") == 500);

    destruirHashTable(&tabla);
}

void test_eliminarToken() {
    hashTable tabla;
    crearHashTable(&tabla, 10);

    insertarToken(&tabla, "borrar", 600);
    assert(buscarToken(tabla, "borrar") == 600);

    eliminarToken(&tabla, "borrar");
    assert(buscarToken(tabla, "borrar") == ERROR_GENERAL);

    destruirHashTable(&tabla);
}

void test_cantidadElementos() {
    hashTable tabla;
    crearHashTable(&tabla, 10);

    assert(cantidadElementosHashTable(tabla) == 0);

    insertarToken(&tabla, "uno", 101);
    insertarToken(&tabla, "dos", 102);
    insertarToken(&tabla, "tres", 103);

    assert(cantidadElementosHashTable(tabla) == 3);

    destruirHashTable(&tabla);
}

void test_imprimirHashTable() {
    hashTable tabla;
    crearHashTable(&tabla, 5);

    insertarToken(&tabla, "test1", 1);
    insertarToken(&tabla, "test2", 2);
    insertarToken(&tabla, "test3", 3);

    printf("\nTabla Hash después de insertar elementos:\n");
    imprimirHashTable(tabla);

    destruirHashTable(&tabla);
}

int main() {
    printf("Ejecutando pruebas...\n");

    test_inicializarHashTable();
    printf("✓ inicializarHashTable pasó\n");

    test_insertarToken();
    printf("✓ insertarToken pasó\n");

    test_buscarToken();
    printf("✓ buscarToken pasó\n");

    test_modificarToken();
    printf("✓ modificarToken pasó\n");

    test_eliminarToken();
    printf("✓ eliminarToken pasó\n");

    test_cantidadElementos();
    printf("✓ cantidadElementosHashTable pasó\n");

    test_imprimirHashTable();
    printf("✓ imprimirHashTable pasó\n");

    printf("\nTodas las pruebas pasaron correctamente.\n");

    return 0;
}
