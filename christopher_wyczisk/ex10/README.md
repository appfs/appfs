# Installiere Doxygen
sudo apt-get install doxygen

# Installiere folgende gcc Version
gcc version 4.9-std=c++14

# Installiere cppcheck und lcov
sudo apt-get install cppcheck
sudo apt-get install lcov 

# Kompelieren und starten
g++ -O3 -std=c++0x source/kommunikator.cpp source/dijkstra.cpp source/graph.cpp source/steiner.cpp source/main.cpp -o main -lboost_timer -fopenmp -lboost_system
# Muster: ./main ANZAHLTHREADS PFADZURDATEI
./main 1 ../test.gph