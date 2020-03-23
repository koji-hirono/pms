#include <inttypes.h>
#include <stdio.h>

#include "str.h"
#include "strreader.h"
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
	char buf[] = {
		0x0a, 0x08, 0x4a, 0x6f, 0x68, 0x6e, 0x20, 0x44,
		0x6f, 0x65, 0x10, 0xd2, 0x09, 0x1a, 0x10, 0x6a,
		0x64, 0x6f, 0x65, 0x40, 0x65, 0x78, 0x61, 0x6d,
		0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d
	};
	StrReader r;
	Person person;
	uint64_t x;
	int idx;
	int type;

	strreader_init(&r, buf, sizeof(buf));

	/* string name = 1; */
	if (protobuf_decode_varint(&r, &x) != 0) {
		return;
	}
	idx = x >> 3;
	type = x & 0x7;
	if (idx != 1) {
		return;
	}
	if (type != PROTOBUF_TYPE_BYTES) {
		return;
	}
	if (protobuf_decode_bytes(&r, &person.name) != 0) {
		return;
	}
	printf("person.name = %.*s\n",
		(int)person.name.len, person.name.s);

	/* int32_t id = 2; */
	if (protobuf_decode_varint(&r, &x) != 0) {
		return;
	}
	idx = x >> 3;
	type = x & 0x7;
	if (idx != 2) {
		return;
	}
	if (type != PROTOBUF_TYPE_VARINT) {
		return;
	}
	if (protobuf_decode_varint(&r, &x) != 0) {
		return;
	}
	person.id = x;
	printf("person.id = %"PRId32"\n", person.id);

	/* string email = 3; */
	if (protobuf_decode_varint(&r, &x) != 0) {
		return;
	}
	idx = x >> 3;
	type = x & 0x7;
	if (idx != 3) {
		return;
	}
	if (type != PROTOBUF_TYPE_BYTES) {
		return;
	}
	if (protobuf_decode_bytes(&r, &person.email) != 0) {
		return;
	}
	printf("person.email = %.*s\n",
		(int)person.email.len, person.email.s);
}

int
main(void)
{
	test1();

	return 0;
}
