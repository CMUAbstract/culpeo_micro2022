OBJECTS = main.o

RUN_ADC = 1

EXEC = capy

DEPS =  libcapybara:gcc libfxl:gcc libads:gcc libio:gcc \
				libmspware:gcc  libradio:gcc libmsp:gcc libmspuartlink:gcc


ifeq ($(RUN_ADC),1)
override CFLAGS +=  -DRUN_ADC
endif

include tools/maker/Makefile




