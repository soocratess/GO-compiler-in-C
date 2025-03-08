#include <stdlib.h>
#include <stdio.h>
// #include "Error/error.h"
#include "sistemaEntrada.h"
#include "definiciones.h"

#include <string.h>

#define TAM_LEXEMA 64 // Tamaño máximo de un lexema
#define TAM_BUFFER (2 * TAM_LEXEMA + 2) // Tamaño del buffer de lectura (doble centinela)
#define MITAD_BUFFER ((TAM_BUFFER / 2) - 1) // Tamaño del buffer de lectura simple
#define IZDA 0 // Lado izquierdo del buffer
#define DCHA 1 // Lado derecho del buffer

// Estructuras de datos

// Estructura de buffer doble centinela (1 buffer físico con 2 divisiones lógicas)
typedef struct dobleCentinela {
    char buffer[TAM_BUFFER]; // Buffer de lectura
    int inicio; // Puntero al inicio del buffer
    int delantero; // Puntero al carácter actual
    int cargas; // Cantidad de cargas del buffer
    FILE *fichero; // Puntero al fichero
};

struct dobleCentinela *ST; // Sistema de entrada (Variable global)

// Declaración de funciones
void _cargarBuffer(int lado); // Cargar la siguiente sección del buffer
char *_copiar_lexema(int inicio, int fin); // Copiar un fragmento del buffer
char *_caso_lexema_no_cabe(); // El lexema no cabe en un solo buffer
char *_caso_misma_mitad(); // Ambos punteros están en la misma mitad del buffer
char *_caso_diferentes_mitad_1(); // Los punteros están en diferentes mitades del buffer
char *_caso_diferentes_mitad_2(); // Inicio está después de la mitad y delantero está al principio
char *_caso_punteros_separados_inicio_medio(); // Los punteros están separados y delantero está en MITAD_BUFFER + 1
char *_caso_punteros_separados_inicio_fin(); // Inicio está después de la mitad y delantero está en la posición 0
char *_caso_delantero_medio_o_final(); // Delantero está en el medio o al final del buffer

void crearSistemaEntrada(char *rutaFichero) {
    // Reservamos memoria para la estructura de doble buffer
    ST = NULL;
    ST = (struct dobleCentinela *) malloc(sizeof(struct dobleCentinela));
    if (ST == NULL) {
        // Comprobamos que se haya reservado memoria
        perror("Error al reservar memoria para el sistema de entrada");
        exit(EXIT_FAILURE);
    }

    // Limpiamos la estructura y la inicializamos
    for (int i = 0; i < TAM_BUFFER; i++) ST->buffer[i] = '\0';
    ST->buffer[TAM_BUFFER - 1] = FIN; // Centinela final
    ST->buffer[TAM_LEXEMA] = FIN; // Centinela intermedio
    ST->inicio = 0; // Inicializamos los punteros y el archivo
    ST->delantero = 0;
    ST->cargas = 0;
    ST->fichero = fopen(rutaFichero, "r");
    if (ST->fichero == NULL) {
        // Comprobamos que se haya abierto el fichero
        perror("Error al abrir el fichero");
        exit(EXIT_FAILURE);
    }

    // Cargamos la primera sección del buffer
    _cargarBuffer(IZDA);
}

void destruirSistemaEntrada() {
    // Cerramos el fichero y liberamos la memoria
    fclose(ST->fichero);
    ST->fichero = NULL;
    free(ST);
    ST = NULL;
}

char siguienteCaracter() {
    // Si el puntero delantero ha llegado al final del buffer, cargamos la siguiente sección
    if (ST->delantero == MITAD_BUFFER) _cargarBuffer(DCHA);
    else if (ST->delantero == TAM_BUFFER - 1) _cargarBuffer(IZDA);

    return ST->buffer[ST->delantero++];
}

void retrocederCaracter() {
    ST->delantero--;
    if (ST->delantero < 0) {
        // TODO: Comprobar qué debo hacer en este caso
        ST->delantero = 0;
    }
}

void notificarRecarga() {
    ST->cargas = 0; // Restablece la carga del buffer
}

void ignorarSecuencia() {
    ST->inicio = ST->delantero;
    if (ST->inicio == MITAD_BUFFER) ST->inicio = MITAD_BUFFER + 1;
    else if (ST->inicio == TAM_BUFFER - 1) ST->inicio = 0;
}


void avanzarPuntero() { // Avanzamos el puntero inicio
    ST->inicio++;
    ST->cargas = 0;

    // En caso de avanzar al FIN, lo saltamos
    if (ST->inicio == TAM_BUFFER - 1) ST->inicio = 0;
    else if (ST->inicio == MITAD_BUFFER) ST->inicio = MITAD_BUFFER + 1;
}

/**
 * Imprime el contenido actual del buffer, mostrando los caracteres almacenados,
 * así como la posición de los punteros 'inicio' y 'delantero'.
 */
void imprimirBuffer() {
    printf("\nContenido del buffer:\n");

    // Primera línea: índices de las posiciones del buffer
    for (int i = 0; i < TAM_BUFFER; i++) {
        printf("%3d ", i);
    }
    printf("\n");

    // Segunda línea: contenido del buffer
    for (int i = 0; i < TAM_BUFFER; i++) {
        if (ST->buffer[i] >= 32 && ST->buffer[i] <= 126) {
            // Imprime solo caracteres visibles
            printf(" %2c ", ST->buffer[i]);
        } else {
            if (ST->buffer[i] == '\n') printf(" \\n "); // Sustituye el salto de línea
            else if (ST->buffer[i] == '\t') printf(" \\t "); // Sustituye el tabulador
            else printf("  . "); // Sustituye caracteres no imprimibles con un punto
        }
    }
    printf("\n");

    // Tercera línea: posiciones de los punteros
    for (int i = 0; i < TAM_BUFFER; i++) {
        if (i == ST->inicio && i == ST->delantero) {
            printf(" I&D "); // Si ambos punteros están en la misma posición
        } else if (i == ST->inicio) {
            printf("  I  "); // Marca la posición del puntero 'inicio'
        } else if (i == ST->delantero) {
            printf("  D  "); // Marca la posición del puntero 'delantero'
        } else {
            printf("     ");
        }
    }
    printf("\n");
}

/**
 * Función principal para obtener el lexema actual del buffer.
 * Determina el caso correspondiente y devuelve el lexema en una cadena de caracteres.
 */
char *obtenerLexema() {
    char *lexema = NULL; // Puntero para almacenar el lexema resultante
    int bandera = 1;     // Bandera para determinar si se debe actualizar ST->inicio

    // Caso 1: Si ha habido múltiples cargas del buffer, el lexema no cabe en una sola lectura
    if (ST->cargas > 1) {
        lexema = _caso_lexema_no_cabe();
    }
    // Caso 2: Ambos punteros (inicio y delantero) están en la misma mitad del buffer
    else if ((ST->delantero < MITAD_BUFFER && ST->inicio < MITAD_BUFFER) || (
                   ST->delantero > MITAD_BUFFER && ST->inicio > MITAD_BUFFER)) {
        lexema = _caso_misma_mitad();
    }
    // Caso 3: inicio en la primera mitad, delantero en la segunda (cruza MITAD_BUFFER + 1)
    else if (ST->inicio < MITAD_BUFFER && ST->delantero > MITAD_BUFFER + 1) {
        lexema = _caso_diferentes_mitad_1();
    }
    // Caso 4: inicio en la segunda mitad, delantero en la primera (pero no al inicio)
    else if (ST->inicio > MITAD_BUFFER && ST->delantero != 0 && ST->delantero < MITAD_BUFFER) {
        lexema = _caso_diferentes_mitad_2();
    }
    // Caso 5: inicio en la primera mitad, delantero en la posición exacta MITAD_BUFFER + 1
    else if (ST->inicio < MITAD_BUFFER && ST->delantero == MITAD_BUFFER + 1) {
        lexema = _caso_punteros_separados_inicio_medio();
    }
    // Caso 6: inicio en la segunda mitad, delantero en la posición 0 (inicio de buffer)
    else if (ST->inicio > MITAD_BUFFER && ST->delantero == 0) {
        lexema = _caso_punteros_separados_inicio_fin();
    }
    // Caso 7: delantero está en la posición MITAD_BUFFER o en la última posición del buffer
    else if (ST->delantero == MITAD_BUFFER || ST->delantero == TAM_BUFFER - 1) {
        lexema = _caso_delantero_medio_o_final();
        bandera = 0; // Evita la reasignación de ST->inicio porque ya se hizo en la función
    }

    // Si bandera sigue en 1, actualizamos ST->inicio al puntero delantero
    if (bandera) ST->inicio = ST->delantero; // Actualiza ST->inicio solo si no fue actualizado antes
    ST->cargas = 0; // Restablece la carga del buffer
    return lexema;
}


/**
 * Función auxiliar para copiar un fragmento del buffer y devolverlo como cadena de caracteres.
 * @param inicio Índice inicial en el buffer.
 * @param fin Índice final en el buffer.
 * @return Cadena de caracteres con el lexema copiado.
 */
char *_copiar_lexema(int inicio, int fin) {
    int longitud = fin - inicio;
    char *lexema = (char *) malloc((longitud + 1) * sizeof(char));

    if (lexema == NULL) {
        return NULL; // Manejo de error en caso de fallo en la reserva de memoria
    }

    for (int i = 0; i < longitud; i++) {
        lexema[i] = ST->buffer[inicio + i]; // Copia los caracteres al nuevo buffer
    }

    lexema[longitud] = '\0'; // Agrega el carácter nulo al final
    return lexema;
}

/**
 * El lexema no cabe en un solo buffer (ST->carga > 1).
 * Se devuelve solo la parte final del lexema.
 */
char *_caso_lexema_no_cabe() {
    // TODO: Notifica que el lexema no cabe en el buffer
    perror("El lexema no cabe en el buffer");

    int longitud, inicio = 0;

    // Determinar la longitud y el inicio del lexema
    if (ST->delantero > MITAD_BUFFER) {
        longitud = ST->delantero - MITAD_BUFFER - 1;
        inicio = MITAD_BUFFER + 1;
    } else longitud = ST->delantero;

    // Copiar el lexema
    return _copiar_lexema(inicio,+ longitud);
}

/**
 * Ambos punteros (inicio y delantero) están en la misma mitad del buffer.
 */
char *_caso_misma_mitad() {
    return _copiar_lexema(ST->inicio, ST->delantero);
}

/**
 * Los punteros están en diferentes mitades del buffer.
 * Se concatena la parte anterior al medio con la parte posterior al medio.
 */
char *_caso_diferentes_mitad_1() {
    int longitud = ST->delantero - ST->inicio - 1;
    char *lexema = (char *) malloc((longitud + 1) * sizeof(char));

    if (lexema == NULL) return NULL; // Manejo de error posible

    // Copiamos los caracteres
    int aux = 0; // Índice auxiliar para copiar los caracteres
    for (int i = ST->inicio; i < MITAD_BUFFER; i++) lexema[aux++] = ST->buffer[i];
    for (int i = MITAD_BUFFER + 1; i < ST->delantero; i++) lexema[aux++] = ST->buffer[i];

    // Agregamos el carácter nulo al final
    lexema[longitud] = '\0';
    return lexema;
}

/**
 * El inicio está después de la mitad y el delantero está al principio.
 */
char *_caso_diferentes_mitad_2() {
    int longitud = (TAM_BUFFER - ST->inicio - 1) + ST->delantero;
    char *lexema = (char *) malloc((longitud + 1) * sizeof(char));

    if (lexema == NULL) return NULL; // Manejo de error posible

    // Copiamos los caracteres
    int aux = 0;
    for (int i = ST->inicio; i < TAM_BUFFER - 1; i++) lexema[aux++] = ST->buffer[i];
    for (int i = 0; i < ST->delantero + 1; i++) lexema[aux++] = ST->buffer[i];

    // Agregamos el carácter nulo al final
    lexema[longitud] = '\0';
    return lexema;
}

/**
 * Los punteros están separados y el delantero está en MITAD_BUFFER + 1.
 */
char *_caso_punteros_separados_inicio_medio() {
    return _copiar_lexema(ST->inicio, MITAD_BUFFER);
}

/**
 * El inicio está después de la mitad y el delantero está en la posición 0.
 */
char *_caso_punteros_separados_inicio_fin() {
    return _copiar_lexema(ST->inicio, TAM_BUFFER - 1);
}

/**
 * El delantero está en el medio o al final del buffer.
 */
char *_caso_delantero_medio_o_final() {
    char *lexema = _copiar_lexema(ST->inicio, ST->delantero);
    ST->inicio = (ST->delantero == MITAD_BUFFER) ? MITAD_BUFFER + 1 : 0;
    return lexema;
}

void _cargarBuffer(int lado) {
    // Determinar el inicio y fin de la parte del buffer a limpiar y cargar
    int inicio, fin;

    if (lado == DCHA) {
        inicio = MITAD_BUFFER + 1;
        fin = TAM_BUFFER - 1;
    } else {  // lado == IZDA
        inicio = 0;
        fin = MITAD_BUFFER;
    }

    // Limpiar la parte del buffer antes de cargar datos
    memset(&ST->buffer[inicio], '\0', (fin - inicio));

    // Leer del archivo en la parte correspondiente del buffer
    size_t bytesLeidos = fread(&ST->buffer[inicio], 1, TAM_LEXEMA, ST->fichero);

    // Si no se han podido leer más datos, marcamos FIN en el buffer
    if (bytesLeidos < TAM_LEXEMA) {
        ST->buffer[inicio + bytesLeidos] = FIN;
    }

    // Ajustar el puntero "delantero" al inicio de la sección cargada
    ST->delantero = inicio;

    // Incrementar el contador de cargas
    ST->cargas++;
}