
#include "defs.h"

int error_number = ERR_OK;

const char *error_messages[] = {
    "",
    "truncated class file",
    "incorrect magic number",
    "illegal class file format",
    "error while allocating memory",
    "error while opening file"
};

const char *get_error_message(void)
{
    return error_messages[error_number];
}
