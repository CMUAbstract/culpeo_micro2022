OBJECTS = main.o

RUN_ADC = 0

RUN_TIMER = 0

EXEC = capy

DEPS =  libcapybara:gcc libfxl:gcc libio:gcc \
				libmspware:gcc  libradio:gcc libmsp:gcc libmspuartlink:gcc

ifeq ($(RUN_ADC),1)
override CFLAGS +=  -DRUN_ADC
endif

ifeq ($(RUN_TIMER),1)
override CFLAGS +=  -DRUN_TIMER
endif

include tools/maker/Makefile




