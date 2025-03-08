#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "../SistemaEntrada/sistemaEntrada.h"

#define ERROR_GENERAL -1
#define EXITO 0

char file[]= "/home/smokey/Documents/ETSE/Compiladores/Practicas/P1/concurrentSum.go";

void test_crearYDestruirSistemaEntrada() {
    crearSistemaEntrada(file);
    destruirSistemaEntrada();
    printf("✓ crearYDestruirSistemaEntrada pasó\n");
}

void test_siguienteCaracter() {
    crearSistemaEntrada(file);

    char c = siguienteCaracter();
    assert(c != EOF);  // Asegurar que el archivo no está vacío

    while (c != EOF) {
        c = siguienteCaracter();
    }

    destruirSistemaEntrada();
    printf("✓ siguienteCaracter pasó\n");
}

void test_retrocederCaracter() {
    crearSistemaEntrada(file);

    char c1 = siguienteCaracter();
    char c2 = siguienteCaracter();
    retrocederCaracter();
    char c3 = siguienteCaracter();

    assert(c2 == c3);  // Asegurar que el retroceso funcionó correctamente

    destruirSistemaEntrada();
    printf("✓ retrocederCaracter pasó\n");
}

void test_obtenerLexema() {
    crearSistemaEntrada(file);

    char *lexema = obtenerLexema();
    assert(lexema != NULL);
    printf("Lexema obtenido: %s\n", lexema);
    free(lexema);

    destruirSistemaEntrada();
    printf("✓ obtenerLexema pasó\n");
}

void test_ignorarSecuencia() {
    crearSistemaEntrada(file);

    ignorarSecuencia();  // No podemos verificarlo fácilmente, pero no debe causar errores

    destruirSistemaEntrada();
    printf("✓ ignorarSecuencia pasó\n");
}

void test_avanzarPuntero() {
    crearSistemaEntrada(file);

    avanzarPuntero();  // Similar a ignorarSecuencia, solo aseguramos que no falle

    destruirSistemaEntrada();
    printf("✓ avanzarPuntero pasó\n");
}

void test_imprimirBuffer() {
    crearSistemaEntrada(file);

    imprimirBuffer();  // Debe mostrar el estado actual del buffer en consola

    destruirSistemaEntrada();
    printf("✓ imprimirBuffer pasó\n");
}

void test_notificarRecarga() {
    crearSistemaEntrada(file);

    notificarRecarga();  // Simulación de recarga del buffer

    destruirSistemaEntrada();
    printf("✓ notificarRecarga pasó\n");
}

int main() {
    printf("Ejecutando pruebas...\n");

    test_crearYDestruirSistemaEntrada();
    test_siguienteCaracter();
    test_retrocederCaracter();
    test_obtenerLexema();
    test_ignorarSecuencia();
    test_avanzarPuntero();
    test_imprimirBuffer();
    test_notificarRecarga();

    printf("\nTodas las pruebas pasaron correctamente.\n");

    return 0;
}
