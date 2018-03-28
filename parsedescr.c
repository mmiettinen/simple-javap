
#include <stdlib.h>
#include <string.h>

#include "classfile.h"
#include "defs.h"
#include "jstring.h"

#define BUFFER_SIZE 1024
#define END 256

static char result_buffer[2 * BUFFER_SIZE];
static char descr[2 * BUFFER_SIZE];
static u2 length;
static int curr_char;
static int curr_index;

static void next_char(void)
{
    if (curr_index == length)
        curr_char = END;
    else
        curr_char = descr[++curr_index];
}

static void init(jstring *sc)
{
    (void *)jstring_to_native(sc, descr);
    length = sc->length;
    curr_index = -1;
    next_char();
}

static void match(char c)
{
    if (curr_index == length)
        curr_char = END;
    else if (descr[curr_index] == c) 
        curr_char = descr[++curr_index];
}

static int parse_basetype(char *buffer)
{
    switch (curr_char) {
    case 'B':
        strcat(buffer, "byte");
        break;
    case 'C':
        strcat(buffer, "char");
        break;
    case 'D':
        strcat(buffer, "double");
        break;
    case 'F':
        strcat(buffer, "float");
        break;
    case 'I':
        strcat(buffer, "int");
        break;
    case 'J':
        strcat(buffer, "long");
        break;
    case 'S':
        strcat(buffer, "short");
        break;
    case 'Z':
        strcat(buffer, "boolean");
        break;
    default:
        /* error */
        return 0;
    }
    next_char();
    return 1;
}

static int parse_objecttype(char *buffer)
{
    char *tmp = (char *)malloc(BUFFER_SIZE * sizeof(char));
    int i = 0;

    if (curr_char == END)
        return 0;
    while (curr_char != ';') {
        if (curr_char == '/' || curr_char == '$')
            tmp[i++] = '.';
        else
            tmp[i++] = curr_char;
        next_char();
    }
    tmp[i] = '\0';
    strcat(buffer, tmp);
    next_char();
    return 1;
}

static int parse_fieldtype(char *buffer)
{
    switch (curr_char) {
    case 'L':
        next_char();
        return parse_objecttype(buffer);
    case '[':
        next_char();
        if (!parse_fieldtype(buffer))
            return 0;
        strcat(buffer, "[]");
        break;
    default:
        if (!parse_basetype(buffer))
            return 0;
        break;
    }
    return 1;
}

static void parse_parameterdescriptor(char *buffer)
{
    if (parse_fieldtype(buffer)) {
        strcat(buffer, ",");
        parse_parameterdescriptor(buffer);
    }
}

static void parse_returndescriptor(char *buffer)
{
    if (curr_char == 'V')
        strcat(buffer, "void");
    else
        parse_fieldtype(buffer);
    next_char();
}

char *gen_field_declaration(jstring *fname, jstring *fdescr)
{
    char buffer[BUFFER_SIZE];
    char tmp[BUFFER_SIZE];

    init(fdescr);
    buffer[0] = '\0';
    tmp[0] = '\0';
    parse_fieldtype(buffer);
    sprintf(result_buffer, "%s %s", buffer, jstring_to_native(fname, tmp));
    return result_buffer;
}

char *gen_method_header(jstring *mname, jstring *mdescr)
{
    char buffer1[BUFFER_SIZE];
    char buffer2[BUFFER_SIZE];
    char tmp[BUFFER_SIZE];

    init(mdescr);
    buffer1[0] = '\0';
    buffer2[0] = '\0';
    tmp[0] = '\0';
    match('(');
    parse_parameterdescriptor(buffer1);
    match(')');
    parse_returndescriptor(buffer2);
    if (buffer1[0] != '\0')
        buffer1[strlen(buffer1) - 1] = '\0';
    sprintf(result_buffer, "%s %s(%s)", buffer2, convert_classname(mname, tmp), buffer1);
    return result_buffer;
}
