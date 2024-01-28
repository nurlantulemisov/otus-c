#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include "hashtable.h"

void printCallback(char* key, int32_t value) {
    printf("Key: %s, Value: %d\n", key, value);
}

void print_help(void) {
    printf("Usage: ./myprog -i <input_path> -o <output_path> -c <charset>\n");
    printf("Options:\n");
    printf("  -i <input_path>  Input file path\n");
    printf("  -h               Print this help message\n");
}

int main(int argc, char* argv[]) 
{
    char *pin = NULL;

    int c;
    while ((c = getopt(argc, argv, "i:h")) != -1) {
        switch (c) {
            case 'i':
                pin = optarg;
                break;
            case 'h':
                print_help();
                return 0;
            default:
                printf("Invalid option: %c\n", c);
                return 1;
        }
    }

    if (pin == NULL) {
        printf("Missing required options: -i, -o, -c\n");
        return EXIT_FAILURE;
    }

    
    FILE *f = fopen(pin, "rb");
    if (f == NULL) {
        printf("Failed open file");
        return EXIT_FAILURE;
    }

    HTable* t = new_table(10, 10);
    if (t == NULL) {
        printf("Failed init hashmap");
        return EXIT_FAILURE;
    }

    int ch;
    size_t str_len = 0;
    while ((ch = fgetc(f)) != EOF) {
        if (ch == ' ' || ch == '.' || ch == ',' || ch == '\n' || ch == '\t') {
            if (str_len != 0) {
                fseek(f, -str_len-1, SEEK_CUR);
                char* word = (char*)malloc((str_len + 1) * sizeof(char));
                fread(word, sizeof(char), str_len, f);
                word[str_len] = '\0'; // Добавляем завершающий нулевой символ

                int32_t v = get(t, word);
                if (v == -1) {
                    v = 0;
                }
                if (!put(t, word, ++v)) {
                    printf("Could not add. key: %s => value: %d\n", word, v);
                    return EXIT_FAILURE;
                }
                free(word);
                str_len = 0;
            }
            continue;
        }
        str_len++;
    }

    iterate(t, printCallback);

    clear(t);
    fclose(f);

    return EXIT_SUCCESS;
}
