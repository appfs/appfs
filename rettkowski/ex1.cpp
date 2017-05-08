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


namespace ex1
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
	ex1::line,
	(int, id)
	(int, location)
	(double, value)
)

namespace ex1
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
	typedef ex1::line_parser<iterator_type> line_parser;

	line_parser g;
	std::string str;

	std::ifstream file(argv[1]);
	int success_lines = 0;

	std::vector<double> values[2];
	double mean[2]{ 0.0, 0.0 };

	long numberOfLines = 0;

	while (getline(file, str))
	{
		numberOfLines++;

		ex1::line emp;
		std::string::const_iterator iter = str.begin();
		std::string::const_iterator end = str.end();
		bool r = phrase_parse(iter, end, g, space, emp);

		if (r && iter == end)
		{
			if (std::isnan(emp.value) || emp.value <= 0 || emp.location < 1 || emp.location > 2)
				continue;

			values[--emp.location].push_back(emp.value);
			/*std::cout << "-------------------------\n";
			std::cout << "Parsing succeeded\n";
			std::cout << "got: " << boost::fusion::as_vector(emp) << std::endl;
			std::cout << "\n-------------------------\n";*/
			success_lines++;
		}
		else
		{
			/*std::cout << "-------------------------\n";
			std::cout << "Parsing failed\n";
			std::cout << str << std::endl;
			std::cout << "-------------------------\n";*/
			//std::cout << str << std::endl;
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
