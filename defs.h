
#ifndef DEFS_H
#define DEFS_H

/*
 * Basic signed and unsigned integer types.
 */
typedef unsigned char  u1;
typedef unsigned short u2;
typedef unsigned int   u4;

typedef signed char  s1;
typedef signed short s2;
typedef signed int   s4;

/* 
 * Type definitions for primitive Java types.  
 */
typedef unsigned short jchar;
typedef int jint;
typedef long long jlong;
typedef float jfloat;
typedef double jdouble;

typedef struct infile_t infile;

#define RS_EOF 0
#define RS_OK  1

extern infile *open_file(const char *name);
extern void close_file(infile *fp);

extern int read_u1(infile *fp, u1 *valp);
extern int read_u2(infile *fp, u2 *valp);
extern int read_u4(infile *fp, u4 *valp);

#define ERR_OK              0
#define ERR_TRUNCATED       1
#define ERR_INCORRECT_MAGIC 2
#define ERR_INCORRECT_CLASS 3
#define ERR_MEM_ALLOC       4
#define ERR_FILE_OPEN       5

extern int error_number;

extern const char *get_error_message(void);

#endif
