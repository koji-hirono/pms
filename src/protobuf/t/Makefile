TOP:=../../..
include $(TOP)/mk/std.mk

SRCS:=t_encode.c
SRCS+=t_decode.c
SRCS+=t_encode_repeated.c
SRCS+=t_decode_repeated.c
DEPLIBS:=protobuf stream buf str memstat
BINPATH:=./bin-$(arch)

include $(TOP)/mk/progs.mk
