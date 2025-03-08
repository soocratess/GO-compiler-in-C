#include <stdio.h>

#include "TablaSimbolos/tablaSimbolos.h"
#include "AnalizadorLexico/analizadorLexico.h"
#include "AnalizadorSintactico/analizadorSintactico.h"
#include "SistemaEntrada/sistemaEntrada.h"
#include "Error/error.h"

void main() {

    crearSistemaEntrada("/home/smokey/Documents/ETSE/Compiladores/Practicas/P1/concurrentSum.go");
    crearTablaSimbolos();
    imprimirTablaSimbolos();

    iniciarAnalisis();

    imprimirTablaSimbolos();
    destruirTablaSimbolos();
    destruirSistemaEntrada();
}