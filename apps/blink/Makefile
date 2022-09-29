OBJECTS = main.o

EXEC = capy

DEPS = libmspuartlink:gcc libfakecatnap:gcc libcapybara:gcc \
       libfxl:gcc libio:gcc \
				libmspware:gcc libmsp:gcc

include tools/maker/Makefile

ifneq ($(UNIT_TEST_POWER_ON),)
override CFLAGS += -DTEST_POWER_ON
endif



