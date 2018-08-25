BINPATH?=$(TOP)/bin-$(arch)
LIBPATH=$(TOP)/lib-$(arch)
INCPATH=$(TOP)/include-$(arch)
OBJPATH=obj-$(arch)
DEPPATH=dep-$(arch)

CPPFLAGS+=$(WARNOPT)
CPPFLAGS+=-I$(INCPATH)
LDFLAGS+=-L$(LIBPATH)

OPROG:=$(BINPATH)/$(PROG)
OBJS:=$(patsubst %.c, $(OBJPATH)/%.o, $(SRCS))
DEPS:=$(patsubst %.c, $(DEPPATH)/%.dep, $(SRCS))
EDEPS:=$(wildcard $(DEPPATH)/*.dep)
LDADD:=$(patsubst %, -l%, $(DEPLIBS))

.PHONY: all clean prog lib inc

all: $(OPROG)

clean:
	rm -f $(OPROG) $(OBJS) $(DEPS) *~
	-rmdir $(OBJPATH) $(DEPPATH) > /dev/null 2>&1 || true
	-rmdir $(BINPATH) > /dev/null 2>&1 || true

prog: $(OPROG)

lib: ;

inc: ;


$(OPROG): $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LDADD)

$(OPROG): $(patsubst %, $(LIBPATH)/lib%.a, $(DEPLIBS))

$(OBJS): $(OBJPATH)/%.o : %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

$(OBJS): $(OBJPATH)/%.o : $(DEPPATH)/%.dep

$(DEPS): $(DEPPATH)/%.dep : %.c
	$(CC) -MM -MT '$(OBJPATH)/$(<:.c=.o) $@' $(CPPFLAGS) -o $@ $<

$(OPROG): | $(BINPATH)

$(OBJS): | $(OBJPATH)

$(DEPS): | $(DEPPATH)

$(BINPATH) $(OBJPATH) $(DEPPATH):
	mkdir -p $@

include $(EDEPS)
