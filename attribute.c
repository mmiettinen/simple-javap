
#include <stdio.h>
#include <stdlib.h>

#include "attribute.h"
#include "classfile.h"
#include "jstring.h"

int create_const_value_attribute(infile *fp, 
                                 u2 attr_name_idx,
                                 u4 attr_length,
                                 attr_const_value *res)
{
    res->attribute_name_index = attr_name_idx;
    res->attribute_length = attr_length;
    if (read_u2(fp, &res->constantvalue_index) == RS_EOF)
        return -1;

    return 0;
}

int create_code_attribute(void *pcf, 
                          infile *fp, 
                          u2 attr_name_idx, 
                          u4 attr_length,
                          attr_code *res)
{
    int i;
    class_file *cfp = (class_file *)pcf;

    res->attribute_name_index = attr_name_idx;
    res->attribute_length = attr_length;
    if (read_u2(fp, &res->max_stack) == RS_EOF)
        return -1;
    if (read_u2(fp, &res->max_locals) == RS_EOF)
        return -1;
    if (read_u4(fp, &res->code_length) == RS_EOF)
        return -1;
    if ((res->code = (u1 *)malloc(res->code_length * sizeof(u1))) == NULL) {
        error_number = ERR_MEM_ALLOC;
        return -1;
    }
    
    for (i = 0; i < res->code_length; i++)
        if (read_u1(fp, &res->code[i]) == RS_EOF)
            return -1;

    if (read_u2(fp, &res->exception_table_length) == RS_EOF)
        return -1;
    if ((res->exception_table = 
        (exception_table_entry *)malloc(res->exception_table_length 
                                        * sizeof(exception_table_entry))) == NULL) {
        error_number = ERR_MEM_ALLOC;
        return -1;
    }

    for (i = 0; i < res->exception_table_length; i++) {
        if (read_u2(fp, &res->exception_table[i].start_pc) == RS_EOF)
            return -1;
        if (read_u2(fp, &res->exception_table[i].end_pc) == RS_EOF)
            return -1;
        if (read_u2(fp, &res->exception_table[i].handler_pc) == RS_EOF)
            return -1;
        if (read_u2(fp, &res->exception_table[i].catch_type) == RS_EOF)
            return -1;
    }

    if (read_u2(fp, &res->attributes_count) == RS_EOF)
        return -1;
    if ((res->attributes = (attribute *)malloc(res->attributes_count 
                                               * sizeof(attribute))) == NULL) {
        error_number = ERR_MEM_ALLOC;
        return -1;
    }
    for (i = 0; i < res->attributes_count; i++) {
        u2 attr_name_idx;
        u4 attr_length;
        jstring *sc;
        if (read_u2(fp, &attr_name_idx) == RS_EOF)
            return -1;
        if (read_u4(fp, &attr_length) == RS_EOF)
            return -1;
        sc = &(cfp->constant_pool[attr_name_idx].data.jstr);
        if (equals_native(sc, "LineNumberTable")) {
            res->attributes[i].attr_type = ATTR_LINENUMBER_TABLE;
            if (create_linenumber_table_attribute(fp, attr_name_idx, attr_length, 
                                                  &(res->attributes[i].data.linenum_tbl)) == -1)
                return -1;
        }
        else if (equals_native(sc, "LocalVariableTable")) {
            res->attributes[i].attr_type = ATTR_LOCAL_VARIABLE_TABLE;
            if (create_local_variable_table_attribute(fp, attr_name_idx, attr_length,
                                                      &(res->attributes[i].data.local_var_tbl)) == -1)
                return -1;
        }
        else if (equals_native(sc, "LocalVariableTypeTable")) {
            res->attributes[i].attr_type = ATTR_LOCAL_VARIABLE_TYPE_TABLE;
            if (create_local_variable_type_table_attribute(fp, attr_name_idx, attr_length,
                                                           &(res->attributes[i].data.local_var_type_tbl)) == -1)
                return -1;
        }
        else {
            error_number = ERR_INCORRECT_CLASS;
            return -1;
        }
    }

    return 0;
}

int create_exceptions_attribute(infile *fp, 
                                u2 attr_name_idx,
                                u4 attr_length,
                                attr_exceptions *res)
{
    int i;

    res->attribute_name_index = attr_name_idx;
    res->attribute_length = attr_length;
    if (read_u2(fp, &res->number_of_exceptions) == RS_EOF)
        return -1;
    if ((res->exception_index_table = (u2 *)malloc(res->number_of_exceptions * sizeof(u2))) == NULL) {
        error_number = ERR_MEM_ALLOC;
        return -1;
    }

    for (i = 0; i < res->number_of_exceptions; i++)
        if (read_u2(fp, &res->exception_index_table[i]) == RS_EOF)
            return -1;

    return 0;
}

int create_inner_classes_attribute(infile *fp, 
                                   u2 attr_name_idx,
                                   u4 attr_length,
                                   attr_inner_classes *res)
{
    int i;

    res->attribute_name_index = attr_name_idx;
    res->attribute_length = attr_length;
    if (read_u2(fp, &res->number_of_classes) == RS_EOF)
        return -1;

    /*printf("<InnerClasses> number_of_classes: %d\n", res->number_of_classes);*/

    if ((res->classes = (classes_entry *)malloc(res->number_of_classes 
                                                * sizeof(classes_entry))) == NULL) {
        error_number = ERR_MEM_ALLOC;
        return -1;
    }

    for (i = 0; i < res->number_of_classes; i++) {
        if (read_u2(fp, &res->classes[i].inner_class_info_index) == RS_EOF)
            return -1;
        if (read_u2(fp, &res->classes[i].outer_class_info_index) == RS_EOF)
            return -1;
        if (read_u2(fp, &res->classes[i].inner_name_index) == RS_EOF)
            return -1;
        if (read_u2(fp, &res->classes[i].inner_class_access_flags) == RS_EOF)
            return -1;
        /*printf("<classes_entry> inner_class_info_index = %d\n", res->classes[i].inner_class_info_index);
        printf("<classes_entry> outer_class_info_index = %d\n", res->classes[i].outer_class_info_index);
        printf("<classes_entry> inner_name_index = %d\n", res->classes[i].inner_name_index);*/
    }

    return 0;
}

int create_enclosing_method_attribute(infile *fp,
                                      u2 attr_name_idx,
                                      u4 attr_length,
                                      attr_enclosing_method *res)
{
    res->attribute_name_index = attr_name_idx;
    res->attribute_length = attr_length;
    if (read_u2(fp, &res->class_index) == RS_EOF)
        return -1;
    if (read_u2(fp, &res->method_index) == RS_EOF)
        return -1;

    return 0;
}

int create_synthetic_attribute(u2 attr_name_idx,
                               u4 attr_length,
                               attr_synthetic *res)
{
    res->attribute_name_index = attr_name_idx;
    res->attribute_length = attr_length;

    return 0;
}

int create_signature_attribute(infile *fp,
                               u2 attr_name_idx,
                               u4 attr_length,
                               attr_signature *res)
{
    res->attribute_name_index = attr_name_idx;
    res->attribute_length = attr_length;
    if (read_u2(fp, &res->signature_index) == RS_EOF)
        return -1;

    return 0;
}

int create_sourcefile_attribute(infile *fp, 
                                u2 attr_name_idx,
                                u4 attr_length,
                                attr_sourcefile *res)
{
    res->attribute_name_index = attr_name_idx;
    res->attribute_length = attr_length;
    if (read_u2(fp, &res->sourcefile_index) == RS_EOF)
        return -1;

    return 0;
}

int create_source_debug_extension_attribute(infile *fp,
                                            u2 attr_name_idx,
                                            u4 attr_length,
                                            attr_source_debug_extension *res)
{
    int i;

    res->attribute_name_index = attr_name_idx;
    res->attribute_length = attr_length;
    if ((res->debug_extension = (u1 *)malloc(attr_length * sizeof(u1))) == NULL) {
        error_number = ERR_MEM_ALLOC;
        return -1;
    }
    for (i = 0; i < attr_length; i++)
        if (read_u1(fp, &res->debug_extension[i]) == RS_EOF)
            return -1;

    return 0;
}

int create_linenumber_table_attribute(infile *fp, 
                                      u2 attr_name_idx,
                                      u4 attr_length,
                                      attr_linenumber_table *res)
{
    int i;
    
    res->attribute_name_index = attr_name_idx;
    res->attribute_length = attr_length;
    if (read_u2(fp, &res->line_number_table_length) == RS_EOF)
        return -1;
    if ((res->line_number_table =
         (line_number_table_entry *)malloc(res->line_number_table_length * 
                                           sizeof(line_number_table_entry))) == RS_EOF) {
        error_number = ERR_MEM_ALLOC;
        return -1;
    }

    for (i = 0; i < res->line_number_table_length; i++) {
        if (read_u2(fp, &res->line_number_table[i].start_pc) == RS_EOF)
            return -1;
        if (read_u2(fp, &res->line_number_table[i].line_number) == RS_EOF)
            return -1;
    }

    return 0;
}

int create_local_variable_table_attribute(infile *fp, 
                                          u2 attr_name_idx,
                                          u4 attr_length,
                                          attr_local_variable_table *res)
{
    int i;

    res->attribute_name_index = attr_name_idx;
    res->attribute_length = attr_length;
    if (read_u2(fp, &res->local_variable_table_length) == RS_EOF)
        return -1;

    if ((res->local_variable_table =
         (local_variable_table_entry *)malloc(res->local_variable_table_length *
                                              sizeof(local_variable_table_entry))) == NULL) {
        error_number = ERR_MEM_ALLOC;
        return -1;
    }

    for (i = 0; i < res->local_variable_table_length; i++) {
        if (read_u2(fp, &res->local_variable_table[i].start_pc) == RS_EOF)
            return -1;
        if (read_u2(fp, &res->local_variable_table[i].length) == RS_EOF)
            return -1;
        if (read_u2(fp, &res->local_variable_table[i].name_index) == RS_EOF)
            return -1;
        if (read_u2(fp, &res->local_variable_table[i].descriptor_index) == RS_EOF)
            return -1;
        if (read_u2(fp, &res->local_variable_table[i].index) == RS_EOF)
            return -1;
    }

    return 0;
}

int create_local_variable_type_table_attribute(infile *fp,
                                               u2 attr_name_idx,
                                               u4 attr_length,
                                               attr_local_variable_type_table *res)
{
    int i;

    res->attribute_name_index = attr_name_idx;
    res->attribute_length = attr_length;
    if (read_u2(fp, &res->local_variable_type_table_length) == RS_EOF)
        return -1;
    if ((res->local_variable_type_table =
         (local_variable_type_table_entry *)malloc(res->local_variable_type_table_length *
                                                   sizeof(local_variable_table_entry))) == NULL) {
        error_number = ERR_MEM_ALLOC;
        return -1;
    }

    for (i = 0; i < res->local_variable_type_table_length; i++) {
        if (read_u2(fp, &res->local_variable_type_table[i].start_pc) == RS_EOF)
            return -1;
        if (read_u2(fp, &res->local_variable_type_table[i].length) == RS_EOF)
            return -1;
        if (read_u2(fp, &res->local_variable_type_table[i].name_index) == RS_EOF)
            return -1;
        if (read_u2(fp, &res->local_variable_type_table[i].signature_index) == RS_EOF)
            return -1;
        if (read_u2(fp, &res->local_variable_type_table[i].index) == RS_EOF)
            return -1;
    }

    return 0;
}

int create_deprecated_attribute(u2 attr_name_idx,
                                u4 attr_length,
                                attr_deprecated *res)
{
    res->attribute_name_index = attr_name_idx;
    res->attribute_length = attr_length;

    return 1;
}

int get_annotation(infile *fp, annotation *res);

int get_element_value(infile *fp, element_value *res)
{
    int i;

    if (read_u1(fp, &res->tag) == RS_EOF)
        return -1;
    switch (res->tag) {
    case 'B': case 'C': case 'D': case 'F': case 'I':
    case 'J': case 'S': case 'Z': case 's':
        if (read_u2(fp, &res->value.const_value_index) == RS_EOF)
            return -1;
        break;
    case 'e':
        if (read_u2(fp, &res->value.enum_const_value.type_name_index) == RS_EOF)
            return -1;
        if (read_u2(fp, &res->value.enum_const_value.const_name_index) == RS_EOF)
            return -1;
        break;
    case 'c':
        if (read_u2(fp, &res->value.class_info_index) == RS_EOF)
            return -1;
        break;
    case '@':
        if (get_annotation(fp, &res->value.annotation_value) == -1)
            return -1;
        break;
    case '[':
        if (read_u2(fp, &res->value.array_value.num_values) == RS_EOF)
            return -1;
        if ((res->value.array_value.values = 
             (element_value *)malloc(res->value.array_value.num_values *
                                     sizeof(element_value))) == NULL) {
            error_number = ERR_MEM_ALLOC;
            return -1;
        }
        for (i = 0; i < res->value.array_value.num_values; i++) {
            if (get_element_value(fp, &res->value.array_value.values[i]) == -1)
                return -1;
        }
        break;
    default:
        return -1;
    }
    
    return 0;
}

int get_annotation(infile *fp, annotation *res)
{
    int i;

    if (read_u2(fp, &res->type_index) == RS_EOF)
        return -1;
    if (read_u2(fp, &res->num_element_value_pairs) == RS_EOF)
        return -1;
    if ((res->element_value_pairs =
         (element_value_pairs_entry *)malloc(res->num_element_value_pairs *
                                             sizeof(element_value_pairs_entry))) == NULL) {
        error_number = ERR_MEM_ALLOC;
        return -1;
    }
    for (i = 0; i < res->num_element_value_pairs; i++) {
        if (read_u2(fp, &res->element_value_pairs[i].element_name_index) == RS_EOF)
            return -1;
        if (get_element_value(fp, &res->element_value_pairs[i].value) == -1)
            return -1;
    }

    return 0;
}

int create_runtime_visible_annotations_attribute(infile *fp,
                                                 u2 attr_name_idx,
                                                 u4 attr_length,
                                                 attr_runtime_visible_annotations *res)
{
    int i;

    res->attribute_name_index = attr_name_idx;
    res->attribute_length = attr_length;
    if (read_u2(fp, &res->num_annotations) == RS_EOF)
        return -1;
    if ((res->annotations = (annotation *)malloc(res->num_annotations *
                                                 sizeof(annotation))) == NULL) {
        error_number = ERR_MEM_ALLOC;
        return -1;
    }

    for (i = 0; i < res->num_annotations; i++)
        if (get_annotation(fp, &res->annotations[i]) == -1)
            return -1;

    return 0;
}

int create_runtime_invisible_annotations_attribute(infile *fp,
                                                   u2 attr_name_idx,
                                                   u4 attr_length,
                                                   attr_runtime_invisible_annotations *res)
{
    int i;

    res->attribute_name_index = attr_name_idx;
    res->attribute_length = attr_length;
    if (read_u2(fp, &res->num_annotations) == RS_EOF)
        return -1;
    if ((res->annotations = (annotation *)malloc(res->num_annotations *
                                                 sizeof(annotation))) == NULL) {
        error_number = ERR_MEM_ALLOC;
        return -1;
    }

    for (i = 0; i < res->num_annotations; i++)
        if (get_annotation(fp, &res->annotations[i]) == -1)
            return -1;

    return 0;
}

int create_runtime_visible_parameter_annotations_attribute(infile *fp,
                                                           u2 attr_name_idx,
                                                           u4 attr_length,
                                                           attr_runtime_visible_parameter_annotations *res)
{
    int i, j;

    res->attribute_name_index = attr_name_idx;
    res->attribute_length = attr_length;
    if (read_u1(fp, &res->num_parameters) == RS_EOF)
        return -1;
    if ((res->parameter_annotations =
         (parameter_annotations_entry *)malloc(res->num_parameters *
                                               sizeof(parameter_annotations_entry))) == NULL) {
        error_number = ERR_MEM_ALLOC;
        return -1;
    }

    for (i = 0; i < res->num_parameters; i++) {
        if (read_u2(fp, &res->parameter_annotations[i].num_annotations) == RS_EOF)
            return -1;
        if ((res->parameter_annotations[i].annotations = 
             (annotation *)malloc(res->parameter_annotations[i].num_annotations *
                                  sizeof(annotation))) == NULL) {
            error_number = ERR_MEM_ALLOC;
            return -1;
        }
        for (j = 0; j < res->parameter_annotations[i].num_annotations; j++)
            if (get_annotation(fp, &(res->parameter_annotations[i].annotations[j])) == -1)
                return -1;
    }

    return 0;
}

int create_runtime_invisible_parameter_annotations_attribute(infile *fp,
                                                             u2 attr_name_idx,
                                                             u4 attr_length,
                                                             attr_runtime_invisible_parameter_annotations *res)
{
    int i, j;

    res->attribute_name_index = attr_name_idx;
    res->attribute_length = attr_length;
    if (read_u1(fp, &res->num_parameters) == RS_EOF)
        return -1;
    if ((res->parameter_annotations =
         (parameter_annotations_entry *)malloc(res->num_parameters *
                                               sizeof(parameter_annotations_entry))) == NULL) {
        error_number = ERR_MEM_ALLOC;
        return -1;
    }

    for (i = 0; i < res->num_parameters; i++) {
        if (read_u2(fp, &res->parameter_annotations[i].num_annotations) == RS_EOF)
            return -1;
        if ((res->parameter_annotations[i].annotations = 
             (annotation *)malloc(res->parameter_annotations[i].num_annotations *
                                  sizeof(annotation))) == NULL) {
            error_number = ERR_MEM_ALLOC;
            return -1;
        }
        for (j = 0; j < res->parameter_annotations[i].num_annotations; j++)
            if (get_annotation(fp, &(res->parameter_annotations[i].annotations[j])) == -1)
                return -1;
    }

    return 0;
}

int create_annotation_default_attribute(infile *fp,
                                        u2 attr_name_idx,
                                        u4 attr_length,
                                        attr_annotation_default *res)
{
    res->attribute_name_index = attr_name_idx;
    res->attribute_length = attr_length;
    if (get_element_value(fp, &res->default_value) == -1)
        return -1;
    else
        return 0;
}

void free_code_attribute(attr_code *ptr)
{
    int i;

    if (ptr == NULL)
        return;
    if (ptr->exception_table != NULL)
        free(ptr->exception_table);
    if (ptr->attributes != NULL) {
        for (i = 0; i < ptr->attributes_count; i++) {
            if (ptr->attributes[i].attr_type == ATTR_LINENUMBER_TABLE)
                free(ptr->attributes[i].data.linenum_tbl.line_number_table);
            else if (ptr->attributes[i].attr_type == ATTR_LOCAL_VARIABLE_TABLE)
                free(ptr->attributes[i].data.local_var_tbl.local_variable_table);
            else
                free(ptr->attributes[i].data.local_var_type_tbl.local_variable_type_table);
        }
        free(ptr->attributes);
    }
}

void free_exceptions_attribute(attr_exceptions *ptr)
{
    free(ptr->exception_index_table);
}

void free_inner_classes_attribute(attr_inner_classes *ptr)
{
    free(ptr->classes);
}

void free_source_debug_extension_attribute(attr_source_debug_extension *ptr)
{
    free(ptr->debug_extension);
}

void free_linenumber_table_attribute(attr_linenumber_table *ptr)
{
    free(ptr->line_number_table);
}

void free_local_variable_table_attribute(attr_local_variable_table *ptr)
{
    free(ptr->local_variable_table);
}

void free_local_variable_type_table_attribute(attr_local_variable_type_table *ptr)
{
    free(ptr->local_variable_type_table);
}

void free_element_value(element_value *ptr);

void free_annotation(annotation *ptr)
{
    int i;
    if (ptr == NULL)
        return;
    for (i = 0; i < ptr->num_element_value_pairs; i++) {
        free_element_value(&ptr->element_value_pairs[i].value);
    }
    free(ptr->element_value_pairs);
}

void free_element_value(element_value *ptr)
{
    int i;
    switch (ptr->tag) {
    case '@':
        free_annotation(&ptr->value.annotation_value);
        break;
    case '[':
        for (i = 0; i < ptr->value.array_value.num_values; i++)
            free_element_value(&ptr->value.array_value.values[i]);
        free(ptr->value.array_value.values);
        break;
    default:
        break;
    }
}

void free_runtime_visible_annotations_attribute(attr_runtime_visible_annotations *ptr)
{
    int i;
    for (i = 0; i < ptr->num_annotations; i++)
        free_annotation(&ptr->annotations[i]);
    free(ptr->annotations);
}
            
void free_runtime_invisible_annotations_attribute(attr_runtime_invisible_annotations *ptr)
{
    int i;
    for (i = 0; i < ptr->num_annotations; i++)
        free_annotation(&ptr->annotations[i]);
    free(ptr->annotations);
}

void free_runtime_visible_parameter_annotations_attribute(attr_runtime_visible_parameter_annotations *ptr)
{
    int i, j;
    for (i = 0; i < ptr->num_parameters; i++) {
        parameter_annotations_entry *p = &ptr->parameter_annotations[i];
        for (j = 0; j < p->num_annotations; j++)
            free_annotation(&p->annotations[j]);
        free(p->annotations);
    }
    free(ptr->parameter_annotations);
}

void free_runtime_invisible_parameter_annotations_attribute(attr_runtime_invisible_parameter_annotations *ptr)
{
    int i, j;
    for (i = 0; i < ptr->num_parameters; i++) {
        parameter_annotations_entry *p = &ptr->parameter_annotations[i];
        for (j = 0; j < p->num_annotations; j++)
            free_annotation(&p->annotations[j]);
        free(p->annotations);
    }
    free(ptr->parameter_annotations);
}

void free_annotation_default_attribute(attr_annotation_default *ptr)
{
    free_element_value(&ptr->default_value);
}




