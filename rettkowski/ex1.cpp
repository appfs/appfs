#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>

#include <iostream>
#include <string>
#include <complex>
#include <fstream>


namespace exercise1
{
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;
	struct line
	{
		int id;
		int location;
		double value;
	};
}


BOOST_FUSION_ADAPT_STRUCT(
	exercise1::line,
	(int, id)
	(int, location)
	(double, value)
)

namespace exercise1
{
	template <typename Iterator>
	struct line_parser : qi::grammar<Iterator, line(), ascii::space_type>
	{
		line_parser() : line_parser::base_type(start)
		{
			using qi::int_;
			using qi::double_;

			start %= int_ >> ';' >> int_ >> ";" >> double_;
		}

		qi::rule<Iterator, line(), ascii::space_type> start;
	};
}

int main(int argc, char *argv[])
{
	using boost::spirit::ascii::space;
	typedef std::string::const_iterator iterator_type;
	typedef exercise1::line_parser<iterator_type> line_parser;
	line_parser parser;
	std::string currentLine;
	std::ifstream file(argv[1]);
	std::vector<double> values[2];
	double mean[2]{0.0, 0.0};
	long numberOfLines = 0;

	while (getline(file, currentLine))
	{
		numberOfLines++;

		exercise1::line parsedLine;
		std::string::const_iterator currentPosition = currentLine.begin();
		std::string::const_iterator lineEnd = currentLine.end();
		bool parsingSucceeded = phrase_parse(currentPosition, lineEnd, parser, space, parsedLine);

		if (parsingSucceeded && currentPosition == lineEnd)
		{
			if (std::isnan(parsedLine.value) || parsedLine.value <= 0 || parsedLine.location < 1 || parsedLine.location > 2)
				continue;
			values[--parsedLine.location].push_back(parsedLine.value);
		}
	}

	file.close();

	for (int i = 0; i < 2; i++)
	{
		for (double d : values[i])
		{
			mean[i] += std::log2(d);
		}
		mean[i] = std::exp2(mean[i] / values[i].size());
	}

	printf("File: %s with %ld lines\n", argv[1], numberOfLines);
	printf("Valid values Loc1: %ld with GeoMean: %f\n", values[0].size(), mean[0]);
	printf("Valid values Loc2: %ld with GeoMean: %f\n", values[1].size(), mean[1]);
	return 0;
}
