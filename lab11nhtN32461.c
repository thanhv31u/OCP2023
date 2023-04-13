#include <sys/types.h>
#include <sys/stat.h>
#include <fts.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <getopt.h>

bool has_substring(unsigned char* str1, unsigned char* str2);
void search_hex_in_file(char* file_path, unsigned char* hex_fin, char* DEBUG);
unsigned char* read_file_to_buffer(char* file_path, unsigned long* buffer_size);
unsigned char* convert_buffer_to_hex(unsigned char* buffer, unsigned long buffer_size);
int is_hexadecimal( char *str);
void process_options(int argc, char *argv[]);
int count =0;
int main(int argc, char *argv[])
{
    process_options(argc, argv);
    if ((argc ==1) || (argc > 3))
    {
        printf("Wrong number of arguments! \n\n");
        printf("Use: -h (--help) to see how the program works\n\n");
        return EXIT_FAILURE;
    }
    int len = strlen(argv[2]);
    if( len %2 ==1 || is_hexadecimal(argv[2]) ){
        printf("Wrong byte sequence input");
        return EXIT_FAILURE ;
    }

    char* DEBUG = getenv("LAB11DEBUG");
    if (DEBUG) {
        printf("*\n");
        printf("The program searches for the sequence of bytes %s in files, starting from the directory %s \n", argv[2], argv[1]);
        printf("*\n");
    }
    FTS *ftsp;
    FTSENT *p;
    char *paths[] = {argv[1], NULL};
    unsigned char* hex_find = (unsigned char*) argv[2];

    ftsp = fts_open(paths, FTS_PHYSICAL | FTS_NOCHDIR, NULL);

    if (ftsp == NULL)
    {
        perror("fts_open");
        return EXIT_FAILURE;
    }

    while ((p = fts_read(ftsp)) != NULL)
    {
        if (p->fts_info != FTS_D && strcmp(p->fts_name, ".") != 0)
        {
            char* file_path = p->fts_accpath;
            search_hex_in_file(file_path, hex_find, DEBUG);
        }
    }

    fts_close(ftsp);
    if (DEBUG){
        printf("*\n");
        printf("Number of files: %d.\n", count);
        printf("*\n");
        printf("The program has ended. \n");
    }
    return EXIT_SUCCESS;
}

// bool has_substring(unsigned char* str1, unsigned char* str2)
// {
//     size_t str1_len = strlen((const char*) str1);
//     size_t str2_len = strlen((const char*) str2);
//     if (str1_len < str2_len)
//     {
//         return false;
//     }
//     for (size_t i = 0; i <= str1_len - str2_len; i++)
//     {
//         if (memcmp(str1 + i, str2, str2_len) == 0)
//         {
//             return true;
//         }
//     }
//     return false;
// }

void search_hex_in_file(char* file_path, unsigned char* hex_find, char *DEBUG)
{
    unsigned long buffer_size;
    unsigned char* buffer = read_file_to_buffer(file_path, &buffer_size);

    if (buffer == NULL)
    {
        //printf("Error: cannot read file %s\n", file_path);
        //fprintf(stderr, "Error: cannot read file %s\n", file_path);
        return;
    }

    unsigned char* hex_data = convert_buffer_to_hex(buffer, buffer_size);
    unsigned char* hex_str = hex_find + 2;

   char* position = strstr((const char*)hex_data, (const char*)hex_str);

    if (position != NULL){
        if (DEBUG){
            size_t offset = ((unsigned char*) position - hex_data) / 2; // поиск смещения
            printf("%s is in file %s at offset %zu from the beginning of the file \n\n", hex_find, file_path, offset);
            count++;
        }
        else {
            printf("%s\n", file_path);
            count++;
        }
    }

    free(hex_data);
    free(buffer);
}

unsigned char* read_file_to_buffer(char* file_path, unsigned long* buffer_size)
{
    FILE* file = fopen(file_path, "rb");
    if (file == NULL)
    {
        //fprintf(stderr, "File opening failed %s: %s\n", file_path, strerror(errno));
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *buffer_size = ftell(file);
    rewind(file);

    unsigned char* buffer = (unsigned char*) malloc(*buffer_size);
    if (buffer == NULL)
    {
        //fprintf(stderr, "Failed to allocate memory for buffer: %s\n", strerror(errno));
        fclose(file);
        return NULL;
    }

    size_t result = fread(buffer, 1, *buffer_size, file);
    if (result != *buffer_size)
    {
        free(buffer);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return buffer;
}

unsigned char* convert_buffer_to_hex(unsigned char* buffer, unsigned long buffer_size)
{
    unsigned char* hex_data = (unsigned char*) malloc(2 * buffer_size + 1);
    if (hex_data == NULL)
    {
        //fprintf(stderr, "Failed to allocate memory for hex_data: %s\n", strerror(errno));
        return NULL;
    }

    unsigned char* p = hex_data;

    for (unsigned long i = 0; i < buffer_size; i++)
    {
        p += sprintf((char*) p, "%02x", buffer[i]);
    }

    *p = '\0';
    return hex_data;
}
int is_hexadecimal( char *str) {
    char *endptr;
    long value = strtol(str, &endptr, 16);
    if (*endptr != '\0' || value < 0 || value > 0xFFFFFFFF) {
        return 1;
    } else {
        return 0;
    }
}
void process_options(int argc, char *argv[]) {
    const char* short_options = "h::v::";
    const struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},
        {NULL, 0, NULL, 0}
    };

    int option_index = 0;
    int c;

    while ((c = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
        switch (c) {
            case 'h':
                printf("lab11nhtN32461: ./lab11nhtN32461 [OPTION]\n");
                printf("                ./lab11nhtN32461 [directory] [target]\n");
                printf("Arguments:\n");
                printf("- directory: The path from which the search begins\n");
                printf("- target: byte sequence in file need to find \n\n");
                printf("Options:\n");
                printf("  -h, --help     Print help message\n");
                printf("  -v, --version  Print version information \n");
                exit(EXIT_SUCCESS);
                break;

            case 'v':
                printf("Лабораторная работа 1.1\n");
                printf("Нгуен Хыу Тхань\n");
                printf("Вариант 5, нечетный\n");

                exit(EXIT_SUCCESS);
                break;

            case '?':
                // getopt_long already printed an error message
                exit(EXIT_FAILURE);
                break;

            default:
                printf("Use -h (--help) to see how the program works.\n");
                exit(EXIT_FAILURE);
                break;
        }
    }
}