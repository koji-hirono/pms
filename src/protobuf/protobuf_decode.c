#include <inttypes.h>

#include "str.h"
#include "strreader.h"
#include "protobuf.h"


int
protobuf_decode_varint(StrReader *r, uint64_t *x)
{
	int c;
	int n;

	*x = 0;
	for (n = 0; n < 64; n += 7) {
		c = strreader_getc(r);
		if (c == -1)
			return -1;
		*x |= (uint64_t)(c & 0x7f) << n;
		if ((c & 0x80) == 0)
			return 0;
	}

	return -1;
}

int
protobuf_decode_zigzag64(StrReader *r, int64_t *x)
{
	uint64_t v;

	if (protobuf_decode_varint(r, &v) != 0)
		return -1;

	*x = (v >> 1) ^ ((((int64_t)v & 1) << 63) >> 63);

	return 0;
}

int
protobuf_decode_zigzag32(StrReader *r, int32_t *x)
{
	uint64_t v;

	if (protobuf_decode_varint(r, &v) != 0)
		return -1;

	*x = (v >> 1) ^ ((((int32_t)v & 1) << 31) >> 31);

	return 0;
}

int
protobuf_decode_fixed64(StrReader *r, uint64_t *x)
{
	int c;
	int n;

	*x = 0;
	for (n = 0; n < 64; n += 8) {
		c = strreader_getc(r);
		if (c == -1)
			return -1;
		*x |= (uint64_t)c << n;
	}

	return 0;
}

int
protobuf_decode_fixed32(StrReader *r, uint32_t *x)
{
	int c;
	int n;

	*x = 0;
	for (n = 0; n < 32; n += 8) {
		c = strreader_getc(r);
		if (c == -1)
			return -1;
		*x |= (uint32_t)c << n;
	}

	return 0;
}

int
protobuf_decode_bytes(StrReader *r, Str *x)
{
	uint64_t len;

	if (protobuf_decode_varint(r, &len) != 0)
		return -1;

	if (strreader_getn(r, len, x) != 0)
		return -1;

	return 0;
}
