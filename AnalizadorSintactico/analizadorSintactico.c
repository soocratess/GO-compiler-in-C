#include "analizadorSintactico.h"

// Imprime el componente léxico en la consola.
void imprimirComponenteLexico(token t){
    printf("%-33s %-10d\n", t.lexema, t.identificador);
}

void iniciarAnalisis(hashTable *tabla, FILE *ficheroEntrada){
    token t;

    printf("%-33s %-10s\n", "Lexema", "Identificador");
    printf("--------------------------------- ----------\n");

    // Solicitamos componentes léxicos al analizador léxico hasta que reciba EOF.
    do {
        printf("--------------------------------- ----------\n");

        // Se solicita el siguiente componente léxico.
        int liberarMemoria = siguienteComponenteLexico(&t, tabla, ficheroEntrada);
        
        // Si no se ha llegado al final del fichero, se imprime el componente léxico.
        if (t.identificador != EOF) imprimirComponenteLexico(t);
        
        // Se libera la memoria del lexema si se ha solicitado.
        if (liberarMemoria) free(t.lexema);

    } while (t.identificador != EOF);
}