#include "hashTable.h"

// --------------------- CONSTANTES Y VARIABLES --------------------- //

#define primoFuncionHash 33 // Número primo recomendado

#define ERROR_GENERAL -1
#define EXITO 0


int TAMANHO_TABLA;
int NUMERO_ELEMENTOS;

// --------------------- FUNCIONES PRIVADAS --------------------- //
unsigned int hash(char *string) {
    if (!string) return 0;  // Manejo de NULL

    unsigned int hash = 5381;  // Mejor valor inicial (DJB2)

    // Guardar la longitud antes del bucle para evitar múltiples llamadas a strlen()
    size_t len = strlen(string);
    for (size_t i = 0; i < len; i++) {
        hash = (hash * primoFuncionHash) ^ string[i];  // Multiplicación + XOR
    }

    return hash % TAMANHO_TABLA;  // Ajuste al tamaño de la tabla hash
}


// --------------------- FUNCIONES PÚBLICAS --------------------- //

// Inicialización de la tabla de hash con tamaño dinámico.
int crearHashTable(hashTable *tabla, int size){

    *tabla = (token **) malloc (sizeof(token*) * size);

    if (*tabla == NULL) {
        perror("Fallo a la hora de asignar memoria para la tabla de hash\n");
        return ERROR_GENERAL;
    }

    for (int i = 0; i < size; i++){
        (*tabla)[i] = NULL;
    }

    TAMANHO_TABLA = size;
    NUMERO_ELEMENTOS = 0;

    return EXITO;
}



// Libera los recursos asociados con la tabla de hash.
int destruirHashTable(hashTable *tabla){

    if(*tabla == NULL){
        printf("ERROR: La tabla no existe\n");
        return ERROR_GENERAL;
    }
    for (int i = 0; i < TAMANHO_TABLA; i++) {
        if (tabla[i] != NULL) {

            // Para cada elemento de la tabla hay una lista enlazada de tokens
            token *actual = (*tabla)[i];

            while (actual != NULL) {
                token *siguiente = actual->next;

                free(actual->lexema); // Libera el lexema del token

                free(actual); // Libera el token

                actual = siguiente;

            }
        }
    }

    free(*tabla);
    *tabla = NULL;

    return EXITO;
}

// Ajusta el tamaño de la tabla para mantener el rendimiento óptimo
int ajustarTamanhoHashTable(hashTable *tabla, int newSize) {

    // Paso 1: Crear una nueva tabla de hash del nuevo tamaño que sera nuestra nueva tabla

    hashTable nuevaTabla = malloc (sizeof(token *) * newSize);

    if (nuevaTabla == NULL) {
        printf("ERROR: Fallo al asignar nueva tabla de hash\n");
        return ERROR_GENERAL;
    }

    for (int i = 0; i < newSize; i++) {
        nuevaTabla[i] = NULL;
    }

    // Paso 2: Rehashear e insertar cada token de la tabla antigua en la nueva tabla

    int oldSize = TAMANHO_TABLA;
    TAMANHO_TABLA = newSize;

    for (int i = 0; i < oldSize; i++) {

        token *actual = (*tabla)[i];

        while (actual != NULL) {
            // Guarda el próximo token antes de modificar el actual
            token *siguiente = actual->next;

            // Calcula el nuevo índice para el token actual basado en el nuevo tamaño de tabla
            int nuevoIndice = hash(actual->lexema);

            // Inserta el token en la nueva tabla (al principio de la lista en ese índice)
            actual->next = nuevaTabla[nuevoIndice];
            nuevaTabla[nuevoIndice] = actual;

            // Avanza al siguiente token en la lista original
            actual = siguiente;
        }
    }

    // No se liberan los elementos, solo la tabla antigua
    free(*tabla);

    *tabla = nuevaTabla;
    TAMANHO_TABLA = newSize;

    return EXITO;
}

// Muestra el contenido actual de la tabla para depuración
void imprimirHashTable(hashTable tabla) {
    printf("\nSTART\n");


    for (int index = 0; index < TAMANHO_TABLA; index++) {
        printf("%d\t", index);
        if (tabla[index] == NULL) {
            printf("---\n"); // Indica que no hay token en esta posición
        }

        else {
            // Recorre la lista enlazada en esta posición de la tabla
            token *actual = tabla[index];
            while (actual != NULL) {
                printf("( %s | %d ) -> ", actual->lexema, actual->identificador);
                actual = actual->next;
            }
            printf("\n");
        }
    }

    printf("END\n");
}

// Inserta un nuevo token, redimensionando la tabla si es necesario
int insertarToken(hashTable *tabla, char *lexema, int identificador){

    if (*tabla == NULL) return ERROR_GENERAL;

    int index = hash(lexema);

    token *nuevoToken = (token *) malloc (sizeof(token));

    if (nuevoToken == NULL) return ERROR_GENERAL;

    // Implementamos el encadenamiento en tablas hash (mas eficiente) debido a las restricciones
    // del sistema
    nuevoToken->lexema = strdup(lexema);
    nuevoToken->identificador = identificador;
    nuevoToken->next = NULL;

    // Si hay colisión, insertamos el nuevo token al principio de la lista enlazada
    if ((*tabla)[index] == NULL) {
        (*tabla)[index] = nuevoToken;
    } else {
        nuevoToken->next = (*tabla)[index];
        (*tabla)[index] = nuevoToken;
    }

    NUMERO_ELEMENTOS++;

    // Redimensionamos la tabla si el factor de carga supera 0.75
    if(NUMERO_ELEMENTOS/TAMANHO_TABLA > 0.75){
        ajustarTamanhoHashTable(tabla, TAMANHO_TABLA * 2);
    }

    return EXITO;
}

// Busca un token por su lexema y devuelve su identificador si existe
int buscarToken(hashTable tabla, char *lexema) {

    int index = hash(lexema);

    token *actual = tabla[index];

    // Recorremos la lista enlazada en la posición de la tabla
    while (actual != NULL) {
        if (strcmp(actual->lexema, lexema) == 0) {
            return actual->identificador;
        }

        actual = actual->next;
    }

    return ERROR_GENERAL;
}

// Elimina un token específico si se encuentra en la tabla
int eliminarToken(hashTable *tabla, char *lexema) {

    int index = hash(lexema);

    token *actual = (*tabla)[index];
    token *anterior = NULL;

    while (actual != NULL) {
        if (strcmp(actual->lexema, lexema) == 0) {
            if (anterior == NULL) {
                (*tabla)[index] = actual->next;
            } else {
                anterior->next = actual->next;
            }

            free(actual->lexema);
            free(actual);

            return ERROR_GENERAL;
        }

        anterior = actual;
        actual = actual->next;
    }

    return EXITO;
}

// Actualiza el identificador de un token existente
int modificarToken(hashTable *tabla, char * lexema, int identificador){

    int identificadorAux = buscarToken(*tabla, lexema);

    if(identificadorAux == 0) return ERROR_GENERAL;
    else {

        eliminarToken(tabla, lexema);

        insertarToken(tabla, lexema, identificador);
        
        return EXITO;
    } 
}

// Devuelve el número de elementos en la tabla
int cantidadElementosHashTable(hashTable tabla) {
    return NUMERO_ELEMENTOS;
}

// Devuelve el tamaño actual de la tabla
int tamanhoHashTable(hashTable tabla) {
    return TAMANHO_TABLA;
}