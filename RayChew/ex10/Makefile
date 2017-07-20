# compiler:
CXX = g++

# compiler flags:
CXXFLAGS = -std=c++14 -O3 -Wall -fopenmp
LIBS = -lboost_timer -lboost_program_options

SRCS = $(wildcard src/*.cxx)
OBJS = $(subst .cxx,.o,$(SRCS))

# target executable to build:
TARGET = ex10

# number of threads to use:
ifndef THREADS
	THREADS = 4
endif

# graph data file to read:
ifndef FILE
	FILE = SP/b15.gph
endif

# variables that are coverage-test specific:
COVERAGEOBJS = $(subst .cxx,.cover,$(SRCS))
GCNOS = $(subst .cxx,.cover.gcno,$(SRCS))
GCDAS = $(subst .cxx,.cover.gcda,$(SRCS))

# flags to use for cppcheck:
CPPCHECKFLAGS = --enable=all --inconclusive --std=posix

.PHONY: all doc test coverage check clean 

all:
	$(MAKE) -j$(THREADS) $(OBJS)
	$(MAKE) $(TARGET)
	
$(TARGET): $(OBJS) $(TARGET).cxx
	$(CXX) $(CXXFLAGS) $(OBJS) $(TARGET).cxx -o $(TARGET) $(LIBS)

#%.o: %.cxx
#	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJS): src/%.o:src/%.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
$(COVERAGEOBJS): src/%.cover:src/%.cxx
	$(CXX) $(CXXFLAGS) --coverage -c $< -o $@
   
test:
	$(MAKE) all
	./$(TARGET) $(FILE) --THREADS=$(THREADS)
	
doc: 
	doxygen -g
	doxygen Doxyfile
	sed -i 's/PROJECT_NAME.*=.*/$/PROJECT_NAME = "Exercise 10"/' Doxyfile
	sed -i 's/EXTRACT_ALL.*=.*/$/EXTRACT_ALL = NO/' Doxyfile
	sed -i 's/EXTRACT_PRIVATE.*=.*/$/EXTRACT_PRIVATE = NO/' Doxyfile
	# if more than one doxygen-readable file is present in directory:
	(cat Doxyfile; echo "INPUT = $(TARGET).cxx") | doxygen - 
	# otherwise:
	# doxygen Doxyfile
	
coverage:	
	$(MAKE) $(COVERAGEOBJS)
	$(CXX) $(CXXFLAGS) --coverage $(COVERAGEOBJS) $(TARGET).cxx -o $(TARGET) $(LIBS)
	mkdir -p gcov
	./$(TARGET) $(FILE) 1 --help
	./$(TARGET) $(FILE) 100 -s
	mv $(TARGET).gcno $(TARGET).gcda gcov
	lcov -t "result" -o result.info -c -d gcov
	genhtml -o result result.info
	rm -f $(GCNOS) $(GCDAS) $(COVERAGEOBJS) result.info

check:
	# if cppcheck binary is in the same folder as source file:
	# ./cppcheck $(CPPCHECKFLAGS) $(TARGET).cxx
	# otherwise, if cppcheck is an installed library:
	cppcheck $(CPPCHECKFLAGS) $(TARGET).cxx

clean:
	rm -f $(OBJS) $(GCNOS) $(GCDAS) $(COVERAGEOBJS)
