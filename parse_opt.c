
#include <stdio.h>
#include <string.h>

#include "parse_opt.h"

static struct {
    char *opt;
    int code;
} opt_array[] = {
    {"-p", OPT_POOL},
    {"-c", OPT_CODE},
    {"-public", OPT_PUBLIC},
    {"-protected", OPT_PROTECTED},
    {"-package", OPT_PACKAGE},
    {"-private", OPT_PRIVATE}
};

char *opt_string;

static char **vecptr = NULL;

int parse_opt(char **argv)
{
    int i;
    char **ptr, *tmp;

    if (argv == NULL)
        return OPT_ERR;
    if (vecptr == NULL)
        vecptr = &argv[1];
    if (*vecptr == NULL)
        return OPT_END;
    
    ptr = vecptr;
    while (*ptr != NULL && (*ptr)[0] != '-')
        ptr++; /* find the next option */
    if (*ptr == NULL || (*ptr)[0] != '-') {
        opt_string = *vecptr;
        return OPT_END;
    }
    opt_string = *ptr;
    if (ptr != vecptr) {
        tmp = *vecptr;
        *vecptr = *ptr;
        *ptr = tmp;
    }
    for (i = 0; i < 6; i++) {
        if (strcmp(opt_array[i].opt, *vecptr) == 0) {
            vecptr++;
            return opt_array[i].code;
        }
    }
    vecptr++;
    return OPT_UNKNOWN;
}
