#include "analizadorLexico.h"
#include "SistemaEntrada/sistemaEntrada.h"

// Variable global para contar líneas
int num_lineas = 0;

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
    int error = 0;
    int estado;

    while (!error) {
        char c = siguienteCaracter();
        estado = tipoCaracterInicial(c);

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
            // case 3: // Comentario TODO: Integrar comentario en sección de operadores
            //     _comentario(componenteLexico);
            //     break;
            case 4: // Operador
                _operador(c, componenteLexico);
                break;
            case 5: // String
                _string(c, componenteLexico);
                break;
            case 6: // Espacio en blanco /Retorno de carro
                avanzarPuntero();
                break;
            case 7: // Caracter no reconocido
                // TODO: Lanzar excepción caracter no reconocido
                avanzarPuntero();
                break;
            case 8: // Fin de fichero
                _finFichero(componenteLexico);
                break;
        }
    }
}

/**
 * @brief Devuelve el tipo de caracter inicial
 *
 * @param c Caracter a analizar
 * @return int Tipo de caracter inicial
 */
int tipoCaracterInicial(char c) {
    // 8. Fin de fichero
    if (c == EOF || c == '\0') {
        return 8;
    }
    // 0. Identificadores: letra o guión bajo
    if (isalpha(c) || c == '_') {
        return 0;
    }
    // 1. Números: dígitos
    else if (isdigit(c)) {
        return 1;
    }
    // 2. Delimitadores en Go: paréntesis, llaves, corchetes, punto, coma, punto y coma, dos puntos...
    else if (c == '.' || c == ',' || c == ';' || c == ':' ||
             c == '(' || c == ')' || c == '[' || c == ']' ||
             c == '{' || c == '}') {
        return 2;
    }

    // // 3. Posible comentario: '/'
    // else if (c == '/') {
    //     return 3;
    // }

    // 4. Operadores de Go: +, -, *, /, %, &, |, ^, !, =, <, >
    else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' ||
             c == '&' || c == '|' || c == '^' || c == '!' ||
             c == '=' || c == '<' || c == '>') {
        return 4;
    }
    // 5. Strings: comillas simples, dobles y backticks
    else if (c == '\'' || c == '\"' || c == '`') {
        return 5;
    }
    // 6. Espacios en blanco: espacio, tab, salto de línea...
    else if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
        if (c == '\n') {
            num_lineas++;
        }
        return 6;
    }
    // 7. Caracter no reconocido
    return 7;
}

void _numero(char c, token *componenteLexico) {
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
