
#include <stdio.h>
#include <stdlib.h>

#include "classfile.h"
#include "defs.h"
#include "jstring.h"

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
 * Bitmasks for UTF-8 conversions. 
 */
#define UTF8_MASK          0xf0
#define ONEBYTE_FLAG       0x00
#define TWOBYTE_HI_FLAG    0xc0
#define TWOBYTE_LO_FLAG    0x80
#define THREEBYTE_HI_FLAG  0xe0
#define THREEBYTE_MID_FLAG 0x80
#define THREEBYTE_LO_FLAG  0x80

static const char *classfile_name;

int get_class(infile *fp, jclass_info *res)
{
    res->tag = CONSTANT_CLASS;
    if (read_u2(fp, &res->name_index) == RS_EOF)
        return -1;
    return 0;
}

int get_fieldref(infile *fp, jfieldref_info *res)
{
    res->tag = CONSTANT_FIELDREF;
    if (read_u2(fp, &res->class_index) == RS_EOF)
        return -1;
    if (read_u2(fp, &res->name_and_type_index) == RS_EOF)
        return -1;
    return 0;
}

int get_methodref(infile *fp, jmethodref_info *res)
{
    res->tag = CONSTANT_METHODREF;
    if (read_u2(fp, &res->class_index) == RS_EOF)
        return -1;
    if (read_u2(fp, &res->name_and_type_index) == RS_EOF)
        return -1;
    return 0;
}

int get_ifmethodref(infile *fp, jifmethodref_info *res)
{
    res->tag = CONSTANT_INTERFACEMETHODREF;
    if (read_u2(fp, &res->class_index) == RS_EOF)
        return -1;
    if (read_u2(fp, &res->name_and_type_index) == RS_EOF)
        return -1;
    return 0;
}

int get_string(infile *fp, jstring_info *res)
{
    res->tag = CONSTANT_STRING;
    if (read_u2(fp, &res->string_index) == RS_EOF)
        return -1;
    return 0;
}

int get_integer(infile *fp, jinteger_info *res)
{
    u4 val;
    res->tag = CONSTANT_INTEGER;
    if (read_u4(fp, &val) == RS_EOF)
        return -1;
    res->value = (jint)val;
    return 0;
}

int get_float(infile *fp, jfloat_info *res)
{
    u4 val;
    float *fval;
    
    if (read_u4(fp, &val) == RS_EOF)
        return -1;
    fval = (float *)(&val);
    res->tag = CONSTANT_FLOAT;
    res->value = *fval;
    return 0;
}

int get_long(infile *fp, jlong_info *res)
{
    u4 high;
    u4 low;

    if (read_u4(fp, &high) == RS_EOF)
        return -1;
    if (read_u4(fp, &low) == RS_EOF)
        return -1;
    res->tag = CONSTANT_LONG;
    res->value = high;
    res->value = (res->value << 32) | low;
    return 0;
}

int get_double(infile *fp, jdouble_info *res)
{
    jdouble *dval;
    jlong lval;
    jlong_info tmp; 
    if (get_long(fp, &tmp) == -1)
        return -1;
    lval = tmp.value;
    dval = (double *)(&lval);

    res->tag = CONSTANT_DOUBLE;
    res->value = *dval;
    return 0;
}

int get_nameandtype(infile *fp, jnametype_info *res)
{
    res->tag = CONSTANT_NAMEANDTYPE;
    if (read_u2(fp, &res->name_index) == RS_EOF)
        return -1;
    if (read_u2(fp, &res->descriptor_index) == RS_EOF)
        return -1;
    return 0;
}

int get_utf8(infile *fp, jstring *res)
{
    u2 bytecnt; 
    if (read_u2(fp, &bytecnt) == RS_EOF)
        return -1;
    if ((res = create_jstring(fp, bytecnt, res)) == NULL)
        return -1;
    res->tag = CONSTANT_UTF8;
    return 0;
}

int create_field_info(infile *fp, class_file *cfp, field_info *res)
{
    int i;
    jstring *sc;

    if (read_u2(fp, &res->access_flags) == RS_EOF)
        return -1;
    if (read_u2(fp, &res->name_index) == RS_EOF)
        return -1;
    if (read_u2(fp, &res->descriptor_index) == RS_EOF)
        return -1;
    if (read_u2(fp, &res->attributes_count) == RS_EOF)
        return -1;
    if ((res->attributes = (attribute *)calloc(res->attributes_count, sizeof(attribute))) == NULL) {
        error_number = ERR_MEM_ALLOC;
        return -1;
    }

    for (i = 0; i < res->attributes_count; i++) {
        u2 attr_name_idx;
        u4 attr_length;
        if (read_u2(fp, &attr_name_idx) == RS_EOF)
            return -1;
        if (read_u4(fp, &attr_length) == RS_EOF)
            return -1;
        sc = &(cfp->constant_pool[attr_name_idx].data.jstr);
        if (equals_native(sc, "ConstantValue")) {
            res->attributes[i].attr_type = ATTR_CONST_VALUE;
            if (create_const_value_attribute(fp, 
                                             attr_name_idx, 
                                             attr_length, 
                                             &res->attributes[i].data.const_value) == -1)
                return -1;
        }
        else if (equals_native(sc, "Synthetic")) {
            res->attributes[i].attr_type = ATTR_SYNTHETIC;
            if (create_synthetic_attribute(attr_name_idx, 
                                           attr_length, 
                                           &res->attributes[i].data.synthetic) == -1)
                return -1;
        }
        else if (equals_native(sc, "Signature")) {
            res->attributes[i].attr_type = ATTR_SIGNATURE;
            if (create_signature_attribute(fp, 
                                           attr_name_idx, 
                                           attr_length, 
                                           &res->attributes[i].data.signature) == -1)
                return -1;
        }
        else if (equals_native(sc, "Deprecated")) {
            res->attributes[i].attr_type = ATTR_DEPRECATED;
            if (create_deprecated_attribute(attr_name_idx, 
                                            attr_length, 
                                            &res->attributes[i].data.deprecated) == -1)
                return -1;
        }
        else if (equals_native(sc, "RuntimeVisibleAnnotations")) {
            res->attributes[i].attr_type = ATTR_RUNTIME_VISIBLE_ANNOTATIONS;
            if (create_runtime_visible_annotations_attribute(fp,
                                                             attr_name_idx,
                                                             attr_length,
                                                             &res->attributes[i].data.run_vis_ann) == -1)
                return -1;
        }
        else if (equals_native(sc, "RuntimeInvisibleAnnotations")) {
            res->attributes[i].attr_type = ATTR_RUNTIME_INVISIBLE_ANNOTATIONS;
            if (create_runtime_invisible_annotations_attribute(fp,
                                                               attr_name_idx,
                                                               attr_length,
                                                               &res->attributes[i].data.run_inv_ann) == -1)
                return -1;
        }
        else {
            error_number = ERR_INCORRECT_CLASS;
            return -1;
        }
    }
    return 0;
}

void free_field_info(field_info *ptr)
{
    int i;

    if (ptr == NULL)
        return;

    if (ptr->attributes != NULL) {
        for (i = 0; i < ptr->attributes_count; i++) {
            if (ptr->attributes[i].attr_type == ATTR_RUNTIME_VISIBLE_ANNOTATIONS)
                free_runtime_visible_annotations_attribute(&ptr->attributes[i].data.run_vis_ann);
            else if (ptr->attributes[i].attr_type == ATTR_RUNTIME_INVISIBLE_ANNOTATIONS)
                free_runtime_invisible_annotations_attribute(&ptr->attributes[i].data.run_inv_ann);
        }
        free(ptr->attributes);
    }
}

int create_method_info(infile *fp, class_file *cfp, method_info *res)
{
    jstring *sc;
    int i;

    if (read_u2(fp, &res->access_flags) == RS_EOF)
        return -1;
    if (read_u2(fp, &res->name_index) == RS_EOF)
        return -1;
    if (read_u2(fp, &res->descriptor_index) == RS_EOF)
        return -1;
    if (read_u2(fp, &res->attributes_count) == RS_EOF)
        return -1;
    if ((res->attributes = (attribute *)calloc(res->attributes_count, sizeof(attribute))) == NULL) {
        error_number = ERR_MEM_ALLOC;
        return -1;
    }

    for (i = 0; i < res->attributes_count; i++) {
        u2 attr_name_idx;
        u4 attr_length;
        if (read_u2(fp, &attr_name_idx) == RS_EOF)
            return -1;
        if (read_u4(fp, &attr_length) == RS_EOF)
            return -1;
        sc = &(cfp->constant_pool[attr_name_idx].data.jstr);
        if (equals_native(sc, "Code")) {
            res->attributes[i].attr_type = ATTR_CODE;
            if (create_code_attribute(cfp, 
                                      fp, 
                                      attr_name_idx, 
                                      attr_length, 
                                      &res->attributes[i].data.code) == -1)
                return -1;
        }
        else if (equals_native(sc, "Exceptions")) {
            res->attributes[i].attr_type = ATTR_EXCEPTIONS; 
            if (create_exceptions_attribute(fp, 
                                            attr_name_idx, 
                                            attr_length, 
                                            &res->attributes[i].data.exceptions) == -1)
                return -1;
        }
        else if (equals_native(sc, "Synthetic")) {
            res->attributes[i].attr_type = ATTR_SYNTHETIC;
            if (create_synthetic_attribute(attr_name_idx, 
                                           attr_length, 
                                           &res->attributes[i].data.synthetic) == -1)
                return -1;
        }
        else if (equals_native(sc, "Signature")) {
            res->attributes[i].attr_type = ATTR_SIGNATURE;
            if (create_signature_attribute(fp, 
                                           attr_name_idx, 
                                           attr_length, 
                                           &res->attributes[i].data.signature) == -1)
                return -1;
        }
        else if (equals_native(sc, "RuntimeVisibleAnnotations")) {
            res->attributes[i].attr_type = ATTR_RUNTIME_VISIBLE_ANNOTATIONS;
            if (create_runtime_visible_annotations_attribute(fp,
                                                             attr_name_idx,
                                                             attr_length,
                                                             &res->attributes[i].data.run_vis_ann) == -1)
                return -1;
        }
        else if (equals_native(sc, "RuntimeInvisibleAnnotations")) {
            res->attributes[i].attr_type = ATTR_RUNTIME_INVISIBLE_ANNOTATIONS;
            if (create_runtime_invisible_annotations_attribute(fp,
                                                               attr_name_idx,
                                                               attr_length,
                                                               &res->attributes[i].data.run_inv_ann) == -1)
                return -1;
        }
        else if (equals_native(sc, "RuntimeVisibleParameterAnnotations")) {
            res->attributes[i].attr_type = ATTR_RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS;
            if (create_runtime_visible_parameter_annotations_attribute(fp,
                                                                       attr_name_idx,
                                                                       attr_length,
                                                                       &res->attributes[i].data.run_vis_param_ann) == -1)
                return -1;
        }
        else if (equals_native(sc, "RuntimeInvisibleParameterAnnotations")) {
            res->attributes[i].attr_type = ATTR_RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS;
            if (create_runtime_invisible_parameter_annotations_attribute(fp,
                                                                         attr_name_idx,
                                                                         attr_length,
                                                                         &res->attributes[i].data.run_inv_param_ann) == -1)
                return -1;
        }
        else if (equals_native(sc, "AnnotationDefault")) {
            res->attributes[i].attr_type = ATTR_ANNOTATION_DEFAULT;
            if (create_annotation_default_attribute(fp, attr_name_idx, attr_length, 
                                                    &res->attributes[i].data.ann_default) == -1)
                return -1;
        }
        else {
            error_number = ERR_INCORRECT_CLASS;
            return -1;
        }
    }
    return 0;
}

void free_method_info(method_info *ptr)
{
    int i;

    if (ptr == NULL)
        return;

    if (ptr->attributes != NULL) {
        for (i = 0; i < ptr->attributes_count; i++) {
            if (ptr->attributes[i].attr_type == ATTR_CODE)
                free_code_attribute(&ptr->attributes[i].data.code);
            else if (ptr->attributes[i].attr_type == ATTR_EXCEPTIONS)
                free_exceptions_attribute(&ptr->attributes[i].data.exceptions);
            else if (ptr->attributes[i].attr_type == ATTR_RUNTIME_VISIBLE_ANNOTATIONS)
                free_runtime_visible_annotations_attribute(&ptr->attributes[i].data.run_vis_ann);
            else if (ptr->attributes[i].attr_type == ATTR_RUNTIME_INVISIBLE_ANNOTATIONS)
                free_runtime_invisible_annotations_attribute(&ptr->attributes[i].data.run_inv_ann);
            else if (ptr->attributes[i].attr_type == ATTR_RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS)
                free_runtime_visible_parameter_annotations_attribute(&ptr->attributes[i].data.run_vis_param_ann);
            else if (ptr->attributes[i].attr_type == ATTR_RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS)
                free_runtime_invisible_parameter_annotations_attribute(&ptr->attributes[i].data.run_inv_param_ann);
            else if (ptr->attributes[i].attr_type == ATTR_ANNOTATION_DEFAULT)
                free_annotation_default_attribute(&ptr->attributes[i].data.ann_default);
        }
        free(ptr->attributes);
    }
}

void free_classfile(class_file *ptr);

class_file *load_classfile(infile *fp, const char *fname)
{
    int i = 1;
    u4 magic;
    u1 tag;
    jstring *sc;
    class_file *cfp = (class_file *)calloc(1, sizeof(class_file));
    cp_entry *cpp;

    if (cfp == NULL) {
        error_number = ERR_MEM_ALLOC;
        return NULL;
    }
    classfile_name = fname;
    /* Start by reading the magic number and version numbers */
    if (read_u4(fp, &magic) == RS_EOF)
        goto error;
    if (magic != 0xcafebabe) {
        error_number = ERR_INCORRECT_MAGIC;
        goto error;
    }
    if (read_u2(fp, &cfp->minor_version) == RS_EOF)
        goto error;
    if (read_u2(fp, &cfp->major_version) == RS_EOF)
        goto error;

    /* Read in the constant pool */
    if (read_u2(fp, &cfp->constant_pool_count) == RS_EOF)
        goto error;
    cfp->constant_pool = (cp_entry *)calloc(cfp->constant_pool_count + 1, 
                                            sizeof(cp_entry));
    if (cfp->constant_pool == NULL) {
        error_number = ERR_MEM_ALLOC;
        goto error;
    }
    cpp = cfp->constant_pool;
    while (i < cfp->constant_pool_count) {
        if (read_u1(fp, &tag) == RS_EOF)
            goto error;
        if (tag == CONSTANT_CLASS) {
            cpp[i].tag = CONSTANT_CLASS;
            if (get_class(fp, &cpp[i].data.class_info) == -1)
                goto error;
        }
        else if (tag == CONSTANT_FIELDREF) {
            cpp[i].tag = CONSTANT_FIELDREF;
            if (get_fieldref(fp, &cpp[i].data.fr_info) == -1)
                goto error;
        }
        else if (tag == CONSTANT_METHODREF) {
            cpp[i].tag = CONSTANT_METHODREF;
            if (get_methodref(fp, &cpp[i].data.mr_info) == -1)
                goto error;
        }
        else if (tag == CONSTANT_INTERFACEMETHODREF) {
            cpp[i].tag = CONSTANT_INTERFACEMETHODREF;
            if (get_ifmethodref(fp, &cpp[i].data.imr_info) == -1)
                goto error;
        }
        else if (tag == CONSTANT_STRING) {
            cpp[i].tag = CONSTANT_STRING;
            if (get_string(fp, &cpp[i].data.string_info) == -1)
                goto error;
        }
        else if (tag == CONSTANT_INTEGER) {
            cpp[i].tag = CONSTANT_INTEGER;
            if (get_integer(fp, &cpp[i].data.int_info) == -1)
                goto error;
        }
        else if (tag == CONSTANT_FLOAT) {
            cpp[i].tag = CONSTANT_FLOAT;
            if (get_float(fp, &cpp[i].data.float_info) == -1)
                goto error;
        }
        else if (tag == CONSTANT_LONG) {
            cpp[i].tag = CONSTANT_LONG;
            if (get_long(fp, &cpp[i].data.long_info) == -1)
                goto error;
        }
        else if (tag == CONSTANT_DOUBLE) {
            cpp[i].tag = CONSTANT_DOUBLE;
            if (get_double(fp, &cpp[i].data.double_info) == -1)
                goto error;
        }
        else if (tag == CONSTANT_NAMEANDTYPE) {
            cpp[i].tag = CONSTANT_NAMEANDTYPE;
            if (get_nameandtype(fp, &cpp[i].data.nt_info) == -1)
                goto error;
        }
        else if (tag == CONSTANT_UTF8) {
            cpp[i].tag = CONSTANT_UTF8;
            if (get_utf8(fp, &cpp[i].data.jstr) == -1)
                goto error;
        }
        else {
            error_number = ERR_INCORRECT_CLASS;
            goto error;
        }
        if (tag == CONSTANT_LONG || tag == CONSTANT_DOUBLE)
            /* For some reason doubles and longs take two entries
               from the constant pool table. This second entry doesn't
               exist in the class file, but the indexes behave as it did.
               So in these cases we must increment the index counter by 2. */
            i += 2;
        else
            i++;
    }

    if (read_u2(fp, &cfp->access_flags) == RS_EOF)
        goto error;
    if (read_u2(fp, &cfp->this_class) == RS_EOF)
        goto error;
    if (read_u2(fp, &cfp->super_class) == RS_EOF)
        goto error;

    /* Interfaces */
    if (read_u2(fp, &cfp->interfaces_count) == RS_EOF)
        goto error;
    if ((cfp->interfaces = (u2 *)calloc(cfp->interfaces_count, sizeof(u2))) == NULL) {
        error_number = ERR_MEM_ALLOC;
        goto error;
    }
    for (i = 0; i < cfp->interfaces_count; i++) {
        if (read_u2(fp, &cfp->interfaces[i]) == RS_EOF)
            goto error;
    }

    /* Fields */
    if (read_u2(fp, &cfp->fields_count) == RS_EOF)
        goto error;
    if ((cfp->fields = (field_info *)calloc(cfp->fields_count, sizeof(field_info))) == NULL) {
        error_number = ERR_MEM_ALLOC;
        goto error;
    }
    for (i = 0; i < cfp->fields_count; i++)
        if (create_field_info(fp, cfp, &cfp->fields[i]) == -1)
            goto error;

    /* Methods */
    if (read_u2(fp, &cfp->methods_count) == RS_EOF)
        goto error;
    if ((cfp->methods = (method_info *)calloc(cfp->methods_count, sizeof(method_info))) == NULL) {
        error_number = ERR_MEM_ALLOC;
        goto error;
    }
    for (i = 0; i < cfp->methods_count; i++)
        if (create_method_info(fp, cfp, &cfp->methods[i]) == -1)
            goto error;

    /* Attributes */
    if (read_u2(fp, &cfp->attributes_count) == RS_EOF)
        goto error;
    if ((cfp->attributes = (attribute *)calloc(cfp->attributes_count, sizeof(attribute))) == NULL) {
        error_number = ERR_MEM_ALLOC;
        goto error;
    }
    for (i = 0; i < cfp->attributes_count; i++) {
        u2 attr_name_idx;
        u4 attr_length;
        if (read_u2(fp, &attr_name_idx) == RS_EOF)
            goto error;
        if (read_u4(fp, &attr_length) == RS_EOF)
            goto error;
        sc = &cfp->constant_pool[attr_name_idx].data.jstr;
        if (equals_native(sc, "InnerClasses")) {
            cfp->attributes[i].attr_type = ATTR_INNER_CLASSES;
            if (create_inner_classes_attribute(fp, attr_name_idx, attr_length, 
                                               &cfp->attributes[i].data.inner_classes) == -1)
                goto error;
        }
        else if (equals_native(sc, "EnclosingMethod")) {
            cfp->attributes[i].attr_type = ATTR_ENCLOSING_METHOD;
            if (create_enclosing_method_attribute(fp, attr_name_idx, attr_length,
                                                  &cfp->attributes[i].data.encl_method) == -1)
                goto error;
        }
        else if (equals_native(sc, "Synthetic")) {
            cfp->attributes[i].attr_type = ATTR_SYNTHETIC;
            if (create_synthetic_attribute(attr_name_idx, 
                                           attr_length, 
                                           &cfp->attributes[i].data.synthetic) == -1)
                goto error;
        }
        else if (equals_native(sc, "Signature")) {
            cfp->attributes[i].attr_type = ATTR_SIGNATURE;
            if (create_signature_attribute(fp, 
                                           attr_name_idx, 
                                           attr_length, 
                                           &cfp->attributes[i].data.signature) == -1)
                goto error;
        }
        else if (equals_native(sc, "SourceFile")) {
            cfp->attributes[i].attr_type = ATTR_SOURCEFILE;
            if (create_sourcefile_attribute(fp, attr_name_idx, attr_length,
                                            &cfp->attributes[i].data.src_file) == -1)
                goto error;
        }
        else if (equals_native(sc, "SourceDebugExtension")) {
            cfp->attributes[i].attr_type = ATTR_SOURCE_DEBUG_EXTENSION;
            if (create_source_debug_extension_attribute(fp, attr_name_idx, attr_length,
                                                        &cfp->attributes[i].data.src_debug_ext) == -1)
                goto error;
        }
        else if (equals_native(sc, "Deprecated")) {
            cfp->attributes[i].attr_type = ATTR_DEPRECATED;
            if (create_deprecated_attribute(attr_name_idx, 
                                            attr_length, 
                                            &cfp->attributes[i].data.deprecated) == -1)
                goto error;
        }
        else if (equals_native(sc, "RuntimeVisibleAnnotations")) {
            cfp->attributes[i].attr_type = ATTR_RUNTIME_VISIBLE_ANNOTATIONS;
            if (create_runtime_visible_annotations_attribute(fp,
                                                             attr_name_idx,
                                                             attr_length,
                                                             &cfp->attributes[i].data.run_vis_ann) == -1)
                goto error;
        }
        else if (equals_native(sc, "RuntimeInvisibleAnnotations")) {
            cfp->attributes[i].attr_type = ATTR_RUNTIME_INVISIBLE_ANNOTATIONS;
            if (create_runtime_invisible_annotations_attribute(fp,
                                                               attr_name_idx,
                                                               attr_length,
                                                               &cfp->attributes[i].data.run_inv_ann) == -1)
                goto error;
        }
        else {
            error_number = ERR_INCORRECT_CLASS;
            goto error;
        }
    }

    return cfp;

 error:
    free_classfile(cfp);
    return NULL;
}

int is_inner_class(class_file *cfp, u2 inneridx)
{
    jstring *thisname, *innername;
    jclass_info *thiscl, *innercl;

    thiscl = &cfp->constant_pool[cfp->this_class].data.class_info;
    innercl = &cfp->constant_pool[inneridx].data.class_info;
    thisname = &cfp->constant_pool[thiscl->name_index].data.jstr;
    innername = &cfp->constant_pool[innercl->name_index].data.jstr;

    return innername->length > thisname->length;
}

class_hierarchy *load_class_hierarchy(char *cfpath)
{
    int i, j;
    attribute *pa;
    attr_inner_classes *pica;
    char cfname[1024];
    class_hierarchy *res = (class_hierarchy *)malloc(sizeof(class_hierarchy));
    class_hierarchy *tmp;
    infile *fp;
    jstring *jstr;

    if ((fp = open_file(cfpath)) == NULL)
        return NULL;
    res->sibling = NULL;
    res->child = NULL;
    if ((res->class = load_classfile(fp, cfpath)) == NULL) {
        close_file(fp);
        return NULL;
    }
    close_file(fp);
    res->access_flags = res->class->access_flags;

    for (i = 0; i < res->class->attributes_count; i++) {
        pa = &res->class->attributes[i];
        if (pa->attr_type != ATTR_INNER_CLASSES)
            continue;
        pica = &pa->data.inner_classes;
        for (j = 0; j < pica->number_of_classes; j++) {
            if (pica->classes[j].inner_name_index == 0 ||
                !is_inner_class(res->class, pica->classes[j].inner_class_info_index))
                continue; /* anonymous inner class */
            jstr = &res->class->constant_pool[pica->classes[j].inner_name_index].data.jstr;
            construct_classfile_name(cfpath, jstr, cfname);
            if ((tmp = load_class_hierarchy(cfname)) == NULL) {
                free(res->class);
                return NULL;
            }
            tmp->access_flags = pica->classes[j].inner_class_access_flags;
            if (res->child == NULL)
                res->child = tmp;
            else {
                tmp->sibling = res->child->sibling;
                res->child->sibling = tmp;
            }
        }
    }

    return res;
}

void free_classfile(class_file *cfp)
{
    u2 i;
    if (cfp == NULL)
        return;
    if (cfp->constant_pool != NULL) {
        for (i = 0; i < cfp->constant_pool_count; i++) {
            if (cfp->constant_pool[i].tag == CONSTANT_UTF8) {
                jstring *str = &cfp->constant_pool[i].data.jstr;
                free(str->data);
            }
        }
        free(cfp->constant_pool);
    }
    free(cfp->interfaces);
    if (cfp->fields != NULL) {
        for (i = 0; i < cfp->fields_count; i++)
            free_field_info(&cfp->fields[i]);
        free(cfp->fields);
    }
    if (cfp->fields != NULL) {
        for (i = 0; i < cfp->methods_count; i++)
            free_method_info(&cfp->methods[i]);
        free(cfp->methods);
    }
    if (cfp->attributes != NULL) {
        for (i = 0; i < cfp->attributes_count; i++) {
            attribute att = cfp->attributes[i];
            if (att.attr_type == ATTR_INNER_CLASSES)
                free_inner_classes_attribute(&att.data.inner_classes);
            else if (att.attr_type == ATTR_SOURCE_DEBUG_EXTENSION)
                free_source_debug_extension_attribute(&att.data.src_debug_ext);
            else if (att.attr_type == ATTR_RUNTIME_VISIBLE_ANNOTATIONS)
                free_runtime_visible_annotations_attribute(&att.data.run_vis_ann);
            else if (att.attr_type == ATTR_RUNTIME_INVISIBLE_ANNOTATIONS)
                free_runtime_invisible_annotations_attribute(&att.data.run_inv_ann);
        }
        free(cfp->attributes);
    }
    free(cfp);
}

void free_class_hierarchy(class_hierarchy *chp)
{
    class_hierarchy *p = chp;

    while (chp != NULL) {
        free_class_hierarchy(chp->child);
        chp->child = NULL;
        free_classfile(chp->class);
        chp->class = NULL;
        p = chp;
        chp = chp->sibling;
        free(p);
    }
}




