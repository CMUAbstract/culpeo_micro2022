TOOLS_REL_ROOT = tools
TOOLS =
TOOLCHAINS = gcc

APPS =  responsive_reporting periodic_sensing noise_monitor \
        blink bluetooth min_capture_test

SHARED_DEPS =

# Since we're running libfakecatnap which includes checkpointing
export CHKPT_TYPE = _chkpt

include Makefile.capy

include tools/maker/Makefile

