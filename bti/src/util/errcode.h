#ifndef UTIL_ERRCODE_H_
#define UTIL_ERRCODE_H_

#define ERRCODE_OK 0
#define ERRCODE_IO 1
#define ERRCODE_INVALID 2
#define ERRCODE_CODE_OVERFLOW 3
#define ERRCODE_NESTING 4
#define ERRCODE_UNBALANCED 5
#define ERRCODE_DATA_OVERFLOW 6

extern const char *bt_errcode_lookup[7];

#endif
