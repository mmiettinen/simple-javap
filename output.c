
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "classfile.h"
#include "defs.h"
#include "jstring.h"
#include "output.h"
#include "parsedescr.h"

#define NOP             0x00
#define ACONST_NULL     0x01
#define ICONST_M1       0x02
#define ICONST_0        0x03
#define ICONST_1        0x04
#define ICONST_2        0x05
#define ICONST_3        0x06
#define ICONST_4        0x07
#define ICONST_5        0x08
#define LCONST_0        0x09
#define LCONST_1        0x0a 
#define FCONST_0        0x0b
#define FCONST_1        0x0c
#define FCONST_2        0x0d
#define DCONST_0        0x0e
#define DCONST_1        0x0f
#define BIPUSH          0x10
#define SIPUSH          0x11
#define LDC             0x12
#define LDC_W           0x13
#define LDC2_W          0x14
#define ILOAD           0x15
#define LLOAD           0x16
#define FLOAD           0x17
#define DLOAD           0x18
#define ALOAD           0x19
#define ILOAD_0         0x1a
#define ILOAD_1         0x1b
#define ILOAD_2         0x1c
#define ILOAD_3         0x1d
#define LLOAD_0         0x1e
#define LLOAD_1         0x1f
#define LLOAD_2         0x20
#define LLOAD_3         0x21
#define FLOAD_0         0x22
#define FLOAD_1         0x23
#define FLOAD_2         0x24
#define FLOAD_3         0x25
#define DLOAD_0         0x26
#define DLOAD_1         0x27
#define DLOAD_2         0x28
#define DLOAD_3         0x29
#define ALOAD_0         0x2a
#define ALOAD_1         0x2b
#define ALOAD_2         0x2c
#define ALOAD_3         0x2d
#define IALOAD          0x2e
#define LALOAD          0x2f
#define FALOAD          0x30
#define DALOAD          0x31
#define AALOAD          0x32
#define BALOAD          0x33
#define CALOAD          0x34
#define SALOAD          0x35
#define ISTORE          0x36
#define LSTORE          0x37
#define FSTORE          0x38
#define DSTORE          0x39
#define ASTORE          0x3a
#define ISTORE_0        0x3b
#define ISTORE_1        0x3c
#define ISTORE_2        0x3d
#define ISTORE_3        0x3e
#define LSTORE_0        0x3f
#define LSTORE_1        0x40
#define LSTORE_2        0x41
#define LSTORE_3        0x42
#define FSTORE_0        0x43
#define FSTORE_1        0x44
#define FSTORE_2        0x45
#define FSTORE_3        0x46
#define DSTORE_0        0x47
#define DSTORE_1        0x48
#define DSTORE_2        0x49
#define DSTORE_3        0x4a
#define ASTORE_0        0x4b
#define ASTORE_1        0x4c
#define ASTORE_2        0x4d
#define ASTORE_3        0x4e
#define IASTORE         0x4f
#define LASTORE         0x50
#define FASTORE         0x51
#define DASTORE         0x52
#define AASTORE         0x53
#define BASTORE         0x54
#define CASTORE         0x55
#define SASTORE         0x56
#define POP             0x57
#define POP2            0x58
#define DUP             0x59
#define DUP_X1          0x5a
#define DUP_X2          0x5b
#define DUP2            0x5c
#define DUP2_X1         0x5d
#define DUP2_X2         0x5e
#define SWAP            0x5f

#define IADD            0x60
#define LADD            0x61
#define FADD            0x62
#define DADD            0x63
#define ISUB            0x64
#define LSUB            0x65
#define FSUB            0x66
#define DSUB            0x67
#define IMUL            0x68
#define LMUL            0x69
#define FMUL            0x6a
#define DMUL            0x6b
#define IDIV            0x6c
#define LDIV            0x6d
#define FDIV            0x6e
#define DDIV            0x6f
#define IREM            0x70
#define LREM            0x71
#define FREM            0x72
#define DREM            0x73
#define INEG            0x74
#define LNEG            0x75
#define FNEG            0x76
#define DNEG            0x77
#define ISHL            0x78
#define LSHL            0x79
#define ISHR            0x7a
#define LSHR            0x7b
#define IUSHR           0x7c
#define LUSHR           0x7d
#define IAND            0x7e
#define LAND            0x7f
#define IOR             0x80
#define LOR             0x81
#define IXOR            0x82
#define LXOR            0x83
#define IINC            0x84

#define I2L             0x85
#define I2F             0x86
#define I2D             0x87
#define L2I             0x88
#define L2F             0x89
#define L2D             0x8a
#define F2I             0x8b
#define F2L             0x8c
#define F2D             0x8d
#define D2I             0x8e
#define D2L             0x8f
#define D2F             0x90
#define I2B             0x91
#define I2C             0x92
#define I2S             0x93

#define LCMP            0x94
#define FCMPL           0x95
#define FCMPG           0x96
#define DCMPL           0x97
#define DCMPG           0x98
#define IFEQ            0x99
#define IFNE            0x9a
#define IFLT            0x9b
#define IFGE            0x9c
#define IFGT            0x9d
#define IFLE            0x9e
#define IF_ICMPEQ       0x9f
#define IF_ICMPNE       0xa0
#define IF_ICMPLT       0xa1
#define IF_ICMPGE       0xa2
#define IF_ICMPGT       0xa3
#define IF_ICMPLE       0xa4
#define IF_ACMPEQ       0xa5
#define IF_ACMPNE       0xa6
#define GOTO            0xa7
#define JSR             0xa8
#define RET             0xa9
#define TABLESWITCH     0xaa
#define LOOKUPSWITCH    0xab

#define IRETURN         0xac
#define LRETURN         0xad
#define FRETURN         0xae
#define DRETURN         0xaf
#define ARETURN         0xb0
#define RETURN          0xb1
#define GETSTATIC       0xb2
#define PUTSTATIC       0xb3
#define GETFIELD        0xb4
#define PUTFIELD        0xb5
#define INVOKEVIRTUAL   0xb6
#define INVOKESPECIAL   0xb7
#define INVOKESTATIC    0xb8
#define INVOKEINTERFACE 0xb9
#define __UNUSED__      0xba
#define NEW             0xbb
#define NEWARRAY        0xbc
#define ANEWARRAY       0xbd
#define ARRAYLENGTH     0xbe
#define ATHROW          0xbf
#define CHECKCAST       0xc0
#define INSTANCEOF      0xc1
#define MONITORENTER    0xc2
#define MONITOREXIT     0xc3
#define WIDE            0xc4
#define MULTIANEWARRAY  0xc5
#define IFNULL          0xc6
#define IFNONNULL       0xc7
#define GOTO_W          0xc8
#define JSR_W           0xc9

/* Reserved opcodes; probably those that are named, but not
   (currently) used. */
#define BREAKPOINT      0xca
#define IMPDEP1         0xfe
#define IMPDEP2         0xff

u1 get_u1(u1 **p)
{
    u1 res = **p;
    (*p)++;
    return res;
}

u2 get_u2(u1 **p)
{
    u1 *q = *p;
    u2 res;
    res = (*q << 8) | *(q + 1);
    *p += 2;
    return res;
}

u4 get_u4(u1 **p)
{
    u1 *q = *p;
    u4 res = (*q << 24) | (*(q + 1) << 16) | (*(q + 2) << 8) | *(q + 3);
    *p += 4;
    return res;
}

s1 get_s1(u1 **p)
{
    return (s1)get_u1(p);
}

s2 get_s2(u1 **p)
{
    return (s2)get_u2(p);
}

s4 get_s4(u1 **p)
{
    return (s4)get_u4(p);
}

void print_classname(class_file *cfp, jclass_info *pci)
{
    jstring *str = &cfp->constant_pool[pci->name_index].data.jstr;
    char buf[128];
    
    printf("%s", convert_classname(str, buf));
}

void print_constant_value(class_file *cfp, cp_entry cpe)
{
    jinteger_info *pi;
    jfloat_info *pf;
    jlong_info *pl;
    jdouble_info *pd;
    jstring_info *ps;
    jstring *jstr;

    switch (cpe.tag) {
    case CONSTANT_INTEGER:
        pi = &cpe.data.int_info;
        printf("<Integer %d>", pi->value);
        break;
    case CONSTANT_FLOAT:
        pf = &cpe.data.float_info;
        printf("<Float %f>", pf->value);
        break;
    case CONSTANT_LONG:
        pl = &cpe.data.long_info;
        printf("<Long %lld>", pl->value);
        break;
    case CONSTANT_DOUBLE:
        pd = &cpe.data.double_info;
        printf("<Double %f>", pd->value);
        break;
    case CONSTANT_STRING:
        ps = &cpe.data.string_info;
        jstr = &cfp->constant_pool[ps->string_index].data.jstr;
        printf("<String \"");
        print_jstring(jstr);
        printf("\">");
        break;
    default:
        break;
    }
}

void print_field_by_ref(class_file *cfp, jfieldref_info *pfr)
{
    jnametype_info *pni;
    jstring *fname, *fdescr;

    pni = &cfp->constant_pool[pfr->name_and_type_index].data.nt_info;
    fname = &cfp->constant_pool[pni->name_index].data.jstr;
    fdescr = &cfp->constant_pool[pni->descriptor_index].data.jstr;
    printf("<Field %s>", gen_field_declaration(fname, fdescr));
}

void print_constant_pool(class_file *cfp)
{
    int i = 1;
    cp_entry p;

    while (i < cfp->constant_pool_count) {
        p = cfp->constant_pool[i];
        printf("%d. ", i);
        switch (p.tag) {
        case CONSTANT_CLASS:
            printf("CONSTANT_Class; ");
            printf("name_index = %u\n", p.data.class_info.name_index);
            break;
        case CONSTANT_FIELDREF:
            printf("CONSTANT_Fieldref; ");
            printf("class_index = %u, name_and_type_index = %u\n",
                   p.data.fr_info.class_index,
                   p.data.fr_info.name_and_type_index);
            break;
        case CONSTANT_METHODREF:
            printf("CONSTANT_Methodref; ");
            printf("class_index = %u, name_and_type_index = %u\n",
                   p.data.mr_info.class_index,
                   p.data.mr_info.name_and_type_index);
            break;
        case CONSTANT_INTERFACEMETHODREF:
            printf("CONSTANT_InterfaceMethodref; ");
            printf("class_index = %u, name_and_type_index = %u\n",
                   p.data.imr_info.class_index,
                   p.data.imr_info.name_and_type_index);
            break;
        case CONSTANT_STRING:
            printf("CONSTANT_String; ");
            printf("string_index = %u\n", p.data.string_info.string_index);
            break;
        case CONSTANT_INTEGER:
            printf("CONSTANT_Integer; ");
            printf("value = %d\n", p.data.int_info.value);
            break;
        case CONSTANT_FLOAT:
            printf("CONSTANT_Float; ");
            printf("value = %f\n", p.data.float_info.value);
            break;
        case CONSTANT_LONG:
            printf("CONSTANT_Long; ");
            printf("value = %lld\n", p.data.long_info.value);
            i++;
            break;
        case CONSTANT_DOUBLE:
            printf("CONSTANT_Double; ");
            printf("value = %f\n", p.data.double_info.value);
            i++;
            break;
        case CONSTANT_NAMEANDTYPE:
            printf("CONSTANT_NameAndType; ");
            printf("name_index = %u, descriptor_index = %u\n",
                   p.data.nt_info.name_index,
                   p.data.nt_info.descriptor_index);
            break;
        case CONSTANT_UTF8:
            printf("CONSTANT_Utf8; ");
            printf("value = ");
            print_jstring(&p.data.jstr);
            printf("\n");
            break;
        default:
            /* This should never happen */
            printf("Internal error.\n");
            break;
        }
        i++;
    }
    printf("\n");
}

int is_printable(u2 acc, int level)
{
    switch (level) {
    case AL_PUBLIC:
        return (acc & ACC_PUBLIC) != 0;
    case AL_PROTECTED:
        return ((acc & ACC_PUBLIC) != 0) || ((acc & AL_PROTECTED) != 0);
    case AL_PACKAGE:
        return ((acc & ACC_PUBLIC) != 0) || ((acc & AL_PROTECTED) != 0)
            || ((acc & 0x0007) == 0);
    default:
        return 1;
    }
}

void print_access(u2 access, int is_method)
{
    char buffer[128];

    buffer[0] = '\0';
    strcat(buffer, (access & ACC_PUBLIC) ? "public " : "");
    strcat(buffer, (access & ACC_PRIVATE) ? "private " : "");
    strcat(buffer, (access & ACC_PROTECTED) ? "protected " : "");
    strcat(buffer, (access & ACC_STATIC) ? "static " : "");
    strcat(buffer, (access & ACC_FINAL) ? "final " : "");
    if (is_method)
        strcat(buffer, (access & ACC_SYNCHRONIZED) ? "synchronized " : "");
    else
        strcat(buffer, (access & ACC_SUPER) ? "super " : "");
    if (is_method)
        strcat(buffer, (access & ACC_BRIDGE) ? "bridge " : "");
    else
        strcat(buffer, (access & ACC_VOLATILE) ? "volatile " : "");
    if (is_method)
        strcat(buffer, (access & ACC_VARARGS) ? "varargs " : "");
    else
        strcat(buffer, (access & ACC_TRANSIENT) ? "transient " : "");
    strcat(buffer, (access & ACC_NATIVE) ? "native " : "");
    strcat(buffer, (access & ACC_INTERFACE) ? "interface " : "");
    strcat(buffer, (access & ACC_ABSTRACT) ? "abstract " : "");
    strcat(buffer, (access & ACC_STRICT) ? "strict " : "");
    strcat(buffer, (access & ACC_SYNTHETIC) ? "synthetic " : "");
    printf("%s", buffer);
}

void print_fields(class_file *cfp, int level, int indent)
{
    int i, j;
    field_info *pfi;
    jstring *fname, *fdescr;
    char *field;

    for (i = 0; i < cfp->fields_count; i++) {
        pfi = &cfp->fields[i];
        if (is_printable(pfi->access_flags, level)) {
            for (j = 0; j < indent; j++)
                putchar(' ');
            print_access(pfi->access_flags, 0);
            printf((pfi->access_flags & ACC_ENUM) ? "enum " : "");
            fname = &cfp->constant_pool[pfi->name_index].data.jstr;
            fdescr = &cfp->constant_pool[pfi->descriptor_index].data.jstr;
            field = gen_field_declaration(fname, fdescr);
            printf("%s;\n", field);
        }
    }
}

void print_methods(class_file *cfp, int level, int indent)
{
    int i, j;
    method_info *pmi;
    jstring *mname, *mdescr;
    char *method;

    for (i = 0; i < cfp->methods_count; i++) {
        pmi = &cfp->methods[i];
        if (is_printable(pmi->access_flags, level)) {
            for (j = 0; j < indent; j++)
                putchar(' ');
            print_access(pmi->access_flags, 1);
            mname = &cfp->constant_pool[pmi->name_index].data.jstr;
            if (equals_native(mname, "<init>") || equals_native(mname, "<clinit>")) {
                jclass_info *tmp = &cfp->constant_pool[cfp->this_class].data.class_info;
                mname = &cfp->constant_pool[tmp->name_index].data.jstr;
            }
            mdescr = &cfp->constant_pool[pmi->descriptor_index].data.jstr;
            method = gen_method_header(mname, mdescr);
            printf("%s;\n", method);
        }
    }
}

void print_class_declaration(class_hierarchy *chp, int level, int indent)
{
    int i;
    class_file *cfp = chp->class;
    u2 access = chp->access_flags;

    if (!is_printable(access, level))
        return;
    for (i = 0; i < indent; i++)
        putchar(' ');
    print_access(access, 0);
    printf((access & ACC_ANNOTATION) ? "annotation " : "");
    printf((access & ACC_ENUM) ? "enum " : "class ");
    print_classname(cfp, &cfp->constant_pool[cfp->this_class].data.class_info);
    /* handle superclasses and superinterfaces */
    if (cfp->super_class > 0) {
        printf(" extends ");
        print_classname(cfp, &cfp->constant_pool[cfp->super_class].data.class_info);
    }
    for (i = 0; i < cfp->interfaces_count; i++) {
        printf(" implements ");
        print_classname(cfp, &cfp->constant_pool[cfp->interfaces[i]].data.class_info);
        printf((i < cfp->interfaces_count - 1) ? ", " : "");
    }
    printf(" {\n");
    print_fields(cfp, level, indent + 4);
    print_methods(cfp, level, indent + 4);
    if (chp->sibling != NULL)
        print_class_declaration(chp->sibling, level, indent + 4);
    else if (chp->child != NULL)
        print_class_declaration(chp->child, level, indent + 4);
    for (i = 0; i < indent; i++)
        putchar(' ');
    printf("}\n");
}

void print_method_code(u1 *code, u4 codelen, class_file *cfp);

void print_method_name(class_file *cfp, method_info *pmi)
{
    jstring *mname, *mdescr;

    mname = &cfp->constant_pool[pmi->name_index].data.jstr;
    mdescr = &cfp->constant_pool[pmi->descriptor_index].data.jstr;
    if (equals_native(mname, "<init>")) {
        jclass_info *tmp = &cfp->constant_pool[cfp->this_class].data.class_info;
        mname = &cfp->constant_pool[tmp->name_index].data.jstr;
    }
    else if (equals_native(mname, "<clinit>")) {
        printf("Method static {}");
        return;
    }
    printf("Method %s", gen_method_header(mname, mdescr));
}

void print_method_name_by_methodref(class_file *cfp, jmethodref_info *pmi)
{
    jstring *mname, *mdescr;
    jnametype_info *pni;

    pni = &cfp->constant_pool[pmi->name_and_type_index].data.nt_info;
    mname = &cfp->constant_pool[pni->name_index].data.jstr;
    mdescr = &cfp->constant_pool[pni->descriptor_index].data.jstr;
    if (equals_native(mname, "<init>")) {
        jclass_info *tmp = &cfp->constant_pool[pmi->class_index].data.class_info;
        mname = &cfp->constant_pool[tmp->name_index].data.jstr;
    }
    printf("<Method %s>", gen_method_header(mname, mdescr));
}

void print_method_name_by_ifmethodref(class_file *cfp, jifmethodref_info *pmi)
{
    jstring *mname, *mdescr;
    jnametype_info *pni;

    pni = &cfp->constant_pool[pmi->name_and_type_index].data.nt_info;
    mname = &cfp->constant_pool[pni->name_index].data.jstr;
    mdescr = &cfp->constant_pool[pni->descriptor_index].data.jstr;
    if (equals_native(mname, "<init>")) {
        jclass_info *tmp = &cfp->constant_pool[pmi->class_index].data.class_info;
        mname = &cfp->constant_pool[tmp->name_index].data.jstr;
    }
    printf("<Method %s>", gen_method_header(mname, mdescr));
}

void print_exception_table(class_file *cfp, attr_code *pca)
{
    int i;
    exception_table_entry *pet;

    if (pca->exception_table_length > 0) {
        printf("Exception table:\n");
        printf("%8s%8s%8s%5s\n", "from", "to", "target", "type");
    }
    for (i = 0; i < pca->exception_table_length; i++) {
        pet = &pca->exception_table[i];
        printf("%8d", pet->start_pc);
        printf("%8d", pet->end_pc);
        printf("%8d", pet->handler_pc);
        if (pet->catch_type > 0) {
            printf("%8s", "<Class ");
            print_classname(cfp, &cfp->constant_pool[pet->catch_type].data.class_info);
            printf(">\n");
        }
    }
}

void print_code(class_hierarchy *chp)
{
    int i, j;
    attr_code *pca;
    method_info *pmi;
    class_file *cfp = chp->class;

    for (i = 0; i < cfp->methods_count; i++) {
        pmi = &cfp->methods[i];
        /*print_classname(cfp, &cfp->constant_pool[cfp->this_class].data.class_info);
          putchar('.');*/
        print_method_name(cfp, pmi);
        printf("\n");
        for (j = 0; j < pmi->attributes_count; j++) {
            if (pmi->attributes[j].attr_type == ATTR_CODE) {
                pca = &pmi->attributes[j].data.code;
                print_method_code(pca->code, pca->code_length, cfp);
                print_exception_table(cfp, pca);
                break;
            }
        }
        printf("\n");
    }
    if (chp->sibling != NULL)
        print_code(chp->sibling);
    if (chp->child != NULL)
        print_code(chp->child);
}

void print_method_code(u1 *code, u4 codelen, class_file *cfp)
{
    u1 *base = code;
    u1 **pc = &code;
    u1 op;
    u1 cpidx;
    u2 cpidx_w;
    s4 low, high;
    int i;

    while (*pc - base < codelen) {
        printf("%6u ", *pc - base);
        op = get_u1(pc);
        switch (op) {
        case ACONST_NULL:
            printf("aconst_null\n");
            break;
        case ICONST_M1:
            printf("iconst_m1\n");
            break;
        case ICONST_0:
            printf("iconst_0\n");
            break;
        case ICONST_1:
            printf("iconst_1\n");
            break;
        case ICONST_2:
            printf("iconst_2\n");
            break;
        case ICONST_3:
            printf("iconst_3\n");
            break;
        case ICONST_4:
            printf("iconst_4\n");
            break;
        case ICONST_5:
            printf("iconst_5\n");
            break;
        case LCONST_0:
            printf("lconst_0\n");
            break;
        case LCONST_1:
            printf("lconst_1\n");
            break;
        case FCONST_0:
            printf("fconst_0\n");
            break;
        case FCONST_1:
            printf("fconst_1\n");
            break;
        case FCONST_2:
            printf("fconst_2\n");
            break;
        case DCONST_0:
            printf("dconst_0\n");
            break;
        case DCONST_1:
            printf("dconst_1\n");
            break;
        case BIPUSH:
            printf("bipush %d\n", get_s1(pc));
            break;
        case SIPUSH:
            printf("sipush %d\n", get_s2(pc));
            break;
        case LDC:
            cpidx = get_u1(pc);
            printf("ldc #%d ", cpidx);
            print_constant_value(cfp, cfp->constant_pool[cpidx]);
            printf("\n");
            break;
        case LDC_W:
            cpidx_w = get_u2(pc);
            printf("ldc_w #%d ", cpidx_w);
            print_constant_value(cfp, cfp->constant_pool[cpidx_w]);
            printf("\n");
            break;
        case LDC2_W:
            cpidx_w = get_u2(pc);
            printf("ldc2_w #%d ", cpidx_w);
            print_constant_value(cfp, cfp->constant_pool[cpidx_w]);
            printf("\n");
            break;
        case ILOAD:
            printf("iload %d\n", get_u1(pc));
            break;
        case LLOAD:
            printf("lload %d\n", get_u1(pc));
            break;
        case FLOAD:
            printf("fload %d\n", get_u1(pc));
            break;
        case DLOAD:
            printf("dload %d\n", get_u1(pc));
            break;
        case ALOAD:
            printf("aload %d\n", get_u1(pc));
            break;
        case ILOAD_0:
            printf("iload_0\n");
            break;
        case ILOAD_1:
            printf("iload_1\n");
            break;
        case ILOAD_2:
            printf("iload_2\n");
            break;
        case ILOAD_3:
            printf("iload_3\n");
            break;
        case LLOAD_0:
            printf("lload_0\n");
            break;
        case LLOAD_1:
            printf("lload_1\n");
            break;
        case LLOAD_2:
            printf("lload_2\n");
            break;
        case LLOAD_3:
            printf("lload_3\n");
            break;
        case FLOAD_0:
            printf("fload_0\n");
            break;
        case FLOAD_1:
            printf("fload_1\n");
            break;
        case FLOAD_2:
            printf("fload_2\n");
            break;
        case FLOAD_3:
            printf("fload_3\n");
            break;
        case DLOAD_0:
            printf("dload_0\n");
            break;
        case DLOAD_1:
            printf("dload_1\n");
            break;
        case DLOAD_2:
            printf("dload_2\n");
            break;
        case DLOAD_3:
            printf("dload_3\n");
            break;
        case ALOAD_0:
            printf("aload_0\n");
            break;
        case ALOAD_1:
            printf("aload_1\n");
            break;
        case ALOAD_2:
            printf("aload_2\n");
            break;
        case ALOAD_3:
            printf("aload_3\n");
            break;
        case IALOAD:
            printf("iaload\n");
            break;
        case LALOAD:
            printf("laload\n");
            break;
        case FALOAD:
            printf("faload\n");
            break;
        case DALOAD:
            printf("daload\n");
            break;
        case AALOAD:
            printf("aaload\n");
            break;
        case BALOAD:
            printf("baload\n");
            break;
        case CALOAD:
            printf("caload\n");
            break;
        case SALOAD:
            printf("saload\n");
            break;
        case ISTORE:
            printf("istore %d\n", get_u1(pc));
            break;
        case LSTORE:
            printf("lstore %d\n", get_u1(pc));
            break;
        case FSTORE:
            printf("fstore %d\n", get_u1(pc));
            break;
        case DSTORE:
            printf("dstore %d\n", get_u1(pc));
            break;
        case ASTORE:
            printf("astore %d\n", get_u1(pc));
            break;
        case ISTORE_0:
            printf("istore_0\n");
            break;
        case ISTORE_1:
            printf("istore_1\n");
            break;
        case ISTORE_2:
            printf("istore_2\n");
            break;
        case ISTORE_3:
            printf("istore_3\n");
            break;
        case LSTORE_0:
            printf("lstore_0\n");
            break;
        case LSTORE_1:
            printf("lstore_1\n");
            break;
        case LSTORE_2:
            printf("lstore_2\n");
            break;
        case LSTORE_3:
            printf("lstore_3\n");
            break;
        case FSTORE_0:
            printf("fstore_0\n");
            break;
        case FSTORE_1:
            printf("fstore_1\n");
            break;
        case FSTORE_2:
            printf("fstore_2\n");
            break;
        case FSTORE_3:
            printf("fstore_3\n");
            break;
        case DSTORE_0:
            printf("dstore_0\n");
            break;
        case DSTORE_1:
            printf("dstore_1\n");
            break;
        case DSTORE_2:
            printf("dstore_2\n");
            break;
        case DSTORE_3:
            printf("dstore_3\n");
            break;
        case ASTORE_0:
            printf("astore_0\n");
            break;
        case ASTORE_1:
            printf("astore_1\n");
            break;
        case ASTORE_2:
            printf("astore_2\n");
            break;
        case ASTORE_3:
            printf("astore_3\n");
            break;
        case IASTORE:
            printf("iastore\n");
            break;
        case LASTORE:
            printf("lastore\n");
            break;
        case FASTORE:
            printf("fastore\n");
            break;
        case DASTORE:
            printf("dastore\n");
            break;
        case AASTORE:
            printf("aastore\n");
            break;
        case BASTORE:
            printf("bastore\n");
            break;
        case CASTORE:
            printf("castore\n");
            break;
        case SASTORE:
            printf("sastore\n");
            break;
        case POP:
            printf("pop\n");
            break;
        case POP2:
            printf("pop2\n");
            break;
        case DUP:
            printf("dup\n");
            break;
        case DUP_X1:
            printf("dup_x1\n");
            break;
        case DUP_X2:
            printf("dup2_x2\n");
            break;
        case DUP2:
            printf("dup2\n");
            break;
        case DUP2_X1:
            printf("dup2_x1\n");
            break;
        case DUP2_X2:
            printf("dup2_x2\n");
            break;
        case SWAP:
            printf("swap\n");
            break;
        case IADD:
            printf("iadd\n");
            break;
        case LADD:
            printf("ladd\n");
            break;
        case FADD:
            printf("fadd\n");
            break;
        case DADD:
            printf("dadd\n");
            break;
        case ISUB:
            printf("isub\n");
            break;
        case LSUB:
            printf("lsub\n");
            break;
        case FSUB:
            printf("fsub\n");
            break;
        case DSUB:
            printf("dsub\n");
            break;
        case IMUL:
            printf("imul\n");
            break;
        case LMUL:
            printf("lmul\n");
            break;
        case FMUL:
            printf("fmul\n");
            break;
        case DMUL:
            printf("dmul\n");
            break;
        case IDIV:
            printf("idiv\n");
            break;
        case LDIV:
            printf("ldiv\n");
            break;
        case FDIV:
            printf("fdiv\n");
            break;
        case DDIV:
            printf("ddiv\n");
            break;
        case IREM:
            printf("irem\n");
            break;
        case LREM:
            printf("lrem\n");
            break;
        case FREM:
            printf("frem\n");
            break;
        case DREM:
            printf("drem\n");
            break;
        case INEG:
            printf("ineg\n");
            break;
        case LNEG:
            printf("lneg\n");
            break;
        case FNEG:
            printf("fneg\n");
            break;
        case DNEG:
            printf("dneg\n");
            break;
        case ISHL:
            printf("ishl\n");
            break;
        case LSHL:
            printf("lshl\n");
            break;
        case ISHR:
            printf("ishr\n");
            break;
        case LSHR:
            printf("lshr\n");
            break;
        case IUSHR:
            printf("iushr\n");
            break;
        case LUSHR:
            printf("lushr\n");
            break;
        case IAND:
            printf("iand\n");
            break;
        case LAND:
            printf("land\n");
            break;
        case IOR:
            printf("ior\n");
            break;
        case LOR:
            printf("lor\n");
            break;
        case IXOR:
            printf("ixor\n");
            break;
        case LXOR:
            printf("lxor\n");
            break;
        case IINC:
            printf("iinc %d, %d\n", get_u1(pc), get_s1(pc));
            break;
        case I2L:
            printf("i2l\n");
            break;
        case I2F:
            printf("i2f\n");
            break;
        case I2D:
            printf("i2d\n");
            break;
        case L2I:
            printf("l2i\n");
            break;
        case L2F:
            printf("l2f\n");
            break;
        case L2D:
            printf("l2d\n");
            break;
        case F2I:
            printf("f2i\n");
            break;
        case F2L:
            printf("f2l\n");
            break;
        case F2D:
            printf("f2d\n");
            break;
        case D2I:
            printf("d2i\n");
            break;
        case D2L:
            printf("d2l\n");
            break;
        case D2F:
            printf("d2f\n");
            break;
        case I2B:
            printf("i2b\n");
            break;
        case I2C:
            printf("i2c\n");
            break;
        case I2S:
            printf("i2s\n");
            break;
        case LCMP:
            printf("lcmp\n");
            break;
        case FCMPL:
            printf("fcmpl\n");
            break;
        case FCMPG:
            printf("fcmpg\n");
            break;
        case DCMPL:
            printf("dcmpl\n");
            break;
        case DCMPG:
            printf("dcmpg\n");
            break;
        case IFEQ:
            printf("ifeq %d\n", get_s2(pc));
            break;
        case IFNE:
            printf("ifne %d\n", get_s2(pc));
            break;
        case IFLT:
            printf("iflt %d\n", get_s2(pc));
            break;
        case IFGE:
            printf("ifge %d\n", get_s2(pc));
            break;
        case IFGT:
            printf("ifgt %d\n", get_s2(pc));
            break;
        case IFLE:
            printf("ifle %d\n", get_s2(pc));
            break;
        case IF_ICMPEQ:
            printf("if_icmpeq %d\n", get_s2(pc));
            break;
        case IF_ICMPNE:
            printf("if_icmpne %d\n", get_s2(pc));
            break;
        case IF_ICMPLT:
            printf("if_icmplt %d\n", get_s2(pc));
            break;
        case IF_ICMPGE:
            printf("if_icmpge %d\n", get_s2(pc));
            break;
        case IF_ICMPGT:
            printf("if_icmpgt %d\n", get_s2(pc));
            break;
        case IF_ICMPLE:
            printf("if_icmple %d\n", get_s2(pc));
            break;
        case IF_ACMPEQ:
            printf("if_acmpeq %d\n", get_s2(pc));
            break;
        case IF_ACMPNE:
            printf("if_acmpne %d\n", get_s2(pc));
            break;
        case GOTO:
            printf("goto %d\n", get_s2(pc));
            break;
        case JSR:
            printf("jsr %d\n", get_s2(pc));
            break;
        case RET:
            printf("ret %d\n", get_u1(pc));
            break;
        case TABLESWITCH:
            while ((*pc - base) % 4 != 0)
                (*pc)++;
            printf("tableswitch %d, ", get_s4(pc));
            low = get_s4(pc);
            high = get_s4(pc);
            printf("(%d .. %d), ", low, high);
            for (i = low; i <= high; i++)
                printf("%d ", get_s4(pc));
            printf("\n");
            break;
        case LOOKUPSWITCH:
            while ((*pc - base) % 4 != 0)
                (*pc)++;
            printf("lookupswitch %d, ", get_s4(pc));
            high = get_s4(pc);
            printf("%d, ", high);
            for (i = 0; i < high; i++) {
                printf("(%d, ", get_s4(pc));
                printf("%d) ", get_s4(pc));
            }
            printf("\n");
            break;
        case IRETURN:
            printf("ireturn\n");
            break;
        case LRETURN:
            printf("lreturn\n");
            break;
        case FRETURN:
            printf("freturn\n");
            break;
        case DRETURN:
            printf("dreturn\n");
            break;
        case ARETURN:
            printf("areturn\n");
            break;
        case RETURN:
            printf("return\n");
            break;
        case GETSTATIC:
            cpidx_w = get_u2(pc);
            printf("getstatic #%d ", cpidx_w);
            print_field_by_ref(cfp, &cfp->constant_pool[cpidx_w].data.fr_info);
            printf("\n");
            break;
        case PUTSTATIC:
            cpidx_w = get_u2(pc);
            printf("putstatic #%d ", cpidx_w);
            print_field_by_ref(cfp, &cfp->constant_pool[cpidx_w].data.fr_info);
            printf("\n");
            break;
        case GETFIELD:
            cpidx_w = get_u2(pc);
            printf("getfield #%d ", cpidx_w);
            print_field_by_ref(cfp, &cfp->constant_pool[cpidx_w].data.fr_info);
            printf("\n");
            break;
        case PUTFIELD:
            cpidx_w = get_u2(pc);
            printf("putfield #%d ", cpidx_w);
            print_field_by_ref(cfp, &cfp->constant_pool[cpidx_w].data.fr_info);
            printf("\n");
            break;
        case INVOKEVIRTUAL:
            cpidx_w = get_u2(pc);
            printf("invokevirtual #%d ", cpidx_w);
            print_method_name_by_methodref(cfp, &cfp->constant_pool[cpidx_w].data.mr_info);
            printf("\n");
            break;
        case INVOKESPECIAL:
            cpidx_w = get_u2(pc);
            printf("invokespecial #%d ", cpidx_w);
            print_method_name_by_methodref(cfp, &cfp->constant_pool[cpidx_w].data.mr_info);
            printf("\n");
            break;
        case INVOKESTATIC:
            cpidx_w = get_u2(pc);
            printf("invokestatic #%d ", cpidx_w);
            print_method_name_by_methodref(cfp, &cfp->constant_pool[cpidx_w].data.mr_info);
            printf("\n");
            break;
        case INVOKEINTERFACE:
            cpidx_w = get_u2(pc);
            printf("invokeinterface #%d ", cpidx_w);
            print_method_name_by_ifmethodref(cfp, &cfp->constant_pool[cpidx_w].data.imr_info);
            printf("\n");
            break;
        case NEW:
            cpidx_w = get_u2(pc);
            printf("new #%d ", cpidx_w);
            printf("<Class ");
            print_classname(cfp, &cfp->constant_pool[cpidx_w].data.class_info);
            printf(">\n");
            break;
        case NEWARRAY:
            printf("newarray %d\n", get_u1(pc));
            break;
        case ANEWARRAY:
            cpidx_w = get_u2(pc);
            printf("anewarray #%d ", cpidx_w);
            printf("<Class ");
            print_classname(cfp, &cfp->constant_pool[cpidx_w].data.class_info);
            printf(">\n");
            break;
        case ARRAYLENGTH:
            printf("arraylength\n");
            break;
        case ATHROW:
            printf("athrow\n");
            break;
        case CHECKCAST:
            cpidx_w = get_u2(pc);
            printf("checkcast #%d ", cpidx_w);
            printf("<Class ");
            print_classname(cfp, &cfp->constant_pool[cpidx_w].data.class_info);
            printf(">\n");
            break;
        case INSTANCEOF:
            cpidx_w = get_u2(pc);
            printf("instanceof #%d ", cpidx_w);
            printf("<Class ");
            print_classname(cfp, &cfp->constant_pool[cpidx_w].data.class_info);
            printf(">\n");
            break;
        case MONITORENTER:
            printf("monitorenter\n");
            break;
        case MONITOREXIT:
            printf("monitorexit\n");
            break;
        case WIDE:
            printf("wide ");
            op = get_u1(pc); /* the opcode */
            if (op == IINC)
                printf("iinc, %d, %d\n", get_u2(pc), get_s2(pc));
            else
                printf("%d, %d\n", op, get_u2(pc));
            break;
        case MULTIANEWARRAY:
            cpidx_w = get_u2(pc);
            printf("multianewarray #%d, %d ", cpidx_w, get_u1(pc));
            printf("<Class ");
            print_classname(cfp, &cfp->constant_pool[cpidx_w].data.class_info);
            printf(">\n");
            break;
        case IFNULL:
            printf("ifnull %d\n", get_s2(pc));
            break;
        case IFNONNULL:
            printf("ifnonnull %d\n", get_s2(pc));
            break;
        case GOTO_W:
            printf("goto_w %d\n", get_s4(pc));
            break;
        case JSR_W:
            printf("jsr_w %d\n", get_s4(pc));
            break;
        case BREAKPOINT:
            printf("breakpoint\n");
            break;
        case IMPDEP1:
            printf("impdep1\n");
            break;
        case IMPDEP2:
            printf("impdep2\n");
            break;
        default:
            printf("<unknown opcode>\n");
            break;
        }
    }
}
