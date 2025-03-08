#ifndef ANALIZADORLEXICO_H
#define ANALIZADORLEXICO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "definiciones.h"
#include "../TablaSimbolos/tablaSimbolos.h"
#include "../HashTable/hashTable.h"

int siguienteComponenteLexico(token *componenteLexico);

#endif // ANALIZADORLEXICO_H