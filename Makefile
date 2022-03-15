TOOLS_REL_ROOT = tools
TOOLS =
TOOLCHAINS = gcc

APPS = blink

SHARED_DEPS = libfakecatnap:gcc

# Since we're running libfakecatnap which includes checkpointing
export CHKPT_TYPE = _chkpt

include Makefile.capy


include tools/maker/Makefile

