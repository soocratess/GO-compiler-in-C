#include "analizadorSintactico.h"

#include <AnalizadorLexico/analizadorLexico.h>

// Imprime el componente léxico en la consola.
void imprimirComponenteLexico(token t){
    printf("%-33s %-10d\n", t.lexema, t.identificador);
}

void iniciarAnalisis(){
    token t;

    printf("%-33s %-10s\n", "Lexema", "Identificador");
    printf("--------------------------------- ----------\n");

    // Solicitamos componentes léxicos al analizador léxico hasta que reciba FIN.
    do {
        printf("--------------------------------- ----------\n");

        // Se solicita el siguiente componente léxico.
        int liberarMemoria = siguienteComponenteLexico(&t);
        
        // Si no se ha llegado al final del fichero, se imprime el componente léxico.
        if (t.identificador != FIN) imprimirComponenteLexico(t);
        
        // Se libera la memoria del lexema si se ha solicitado.
        if (liberarMemoria) free(t.lexema);

    } while (t.identificador != FIN);
}