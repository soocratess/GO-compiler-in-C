#ifndef SISTEMAENTRADA_H
#define SISTEMAENTRADA_H

/**
 * @brief Inicializa la estructura de sistema de entrada con un archivo de texto.
 *
 * Crea y configura internamente los buffers y punteros necesarios
 * para gestionar la lectura de caracteres, así como almacenar el
 * archivo que se va a procesar.
 *
 * @param rutaFichero Cadena con la ruta del fichero que se desea leer.
 */
void crearSistemaEntrada(char *rutaFichero);

/**
 * @brief Libera la memoria y recursos asociados al sistema de entrada.
 *f
 * Cierra el archivo si es necesario y destruye la estructura donde
 * se almacenaban los datos de buffer. Tras llamar a esta función,
 * no se debe volver a invocar funciones de lectura o acceso al buffer.
 */
void destruirSistemaEntrada();

/**
 * @brief Retorna el siguiente carácter del buffer y avanza el puntero "delantero".
 *
 * Si se alcanza el final de la parte de buffer cargada, se procede a recargar
 * automáticamente la siguiente sección. Devuelve un carácter (o un centinela
 * como EOF en caso de fin de archivo).
 *
 * @return El siguiente carácter de la entrada o EOF si ya no hay más caracteres.
 */
char siguienteCaracter();

/**
 * @brief Retrocede un carácter en el buffer, deshaciendo la última lectura.
 *
 * Esta función permite que el analizador léxico "devuelva" un carácter
 * cuando, al leerlo, concluye que no forma parte del lexema en curso.
 */
void retrocederCaracter(); // Devolver es más eficiente????

/**
 * @brief Construye y devuelve el lexema actual leído desde el último "inicio" hasta el "delantero".
 *
 * Reserva memoria dinámica y copia todos los caracteres que comprenden
 * el lexema en curso. Al finalizar, puede ajustar el puntero "inicio"
 * para el próximo token.
 *
 * @return Puntero a una cadena con el lexema. Debe liberarse o procesarse
 *         cuando ya no se necesite.
 */
char *obtenerLexema();

/**
 * @brief Muestra el contenido interno del buffer y/o punteros para propósitos de depuración.
 *
 * Imprime por consola el estado actual de los buffers, límites,
 * y cualquier otra información relevante para diagnosticar la lectura.
 */
void imprimirBuffer();

/**
 * @brief Notifica que se va a recargar (o se ha recargado) el buffer de lectura.
 *
 * Se utiliza para actualizar el estado interno relativo a la carga del buffer,
 * por ejemplo, para manejar la situación de un lexema muy largo que abarque
 * más de un bloque de buffer.
 */
void notificarRecarga();

/**
 * @brief Ignora o descarta de manera directa una secuencia de caracteres en el buffer.
 *
 * Suele utilizarse cuando se detecta que la porción leída corresponde
 * a espacios, saltos de línea o comentarios, y se desea avanzar
 * sin que esos caracteres formen parte de un token.
 */
void ignorarSecuencia();

/**
 * @brief Avanza el puntero de inicio al puntero delantero, confirmando el lexema.
 *
 * Tras reconocer un token, esta función marca que los caracteres
 * ya usados no se necesitan para el siguiente token; se suele hacer
 * estableciendo `inicio = delantero`.
 */
void avanzarPuntero();

#endif // SISTEMAENTRADA_H
