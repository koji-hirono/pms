SHELL := /bin/sh

.SUFFIXES:

ifeq ($(MAKELEVEL),0)
export host_arch := $(shell uname -m | tr '[:upper:]' '[:lower:]')
export arch := $(host_arch)
export host_os := $(shell uname -s | tr '[:upper:]' '[:lower:]')
export os := $(host_os)
endif

ifndef WARNINGSET
WARNINGSET := normal
endif

define make-subdir
	@for DIR in $(1); do \
		if [ -f $$DIR/Makefile ]; then \
			$(MAKE) -C $$DIR $(2); \
		fi \
	done
endef

include $(TOP)/mk/arch/conf-$(arch).mk
include $(TOP)/mk/warningset/$(WARNINGSET)
