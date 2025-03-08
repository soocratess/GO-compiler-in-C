#include <stdio.h>
#include <assert.h>
#include "../TablaSimbolos/tablaSimbolos.h"

#include <definiciones.h>

#define ERROR_GENERAL -1
#define EXITO 0

void test_crearTablaSimbolos() {
    int resultado = crearTablaSimbolos();
    assert(resultado == EXITO);
    destruirTablaSimbolos();
}

void test_insertarElemento() {
    crearTablaSimbolos();

    int resultado = buscarElemento("identificador");
    assert(resultado == EXITO);
    assert(buscarElemento("identificador") == ID);

    destruirTablaSimbolos();
}

void test_buscarElemento() {
    crearTablaSimbolos();

    buscarElemento("clave1");
    buscarElemento("clave2");

    assert(buscarElemento("clave1") == ID);
    assert(buscarElemento("clave2") == ID);
    // assert(buscarElemento("no_existe") == ERROR_GENERAL);

    destruirTablaSimbolos();
}

void test_imprimirTablaSimbolos() {
    crearTablaSimbolos();

    buscarElemento("test1");
    buscarElemento("test2");
    buscarElemento("test3");

    printf("\nTabla de Símbolos después de insertar elementos:\n");
    imprimirTablaSimbolos();

    destruirTablaSimbolos();
}

int main() {
    printf("Ejecutando pruebas de tabla de símbolos...\n");

    test_crearTablaSimbolos();
    printf("✓ crearTablaSimbolos pasó\n");

    test_insertarElemento();
    printf("✓ insertarElemento pasó\n");

    test_buscarElemento();
    printf("✓ buscarElemento pasó\n");

    test_imprimirTablaSimbolos();
    printf("✓ imprimirTablaSimbolos pasó\n");

    printf("\nTodas las pruebas pasaron correctamente.\n");

    return 0;
}
