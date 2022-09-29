OBJECTS = main.o fft.o mic.o


EXEC = combo

#Just the starter vals, they'll change at runtime
export CATNAP_EVENT_THRESHOLD = V_2_20
export CATNAP_TASK_UPPER_HYST = V_2_30

export CATNAP_ADC_THRESHOLD = 220
export CULPEO_ADC_THRESHOLD = 2200

RUN_INT = 1

LOCAL_CFLAGS  = 

DEPS =  liblsm:gcc libapds:gcc libfakecatnap:gcc libcapybara:gcc \
       libfxl:gcc libio:gcc libradio:gcc\
				libmspware:gcc libmsp:gcc libmspuartlink:gcc libdsp:gcc


ifeq ($(RUN_INT),1)
LOCAL_CFLAGS += -DRUN_INT
endif



override CFLAGS += -I../../data/ \
                  $(LOCAL_CFLAGS)

include tools/maker/Makefile




