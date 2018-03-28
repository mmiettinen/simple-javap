
#ifndef INFILE_H
#define INFILE_H

#include "defs.h"

typedef struct infile_t infile;

#define RS_EOF 0
#define RS_OK  1

extern infile *open_file(const char *name);
extern void close_file(infile *fp);

extern int read_u1(infile *fp, u1 *valp);
extern int read_u2(infile *fp, u2 *valp);
extern int read_u4(infile *fp, u4 *valp);

#endif
