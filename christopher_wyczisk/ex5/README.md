# Installiere Doxygen
sudo apt-get install doxygen

# Installiere folgende gcc Version
gcc version 4.9-std=c++14

# Installiere cppcheck und lcov
sudo apt-get install cppcheck
sudo apt-get install lcov 

# Kompelieren und starten
g++ -std=c++14 dijkstra.cpp ex5.cpp -o ex5 -lboost_system -lboost_timer
./ex5 ../acycpos-1.gph