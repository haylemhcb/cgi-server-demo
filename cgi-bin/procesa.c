#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 1024

void print_header() {
    printf("Content-Type: text/html\n\n");
}

void parse_input(char *input) {
    char *nombre = NULL;
    char *email = NULL;

    // Parsear el input
    char *token = strtok(input, "&");
    while (token != NULL) {
        if (strncmp(token, "nombre=", 7) == 0) {
            nombre = token + 7; // Saltar "nombre="
        } else if (strncmp(token, "email=", 6) == 0) {
            email = token + 6; // Saltar "email="
        }
        token = strtok(NULL, "&");
    }

    // Decodificar URL
    if (nombre) {
        for (char *p = nombre; *p; p++) {
            if (*p == '+') *p = ' ';
            if (*p == '%') {
                int value;
                sscanf(p + 1, "%2x", &value);
                *p = (char)value;
                memmove(p + 1, p + 3, strlen(p + 3) + 1);
            }
        }
    }
    if (email) {
        for (char *p = email; *p; p++) {
            if (*p == '+') *p = ' ';
            if (*p == '%') {
                int value;
                sscanf(p + 1, "%2x", &value);
                *p = (char)value;
                memmove(p + 1, p + 3, strlen(p + 3) + 1);
            }
        }
    }

    // Imprimir resultados
    printf("<html><body>\n");
    printf("<h1>Datos recibidos</h1>\n");
    printf("<p>Nombre: %s</p>\n", nombre ? nombre : "No proporcionado");
    printf("<p>Email: %s</p>\n", email ? email : "No proporcionado");
    printf("</body></html>\n");
}

int main() {
    char *lenstr;
    int len;

    // Obtener la longitud del contenido
    lenstr = getenv("CONTENT_LENGTH");
    if (lenstr == NULL) {
        len = 0;
    } else {
        len = atoi(lenstr);
    }

    // Leer el input
    char input[MAX_INPUT];
    if (len > 0 && len < MAX_INPUT) {
        fread(input, 1, len, stdin);
        input[len] = '\0'; // Asegurarse de que la cadena esté terminada
    } else {
        input[0] = '\0';
    }

    // Imprimir cabecera
    print_header();

    // Procesar el input
    parse_input(input);

    return 0;
}
