#include <stdio.h>
#include <assert.h>
#include "../HashTable/hashTable.h"
#include "../TablaSimbolos/tablaSimbolos.h"

#define ERROR_GENERAL -1
#define EXITO 0

void test_inicializarTS() {
    hashTable tabla;
    int resultado = inicializarTS(&tabla);
    assert(resultado == EXITO);
    destruirTS(&tabla);
}

void test_insertarElemento() {
    hashTable tabla;
    inicializarTS(&tabla);

    token nuevoToken = {100, "identificador", NULL};
    int resultado = insertarElemento(nuevoToken, &tabla);
    assert(resultado == EXITO);
    assert(buscarElemento("identificador", tabla) == 100);

    destruirTS(&tabla);
}

void test_buscarElemento() {
    hashTable tabla;
    inicializarTS(&tabla);

    token t1 = {200, "clave1", NULL};
    token t2 = {300, "clave2", NULL};

    insertarElemento(t1, &tabla);
    insertarElemento(t2, &tabla);

    assert(buscarElemento("clave1", tabla) == 200);
    assert(buscarElemento("clave2", tabla) == 300);
    assert(buscarElemento("no_existe", tabla) == ERROR_GENERAL);

    destruirTS(&tabla);
}

void test_modificarElemento() {
    hashTable tabla;
    inicializarTS(&tabla);

    token t = {400, "modificar", NULL};
    insertarElemento(t, &tabla);
    assert(buscarElemento("modificar", tabla) == 400);

    t.identificador = 500;
    modificarElemento(t, tabla);
    assert(buscarElemento("modificar", tabla) == 500);

    destruirTS(&tabla);
}

void test_borrarElemento() {
    hashTable tabla;
    inicializarTS(&tabla);

    token t = {600, "borrar", NULL};
    insertarElemento(t, &tabla);
    assert(buscarElemento("borrar", tabla) == 600);

    borrarElemento("borrar", &tabla);
    assert(buscarElemento("borrar", tabla) == ERROR_GENERAL);

    destruirTS(&tabla);
}

void test_imprimirTS() {
    hashTable tabla;
    inicializarTS(&tabla);

    insertarElemento((token){1, "test1", NULL}, &tabla);
    insertarElemento((token){2, "test2", NULL}, &tabla);
    insertarElemento((token){3, "test3", NULL}, &tabla);

    printf("\nTabla de Símbolos después de insertar elementos:\n");
    imprimirTS(tabla);

    destruirTS(&tabla);
}

int main() {
    printf("Ejecutando pruebas de tabla de símbolos...\n");

    test_inicializarTS();
    printf("✓ inicializarTS pasó\n");

    test_insertarElemento();
    printf("✓ insertarElemento pasó\n");

    test_buscarElemento();
    printf("✓ buscarElemento pasó\n");

    test_modificarElemento();
    printf("✓ modificarElemento pasó\n");

    test_borrarElemento();
    printf("✓ borrarElemento pasó\n");

    test_imprimirTS();
    printf("✓ imprimirTS pasó\n");

    printf("\nTodas las pruebas pasaron correctamente.\n");

    return 0;
}
