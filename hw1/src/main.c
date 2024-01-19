#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Size of the End of Central Directory Record, not including comment. */
#define EOCDR_BASE_SZ 22
#define EOCDR_SIGNATURE 0x06054b50  /* "PK\5\6" little-endian. */
#define CFH_BASE_SZ 46
#define CFH_SIGNATURE 0x02014b50 /* "PK\1\2" little-endian. */
/* Size of a Local File Header, not including name and extra. */
#define LFH_BASE_SZ 30
#define LFH_SIGNATURE 0x04034b50 /* "PK\3\4" little-endian. */

/* End of Central Directory Record. */
struct eocdr {
        uint16_t disk_nbr;        /* Number of this disk. */
        uint16_t cd_start_disk;   /* Nbr. of disk with start of the CD. */
        uint16_t disk_cd_entries; /* Nbr. of CD entries on this disk. */
        uint16_t cd_entries;      /* Nbr. of Central Directory entries. */
        uint32_t cd_size;         /* Central Directory size in bytes. */
        uint32_t cd_offset;       /* Central Directory file offset. */
        uint16_t comment_len;     /* Archive comment length. */
        const uint8_t *comment;   /* Archive comment. */
};

/* Central File Header (Central Directory Entry) */
struct cfh {
        uint16_t made_by_ver;    /* Version made by. */
        uint16_t extract_ver;    /* Version needed to extract. */
        uint16_t gp_flag;        /* General purpose bit flag. */
        uint16_t method;         /* Compression method. */
        uint16_t mod_time;       /* Modification time. */
        uint16_t mod_date;       /* Modification date. */
        uint32_t crc32;          /* CRC-32 checksum. */
        uint32_t comp_size;      /* Compressed size. */
        uint32_t uncomp_size;    /* Uncompressed size. */
        uint16_t name_len;       /* Filename length. */
        uint16_t extra_len;      /* Extra data length. */
        uint16_t comment_len;    /* Comment length. */
        uint16_t disk_nbr_start; /* Disk nbr. where file begins. */
        uint16_t int_attrs;      /* Internal file attributes. */
        uint32_t ext_attrs;      /* External file attributes. */
        uint32_t lfh_offset;     /* Local File Header offset. */
        const uint8_t *name;     /* Filename. */
        const uint8_t *extra;    /* Extra data. */
        const uint8_t *comment;  /* File comment. */
};

/* Local File Header. */
struct lfh {
        uint16_t extract_ver;
        uint16_t gp_flag;
        uint16_t method;
        uint16_t mod_time;
        uint16_t mod_date;
        uint32_t crc32;
        uint32_t comp_size;
        uint32_t uncomp_size;
        uint16_t name_len;
        uint16_t extra_len;
        const uint8_t *name;
        const uint8_t *extra;
};

bool read_eocdr(FILE *f, struct eocdr *r, const size_t sz)
{
    uint32_t signature;

    fseek(f, sz - EOCDR_BASE_SZ, SEEK_SET);

    size_t retval = fread(&signature, sizeof(signature), 1, f);
    if (retval == 0) {
        return false;
    }
    if (signature == EOCDR_SIGNATURE) {
        // printf("sign %p\n", signature);

        fread(&r->disk_nbr, sizeof(r->disk_nbr), 1, f);
        fread(&r->cd_start_disk, sizeof(r->cd_start_disk), 1, f);
        fread(&r->disk_cd_entries, sizeof(r->disk_cd_entries), 1, f);
        fread(&r->cd_entries, sizeof(r->cd_entries), 1, f);
        fread(&r->cd_size, sizeof(r->cd_size), 1, f);
        fread(&r->cd_offset, sizeof(r->cd_offset), 1, f);
        fread(&r->comment_len, sizeof(r->comment_len), 1, f);

        return true;
    }

    return false;
}


bool read_cfh(FILE *f, struct cfh *c, size_t f_sz, size_t offset)
{
    uint32_t signature;

    if (offset > f_sz || f_sz - offset < CFH_BASE_SZ) {
        return false;
    }

    fseek(f, offset, SEEK_SET);

    size_t retval = fread(&signature, sizeof(signature), 1, f);
    if (retval == 0) {
        return false;
    }
    // printf("sign %p\n", signature);
    if (signature != CFH_SIGNATURE) {
        return false;
    }

    fread(&c->made_by_ver, sizeof(c->made_by_ver), 1, f);
    fread(&c->extract_ver, sizeof(c->extract_ver), 1, f);
    fread(&c->gp_flag, sizeof(c->gp_flag), 1, f);
    fread(&c->method, sizeof(c->method), 1, f);
    fread(&c->mod_time, sizeof(c->mod_time), 1, f);
    fread(&c->mod_date, sizeof(c->mod_date), 1, f);
    fread(&c->crc32, sizeof(c->crc32), 1, f);
    fread(&c->comp_size, sizeof(c->comp_size), 1, f);
    fread(&c->uncomp_size, sizeof(c->uncomp_size), 1, f);
    fread(&c->name_len, sizeof(c->name_len), 1, f);
    fread(&c->extra_len, sizeof(c->extra_len), 1, f);
    fread(&c->comment_len, sizeof(c->comment_len), 1, f);
    fread(&c->disk_nbr_start, sizeof(c->disk_nbr_start), 1, f);
    fread(&c->int_attrs, sizeof(c->int_attrs), 1, f);
    fread(&c->ext_attrs, sizeof(c->ext_attrs), 1, f);
    fread(&c->lfh_offset, sizeof(c->lfh_offset), 1, f);

    char string_buf[c->name_len];
    fread(&string_buf, 1, c->name_len, f);

    char comment[c->comment_len];
    fread(&comment, 1, c->comment_len, f);
    puts(string_buf);

    return true;
}


bool read_lfh(FILE *f, struct lfh *l, size_t f_sz, size_t offset)
{
    uint32_t signature;

    if (offset > f_sz || f_sz - offset < LFH_BASE_SZ) {
        return false;
    }

    fseek(f, offset, SEEK_SET);
    
    size_t retval = fread(&signature, sizeof(signature), 1, f);
    if (retval == 0) {
        return false;
    }

    // printf("sign %p\n", signature);

    if (signature != LFH_SIGNATURE) {
        return false;
    }

    fread(&l->extract_ver, sizeof(l->extract_ver), 1, f);
    fread(&l->gp_flag, sizeof(l->gp_flag), 1, f);
    fread(&l->method, sizeof(l->method), 1, f);
    fread(&l->mod_time, sizeof(l->mod_time), 1, f);
    fread(&l->mod_date, sizeof(l->mod_date), 1, f);
    fread(&l->crc32, sizeof(l->mod_date), 1, f);
    fread(&l->comp_size, sizeof(l->comp_size), 1, f);
    fread(&l->uncomp_size, sizeof(l->uncomp_size), 1, f);
    fread(&l->name_len, sizeof(l->name_len), 1, f);
    fread(&l->extra_len, sizeof(l->extra_len), 1, f);

    return true;
}

uint32_t jpeg_offset(FILE *f)
{
    bool is_start_jpeg = false;
    unsigned char buffer[2];
    // fread(buffer, sizeof(unsigned char), 2, f);

    const unsigned char jpeg_soi[2] = {0xFF, 0xD8};

    fread(buffer, sizeof(unsigned char), 2, f);
    if (memcmp(buffer, jpeg_soi, 2) == 0 && sizeof(buffer) == 2) {
        is_start_jpeg = true;
    }

    const unsigned char jpeg_eoi[2] = {0xFF, 0xD9};
    // Read two bytes from the file into the buffer
    size_t bytesRead;
    while ((bytesRead = fread(buffer, sizeof(unsigned char), 2, f)) == 2) {
        if (memcmp(buffer, jpeg_eoi, 2) == 0 && sizeof(buffer) == 2) {
            if (is_start_jpeg) {
                printf("FIle is JPEG\n");
                return ftell(f);
            }
        }
    }

    return 0;
}

bool is_zip(const char* path)
{
    if (path == NULL) {
        // return 1;
        path = "/home/nurlantulemisov/myprojects/otus-clang/hw1/assets/zipjpeg2.jpg";
    }
    FILE *f = fopen(path, "rb");

    fseek(f, 0, SEEK_END); // seek to end of file
    size_t size = ftell(f); // get current file pointer
    fseek(f, 0, SEEK_SET); // seek back to beginning of file
    printf("size: %d\n",size);

    uint32_t jp_offset = jpeg_offset(f);
    // fseek(f, 0, SEEK_SET); // seek back to beginning of file
    printf("start offset: %d\n",jp_offset);

    struct eocdr e;

    if (!read_eocdr(f, &e, size)) {
        return EXIT_FAILURE;
    }

    size_t i = 0;
    uint32_t offset = e.cd_offset + jp_offset + 2;

    for (i = 0; i < e.cd_entries; i++) {
        struct cfh c;
        struct lfh l;

        if (!read_cfh(f, &c, size, offset)) {
            return EXIT_FAILURE;
        }

        if (!read_lfh(f, &l, size, c.lfh_offset)) {
            return EXIT_FAILURE;
        }
        offset += CFH_BASE_SZ + c.name_len+c.comment_len+c.extra_len;
        printf("offset %d \n", offset);
    }

    fclose(f);

    return 0;
}

int main(int argc, char* argv[]) 
{
    char* path = argv[1];
    
    printf("filename %s\n", path);

    return is_zip(path);
}
