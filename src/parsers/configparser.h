#pragma once

#include "logutil/logger.h"
#include "os/interface.h"

#include <boost/spirit/include/qi.hpp>
#include <string>

namespace qi = boost::spirit::qi;

using Iter = std::string::const_iterator;
using Skipper = qi::rule<Iter>;

class ConfigParser {
  /*std::chrono::seconds period;
  struct time_ : qi::symbols<char, unsigned> {
    time_() { add("h", 3600)("m", 60)("s", 1); }
  } time;*/

  struct Config_ : qi::grammar<Iter, string(), Skipper> {
    Config_() : Config_::base_type(text) {
      using qi::_1;
      using qi::_val;

      text = +qi::char_ >> qi::eol;
    }
    qi::rule<Iter, string(), Skipper> text;
  } Config;

  Skipper skipgg = qi::lit(" ") | qi::lit("\n") - +(qi::char_ - qi::space) >> qi::space  |
                   qi::lit("/*") >>
                       *(qi::char_ - qi::lit("*/")) >> qi::lit("*/") >> qi::space;

public:
  ConfigParser(const path &confFile);
  ~ConfigParser();
};
