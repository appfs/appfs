#include <fstream>
#include <iostream>
using namespace std;

int main() {
    ifstream file("ex3.cpp");
    string str;
    while (getline(file, str)) {
        cout << str << endl;
    }
}
