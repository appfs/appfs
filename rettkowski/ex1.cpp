#include <iostream>
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

int main(int argc, char *argv[])
{
    long loc1size = 0, loc2size = 0, numberOfLines = 0;

    ifstream file(argv[1]);
    string str;

    double loc1log = 0.0, loc2log = 0.0, value = 0;
    int pos, location = 0;
    stringstream tmp;

    while (getline(file, str))
    {
        numberOfLines++;
        if (!starts_with(str, "#") && str.length() >= 1)
        {
            pos = str.find_first_of(';');
            if (pos != string::npos)
            {
                str = str.substr(pos + 2);
                pos = str.find_first_of(';');
                if (pos != string::npos)
                {
                    tmp.str(str.substr(0, pos));
                    tmp >> location;
                    tmp.clear();
                    tmp.str(str.substr(pos + 2));
                    tmp >> value;
                    tmp.clear();
                }
            }
            if (location == 1)
            {
                loc1log += log(value);
                loc1size++;
            }
            else if (location == 2)
            {
                loc2log += log(value);
                loc2size++;
            }
        }
    }

    double loc1mean = exp(loc1log / loc1size);
    double loc2mean = exp(loc2log / loc2size);

    printf("File: %s with %ld lines\n", argv[1], numberOfLines);
    printf("Valid values Loc1: %ld with GeoMean: %f\n", loc1size, loc1mean);
    printf("Valid values Loc2: %ld with GeoMean: %f\n", loc2size, loc2mean);
}
