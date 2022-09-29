OBJECTS = main.o rsa.o

EXEC = atomic_sense

#Just the starter vals, they'll change at runtime
export CATNAP_EVENT_THRESHOLD = V_2_20
export CATNAP_TASK_UPPER_HYST = V_2_30

export CATNAP_ADC_THRESHOLD = 220
export CULPEO_ADC_THRESHOLD = 2200


DEPS =  liblsm:gcc libapds:gcc libfakecatnap:gcc libcapybara:gcc \
       libfxl:gcc libio:gcc libradio:gcc\
				libmspware:gcc libmsp:gcc libmspuartlink:gcc

override CFLAGS += -I../../data/

include tools/maker/Makefile




