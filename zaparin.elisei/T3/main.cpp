#include <fstream>
#include <iterator>
#include <algorithm>
#include <map>
#include "polygon.hpp"
#include "commands.hpp"

//int main(int argc, const char* argv[])
//{
//  if (argc != 2)
//  {
//    std::cerr << "Error: invalid argument\n";
//    return 1;
//  }
//
//  std::ifstream fin(argv[1]);
//  if (!fin.is_open())
//  {
//    std::cerr << "Error: invalid filename\n";
//    return 2;
//  }

int main()
{
  std::ifstream fin;
  fin.open("filename.txt");
  using namespace zaparin;

  std::vector< Polygon > polygons;
  using in_it = std::istream_iterator< Polygon >;
  using limits = std::numeric_limits< std::streamsize >;

  while (!fin.eof())
  {
    std::copy(in_it{ fin }, in_it{}, std::back_inserter(polygons));
    if (fin.fail())
    {
      fin.clear();
      fin.ignore(limits::max(), '\n');
    }
  }
  fin.close();

  std::map< std::string, std::function< void(size_t numOfVetexes, std::istream&, std::ostream&) > > cmds;
  using namespace std::placeholders;
  cmds["AREAEVEN"] = std::bind(cmdArea, polygons, _1, _2, _3, "EVEN");
  cmds["AREAODD"] = std::bind(cmdArea, polygons, _1, _2, _3, "ODD");
  cmds["AREAMEAN"] = std::bind(cmdArea, polygons, _1, _2, _3, "MEAN");
  cmds["AREANOV"] = std::bind(cmdArea, polygons, _1, _2, _3, "NOV");

  cmds["MAXAREA"] = std::bind(cmdMax, polygons, _1, _2, _3, "AREA");
  cmds["MAXVERTEXES"] = std::bind(cmdMax, polygons, _1, _2, _3, "VERTEXES");

  cmds["MINAREA"] = std::bind(cmdMin, polygons, _1, _2, _3, "AREA");
  cmds["MINVERTEXES"] = std::bind(cmdMin, polygons, _1, _2, _3, "VERTEXES");

  cmds["COUNTEVEN"] = std::bind(cmdCount, polygons, _1, _2, _3, "EVEN");
  cmds["COUNTODD"] = std::bind(cmdCount, polygons, _1, _2, _3, "ODD");
  cmds["COUNTNOV"] = std::bind(cmdCount, polygons, _1, _2, _3, "NOV");

  cmds["MAXSEQNOV"] = std::bind(cmdMaxSeq, polygons, _1, _2, _3);
  cmds["INTERSECTIONSNOV"] = std::bind(cmdIntersections, polygons, _1, _2, _3);

  size_t nov;
  std::string command, parameter;
  while (std::cin >> command >> parameter)
  {
    nov = 3;
    if (isNumeric(parameter))
    {
      nov = std::stoi(parameter);
      parameter = "NOV";
    }

    try
    {
      if (nov < 3)
      {
        throw InvalidCommand();
      }
      cmds.at(command + parameter)(nov, std::cin, std::cout);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    std::cin.clear();
    std::cin.ignore(limits::max(), '\n');
  }
}
