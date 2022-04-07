OBJECTS = main.o rsa.o

EXEC = atomic_sense

#
#export CATNAP_EVENT_THRESHOLD = V_2_20
#export CATNAP_TASK_UPPER_HYST = V_2_30
#
#export CATNAP_ADC_THRESHOLD = 220
#export CULPEO_ADC_THRESHOLD = 2200
#
export CATNAP_EVENT_THRESHOLD = V_1_96
export CATNAP_TASK_UPPER_HYST = V_2_06

export CATNAP_ADC_THRESHOLD = 196
export CULPEO_ADC_THRESHOLD = 1960


DEPS =  liblsm:gcc libapds:gcc libfakecatnap:gcc libcapybara:gcc \
       libfxl:gcc libads:gcc libio:gcc libradio:gcc\
				libmspware:gcc libmsp:gcc libmspuartlink:gcc

override CFLAGS += -I../../data/

include tools/maker/Makefile




