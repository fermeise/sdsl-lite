include ../Make.helper
LIBS=-lsdsl -ldivsufsort -ldivsufsort64
SRC_DIR=src
TMP_DIR=../tmp

TC_IDS:=$(call config_ids,test_case.config)
TC_PATHS:=$(call config_column,test_case.config,2)
COMPILE_IDS:=$(call config_ids,compile_options.config)

FCST_EXECS=$(foreach COMPILE_ID,$(COMPILE_IDS),bin/fcst_time_and_space_$(COMPILE_ID))

RES_FILES=$(foreach TC_ID,$(TC_IDS),\
			$(foreach COMPILE_ID,$(COMPILE_IDS),\
			  results/$(TC_ID).$(COMPILE_ID)))

RES_FILE=results/all.txt

all: execs

timing: input execs $(RES_FILES)
	cat $(RES_FILES) > $(RES_FILE)
	@cd visualize; make

input: $(TC_PATHS)

execs: $(FCST_EXECS)

# Targets for the cst_fully experiment
# Format: bin/fcst_time_and_space_[COMPILE_ID]
bin/fcst_time_and_space_%: $(SRC_DIR)/fcst_time_and_space.cpp
	$(eval COMPILE_ID:=$(call dim,1,$*))
	$(eval COMPILE_OPTIONS:=$(call config_select,compile_options.config,$(COMPILE_ID),2))
	$(MY_CXX) $(MY_CXX_FLAGS) $(COMPILE_OPTIONS) -L$(LIB_DIR) -I$(INC_DIR) -o $@ $< $(LIBS)

# Format: results/[TC_ID].[COMPILE_ID]
results/%:
	$(eval TC_ID:=$(call dim,1,$*))
	$(eval COMPILE_ID:=$(call dim,2,$*))
	$(eval TC_PATH:=$(call config_select,test_case.config,$(TC_ID),2))
	@echo "Running bin/fcst_time_and_space_$(COMPILE_ID) on $(TC_ID)"
	@echo "# TC_ID = $(TC_ID)" >> $@
	@echo "# COMPILE_ID = $(COMPILE_ID)" >> $@
	@bin/fcst_time_and_space_$(COMPILE_ID) $(TC_PATH) $(t) >> $@

include ../Make.download

clean:
	rm -f $(FCST_EXECS)

clean_results:
	rm -f $(RES_FILES)

cleanall: clean clean_results

