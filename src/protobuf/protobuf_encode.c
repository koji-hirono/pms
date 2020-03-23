#include <inttypes.h>

#include "buf.h"
#include "protobuf.h"


int
protobuf_encodelen_varint(uint64_t x)
{
	int n;

	for (n = 1; x > 0x7f; x >>= 7, n++)
		;

	return n;
}

int
protobuf_encode_varint(Buf *b, uint64_t x)
{
	for (; x > 0x7f; x >>= 7)
		if (buf_pushc(b, (x & 0x7f) | 0x80) != 0)
			return -1;

	if (buf_pushc(b, x & 0x7f) != 0)
		return -1;

	return 0;
}

int
protobuf_encode_zigzag64(Buf *b, int64_t x)
{
	return protobuf_encode_varint(b, (x << 1) ^ (x >> 63));
}

int
protobuf_encode_zigzag32(Buf *b, int32_t x)
{
	return protobuf_encode_varint(b, (x << 1) ^ (x >> 31));
}

int
protobuf_encode_fixed64(Buf *b, uint64_t x)
{
	int i;

	for (i = 0; i < 8; i++, x >>= 8)
		if (buf_pushc(b, x & 0xff) != 0)
			return -1;

	return 0;
}

int
protobuf_encode_fixed32(Buf *b, uint32_t x)
{
	int i;

	for (i = 0; i < 4; i++, x >>= 8)
		if (buf_pushc(b, x & 0xff) != 0)
			return -1;

	return 0;
}

int
protobuf_encode_bytes(Buf *b, const void *data, size_t len)
{
	if (protobuf_encode_varint(b, len) != 0)
		return -1;

	if (buf_pushoct(b, data, len) != 0)
		return -1;

	return 0;
}

int
protobuf_encode_string(Buf *b, const char *s)
{
	return protobuf_encode_bytes(b, s, strlen(s));
}
