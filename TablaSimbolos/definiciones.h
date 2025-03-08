#ifndef DEFINICIONES_H
#define DEFINICIONES_H

// Palabras clave de Go (asignadas desde 300 en adelante)
#define BREAK        300
#define CASE         301
#define CHAN         302
#define CONST        303
#define CONTINUE     304
#define DEFAULT      305
#define DEFER        306
#define ELSE         307
#define FALLTHROUGH  308
#define FOR          309
#define FUNC         310
#define GO           311
#define GOTO         312
#define IF           313
#define IMPORT       314
#define INTERFACE    315
#define MAP          316
#define PACKAGE      317
#define RANGE        318
#define RETURN       319
#define SELECT       320
#define STRUCT       321
#define SWITCH       322
#define TYPE         323
#define VAR          324

// Otros tokens léxicos
#define ID           325  // Identificadores (nombres de variables, funciones, etc.)
#define NUMERO       326  // Números enteros y flotantes
#define STRING       327  // Cadenas de texto
#define OPERADOR     328  // Operadores (+, -, *, /, etc.)
#define DELIMITADOR  329  // Delimitadores como {}, (), [], ;
#define COMENTARIO   330  // Comentarios (// y /* ... */)
#define FIN          EOF  // Fin del archivo

#endif // DEFINICIONES_H