
#include "defs.h"

#ifndef JSTRING_H
#define JSTRING_H

typedef struct {
    u1 tag;
    u2 length;
    u2 *data;
} jstring;

extern jstring *create_jstring(infile *fp, u2 bytecnt, jstring *res);
extern int equals_native(jstring *str, char *nstr);
extern char *jstring_to_native(jstring *str, char *res);
extern void print_jstring(jstring *str);
extern char *convert_classname(jstring *cname, char *res);
extern char *construct_classfile_name(char *basename, jstring *icname, char *res);

#endif
