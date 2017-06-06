# compiler:
CC = g++

# compiler flags:
CFLAGS = -std=c++11 -O3
LIBS = -lxerces-c

# target executable to build:
TARGET = ex2

# xml data file to read:
DATA = measured-1.0.0.2017-02-03.b0050c5c8deb1db59c7b2644414b079d.xml
# DATA = example.measured-1-1-0.xml
VALID = -V

# path of cppcheck:
CPPCHECKPATH = ./

# flags to use for cppcheck:
CPPCHECKFLAGS = --enable=all --inconclusive --std=posix --check-config

all: $(TARGET)
$(TARGET): $(TARGET).cxx
	$(CC) $(CFLAGS) $(TARGET).cxx -o $(TARGET) $(LIBS)

doc: 
	doxygen -g
	doxygen Doxyfile
	sed -i 's/PROJECT_NAME.*=.*/$/PROJECT_NAME = "Exercise 2"/' Doxyfile
	sed -i 's/EXTRACT_ALL.*=.*/$/EXTRACT_ALL = YES/' Doxyfile
	sed -i 's/EXTRACT_PRIVATE.*=.*/$/EXTRACT_PRIVATE = YES/' Doxyfile
	# if more than one doxygen-readable file is present in directory:
	(cat Doxyfile; echo "INPUT = $(TARGET).cxx") | doxygen - 
	# otherwise:
	# doxygen Doxyfile

coverage:
	$(CC) $(CFLAGS) --coverage $(TARGET).cxx -o $(TARGET) $(LIBS)
	mkdir -p gcov
	./$(TARGET) $(DATA) $(VALID)
	mv $(TARGET).gcno $(TARGET).gcda gcov
	lcov -t "result" -o result.info -c -d gcov
	genhtml -o result result.info

check:
	# if cppcheck binary is in the same folder as source file:
	# ./cppcheck $(CPPCHECKFLAGS) $(TARGET).cxx
	# otherwise, if cppcheck is an installed library:
	cppcheck $(CPPCHECKFLAGS) $(TARGET).cxx

clean:
	$(RM) $(TARGET)
	
.PHONY: all doc coverage check clean

