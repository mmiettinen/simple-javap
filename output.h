
#ifndef OUTPUT_H
#define OUTPUT_H

#include "classfile.h"

/* Constants defining what fields and methods to print */
#define AL_PUBLIC     0x0001 /* public only */
#define AL_PROTECTED  0x0005 /* public + protected */
#define AL_PACKAGE    0x0006 /* public + protected + package */
#define AL_PRIVATE    0x0007 /* all */

extern void print_class_declaration(class_hierarchy *chp, int level, int indent);
extern void print_code(class_hierarchy *chp);
extern void print_constant_pool(class_file *cfp);

#endif
