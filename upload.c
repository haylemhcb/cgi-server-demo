#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

void print_header() {
    printf("Content-Type: text/html\n\n");
}

int main() {
    char *content_length_str = getenv("CONTENT_LENGTH");
    int content_length = content_length_str ? atoi(content_length_str) : 0;

    if (content_length <= 0) {
        print_header();
        printf("<html><body><h1>Error: No file uploaded.</h1></body></html>");
        return 1;
    }

    // Leer el contenido del formulario
    char *buffer = malloc(content_length);
    fread(buffer, 1, content_length, stdin);

    // Buscar el nombre del archivo en el contenido
    char *file_name_start = strstr(buffer, "filename=\"");
    if (!file_name_start) {
        print_header();
        printf("<html><body><h1>Error: No file name provided.</h1></body></html>");
        free(buffer);
        return 1;
    }
    file_name_start += 10; // Mover el puntero después de "filename=\""
    char *file_name_end = strchr(file_name_start, '\"');
    if (!file_name_end) {
        print_header();
        printf("<html><body><h1>Error: Invalid file name.</h1></body></html>");
        free(buffer);
        return 1;
    }

    // Extraer el nombre del archivo
    *file_name_end = '\0'; // Terminar la cadena
    char file_path[BUFFER_SIZE];
    snprintf(file_path, sizeof(file_path), "./upload/%s", file_name_start);

    // Encontrar el inicio del contenido del archivo
    char *file_content_start = strstr(file_name_end + 1, "\r\n\r\n");
    if (!file_content_start) {
        print_header();
        printf("<html><body><h1>Error: Invalid file content.</h1></body></html>");
        free(buffer);
        return 1;
    }
    file_content_start += 4; // Mover el puntero después de "\r\n\r\n"

    // Escribir el contenido del archivo
    FILE *fp = fopen(file_path, "wb");
    if (!fp) {
        print_header();
        printf("<html><body><h1>Error: Unable to open file for writing.</h1></body></html>");
        free(buffer);
        return 1;
    }

    fwrite(file_content_start, 1, content_length - (file_content_start - buffer), fp);
    fclose(fp);

    print_header();
    printf("<html><body><h1>File uploaded successfully: %s</h1></body></html>", file_path);
    free(buffer);
    return 0;
}
