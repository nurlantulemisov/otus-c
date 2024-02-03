#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "hashtable.h"

void printCallback(const char *key, uint32_t value)
{
    printf("Key: %s, Value: %d\n", key, value);
}

void print_help(void)
{
    printf("Usage: ./myprog -i <input_path>\n");
    printf("Options:\n");
    printf("  -i <input_path>  Input file path\n");
    printf("  -h               Print this help message\n");
}

int main(int argc, char *argv[])
{
    char *pin = NULL;

    int c;
    while ((c = getopt(argc, argv, "i:h")) != -1)
    {
        switch (c)
        {
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

    if (pin == NULL)
    {
        perror("Missing required options: -i, -o, -c\n");
        return EXIT_FAILURE;
    }

    FILE *f = fopen(pin, "rb");
    if (f == NULL)
    {
        perror("Failed open file");
        return EXIT_FAILURE;
    }

    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buffer = (char *)malloc(file_size + 1);

    if (buffer == NULL)
    {
        perror("Not init buffer");
        fclose(f);
        return EXIT_FAILURE;
    }

    size_t read_size = fread(buffer, 1, file_size, f);

    buffer[read_size] = '\0';

    const char *word = strtok(buffer, " ,.\n\t");

    HTable *t = new_table(10, 10);
    if (t == NULL)
    {
        fclose(f);
        free(buffer);
        perror("Failed init hashmap");
        return EXIT_FAILURE;
    }

    while (word != NULL)
    {
        uint32_t v = get(t, word);
        if (v == UINT32_MAX)
        {
            v = 0;
        }

        if (!put(t, word, ++v))
        {
            printf("Could not add. key: %s => value: %d\n", word, v);
            return EXIT_FAILURE;
        }

        word = strtok(NULL, " ,.\n\t");
    }

    iterate(t, printCallback);

    clear(t);
    fclose(f);
    free(buffer);

    return EXIT_SUCCESS;
}
