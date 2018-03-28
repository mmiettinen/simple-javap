
#ifndef CLASSFILE_H
#define CLASSFILE_H

#include <stdio.h>

#include "attribute.h"
#include "defs.h"
#include "jstring.h"

/* 
 * Access flags: first class flags, then field flags and finally method flags.
 */
#define ACC_CLASS_PUBLIC     0x0001
#define ACC_CLASS_PRIVATE    0x0002
#define ACC_CLASS_PROTECTED  0x0004
#define ACC_CLASS_STATIC     0x0008
#define ACC_CLASS_FINAL      0x0010
#define ACC_CLASS_SUPER      0x0020
#define ACC_CLASS_INTERFACE  0x0200
#define ACC_CLASS_ABSTRACT   0x0400
#define ACC_CLASS_SYNTHETIC  0x1000
#define ACC_CLASS_ANNOTATION 0x2000
#define ACC_CLASS_ENUM       0x4000

#define ACC_FIELD_PUBLIC    0x0001
#define ACC_FIELD_PRIVATE   0x0002
#define ACC_FIELD_PROTECTED 0x0004
#define ACC_FIELD_STATIC    0x0008
#define ACC_FIELD_FINAL     0x0010
#define ACC_FIELD_VOLATILE  0x0040
#define ACC_FIELD_TRANSIENT 0x0080
#define ACC_FIELD_SYNTHETIC 0x1000
#define ACC_FIELD_ENUM      0x4000

#define ACC_METHOD_PUBLIC       0x0001
#define ACC_METHOD_PRIVATE      0x0002
#define ACC_METHOD_PROTECTED    0x0004
#define ACC_METHOD_STATIC       0x0008
#define ACC_METHOD_FINAL        0x0010
#define ACC_METHOD_SYNCHRONIZED 0x0020
#define ACC_METHOD_BRIDGE       0x0040
#define ACC_METHOD_VARARGS      0x0080
#define ACC_METHOD_NATIVE       0x0100
#define ACC_METHOD_ABSTRACT     0x0400
#define ACC_METHOD_STRICT       0x0800
#define ACC_METHOD_SYNTHETIC    0x1000

#define ACC_PUBLIC        0x0001
#define ACC_PRIVATE       0x0002
#define ACC_PROTECTED     0x0004
#define ACC_STATIC        0x0008
#define ACC_FINAL         0x0010
#define ACC_SUPER         0x0020
#define ACC_SYNCHRONIZED  0x0020
#define ACC_VOLATILE      0x0040
#define ACC_BRIDGE        0x0040
#define ACC_TRANSIENT     0x0080
#define ACC_VARARGS       0x0080
#define ACC_NATIVE        0x0100
#define ACC_INTERFACE     0x0200
#define ACC_ABSTRACT      0x0400
#define ACC_STRICT        0x0800
#define ACC_SYNTHETIC     0x1000
#define ACC_ANNOTATION    0x2000
#define ACC_ENUM          0x4000

/* 
 * Tags for different constant pool entries. 
 */
#define CONSTANT_CLASS              7
#define CONSTANT_FIELDREF           9
#define CONSTANT_METHODREF          10
#define CONSTANT_INTERFACEMETHODREF 11
#define CONSTANT_STRING             8
#define CONSTANT_INTEGER            3
#define CONSTANT_FLOAT              4
#define CONSTANT_LONG               5
#define CONSTANT_DOUBLE             6
#define CONSTANT_NAMEANDTYPE        12
#define CONSTANT_UTF8               1


/* 
 * Type definitions for constant pool entries. 
 */
typedef struct {
    u1 tag;
    u2 name_index;
} jclass_info;

typedef struct {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
} jfieldref_info;

typedef struct {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
} jmethodref_info;

typedef struct {
    u1 tag;
    u2 class_index;
    u2 name_and_type_index;
} jifmethodref_info;

typedef struct {
    u1 tag;
    u2 string_index;
} jstring_info;

typedef jstring_info jutf8_info;

typedef struct {
    u1 tag;
    jint value;
} jinteger_info;

typedef struct {
    u1 tag;
    jfloat value;
} jfloat_info;

typedef struct {
    u1 tag;
    jlong value;
} jlong_info;

typedef struct {
    u1 tag;
    jdouble value;
} jdouble_info;

typedef struct {
    u1 tag;
    u2 name_index;
    u2 descriptor_index;
} jnametype_info;


/* 
 * The field_info structure.
 */
typedef struct {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    attribute *attributes;
} field_info;


/* 
 * The method_info structure.
 */
typedef struct {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    attribute *attributes;
} method_info;


typedef struct {
    u1 entry_type;
    void *entry;
} cpentry;

typedef struct {
    u1 tag;
    union {
        jclass_info class_info;
        jfieldref_info fr_info;
        jmethodref_info mr_info;
        jifmethodref_info imr_info;
        jstring_info string_info;
        jstring jstr;
        jinteger_info int_info;
        jfloat_info float_info;
        jlong_info long_info;
        jdouble_info double_info;
        jnametype_info nt_info;
    } data;
} cp_entry;

/*
 * The class file structure.
 */
typedef struct {
    u2 minor_version;
    u2 major_version;
    u2 constant_pool_count;
    cp_entry *constant_pool;
    u2 access_flags;
    u2 this_class;
    u2 super_class;
    u2 interfaces_count;
    u2 *interfaces;
    u2 fields_count;
    field_info *fields;
    u2 methods_count;
    method_info *methods;
    u2 attributes_count;
    attribute *attributes;
} class_file;

typedef struct class_hierarchy_t class_hierarchy;

struct class_hierarchy_t {
    class_file *class;
    u2 access_flags;
    class_hierarchy *sibling;
    class_hierarchy *child;
};

extern class_hierarchy *load_class_hierarchy(char *fname);
extern void free_class_hierarchy(class_hierarchy *chp);


#endif



