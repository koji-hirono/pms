#ifndef PROTOBUF_H__
#define PROTOBUF_H__

#include <inttypes.h>

#include "buf.h"
#include "str.h"
#include "strreader.h"

enum {
	PROTOBUF_TYPE_VARINT,
	PROTOBUF_TYPE_FIXED64,
	PROTOBUF_TYPE_BYTES,
	PROTOBUF_TYPE_START,
	PROTOBUF_TYPE_END,
	PROTOBUF_TYPE_FIXED32
};

extern int protobuf_encodelen_varint(uint64_t);
extern int protobuf_encode_varint(Buf *, uint64_t);
extern int protobuf_encode_zigzag64(Buf *, int64_t);
extern int protobuf_encode_zigzag32(Buf *, int32_t);
extern int protobuf_encode_fixed64(Buf *, uint64_t);
extern int protobuf_encode_fixed32(Buf *, uint32_t);
extern int protobuf_encode_bytes(Buf *, const void *, size_t);
extern int protobuf_encode_string(Buf *, const char *);

extern int protobuf_decode_varint(StrReader *, uint64_t *);
extern int protobuf_decode_zigzag64(StrReader *, int64_t *);
extern int protobuf_decode_zigzag32(StrReader *, int32_t *);
extern int protobuf_decode_fixed64(StrReader *, uint64_t *);
extern int protobuf_decode_fixed32(StrReader *, uint32_t *);
extern int protobuf_decode_bytes(StrReader *, Str *);

#endif
