TOOLS_REL_ROOT = tools
TOOLS =
TOOLCHAINS = gcc

APPS = blink

SHARED_DEPS = libfakecatnap:gcc

include Makefile.capy


include tools/maker/Makefile

