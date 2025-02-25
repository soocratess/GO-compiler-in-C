#ifndef DEFINICIONES_H
#define DEFINICIONES_H

// Palabras clave de Go (asignadas desde 300 en adelante)
#define BREAK        300
#define DEFAULT      301
#define FUNC         302
#define INTERFACE    303
#define SELECT       304
#define CASE         305
#define DEFER        306
#define GO           307
#define MAP          308
#define STRUCT       309
#define CHAN         310
#define ELSE         311
#define GOTO         312
#define PACKAGE      313
#define SWITCH       314
#define CONST        315
#define FALLTHROUGH  316
#define IF           317
#define RANGE        318
#define TYPE         319
#define CONTINUE     320
#define FOR          321
#define IMPORT       322
#define RETURN       323
#define VAR          324

// Otros tokens léxicos
#define ID           325  // Identificadores (nombres de variables, funciones, etc.)
#define NUMERO       326  // Números enteros y flotantes
#define STRING       327  // Cadenas de texto
#define OPERADOR     328  // Operadores (+, -, *, /, etc.)
#define DELIMITADOR  329  // Delimitadores como {}, (), [], ;
#define COMENTARIO   330  // Comentarios (// y /* ... */)
#define FIN          331  // Fin del archivo

#endif // DEFINICIONES_H