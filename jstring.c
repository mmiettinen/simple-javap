
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jstring.h"

/* 
 * Bitmasks for UTF-8 conversions. 
 */
#define UTF8_MASK          0xf0
#define ONEBYTE_FLAG       0x00
#define TWOBYTE_HI_FLAG    0xc0
#define TWOBYTE_LO_FLAG    0x80
#define THREEBYTE_HI_FLAG  0xe0
#define THREEBYTE_MID_FLAG 0x80
#define THREEBYTE_LO_FLAG  0x80

jstring *create_jstring(infile *fp, u2 bytecnt, jstring *res)
{
    int i = 0;
    int j = 0;
    u1 x, y, z;

    if ((res->data = (jchar *)malloc(bytecnt * sizeof(jchar))) == NULL) {
        error_number = ERR_MEM_ALLOC;
        return NULL;
    }

    while (i < bytecnt) {
        /* read the contents of the string */
        if (read_u1(fp, &x) == RS_EOF)
            return NULL;
        i++;
        if ((x & 0x80) == ONEBYTE_FLAG)
            res->data[j] = x; /* 7-bit ascii character */
        else if ((x & 0xe0) == TWOBYTE_HI_FLAG) {
            /* two byte character in the range of \u0080 and \u07ff */
            if (read_u1(fp, &y) == RS_EOF)
                return NULL;
            i++;
            if ((y & 0x80) != TWOBYTE_LO_FLAG) {
                error_number = ERR_INCORRECT_CLASS;
                return NULL;
            }
            res->data[j] = ((x & 0x1f) << 6) + (y & 0x3f);
        }
        else if ((x & 0xe0) == THREEBYTE_HI_FLAG) {
            /* two byte character in the range of \u0800 and \uffff */
            if (read_u1(fp, &y) == RS_EOF)
                return NULL;
            if (read_u1(fp, &z) == RS_EOF)
                return NULL;
            i += 2;
            if ((y & 0x80) != THREEBYTE_MID_FLAG || (z & 0x80) != THREEBYTE_LO_FLAG) {
                error_number = ERR_INCORRECT_CLASS;
                return NULL;
            }
            res->data[j] = ((x & 0xf0) << 12) + ((y & 0x3f) << 6) + (z & 0x3f);
        }
        else {
            error_number = ERR_INCORRECT_CLASS;
            return NULL;
        }
        j++;
    }
    res->length = j;
    return res;
}

/* Compares Unicode (16-bit) character string to native (8-bit)
   character string by just ignoring the high order byte of 16-bit
   characters. Returns 1 if strings are identical, 0 otherwise. */
int equals_native(jstring *str, char *nstr)
{
    int i;

    for (i = 0; (i < str->length) && (nstr[i] != '\0'); i++) {
        if ((u1)(str->data[i]) != (u1)nstr[i])
            return 0;
    }
    if (i == str->length && nstr[i] == '\0')
        return 1;
    else
        return 0;
}

char *jstring_to_native(jstring *str, char *res)
{
    int i;

    for (i = 0; i < str->length; i++)
        res[i] = (char)str->data[i];
    res[str->length] = '\0';
    return res;
}

char *construct_classfile_name(char *basename, jstring *icname, char *res)
{
    char buf[128];
    char *cp;

    strcpy(res, basename);
    cp = strrchr(res, '.');
    *cp = '$';
    *(cp + 1) = '\0';
    strcat(res, jstring_to_native(icname, buf));
    strcat(res, ".class");
    return res;
}

void print_jstring(jstring *str)
{
    int i;

    for (i = 0; i < str->length; i++)
        putchar((char)(str->data[i]));
}

char *convert_classname(jstring *cname, char *res)
{
    int i;

    for (i = 0; i < cname->length; i++) {
        if (cname->data[i] == (u2)'/' || cname->data[i] == (u2)'$')
            res[i] = '.';
        else
            res[i] = (char)(cname->data[i]);
    }
    res[cname->length] = '\0';
    return res;
}
    


