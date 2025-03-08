#include "analizadorLexico.h"
#include "SistemaEntrada/sistemaEntrada.h"
#include "definiciones.h"
#include "TablaSimbolos/tablaSimbolos.h"

// Variable global para contar líneas
int num_linea = 0;

// TODO: Implementar estructura para estados de los autómatas. Ejemplo:
// Estados simbólicos para literales enteros
typedef enum {
    ST_INICIO = 0, // Leímos '0' o un dígito [1-9]
    ST_DEC, // Lectura decimal
    ST_OCT, // Lectura octal (0o, 0 y dígitos 0..7)
    ST_BIN, // Lectura binaria (0b)
    ST_HEX, // Lectura hex (0x)
    ST_ERROR, // Estado de error (opcional)
} NumState;

// Declaración de funciones
void _identificador(token *componenteLexico);

void _numero(char c, token *componenteLexico);

void _delimitador(char c, token *componenteLexico);

void _comentario();

void _operador(char c, token *componenteLexico);

void _string(char c, token *componenteLexico);

void _finFichero(token *componenteLexico);

int tipoCaracterInicial(char c);

int siguienteComponenteLexico(token *componenteLexico) {
    char c;
    int estado;

    // Avanzamos el puntero hasta encontrar un caracter válido
    do {
        c = siguienteCaracter();
        estado = tipoCaracterInicial(c);
        if (estado == 6) {
            avanzarPuntero();
        }
    } while (estado == 6); // Espacios en blanco

    // Avanzamos al autómata correspondiente
    switch (estado) {
        case 0: // Identificador
            _identificador(componenteLexico);
            break;
        case 1: // Número
            _numero(c, componenteLexico);
            break;
        case 2: // Delimitador
            _delimitador(c, componenteLexico);
            break;
        case 3: // Comentario
            break;
        case 4: // Operador
            _operador(c, componenteLexico);
            break;
        case 5: // String
            _string(c, componenteLexico);
            break;
        case 6: // Espacios en blanco o salto de línea
            avanzarPuntero();
            break;
        case 7: // Caracter no reconocido
            // TODO: Lanzar excepción caracter no reconocido
            avanzarPuntero();
            break;
        case 8: // Fin de fichero
            componenteLexico->lexema = obtenerLexema();
            componenteLexico->identificador = FIN;
            break;
    }
// TODO: Falta puntoycoma, el identificador hacer salida a keyword, operadorypuntuacion, en numeros ver si hay imaginarios comentarios y strings
}

/**
 * @brief Devuelve el tipo de caracter inicial
 *
 * @param c Caracter a analizar
 * @return int Tipo de caracter inicial
 */
int tipoCaracterInicial(char c) {
    if (isalpha(c) || c == '_') return 0; // 0. Identificadores
    if (isdigit(c)) return 1; // 1. Números: dígitos
    if (c == '.' || c == ',' || c == ';' || c == ':' || c == '(' ||
        c == ')' || c == '[' || c == ']' || c == '{' || c == '}' ||
        c == '~')
        return 2; // 2. Delimitadores
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '&' ||
        c == '|' || c == '^' || c == '!' || c == '=' || c == '<' || c == '>' ||
        c == '#' || c == '@' || c == '$')
        return 4; // 4. Operadores
    if (c == '\'' || c == '\"' || c == '`') return 5; // 5. Strings
    if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
        if (c == '\n') num_linea++;
        return 6; // 6. Espacios en blanco
    }
    if (c == EOF || c == '\0') return 8; // 8. Fin de fichero
    return 7; // 7. Caracter no reconocido
}

void _numero(char c, token *componenteLexico) { //TODO: Falta poner nums imaginarios
    // Flags
    int fin = 0; // Flag para terminar la función
    int letra_octal = 0; // Flag para saber si es octal

    // Estado y transiciones del autómata
    int estadoAutomata = c == '0' ? 0 : c == '.' ? 6 : 1;

    char anterior = '\0'; // Caracter anterior
    char siguiente = '\0'; // Caracter siguiente

    // Transiciones del autómata
    while (!fin) {
        anterior = siguiente;
        siguiente = siguienteCaracter();

        // Si hay un '_' inmediatamente después de otro '_', lanzamos error
        if (anterior == '_' && siguiente == '_') {
            // TODO: Lanzar error doble '_'
            fin = 1;
        }

        // TODO: Hay que poner fin = 1 en cada error?? (ej// 6)
        switch (estadoAutomata) {
            case 0: // Empieza por 0 -> Deriva al subautómata correspondiente
                if (siguiente == 'o' || siguiente == 'O') {
                    letra_octal = 1;
                    estadoAutomata = 2;
                } // Octal
                else if (siguiente < '8' && siguiente >= '0' || siguiente == '_') estadoAutomata = 2; // Octal
                else if (siguiente == 'b' || siguiente == 'B') estadoAutomata = 3; // Binario
                else if (siguiente == 'x' || siguiente == 'X') estadoAutomata = 4; // Hexadecimal
                else if (siguiente == '8' || siguiente == '9') estadoAutomata = 5; // Float (parte entera)
                else if (siguiente == '.') estadoAutomata = 6; // Float (parte decimal)
                else if (siguiente == 'e' || siguiente == 'E') estadoAutomata = 7; // Float (parte exponencial)
                else {
                    estadoAutomata = 1;
                    fin = 1;
                } // Fin (0 es decimal)
                break;

            case 1: // Es decimal [1-9]
                // Si no es un dígito o un '_', finalizamos o avanzamos al subautómata de float correspondiente
                if (!(isdigit(siguiente) || siguiente == '_')) {
                    // Avanzamos al subautómata de float correspondiente
                    if (siguiente == '.') estadoAutomata = 6; // Decimal
                    else if (siguiente == 'e' || siguiente == 'E') estadoAutomata = 7; // Exponencial
                    else fin = 1; // Fin
                }
                break;

            case 2: // Es octal
                // Si no es un dígito octal o un '_', finalizamos o avanzamos al subautómata de float correspondiente
                if (!(siguiente == '_' || (siguiente < '8' && siguiente >= '0'))) {
                    if ((siguiente == '8' || siguiente == '9') && !letra_octal) estadoAutomata = 5; // Float
                    else if (siguiente == '.' && !letra_octal) estadoAutomata = 6; // Decimal
                    else if (siguiente == 'e' || siguiente == 'E') estadoAutomata = 7; // Exponencial
                    else fin = 1; // Fin
                }
                break;

            case 3: // Es binario
                // Si no es un dígito binario o un '_', finalizamos
                if (!(siguiente == '0' || siguiente == '1' || siguiente == '_')) fin = 1;
                break;

            case 4: // Es hexadecimal
                // Si no es un dígito hexadecimal o un '_', finalizamos
                if (!(siguiente == '_' || (siguiente >= '0' && siguiente <= '9') ||
                      (siguiente >= 'A' && siguiente <= 'F') || (siguiente >= 'a' && siguiente <= 'f')))
                    fin = 1;
                break;

            // FLOAT
            case 5: // Parte entera de float
                // Si no es un dígito o un '_', finalizamos o avanzamos al subautómata de float correspondiente
                if (!(isdigit(siguiente) || siguiente == '_')) {
                    if (siguiente == '.') estadoAutomata = 6; // Decimal
                    else if (siguiente == 'e' || siguiente == 'E') estadoAutomata = 7; // Exponencial
                    else fin = 1; // Fin
                }
                break;

            case 6: // Parte decimal de float
                // Si no es un dígito o un '_', finalizamos o avanzamos al subautómata de parte exponencial
                if (!(isdigit(siguiente) || siguiente == '_')) {
                    if (siguiente == 'e' || siguiente == 'E') estadoAutomata = 7; // Exponencial
                    else fin = 1;
                }
                // Si hay un '_' inmediatamente después de '.', lanzamos error
                else if (anterior == '.' && siguiente == '_') {
                    // TODO: Lanzar error '_' después de '.'
                    fin = 1;
                }
                break;
            case 7: // Parte exponencial de float
                // Si no es un dígito o un '_', finalizamos o aceptamos signo
                if (!(isdigit(siguiente) || siguiente == '_')) {
                    // Si no es un '+' o '-' después de 'e' o 'E', lanzamos error
                    if (!((siguiente == '+' || siguiente == '-') && (anterior == 'e' || anterior == 'E'))) fin = 1;
                }
                break;

            default:
                // TODO: Lanzar excepción estado no reconocido
                break;
        }
    }

    // TODO: Dividir errores de formato en finalización de número
    // Errores de formato en finalización de número
    if (anterior != '\0' && (anterior == 'x' || anterior == 'X' || anterior == 'b' || anterior == 'B' || anterior == 'o'
                             || anterior == 'O' || anterior == 'e' || anterior == 'E' || anterior == '+' ||
                             anterior == '-' || anterior == '_' || estadoAutomata == 5)
    ) {
        // TODO: Lanzar excepción de formato de número
    }

    // Retrocedemos el caracter que no pertenece al número
    retrocederCaracter();

    // Registramos el componente léxico
    componenteLexico->lexema = obtenerLexema();
    componenteLexico->identificador = NUMERO;
}

void _delimitador(char c, token *componenteLexico) {
    if (c == '.') {
        char siguiente = siguienteCaracter();
        if (isdigit(siguiente)) _numero(siguiente, componenteLexico); // Número
        else {
            retrocederCaracter(); // Retrocedemos el caracter
            componenteLexico->lexema = obtenerLexema(); // Componente léxico
            componenteLexico->identificador = DELIMITADOR; // Identificador
        }
    } else {
        componenteLexico->lexema = obtenerLexema(); // Componente léxico
        componenteLexico->identificador = DELIMITADOR; // Identificador
    }
}

void _operador(char c, token *componenteLexico) {
    while (true) {
        if (c == '+' || c == '-') {
            c = siguienteCaracter();
            if (c != '+' && c != '-' && c != '=') retrocederCaracter();
            break;
        }
        else if (c == ':' || c == '*' || c == '%' || c == '|' || c == '^' || c == '!' || c == '=') {
            c = siguienteCaracter();
                if (c != '=') {
                    retrocederCaracter();
                }
            break;
        }
        else if (c == '/') {
            c = siguienteCaracter();

            if (c == '=') break;
            if (c == '/' || c == '*') _comentario();
            else retrocederCaracter();

            break;
        }
        else if (c == '<' || c == '>') {
            char operador = c;  // Guardamos el operador original ('<' o '>')
            c = siguienteCaracter();

            if (c == '=') break;
            if ((operador == '<' && c == '<') || (operador == '>' && c == '>')) {
                c = siguienteCaracter();
                if (c != '=') retrocederCaracter();
                break;
            }
            if (operador == '<' && c == '-') break;

            retrocederCaracter();
            break;
        }

        else if (c == '&') {
            c = siguienteCaracter();

            if (c == '&' || c == '=') break;
            if (c == '^') {
                c = siguienteCaracter();
                if (c != '=') retrocederCaracter();
                break;
            }

            retrocederCaracter();
            break;
        }

    }
    componenteLexico->lexema = obtenerLexema();
    componenteLexico->identificador = OPERADOR;
}

void _comentario() {
    int fin = 0;
    char siguiente = '\0';

    while (!fin) {
        siguiente = siguienteCaracter(); // Leemos el siguiente caracter
        if (siguiente == '\n' || siguiente == FIN) {
            num_linea++;
            fin = 1;
        }
    }
    avanzarPuntero(); // Avanzamos el puntero
    notificarRecarga(); // Notificamos que se ha recargado el buffer
}

void _identificador(token *componenteLexico) {
    char siguiente = '\0';
    int fin = 0;

    while (!fin) {
        siguiente = siguienteCaracter();
        if (!isalnum(siguiente) && siguiente != '_') fin = 1;
    }

    retrocederCaracter(); // Retrocedemos el caracter que no pertenece al identificador
    componenteLexico->lexema = obtenerLexema(); // Componente léxico
    componenteLexico->identificador = buscarElemento(componenteLexico->lexema); // Buscamos/Registramos el elemento en la tabla de símbolos

}

void _string(char c, token *componenteLexico) {
    if (c == '"') { // Comienza una cadena con comillas dobles
        char siguiente;
        while ((siguiente = siguienteCaracter()) != '"' && siguiente != EOF) {
            if (siguiente == '\\') {
                siguienteCaracter(); // Saltar el carácter escapado
            }
        }
        if (siguiente == '"') {
            componenteLexico->lexema = obtenerLexema();
            componenteLexico->identificador = STRING;
        } else {
            // Manejo de error: cadena no cerrada
        }
    } else if (c == '`') { // Comienza una cadena con comillas invertidas (raw string)
        char siguiente;
        while ((siguiente = siguienteCaracter()) != '`' && siguiente != EOF);
        if (siguiente == '`') {
            componenteLexico->lexema = obtenerLexema();
            componenteLexico->identificador = STRING;
        } else {
            // Manejo de error: cadena no cerrada
        }
    }
}

