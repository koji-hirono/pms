.PHONY: all clean prog lib inc

DIRS:=$(wildcard *)

all:
	$(call make-subdir,$(DIRS),$@)

clean:
	$(call make-subdir,$(DIRS),$@)

prog:
	$(call make-subdir,$(DIRS),$@)

lib:
	$(call make-subdir,$(DIRS),$@)

inc:
	$(call make-subdir,$(DIRS),$@)
