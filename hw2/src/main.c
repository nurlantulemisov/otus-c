#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

const uint16_t cp_1251[] = 
{
    0x0402, 0x0403, 0x201A, 0x0453, 0x201E, 0x2026, 0x2020, 0x2021, 0x20AC, 0x2030, 0x0409, 0x2039, 0x040A, 0x040C, 0x040B, 0x040F,
    0x0452, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014, 0x0020, 0x2122, 0x0459, 0x203A, 0x045A, 0x045C, 0x045B, 0x045F,
    0x00A0, 0x040E, 0x045E, 0x0408, 0x00A4, 0x0490, 0x00A6, 0x00A7, 0x0401, 0x00A9, 0x0404, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x0407,
    0x00B0, 0x00B1, 0x0406, 0x0456, 0x0491, 0x00B5, 0x00B6, 0x00B7, 0x0451, 0x2116, 0x0454, 0x00BB, 0x0458, 0x0405, 0x0455, 0x0457,
    0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417, 0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E, 0x041F,
    0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427, 0x0428, 0x0429, 0x042A, 0x042B, 0x042C, 0x042D, 0x042E, 0x042F,
    0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437, 0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E, 0x043F,
    0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447, 0x0448, 0x0449, 0x044A, 0x044B, 0x044C, 0x044D, 0x044E, 0x044F,
};

const uint16_t koi8[] = 
{
    0x2500, 0x2502, 0x250C, 0x2510, 0x2514, 0x2518, 0x251C, 0x2524, 0x252C, 0x2534, 0x253C, 0x2580, 0x2584, 0x2588, 0x258C, 0x2590,
    0x2591, 0x2592, 0x2593, 0x2320, 0x25A0, 0x2219, 0x221A, 0x2248, 0x2264, 0x2265, 0x00A0, 0x2321, 0x00B0, 0x00B2, 0x00B7, 0x00F7,
    0x2550, 0x2551, 0x2552, 0x0451, 0x2553, 0x2554, 0x2555, 0x2556, 0x2557, 0x2558, 0x2559, 0x255A, 0x255B, 0x255C, 0x255D, 0x255E,
    0x255F, 0x2560, 0x2561, 0x0401, 0x2562, 0x2563, 0x2564, 0x2565, 0x2566, 0x2567, 0x2568, 0x2569, 0x256A, 0x256B, 0x256C, 0x00A9,
    0x044E, 0x0430, 0x0431, 0x0446, 0x0434, 0x0435, 0x0444, 0x0433, 0x0445, 0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E,
    0x043F, 0x044F, 0x0440, 0x0441, 0x0442, 0x0443, 0x0436, 0x0432, 0x044C, 0x044B, 0x0437, 0x0448, 0x044D, 0x0449, 0x0447, 0x044A,
    0x042E, 0x0410, 0x0411, 0x0426, 0x0414, 0x0415, 0x0424, 0x0413, 0x0425, 0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E,
    0x041F, 0x042F, 0x0420, 0x0421, 0x0422, 0x0423, 0x0416, 0x0412, 0x042C, 0x042B, 0x0417, 0x0428, 0x042D, 0x0429, 0x0427, 0x042A,
};

const uint16_t iso_8859_5[] = 
{
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087, 0x0088, 0x0089, 0x008A, 0x008B, 0x008C, 0x008D, 0x008E, 0x008F,
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097, 0x0098, 0x0099, 0x009A, 0x009B, 0x009C, 0x009D, 0x009E, 0x009F,
    0x00A0, 0x0401, 0x0402, 0x0403, 0x0404, 0x0405, 0x0406, 0x0407, 0x0408, 0x0409, 0x040A, 0x040B, 0x040C, 0x00AD, 0x040E, 0x040F,
    0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417, 0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E, 0x041F,
    0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427, 0x0428, 0x0429, 0x042A, 0x042B, 0x042C, 0x042D, 0x042E, 0x042F,
    0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437, 0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E, 0x043F,
    0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447, 0x0448, 0x0449, 0x044A, 0x044B, 0x044C, 0x044D, 0x044E, 0x044F,
    0x2116, 0x0451, 0x0452, 0x0453, 0x0454, 0x0455, 0x0456, 0x0457, 0x0458, 0x0459, 0x045A, 0x045B, 0x045C, 0x00A7, 0x045E, 0x045F,
};

typedef struct
{
    const char *name;
    const uint16_t *table;
} coding_type;

const coding_type coding_name[] = 
{
    {"cp1251", cp_1251},
    {"koi8", koi8},
    {"iso-8859-5", iso_8859_5},
};


void print_help(void) {
    printf("Usage: ./myprog -i <input_path> -o <output_path> -c <charset>\n");
    printf("Options:\n");
    printf("  -i <input_path>  Input file path\n");
    printf("  -o <output_path> Output file path\n");
    printf("  -c <charset>     File charset\n");
    printf("  -h               Print this help message\n");
}

int main(int argc, char* argv[]) 
{
    char *pin = NULL;
    char *pout = NULL;
    char *charset = NULL;

    int c;
    while ((c = getopt(argc, argv, "i:o:c:h")) != -1) {
        switch (c) {
            case 'i':
                pin = optarg;
                break;
            case 'o':
                pout = optarg;
                break;
            case 'c':
                charset = optarg;
                break;
            case 'h':
                print_help();
                return 0;
            default:
                printf("Invalid option: %c\n", c);
                return 1;
        }
    }

    if (pin == NULL || pout == NULL || charset == NULL) {
        printf("Missing required options: -i, -o, -c\n");
        return EXIT_FAILURE;
    }
    coding_type* charset_table = NULL;
    for (size_t i = 0; i < sizeof(coding_name); i++)
    {
        if (strncmp(charset, coding_name[i].name, strlen(charset)) == 0) {
            const coding_type *temp = &coding_name[i];
            charset_table = (coding_type *)temp;
            break;
        }
    }

    if (charset_table == NULL) {
        printf("Invalid charset option: %s\n", charset);
        return EXIT_FAILURE;
    }

    printf("filename: %s\n", pin);

    FILE *f = fopen(pin, "rb");
    FILE *fout = fopen(pout, "wb");

    if (f == NULL || fout == NULL) {
        printf("Cannot open file");
        return EXIT_FAILURE;
    }

    uint8_t buf_8 = '\0';
    while(fread(&buf_8, sizeof(buf_8), 1, f) == sizeof(buf_8))
    {
        if (buf_8 < 0x80) {
            fwrite(&buf_8, 1, 1, fout);
            continue;
        }

        uint16_t out_val = charset_table->table[buf_8 - 0x80];
        if (out_val >= 0x0080 && out_val <= 0x07FF) {
            unsigned char utf8Bytes[2];
            utf8Bytes[0] = 0xC0 | (out_val >> 6);
            utf8Bytes[1] = 0x80 | (out_val & 0x3F);
            fwrite(utf8Bytes, sizeof(utf8Bytes), 1, fout);
            continue;
        }

        if (out_val >= 0x0800 && out_val <= 0xFFFF) {
            unsigned char utf8Bytes[3];
            utf8Bytes[0] = 0xE0 | (out_val >> 12);
            utf8Bytes[1] = 0x80 | ((out_val >> 6) & 0x3F);
            utf8Bytes[2] = 0x80 | (out_val & 0x3F);
            fwrite(utf8Bytes, sizeof(utf8Bytes), 1, fout);
        }
    }

    fclose(f);
    fclose(fout);

    return EXIT_SUCCESS;
}
