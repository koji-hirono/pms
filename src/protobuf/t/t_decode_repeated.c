#include <inttypes.h>
#include <stdio.h>

#include "str.h"
#include "buf.h"
#include "protobuf.h"

/*
 * message Test4 {
 *   repeated int32 d = 4 [packed=true];
 * }
 *
 * test4.d[0] = 3
 * test4.d[1] = 270
 * test4.d[2] = 86942
 */
static void
test1(void)
{
	char buf[] = {
		0x22, 0x06, 0x03, 0x8e, 0x02, 0x9e, 0xa7, 0x05
	};
	StrReader r;
	StrReader r2;
	Str in;
	int32_t d;
	uint64_t x;
	int idx;
	int type;

	strreader_init(&r, buf, sizeof(buf));

	if (protobuf_decode_varint(&r, &x) != 0) {
		return;
	}
	idx = x >> 3;
	type = x & 0x7;
	if (idx != 4) {
		return;
	}
	if (type != PROTOBUF_TYPE_BYTES) {
		return;
	}

	if (protobuf_decode_bytes(&r, &in) != 0) {
		return;
	}

	strreader_init(&r2, in.s, in.len);

	while (protobuf_decode_varint(&r2, &x) != -1) {
		d = x;
		printf("d = %"PRId32"\n", d);
	}
}

int
main(void)
{
	test1();

	return 0;
}
