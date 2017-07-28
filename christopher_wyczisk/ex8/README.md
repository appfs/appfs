# Installiere Doxygen
sudo apt-get install doxygen

# Installiere folgende gcc Version
gcc version 4.9-std=c++14

# Installiere cppcheck und lcov
sudo apt-get install cppcheck
sudo apt-get install lcov 

# Kompelieren und starten, hinter --start geben die die Startknoten an
g++ -O3 -std=c++0x source/dijkstra.cpp source/steiner.cpp source/kommunikator.cpp source/main.cpp -o main -lboost_program_options
./main --pfad ../../test.gph --start 1 2 3