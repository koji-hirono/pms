#include <inttypes.h>
#include <stdio.h>

#include "str.h"
#include "buf.h"
#include "protobuf.h"

/*
 * message Person {
 *   required string name = 1;
 *   required int32 id = 2;
 *   optional string email = 3;
 * }
 *
 * person.id = 1234
 * person.name = "John Doe"
 * person.email = "jdoe@example.com"
 */
typedef struct Person Person;

struct Person {
	Str name;
	int32_t id;
	Str email;
};

static void
test1(void)
{
	Person person = {
		.id = 1234,
		.name = STR_L("John Doe"),
		.email = STR_L("jdoe@example.com")
	};
	Buf b;
	uint64_t x;

	buf_init(&b, 64);

	/* string name = 1; */
	x = (1 << 3) | PROTOBUF_TYPE_BYTES;
	if (protobuf_encode_varint(&b, x) != 0) {
		buf_free(&b);
		return;
	}
	if (protobuf_encode_string(&b, person.name.s) != 0) {
		buf_free(&b);
		return;
	}

	/* int32_t id = 2; */
	x = (2 << 3) | PROTOBUF_TYPE_VARINT;
	if (protobuf_encode_varint(&b, x) != 0) {
		buf_free(&b);
		return;
	}
	if (protobuf_encode_varint(&b, person.id) != 0) {
		buf_free(&b);
		return;
	}

	/* string email = 3; */
	x = (3 << 3) | PROTOBUF_TYPE_BYTES;
	if (protobuf_encode_varint(&b, x) != 0) {
		buf_free(&b);
		return;
	}
	if (protobuf_encode_string(&b, person.email.s) != 0) {
		buf_free(&b);
		return;
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
