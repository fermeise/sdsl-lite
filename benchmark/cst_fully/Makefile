include ../Make.helper
LIBS=-lsdsl -ldivsufsort -ldivsufsort64
SRC_DIR=src
TMP_DIR=../tmp

TC_IDS:=$(call config_ids,test_case.config)
TC_IDS_INT:=$(call config_ids,test_case_int.config)
TC_PATHS:=$(call config_column,test_case.config,2)
TC_PATHS_INT:=$(call config_column,test_case_int.config,2)
IDX_IDS:=$(call config_ids,index.config)
IDX_IDS_INT:=$(call config_ids,index_int.config)
COMPILE_IDS:=$(call config_ids,compile_options.config)
DEFAULT_COMPILE_ID:=$(word 1, $(COMPILE_IDS))

BUILD_EXECS           = $(foreach IDX_ID,$(IDX_IDS),bin/build_fcst_$(IDX_ID))
BUILD_EXECS_INT       = $(foreach IDX_ID,$(IDX_IDS_INT),bin/build_int_fcst_$(IDX_ID))
FCST_EXECS            = $(foreach IDX_ID,$(IDX_IDS),\
						  $(foreach COMPILE_ID,$(COMPILE_IDS),bin/time_and_space_fcst_$(IDX_ID).$(COMPILE_ID)))
FCST_EXECS_INT        = $(foreach IDX_ID,$(IDX_IDS_INT),\
						  $(foreach COMPILE_ID,$(COMPILE_IDS),bin/time_and_space_int_fcst_$(IDX_ID).$(COMPILE_ID)))
FCST_VERIFY_EXECS     = $(foreach IDX_ID,$(IDX_IDS),\
						  $(foreach COMPILE_ID,$(COMPILE_IDS),bin/verify_fcst_$(IDX_ID).$(COMPILE_ID)))
FCST_VERIFY_EXECS_INT = $(foreach IDX_ID,$(IDX_IDS_INT),\
						  $(foreach COMPILE_ID,$(COMPILE_IDS),bin/verify_int_fcst_$(IDX_ID).$(COMPILE_ID)))
INDEXES               = $(foreach TC_ID,$(TC_IDS),\
						  $(foreach IDX_ID,$(IDX_IDS),indexes/$(TC_ID).$(IDX_ID).byte))
INDEXES_INT           = $(foreach TC_ID,$(TC_IDS_INT),\
						  $(foreach IDX_ID,$(IDX_IDS_INT),indexes/$(TC_ID).$(IDX_ID).int))
INDEXES_REF           = $(foreach TC_ID,$(TC_IDS),indexes/$(TC_ID).CST.byte)
INDEXES_REF_INT       = $(foreach TC_ID,$(TC_IDS),indexes_ref/$(TC_ID).CST.int)
INDEX_FILES           = $(foreach IDX,$(INDEXES),$(IDX)) $(foreach IDX,$(INDEXES_REF),$(IDX))
INDEX_FILES_INT       = $(foreach IDX,$(INDEXES_INT),$(IDX)) $(foreach IDX,$(INDEXES_REF_INT),$(IDX))
RES_FILES             = $(foreach TC_ID,$(TC_IDS),\
						  $(foreach IDX_ID,$(IDX_IDS),\
							$(foreach COMPILE_ID,$(COMPILE_IDS),results/$(TC_ID).$(IDX_ID).$(COMPILE_ID).byte)))
RES_FILES_INT         = $(foreach TC_ID,$(TC_IDS_INT),\
						  $(foreach IDX_ID,$(IDX_IDS_INT),\
							$(foreach COMPILE_ID,$(COMPILE_IDS),results/$(TC_ID).$(IDX_ID).$(COMPILE_ID).int)))
VERIFICATION          = $(foreach TC_ID,$(TC_IDS),\
						  $(foreach IDX_ID,$(IDX_IDS),\
							$(foreach COMPILE_ID,$(COMPILE_IDS),verification/$(TC_ID).$(IDX_ID).$(COMPILE_ID).byte)))
VERIFICATION_INT      = $(foreach TC_ID,$(TC_IDS_INT),\
						  $(foreach IDX_ID,$(IDX_IDS_INT),\
							$(foreach COMPILE_ID,$(COMPILE_IDS),verification/$(TC_ID).$(IDX_ID).$(COMPILE_ID).int)))

RES_FILE = results/all.txt

all: execs

timing: input\
		$(BUILD_EXECS) $(BUILD_EXECS_INT)\
		$(FCST_EXECS) $(FCST_EXECS_INT)\
		indexes\
		$(RES_FILES) $(RES_FILES_INT)
	cat $(RES_FILES) $(RES_FILES_INT) > $(RES_FILE)
	@cd visualize; make

verify: input\
		$(BUILD_EXECS) $(BUILD_EXECS_INT)\
		$(FCST_VERIFY_EXECS) $(FCST_VERIFY_EXECS_INT)\
		indexes\
		$(VERIFICATION) $(VERIFICATION_INT)

input: $(TC_PATHS) $(TC_PATHS_INT)

execs: $(BUILD_EXECS) $(BUILD_EXECS_INT) $(FCST_EXECS) $(FCST_EXECS_INT) $(FCST_VERIFY_EXECS) $(FCST_VERIFY_EXECS_INT)

indexes: $(INDEXES) $(INDEXES_INT)

# bin/build_fcst_[IDX_ID]
bin/build_fcst_%: $(SRC_DIR)/build_fcst.cpp
	$(eval IDX_ID:=$(call dim,1,$*))
	$(eval IDX_TYPE:=$(call config_select,index.config,$(IDX_ID),2))
	$(eval IDX_REF_TYPE:=$(call config_select,index.config,$(IDX_ID),3))
	$(eval COMPILE_ID:=$(DEFAULT_COMPILE_ID))
	$(eval COMPILE_OPTIONS:=$(call config_select,compile_options.config,$(COMPILE_ID),2))
	$(MY_CXX) $(MY_CXX_FLAGS) $(COMPILE_OPTIONS) -DFCST_TYPE="$(IDX_TYPE)" -DCST_TYPE="$(IDX_REF_TYPE)" -L$(LIB_DIR) -I$(INC_DIR) -o $@ $< $(LIBS)

# bin/build_int_fcst_[IDX_ID]
bin/build_int_fcst_%: $(SRC_DIR)/build_fcst.cpp
	$(eval IDX_ID:=$(call dim,1,$*))
	$(eval IDX_TYPE:=$(call config_select,index_int.config,$(IDX_ID),2))
	$(eval IDX_REF_TYPE:=$(call config_select,index_int.config,$(IDX_ID),3))
	$(eval COMPILE_ID:=$(DEFAULT_COMPILE_ID))
	$(eval COMPILE_OPTIONS:=$(call config_select,compile_options.config,$(COMPILE_ID),2))
	$(MY_CXX) $(MY_CXX_FLAGS) $(COMPILE_OPTIONS) -DFCST_TYPE="$(IDX_TYPE)" -DCST_TYPE="$(IDX_REF_TYPE)" -L$(LIB_DIR) -I$(INC_DIR) -o $@ $< $(LIBS)

# bin/time_and_space_fcst_[INDEX_ID].[COMPILE_ID]
bin/time_and_space_fcst_%: $(SRC_DIR)/time_and_space_fcst.cpp
	$(eval IDX_ID:=$(call dim,1,$*))
	$(eval IDX_TYPE:=$(call config_select,index.config,$(IDX_ID),2))
	$(eval IDX_REF_TYPE:=$(call config_select,index.config,$(IDX_ID),3))
	$(eval COMPILE_ID:=$(call dim,2,$*))
	$(eval COMPILE_OPTIONS:=$(call config_select,compile_options.config,$(COMPILE_ID),2))
	$(MY_CXX) $(MY_CXX_FLAGS) $(COMPILE_OPTIONS) -DFCST_TYPE="$(IDX_TYPE)" -DCST_TYPE="$(IDX_REF_TYPE)" -L$(LIB_DIR) -I$(INC_DIR) -o $@ $< $(LIBS)

# bin/time_and_space_int_fcst_[INDEX_ID].[COMPILE_ID]
bin/time_and_space_int_fcst_%: $(SRC_DIR)/time_and_space_fcst.cpp
	$(eval IDX_ID:=$(call dim,1,$*))
	$(eval IDX_TYPE:=$(call config_select,index_int.config,$(IDX_ID),2))
	$(eval IDX_REF_TYPE:=$(call config_select,index_int.config,$(IDX_ID),3))
	$(eval COMPILE_ID:=$(call dim,2,$*))
	$(eval COMPILE_OPTIONS:=$(call config_select,compile_options.config,$(COMPILE_ID),2))
	$(MY_CXX) $(MY_CXX_FLAGS) $(COMPILE_OPTIONS) -DFCST_TYPE="$(IDX_TYPE)" -DCST_TYPE="$(IDX_REF_TYPE)" -L$(LIB_DIR) -I$(INC_DIR) -o $@ $< $(LIBS)

# bin/verify_fcst_[INDEX_ID].[COMPILE_ID]
bin/verify_fcst_%: $(SRC_DIR)/verify_fcst.cpp
	$(eval IDX_ID:=$(call dim,1,$*))
	$(eval IDX_TYPE:=$(call config_select,index.config,$(IDX_ID),2))
	$(eval IDX_REF_TYPE:=$(call config_select,index.config,$(IDX_ID),3))
	$(eval COMPILE_ID:=$(call dim,2,$*))
	$(eval COMPILE_OPTIONS:=$(call config_select,compile_options.config,$(COMPILE_ID),2))
	$(MY_CXX) $(MY_CXX_FLAGS) $(COMPILE_OPTIONS) -DFCST_TYPE="$(IDX_TYPE)" -DCST_TYPE="$(IDX_REF_TYPE)" -L$(LIB_DIR) -I$(INC_DIR) -o $@ $< $(LIBS)

# bin/verify_int_fcst_[INDEX_ID].[COMPILE_ID]
bin/verify_int_fcst_%: $(SRC_DIR)/verify_fcst.cpp
	$(eval IDX_ID:=$(call dim,1,$*))
	$(eval IDX_TYPE:=$(call config_select,index_int.config,$(IDX_ID),2))
	$(eval IDX_REF_TYPE:=$(call config_select,index_int.config,$(IDX_ID),3))
	$(eval COMPILE_ID:=$(call dim,2,$*))
	$(eval COMPILE_OPTIONS:=$(call config_select,compile_options.config,$(COMPILE_ID),2))
	$(MY_CXX) $(MY_CXX_FLAGS) $(COMPILE_OPTIONS) -DFCST_TYPE="$(IDX_TYPE)" -DCST_TYPE="$(IDX_REF_TYPE)" -L$(LIB_DIR) -I$(INC_DIR) -o $@ $< $(LIBS)

# indexes/[TC_ID].[IDX_ID].byte
indexes/%.byte:
	$(eval TC_ID:=$(call dim,1,$*))
	$(eval IDX_ID:=$(call dim,2,$*))
	$(eval TC_PATH:=$(call config_select,test_case.config,$(TC_ID),2))
	@echo "Building index $(IDX_ID) on $(TC_ID)"
	@bin/build_fcst_$(IDX_ID) $(TC_PATH) $(TMP_DIR) $@ indexes/$(TC_ID).CST.byte

# indexes/[TC_ID].[IDX_ID].int
indexes/%.int:
	$(eval TC_ID:=$(call dim,1,$*))
	$(eval IDX_ID:=$(call dim,2,$*))
	$(eval TC_PATH:=$(call config_select,test_case_int.config,$(TC_ID),2))
	@echo "Building index $(IDX_ID) on $(TC_ID)"
	@bin/build_int_fcst_$(IDX_ID) $(TC_PATH) $(TMP_DIR) $@ indexes/$(TC_ID).CST.int

# Format: results/[TC_ID].[INDEX_ID].[COMPILE_ID].byte
results/%.byte:
	$(eval TC_ID:=$(call dim,1,$*))
	$(eval IDX_ID:=$(call dim,2,$*))
	$(eval COMPILE_ID:=$(call dim,3,$*))
	@echo "Running bin/time_and_space_fcst_$(IDX_ID).$(COMPILE_ID) on $(TC_ID)"
	@echo "# TC_ID = $(TC_ID)" >> $@
	@echo "# IDX_ID = $(IDX_ID)" >> $@
	@echo "# COMPILE_ID = $(COMPILE_ID)" >> $@
	@bin/time_and_space_fcst_$(IDX_ID).$(COMPILE_ID) indexes/$(TC_ID).$(IDX_ID).byte indexes/$(TC_ID).CST.byte >> $@

# Format: results/[TC_ID].[INDEX_ID].[COMPILE_ID].int
results/%.int:
	$(eval TC_ID:=$(call dim,1,$*))
	$(eval IDX_ID:=$(call dim,2,$*))
	$(eval COMPILE_ID:=$(call dim,3,$*))
	@echo "Running bin/time_and_space_fcst_$(IDX_ID).$(COMPILE_ID) on $(TC_ID)"
	@echo "# TC_ID = $(TC_ID)" >> $@
	@echo "# IDX_ID = $(IDX_ID)" >> $@
	@echo "# COMPILE_ID = $(COMPILE_ID)" >> $@
	@bin/time_and_space_int_fcst_$(IDX_ID).$(COMPILE_ID) indexes/$(TC_ID).$(IDX_ID).int indexes/$(TC_ID).CST.int >> $@

verification/%.byte:
	$(eval TC_ID:=$(call dim,1,$*))
	$(eval IDX_ID:=$(call dim,2,$*))
	$(eval COMPILE_ID:=$(call dim,3,$*))
	@echo "Running bin/verify_fcst_$(IDX_ID).$(COMPILE_ID) on $(TC_ID)"
	@echo "# TC_ID = $(TC_ID)"
	@echo "# IDX_ID = $(IDX_ID)"
	@echo "# COMPILE_ID = $(COMPILE_ID)"
	@bin/verify_fcst_$(IDX_ID).$(COMPILE_ID) indexes/$(TC_ID).$(IDX_ID).byte indexes/$(TC_ID).CST.byte

verification/%.int:
	$(eval TC_ID:=$(call dim,1,$*))
	$(eval IDX_ID:=$(call dim,2,$*))
	$(eval COMPILE_ID:=$(call dim,3,$*))
	@echo "Running bin/verify_int_fcst_$(IDX_ID).$(COMPILE_ID) on $(TC_ID)"
	@echo "# TC_ID = $(TC_ID)"
	@echo "# IDX_ID = $(IDX_ID)"
	@echo "# COMPILE_ID = $(COMPILE_ID)"
	@bin/verify_int_fcst_$(IDX_ID).$(COMPILE_ID) indexes/$(TC_ID).$(IDX_ID).int indexes/$(TC_ID).CST.int

../data/%:
	$(eval URL:=$(call config_filter,test_case.config,$@,4))
	@$(if $(URL),,\
		$(eval URL:=$(call config_filter,test_case_int.config,$@,4))\
		@$(if $(URL),,\
			$(error "No download link nor generation program specified for test case $@") ) )
	@echo "Download input from $(URL) using curl"
	$(eval DEST_DIR:=$(shell dirname $@))
	cd $(DEST_DIR); curl -O $(URL)
	$(eval FILE:=$(DEST_DIR)/$(notdir $(URL)))
	@$(if $(filter-out ".gz",$(FILE)),\
		echo "Extract file $(FILE) using gunzip";\
		gunzip $(FILE))

clean:
	rm -f $(BUILD_EXECS) $(BUILD_EXECS_INT) $(FCST_EXECS) $(FCST_EXECS_INT) $(FCST_VERIFY_EXECS) $(FCST_VERIFY_EXECS_INT) $(TMP_DIR)/*

clean_indexes:
	rm -f $(INDEX_FILES) $(INDEX_FILES_INT)

clean_results:
	rm -f $(RES_FILES) $(RES_FILES_INT)

cleanall: clean clean_indexes clean_results
