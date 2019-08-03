TOP:=../../..
include $(TOP)/mk/std.mk

SRCS:=t_xmllexer_parse.c
SRCS+=t_xmlreader_parse.c
SRCS+=t_table_print.c
DEPLIBS:=xmllexer stream buf memstat
BINPATH:=./bin-$(arch)

include $(TOP)/mk/progs.mk
CPPFLAGS+=$(shell pkg-config --cflags libxml-2.0)
LDADD+=$(shell pkg-config --libs libxml-2.0)
