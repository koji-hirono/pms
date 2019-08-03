#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libxml/xmlreader.h>

#include "memstat.h"


static void
print_memstat(void)
{
	Memstat m;

	memstat_get(&m);
	printf("vsize: %zu, rssize: %zu\n", m.vs, m.rs);
}

int
parse(int fd)
{
	xmlTextReader *reader;
	int type;
	const xmlChar *name;
	const xmlChar *value;
	int r;

	reader = xmlReaderForFd(fd, NULL, NULL, 0);
	if (reader == NULL)
		return -1;

	print_memstat();
	r = xmlTextReaderRead(reader);
	while (r == 1) {
		print_memstat();
		type = xmlTextReaderNodeType(reader);
		name = xmlTextReaderConstName(reader);
		value = xmlTextReaderConstValue(reader);
		switch (type) {
		case XML_READER_TYPE_ELEMENT:
			/* <name */
			printf("stag name = '%s'\n", name);
			break;
		case XML_READER_TYPE_END_ELEMENT:
			/* </name */
			printf("etag name = '%s'\n", name);
			break;
		case XML_READER_TYPE_ATTRIBUTE:
			/* <stag name="value" */
			printf("attr name = '%s'\n", name);
			/* "value" | 'value' */
			printf("attr value = '%s'\n", value);
			break;
		case XML_READER_TYPE_DOCUMENT:
			/* <!name */
			printf("directive name = '%s'\n", name);
			break;
		case XML_READER_TYPE_XML_DECLARATION:
			/* <?name */
			printf("pi name = '%s'\n", name);
			break;
		case XML_READER_TYPE_COMMENT:
			/* <!-- comment --> */
			printf("comment = '%s'\n", value);
			break;
		case XML_READER_TYPE_CDATA:
			/* <![CDATA[ cdata ]]> */
			printf("cdata = '%s'\n", value);
			break;
		case XML_READER_TYPE_TEXT:
			printf("text = '%s'\n", value);
			break;
		default:
			break;
		}
		r = xmlTextReaderRead(reader);
	}

	xmlFreeTextReader(reader);

	print_memstat();

	xmlCleanupParser();

	return 0;
}

static void
test1(void)
{
	print_memstat();

	parse(0);

	print_memstat();
}

int
main(void)
{
	test1();

	return 0;
}
