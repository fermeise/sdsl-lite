include ../Make.helper
LIBS=-lsdsl -ldivsufsort -ldivsufsort64
SRC_DIR=src
TMP_DIR=../tmp

TC_IDS:=$(call config_ids,test_case.config)
TC_PATHS:=$(call config_column,test_case.config,2)
IDX_IDS:=$(call config_ids,index.config)
COMPILE_IDS:=$(call config_ids,compile_options.config)
DEFAULT_COMPILE_ID:=$(word 1, $(COMPILE_IDS))

BUILD_EXECS=$(foreach IDX_ID,$(IDX_IDS),\
			bin/build_fcst_$(IDX_ID))
FCST_EXECS=$(foreach IDX_ID,$(IDX_IDS),\
              $(foreach COMPILE_ID,$(COMPILE_IDS),\
               bin/fcst_time_and_space_$(IDX_ID).$(COMPILE_ID)))
INDEXES=$(foreach TC_ID,$(TC_IDS),\
          $(foreach IDX_ID,$(IDX_IDS),\
            indexes/$(TC_ID).$(IDX_ID)))
INDEXES_REF=$(foreach TC_ID,$(TC_IDS),\
          indexes/$(TC_ID).CST)
INDEX_FILES=$(foreach IDX,$(INDEXES),$(IDX)) $(foreach IDX,$(INDEXES_REF),$(IDX))
RES_FILES=$(foreach TC_ID,$(TC_IDS),\
            $(foreach IDX_ID,$(IDX_IDS),\
              $(foreach COMPILE_ID,$(COMPILE_IDS),\
                results/$(TC_ID).$(IDX_ID).$(COMPILE_ID))))

RES_FILE=results/all.txt

all: execs

timing: input execs indexes $(RES_FILES)
	cat $(RES_FILES) > $(RES_FILE)
	@cd visualize; make

input: $(TC_PATHS)

execs: $(BUILD_EXECS) $(FCST_EXECS)

indexes: $(INDEXES)

# Targets for fcst construction
# Format: bin/build_fcst_[IDX_ID]
bin/build_fcst_%: $(SRC_DIR)/build_fcst.cpp
	$(eval IDX_ID:=$(call dim,1,$*))
	$(eval IDX_TYPE:=$(call config_select,index.config,$(IDX_ID),2))
	$(eval IDX_REF_TYPE:=$(call config_select,index.config,$(IDX_ID),3))
	$(eval COMPILE_ID:=$(DEFAULT_COMPILE_ID))
	$(eval COMPILE_OPTIONS:=$(call config_select,compile_options.config,$(COMPILE_ID),2))
	$(MY_CXX) $(MY_CXX_FLAGS) $(COMPILE_OPTIONS) -DFCST_TYPE="$(IDX_TYPE)" -DCST_TYPE="$(IDX_REF_TYPE)" -L$(LIB_DIR) -I$(INC_DIR) -o $@ $< $(LIBS)

# Targets for the cst_fully experiment
# Format: bin/fcst_time_and_space_[INDEX_ID].[COMPILE_ID]
bin/fcst_time_and_space_%: $(SRC_DIR)/fcst_time_and_space.cpp
	$(eval IDX_ID:=$(call dim,1,$*))
	$(eval IDX_TYPE:=$(call config_select,index.config,$(IDX_ID),2))
	$(eval IDX_REF_TYPE:=$(call config_select,index.config,$(IDX_ID),3))
	$(eval COMPILE_ID:=$(call dim,2,$*))
	$(eval COMPILE_OPTIONS:=$(call config_select,compile_options.config,$(COMPILE_ID),2))
	$(MY_CXX) $(MY_CXX_FLAGS) $(COMPILE_OPTIONS) -DFCST_TYPE="$(IDX_TYPE)" -DCST_TYPE="$(IDX_REF_TYPE)" -L$(LIB_DIR) -I$(INC_DIR) -o $@ $< $(LIBS)

# indexes/[TC_ID].[IDX_ID]
indexes/%:
	$(eval TC_ID:=$(call dim,1,$*))
	$(eval IDX_ID:=$(call dim,2,$*))
	$(eval TC_PATH:=$(call config_select,test_case.config,$(TC_ID),2))
	@echo "Building index $(IDX_ID) on $(TC_ID)"
	@bin/build_fcst_$(IDX_ID) $(TC_PATH) $(TMP_DIR) $@ indexes/$(TC_ID).CST

# Format: results/[TC_ID].[INDEX_ID].[COMPILE_ID]
results/%:
	$(eval TC_ID:=$(call dim,1,$*))
	$(eval IDX_ID:=$(call dim,2,$*))
	$(eval COMPILE_ID:=$(call dim,3,$*))
	@echo "Running bin/fcst_time_and_space_$(IDX_ID).$(COMPILE_ID) on $(TC_ID)"
	@echo "# TC_ID = $(TC_ID)" >> $@
	@echo "# IDX_ID = $(IDX_ID)" >> $@
	@echo "# COMPILE_ID = $(COMPILE_ID)" >> $@
	@bin/fcst_time_and_space_$(IDX_ID).$(COMPILE_ID) indexes/$(TC_ID).$(IDX_ID) indexes/$(TC_ID).CST >> $@

include ../Make.download

clean:
	rm -f $(FCST_EXECS) $(BUILD_EXECS) $(INDEX_FILES)

clean_results:
	rm -f $(RES_FILES)

cleanall: clean clean_results
