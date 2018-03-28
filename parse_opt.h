
#ifndef PARSE_OPT_H
#define PARSE_OPT_H

#define OPT_POOL      1
#define OPT_CODE      2
#define OPT_PUBLIC    3
#define OPT_PROTECTED 4
#define OPT_PACKAGE   5
#define OPT_PRIVATE   6
#define OPT_UNKNOWN   7
#define OPT_END       8
#define OPT_ERR       9

extern char *opt_string;

extern int parse_opt(char **argv);

#endif
