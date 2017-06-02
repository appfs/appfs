/*
 * ex1x.cpp
 *
 *  Created on: 25.04.2017
 *
 *  Author: Daniel Rehfeldt
 *
 *  Compile: g++ ex1x.cpp -o ex1x -O3 -std=c++14 -Wall
 */

#include <vector>
#include <cmath>
#include <limits>
#include <iostream>
#include <fstream>
#include <cassert>
#include <boost/spirit/include/qi.hpp>


int main(int argc, char* argv[])
{
    // check number of parameters
    if( argc != 2 )
    {
       // tell the user how to run the program
       std::cerr << "Usage: " << argv[0] << " filename" << std::endl;
       exit(EXIT_FAILURE);
    }

    /* start of definitions for geometric mean */

    std::vector<double> valvec[2];
    double geomean[2] {0.0, 0.0};
    int geomeancount[2] {0, 0};

    // get number of multiplications in geo mean computation guaranteed to be without underflow; unfortunately, constexpr is not supported here by c++14
    const int nmults = std::log2(std::numeric_limits<double>::min()) / std::log2(0.5);

    assert(nmults > 0);
    assert(std::ilogb(std::numeric_limits<double>::max()) + 1 <= std::numeric_limits<int>::max() / nmults); // guard against overflow of exponent

    // compute logarithm of product of values entries in range [start, end)
    auto subsetlog = [](auto start, auto end, std::vector<double> & values)
    {
       // split values into mantissa and exponent
       int exponentsum = 0;
       double mantissaprod = 1.0;
       for( auto j = start; j < end; j++ )
       {
            int ex;
            mantissaprod *= std::frexp(values[j], &ex);
            exponentsum += ex;
       }
       return std::log2(mantissaprod) + exponentsum;
    };

    /* end of definitions for geometric mean */

    unsigned int nlines = 0;

    std::ifstream file (argv[1], std::ifstream::in);
    std::string strline;

    // read line by line
    while( getline(file, strline) )
    {
       nlines++;

       using namespace boost::spirit;
       using qi::int_;
       using qi::double_;
       using qi::phrase_parse;
       using ascii::space;

       double value;
       int location;

       // iterator which will be changed by parse method
       auto it = strline.begin();

       // parse line as int;int;double and ignore whitespaces
       bool success = phrase_parse(it, strline.end(),
             int_ >> ';' >> int_[([&location](int i){ location = i; })] >> ';' >> double_[([&value](double i){ value = i; })]
                      , space);

       if( success && it == strline.end() )
       {
          if( location < 1 || location > 2 || value <= 0 || std::isnan(value) || value <= 0 )
             continue;

          // finally, store the value
          valvec[--location].push_back(value);

          // update geo mean?
          assert(valvec[location].size() <= std::numeric_limits<int>::max());
          assert(int(valvec[location].size()) - geomeancount[location] <= nmults);
          if( int(valvec[location].size()) - geomeancount[location] == nmults )
          {
             geomean[location] += subsetlog(geomeancount[location], int(valvec[location].size()), valvec[location]);
             geomeancount[location] = int(valvec[location].size());
          }
       }
    }

    file.close();

    // update and print geo mean
    for( int i = 0; i < 2; i++ )
    {
      geomean[i] += subsetlog(geomeancount[i], int(valvec[i].size()), valvec[i]);
      geomean[i] /= double(valvec[i].size());
      geomean[i] = std::pow(2.0, geomean[i]);

      std::cout << "Valid values Loc" << i + 1 <<  " " << valvec[i].size() << " with geo mean: " << geomean[i] << std::endl;
    }
    std::cout << "File: " << argv[1] << " with " << nlines << " lines " << std::endl;

    return EXIT_SUCCESS;
}
