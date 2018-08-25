#ifndef TEXTFMT_H__
#define TEXTFMT_H__

#include <stdint.h>

#include "buf.h"

extern void textfmt_int(Buf *, int, int, int, int, intmax_t);
extern void textfmt_uint(Buf *, int, int, int, int, uintmax_t);

#endif
