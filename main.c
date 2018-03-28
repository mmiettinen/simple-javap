
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "classfile.h"
#include "defs.h"
#include "output.h"
#include "parse_opt.h"

void print_help(void)
{
    printf("Javap - Java class file disassembler\n");
    printf("Usage: javap <options> classes\n");
}

int main(int argc, char *argv[])
{
    int ch;
    int fprint_code = 0;
    int print_pool = 0;
    int level = AL_PACKAGE;
    class_hierarchy *chp;

    while ((ch = parse_opt(argv)) != OPT_END) {
        switch (ch) {
        case OPT_CODE:
            fprint_code = 1;
            break;
        case OPT_POOL:
            print_pool = 1;
            break;
        case OPT_PUBLIC:
            level = AL_PUBLIC;
            break;
        case OPT_PROTECTED:
            level = AL_PROTECTED;
            break;
        case OPT_PACKAGE:
            level = AL_PACKAGE;
            break;
        case OPT_PRIVATE:
            level = AL_PRIVATE;
            break;
        default:
            print_help();
            return EXIT_FAILURE;
        }
    }

    if ((chp = load_class_hierarchy(opt_string)) == NULL) {
        printf("%s: %s\n", argv[0], get_error_message());
        return EXIT_FAILURE;
    }

    print_class_declaration(chp, level, 0);
    if (print_pool)
        print_constant_pool(chp->class);
    if (fprint_code)
        print_code(chp);

    free_class_hierarchy(chp);

    return EXIT_SUCCESS;
}


