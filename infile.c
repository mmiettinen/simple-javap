
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "defs.h"

struct infile_t {
    int fd;
    u1 *bytes;
    u4 length;
    u4 pos;
};

infile *open_file(const char *name)
{
    infile *res = (infile *)calloc(1, sizeof(infile));
    struct stat statbuf;

    res->fd = -1;
    res->pos = 0;
    error_number = ERR_FILE_OPEN;
    if ((res->fd = open(name, O_RDONLY)) == -1)
        return NULL;
    if (fstat(res->fd, &statbuf) == -1)
        return NULL;
    res->length = (u4)statbuf.st_size;
    res->bytes = mmap(NULL, res->length, PROT_READ, MAP_PRIVATE, res->fd, 0);
    if (res->bytes == MAP_FAILED)
        return NULL;
    error_number = ERR_OK;
    return res;
}

void close_file(infile *fp)
{
    if (fp == NULL || fp->fd == -1 || fp->bytes == NULL)
        return;
    munmap(fp->bytes, fp->length);
    close(fp->fd);
    free(fp);
}

int read_u1(infile *fp, u1 *valp)
{
    if (fp->pos >= fp->length) {
        error_number = ERR_TRUNCATED;
        return RS_EOF;
    }
    if (valp != NULL)
        *valp = fp->bytes[fp->pos];
    fp->pos++;
    error_number = ERR_OK;
    return RS_OK;
}

int read_u2(infile *fp, u2 *valp)
{
    int i;
    u1 byte;
    u2 val = 0;

    for (i = 0; i < 2; i++) {
        if (read_u1(fp, &byte) == RS_EOF)
            return RS_EOF;
        val <<= 8;
        val |= byte;
    }
    if (valp != NULL)
        *valp = val;
    return RS_OK;
}

int read_u4(infile *fp, u4 *valp)
{
    int i;
    u1 byte;
    u4 val = 0;

    for (i = 0; i < 4; i++) {
        if (read_u1(fp, &byte) == RS_EOF)
            return RS_EOF;
        val <<= 8;
        val |= byte;
    }
    if (valp != NULL)
        *valp = val;
    return RS_OK;
}



