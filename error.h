
#ifndef ERROR_H
#define ERROR_H

#define ERR_OK              0
#define ERR_TRUNCATED       1
#define ERR_INCORRECT_MAGIC 2
#define ERR_INCORRECT_CLASS 3
#define ERR_MEM_ALLOC       4
#define ERR_FILE_OPEN       5

extern int error_number;

extern char *get_error_message(void);

#endif

