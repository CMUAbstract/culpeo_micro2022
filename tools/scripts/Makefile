
RESULTS_PATH = ../../../culpeo_measurements/runtime_tests

calc_periodic_sensing:
	python3 process_timing_sense.py 3.5 $(RESULTS_PATH)/atomic_sense_culpeo_2s_*
	python3 process_timing_sense.py 4.5 $(RESULTS_PATH)/atomic_sense_culpeo_3s_*
	python3 process_timing_sense.py 5.7 $(RESULTS_PATH)/atomic_sense_culpeo_4s_*
	python3 process_timing_sense.py 3.5 $(RESULTS_PATH)/atomic_sense_catnap_5min_recalc_disable_2s_*
	python3 process_timing_sense.py 4.5 $(RESULTS_PATH)/atomic_sense_catnap_5min_recalc_disable_3s_*
	python3 process_timing_sense.py 5.7 $(RESULTS_PATH)/atomic_sense_catnap_5min_recalc_disable_4s_*
