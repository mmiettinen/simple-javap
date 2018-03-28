
#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "defs.h"

#define ATTR_CONST_VALUE                             1
#define ATTR_CODE                                    2
#define ATTR_EXCEPTIONS                              3
#define ATTR_INNER_CLASSES                           4
#define ATTR_ENCLOSING_METHOD                        5
#define ATTR_SYNTHETIC                               6
#define ATTR_SIGNATURE                               7
#define ATTR_SOURCEFILE                              8
#define ATTR_SOURCE_DEBUG_EXTENSION                  9
#define ATTR_LINENUMBER_TABLE                        10
#define ATTR_LOCAL_VARIABLE_TABLE                    11
#define ATTR_LOCAL_VARIABLE_TYPE_TABLE               12
#define ATTR_DEPRECATED                              13
#define ATTR_RUNTIME_VISIBLE_ANNOTATIONS             14
#define ATTR_RUNTIME_INVISIBLE_ANNOTATIONS           15
#define ATTR_RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS   16
#define ATTR_RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS 17
#define ATTR_ANNOTATION_DEFAULT                      18


typedef struct attribute_t attribute;

/* 
 * Type definitions for different attributes.
 */
typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 constantvalue_index;
} attr_const_value;

typedef struct {
    u2 start_pc;
    u2 end_pc;
    u2 handler_pc;
    u2 catch_type;
} exception_table_entry;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 max_stack;
    u2 max_locals;
    u4 code_length;
    u1 *code;
    u2 exception_table_length;
    exception_table_entry *exception_table;
    u2 attributes_count;
    attribute *attributes;
} attr_code;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 number_of_exceptions;
    u2 *exception_index_table;
} attr_exceptions;

typedef struct {
    u2 inner_class_info_index;
    u2 outer_class_info_index;
    u2 inner_name_index;
    u2 inner_class_access_flags;
} classes_entry;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 number_of_classes;
    classes_entry *classes;
} attr_inner_classes;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 class_index;
    u2 method_index;
} attr_enclosing_method;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
} attr_synthetic;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 signature_index;
} attr_signature;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 sourcefile_index;
} attr_sourcefile;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u1 *debug_extension;
} attr_source_debug_extension;

typedef struct {
    u2 start_pc;
    u2 line_number;
} line_number_table_entry;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 line_number_table_length;
    line_number_table_entry *line_number_table;
} attr_linenumber_table;

typedef struct {
    u2 start_pc;
    u2 length;
    u2 name_index;
    u2 descriptor_index;
    u2 index;
} local_variable_table_entry;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 local_variable_table_length;
    local_variable_table_entry *local_variable_table;
} attr_local_variable_table;

typedef struct {
    u2 start_pc;
    u2 length;
    u2 name_index;
    u2 signature_index;
    u2 index;
} local_variable_type_table_entry;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 local_variable_type_table_length;
    local_variable_type_table_entry *local_variable_type_table;
} attr_local_variable_type_table;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
} attr_deprecated;

struct element_value_pairs_entry_t;
typedef struct element_value_pairs_entry_t element_value_pairs_entry;

typedef struct {
    u2 type_index;
    u2 num_element_value_pairs;
    element_value_pairs_entry *element_value_pairs;
} annotation;

struct element_value_t {
    u1 tag;
    union {
        u2 const_value_index;
        struct {
            u2 type_name_index;
            u2 const_name_index;
        } enum_const_value;
        u2 class_info_index;
        annotation annotation_value;
        struct {
            u2 num_values;
            struct element_value_t *values;
        } array_value;
    } value;
};

typedef struct element_value_t element_value;

struct element_value_pairs_entry_t {
    u2 element_name_index;
    element_value value;
};

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 num_annotations;
    annotation *annotations;
} attr_runtime_visible_annotations;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 num_annotations;
    annotation *annotations;
} attr_runtime_invisible_annotations;

typedef struct {
    u2 num_annotations;
    annotation *annotations;
} parameter_annotations_entry;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u1 num_parameters;
    parameter_annotations_entry *parameter_annotations;
} attr_runtime_visible_parameter_annotations;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    u1 num_parameters;
    parameter_annotations_entry *parameter_annotations;
} attr_runtime_invisible_parameter_annotations;

typedef struct {
    u2 attribute_name_index;
    u4 attribute_length;
    element_value default_value;
} attr_annotation_default;

struct attribute_t {
    u1 attr_type;
    union {
        attr_const_value const_value;
        attr_code code;
        attr_exceptions exceptions;
        attr_inner_classes inner_classes;
        attr_enclosing_method encl_method;
        attr_synthetic synthetic;
        attr_signature signature;
        attr_sourcefile src_file;
        attr_source_debug_extension src_debug_ext;
        attr_linenumber_table linenum_tbl;
        attr_local_variable_table local_var_tbl;
        attr_local_variable_type_table local_var_type_tbl;
        attr_deprecated deprecated;
        attr_runtime_visible_annotations run_vis_ann;
        attr_runtime_invisible_annotations run_inv_ann;
        attr_runtime_visible_parameter_annotations run_vis_param_ann;
        attr_runtime_invisible_parameter_annotations run_inv_param_ann;
        attr_annotation_default ann_default;
    } data;
};


extern int create_const_value_attribute(infile *fp, 
                                        u2 attr_name_idx, 
                                        u4 attr_length,
                                        attr_const_value *res);

/* The first parameter has to be a pointer to class_file structure.
   Here we use a generic pointer to avoid circular dependencies when
   including this file from classfile.h (where class_file is defined) */
extern int create_code_attribute(void *pcf, 
                                 infile *fp, 
                                 u2 attr_name_idx, 
                                 u4 attr_length,
                                 attr_code *res);

extern int create_exceptions_attribute(infile *fp, 
                                       u2 attr_name_idx, 
                                       u4 attr_length,
                                       attr_exceptions *res);

extern int create_inner_classes_attribute(infile *fp, 
                                          u2 attr_name_idx, 
                                          u4 attr_length,
                                          attr_inner_classes *res);

extern int create_enclosing_method_attribute(infile *fp,
                                             u2 attr_name_idx,
                                             u4 attr_length,
                                             attr_enclosing_method *res);

extern int create_synthetic_attribute(u2 attr_name_idx, 
                                      u4 attr_length,
                                      attr_synthetic *res);

extern int create_signature_attribute(infile *fp,
                                      u2 attr_name_idx,
                                      u4 attr_length,
                                      attr_signature *res);

extern int create_sourcefile_attribute(infile *fp, 
                                       u2 attr_name_idx, 
                                       u4 attr_length,
                                       attr_sourcefile *res);

extern int create_source_debug_extension_attribute(infile *fp,
                                                   u2 attr_name_idx,
                                                   u4 attr_length,
                                                   attr_source_debug_extension *res);

extern int create_linenumber_table_attribute(infile *fp, 
                                             u2 attr_name_idx, 
                                             u4 attr_length,
                                             attr_linenumber_table *res);

extern int create_local_variable_table_attribute(infile *fp, 
                                                 u2 attr_name_idx, 
                                                 u4 attr_length,
                                                 attr_local_variable_table *res);

extern int create_local_variable_type_table_attribute(infile *fp,
                                                      u2 attr_name_idx,
                                                      u4 attr_length,
                                                      attr_local_variable_type_table *res);

extern int create_deprecated_attribute(u2 attr_name_idx,
                                       u4 attr_length,
                                       attr_deprecated *res);

extern int create_runtime_visible_annotations_attribute(infile *fp,
                                                        u2 attr_name_idx,
                                                        u4 attr_length,
                                                        attr_runtime_visible_annotations *res);

extern int create_runtime_invisible_annotations_attribute(infile *fp,
                                                          u2 attr_name_idx,
                                                          u4 attr_length,
                                                          attr_runtime_invisible_annotations *res);

extern int 
create_runtime_visible_parameter_annotations_attribute(infile *fp,
                                                       u2 attr_name_idx,
                                                       u4 attr_length,
                                                       attr_runtime_visible_parameter_annotations *res);

extern int 
create_runtime_invisible_parameter_annotations_attribute(infile *fp,
                                                         u2 attr_name_idx,
                                                         u4 attr_length,
                                                         attr_runtime_invisible_parameter_annotations *res);

extern int create_annotation_default_attribute(infile *fp,
                                               u2 attr_name_idx,
                                               u4 attr_length,
                                               attr_annotation_default *res);

extern void free_code_attribute(attr_code *ptr);
extern void free_exceptions_attribute(attr_exceptions *ptr);
extern void free_inner_classes_attribute(attr_inner_classes *ptr);
extern void free_source_debug_extension_attribute(attr_source_debug_extension *ptr);
extern void free_linenumber_table_attribute(attr_linenumber_table *ptr);
extern void free_local_variable_table_attribute(attr_local_variable_table *ptr);
extern void free_local_variable_type_table_attribute(attr_local_variable_type_table *ptr);
extern void free_runtime_visible_annotations_attribute(attr_runtime_visible_annotations *ptr);
extern void free_runtime_invisible_annotations_attribute(attr_runtime_invisible_annotations *ptr);
extern void free_runtime_visible_parameter_annotations_attribute(attr_runtime_visible_parameter_annotations *ptr);
extern void free_runtime_invisible_parameter_annotations_attribute(attr_runtime_invisible_parameter_annotations *ptr);
extern void free_annotation_default_attribute(attr_annotation_default *ptr);

#endif
