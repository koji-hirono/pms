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
	int32_t d[] = { 3, 270, 86942 };
	Buf b;
	uint64_t x;
	uint64_t len;
	int i;

	len = 0;
	for (i = 0; i < 3; i++)
		len += protobuf_encodelen_varint(d[i]);

	buf_init(&b, 64);

	x = (4 << 3) | PROTOBUF_TYPE_BYTES;
	if (protobuf_encode_varint(&b, x) != 0) {
		buf_free(&b);
		return;
	}

	if (protobuf_encode_varint(&b, len) != 0) {
		buf_free(&b);
		return;
	}

	for (i = 0; i < 3; i++) {
		if (protobuf_encode_varint(&b, d[i]) != 0) {
			buf_free(&b);
			return;
		}
	}

	fwrite(b.data, b.len, 1, stdout);

	buf_free(&b);
}

int
main(void)
{
	test1();

	return 0;
}
