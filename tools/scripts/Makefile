
RESULTS_PATH = ../../../culpeo_measurements/runtime_tests

calc_periodic_sensing:
	python3 process_timing_sense.py 3.5 culpeo_sense_2s $(RESULTS_PATH)/atomic_sense_culpeo_2s_*
	python3 process_timing_sense.py 4.5 culpeo_sense_3s $(RESULTS_PATH)/atomic_sense_culpeo_3s_*
	python3 process_timing_sense.py 6 culpeo_sense_4s $(RESULTS_PATH)/atomic_sense_culpeo_4s_*
	python3 process_timing_sense.py 3.5 catnap_sense_2s $(RESULTS_PATH)/atomic_sense_catnap_5min_recalc_disable_2s_*
	python3 process_timing_sense.py 4.5 catnap_sense_3s $(RESULTS_PATH)/atomic_sense_catnap_5min_recalc_disable_3s_*
	python3 process_timing_sense.py 6 catnap_sense_4s $(RESULTS_PATH)/atomic_sense_catnap_5min_recalc_disable_4s_*



calc_interrupt_sense_send:
	python3 process_timing_ble.py culpeo_ble_60s.pkl $(RESULTS_PATH)/atomic_ble_culpeo_5min_60s*
	python3 process_timing_ble.py culpeo_ble_45s.pkl $(RESULTS_PATH)/atomic_ble_culpeo_5min_45s*
	python3 process_timing_ble.py culpeo_ble_30s.pkl $(RESULTS_PATH)/atomic_ble_culpeo_5min_30s*
	python3 process_timing_ble.py catnap_ble_60s.pkl $(RESULTS_PATH)/atomic_ble_catnap_5min_recalc_disable_60s*
	# Note, this next one is 45s, the names just don't reflect that
	python3 process_timing_ble.py catnap_ble_45s.pkl\
  $(RESULTS_PATH)/atomic_ble_catnap_5min_recalc_disable_[0-9].csv
	python3 process_timing_ble.py catnap_ble_30s.pkl $(RESULTS_PATH)/atomic_ble_catnap_5min_recalc_disable_30s*

calc_full_apps:
	#python3 process_timing_combo.py combo_culpeo \
  #$(RESULTS_PATH)/combo_catnap_slow_5min_0.csv
	python3 process_timing_combo.py combo_culpeo $(RESULTS_PATH)/combo_culpeo_slow*
	python3 process_timing_combo.py combo_catnap \
  $(RESULTS_PATH)/combo_catnap_slow_5min_[12].csv



