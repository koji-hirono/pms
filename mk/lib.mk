LIBPATH=$(TOP)/lib-$(arch)
INCPATH=$(TOP)/include-$(arch)
OBJPATH=obj-$(arch)
DEPPATH=dep-$(arch)

CPPFLAGS+=$(WARNOPT)
CPPFLAGS+=-I$(INCPATH)

SLIB:=$(LIBPATH)/lib$(LIB).a
TINCS:=$(patsubst %.h, $(INCPATH)/%.h, $(INCS))
OBJS:=$(patsubst %.c, $(OBJPATH)/%.o, $(SRCS))
DEPS:=$(patsubst %.c, $(DEPPATH)/%.dep, $(SRCS))
EDEPS:=$(wildcard $(DEPPATH)/*.dep)

.PHONY: all clean prog lib inc

all: $(SLIB) $(TINCS)

clean:
	rm -f $(SLIB) $(OBJS) $(DEPS) $(TINCS) *~
	-rmdir $(OBJPATH) $(DEPPATH) > /dev/null 2>&1 || true
	-rmdir $(LIBPATH) $(INCPATH) > /dev/null 2>&1 || true

prog: ;

lib: $(SLIB)

inc: $(TINCS)


$(SLIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(OBJS): $(OBJPATH)/%.o : %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

$(OBJS): $(OBJPATH)/%.o : $(DEPPATH)/%.dep

$(DEPS): $(DEPPATH)/%.dep : %.c
	$(CC) -MM -MT '$(OBJPATH)/$(<:.c=.o) $@' $(CPPFLAGS) -o $@ $<

$(TINCS): $(INCPATH)/%.h : %.h
	@echo "===> Update $<"
	$(INSTALL) $< $@

$(DEPS): $(TINCS)

$(SLIB): | $(LIBPATH)

$(OBJS): | $(OBJPATH)

$(DEPS): | $(DEPPATH)

$(TINCS): | $(INCPATH)

$(LIBPATH) $(OBJPATH) $(DEPPATH) $(INCPATH):
	mkdir -p $@

ifneq ($(MAKECMDGOALS),clean)
include $(EDEPS)
endif
