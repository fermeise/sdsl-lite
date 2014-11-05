include ../Make.helper
CXX_FLAGS=$(MY_CXX_FLAGS) $(MY_CXX_OPT_FLAGS) -I$(INC_DIR) -L$(LIB_DIR)
TESTLIB=/usr/lib/gtest/libgtest.a /usr/lib/gtest/libgtest_main.a -lpthread
CCLIB=-lsdsl -ldivsufsort -ldivsufsort64
HEADERS=$(wildcard $(INC_DIR)/sdsl/*.hpp)

all: main.x

run-test: build-test
	./test_cst_fully.x

build-test: test_cst_fully.x

main.x: main.cpp $(HEADERS)
	$(MY_CXX) $(CXX_FLAGS) -o $@ $< $(CCLIB)

test_cst_fully.x: test_cst_fully.cpp $(HEADERS)
	$(MY_CXX) $(CXX_FLAGS) -o $@ $< $(TESTLIB) $(CCLIB)

$(INC_DIR)/sdsl/%.hpp: ../include/sdsl/%.hpp
	cp $< $@

clean:
	rm -f *.o *.x
