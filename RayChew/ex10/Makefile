# compiler:
CXX = g++

# compiler flags:
CXXFLAGS = -std=c++14 -O3 -march=native -pedantic -Wall

# libraries to use:
LIBS = -lboost_timer -lboost_program_options -fopenmp

# find source and object files.
#SRCS = $(wildcard src/*.cxx)
SRCS = src/utils.cxx src/heap.cxx src/steiner.cxx src/checker.cxx
OBJS = $(subst .cxx,.o,$(SRCS))

# target executable to build:
TARGET = ex10

# graph data file to read:
ifndef FILE
	FILE = big8.gph
endif

# files that are coverage-test specific:
COVERAGEOBJS = $(subst .cxx,.cover,$(SRCS))
GCNOS = $(subst .cxx,.cover.gcno,$(SRCS))
GCDAS = $(subst .cxx,.cover.gcda,$(SRCS))

# flags to use for cppcheck:
CPPCHECKFLAGS = --enable=all --inconclusive --std=posix

.PHONY: all doc test coverage check clean 

# use number of threads specified also in this make file - faster when compiling the object-files.
all:
	$(MAKE) -j$(THREADS) $(OBJS)
	$(MAKE) $(TARGET)
	./$(TARGET) $(HOME)/SP/$(FILE)

# if threads is not defined, then compile without passing the threads number as a compiler option.
ifndef THREADS
$(TARGET): $(OBJS) src/$(TARGET).cxx
	$(CXX) $(CXXFLAGS) $(OBJS) src/$(TARGET).cxx -o $(TARGET) $(LIBS)
else
# else compile it with the custom option that THREADS = number of threads specified.
$(TARGET): $(OBJS) src/$(TARGET).cxx
	$(CXX) -DTHREADS=$(THREADS) $(CXXFLAGS) $(OBJS) src/$(TARGET).cxx -o $(TARGET) $(LIBS)
endif

# compile the objects but do not link them.
$(OBJS): src/%.o:src/%.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
# compile the objects for coverage test without linking them...
$(COVERAGEOBJS): src/%.cover:src/%.cxx
	$(CXX) $(CXXFLAGS) --coverage -c $< -o $@
	
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
	
# coverage test
coverage:
	$(MAKE) $(COVERAGEOBJS)
	$(CXX) $(CXXFLAGS) --coverage $(COVERAGEOBJS) src/$(TARGET).cxx -o $(TARGET) $(LIBS)
	mkdir -p gcov
	./$(TARGET) $(FILE) 1 --help
	./$(TARGET) $(FILE) 100 -s
	mv $(TARGET).gcno $(TARGET).gcda gcov
	lcov -t "result" -o result.info -c -d gcov
	genhtml -o result result.info
	rm -f $(GCNOS) $(GCDAS) $(COVERAGEOBJS) result.info

# static checker
check:
	# if cppcheck binary is in the same folder as source file:
	# ./cppcheck $(CPPCHECKFLAGS) $(TARGET).cxx
	# otherwise, if cppcheck is an installed library:
	cppcheck $(CPPCHECKFLAGS) src/$(TARGET).cxx

clean:
	rm -f $(OBJS) $(GCNOS) $(GCDAS) $(COVERAGEOBJS) result.info
