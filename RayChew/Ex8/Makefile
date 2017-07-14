# compiler:
CXX = g++

# compiler flags:
CXXFLAGS = -std=c++14 -O3 -Wall
LIBS = -lboost_timer

OBJ = utils.o checker.o boost_dijkstra.o m2alg.o

# target executable to build:
TARGET = ex8

# graph data file to read:
DATA = big8.gph

# path of cppcheck:
#CPPCHECKPATH = ./

# flags to use for cppcheck:
CPPCHECKFLAGS = --enable=all --inconclusive --std=posix --check-config

.PHONY: all doc coverage check clean 

ex8: $(OBJ) $(TARGET).cxx
	$(CXX) $(CXXFLAGS) $(OBJ) $(TARGET).cxx -o $(TARGET) $(LIBS)

# utils.o: utils.cxx utils.h
# 	$(CXX) $(CXXFLAGS) -c utils.cxx -o utils.o
#     
# checker.o: checker.cxx checker.h
# 	$(CXX) $(CXXFLAGS) -c checker.cxx -o checker.o
#     
# boost_dijkstra.o: boost_dijkstra.cxx boost_dijkstra.h
# 	$(CXX) $(CXXFLAGS) -c boost_dijkstra.cxx -o boost_dijkstra.o
#     
# m2alg.o: m2alg.cxx m2alg.h
# 	$(CXX) $(CXXFLAGS) -c m2alg.cxx -o m2alg.o

%.o: %.cxx
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c $< -o $@
   
doc: 
	doxygen -g
	doxygen Doxyfile
	sed -i 's/PROJECT_NAME.*=.*/$/PROJECT_NAME = "Exercise 7"/' Doxyfile
	sed -i 's/EXTRACT_ALL.*=.*/$/EXTRACT_ALL = NO/' Doxyfile
	sed -i 's/EXTRACT_PRIVATE.*=.*/$/EXTRACT_PRIVATE = NO/' Doxyfile
	# if more than one doxygen-readable file is present in directory:
	(cat Doxyfile; echo "INPUT = $(TARGET).cxx") | doxygen - 
	# otherwise:
	# doxygen Doxyfile

coverage:
	$(CXX) $(CXXFLAGS) --coverage $(TARGET).cxx -o $(TARGET) $(LIBS)
	mkdir -p gcov
	./$(TARGET) $(DATA) -m1
	./$(TARGET) $(DATA) -m2 
	mv $(TARGET).gcno $(TARGET).gcda gcov
	lcov -t "result" -o result.info -c -d gcov
	genhtml -o result result.info

check:
	# if cppcheck binary is in the same folder as source file:
	# ./cppcheck $(CPPCHECKFLAGS) $(TARGET).cxx
	# otherwise, if cppcheck is an installed library:
	cppcheck $(CPPCHECKFLAGS) $(TARGET).cxx

clean:
	rm -f $(OBJS) *.o