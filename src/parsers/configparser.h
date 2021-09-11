#pragma once

#include "logutil/logger.h"
#include "os/interface.h"

#include <boost/spirit/include/qi.hpp>
#include <string>

namespace qi = boost::spirit::qi;

using Iter = std::string::const_iterator;
using Skipper = qi::rule<Iter>;

namespace elements {
struct _base {};
struct time : _base {
  size_t h;
  size_t m;
  size_t s;
};

template <typename first, typename second> struct relation : _base {
  first _first;
  second _second;
};
} // namespace elements

BOOST_FUSION_ADAPT_STRUCT(elements::time, (int, h)(int, m)(int, s))

class ConfigParser {
  /*std::chrono::seconds period;
  struct time_ : qi::symbols<char, unsigned> {
    time_() { add("h", 3600)("m", 60)("s", 1); }
  } time;*/

  /* struct period { */
  /* 	time t; */
  /* }; */
  struct Config_ : qi::grammar<Iter, string(), Skipper> {
    Config_() : Config_::base_type(text) {
      using qi::_1;
      using qi::_val;

      text = +qi::char_ >> qi::eol;
    }
    qi::rule<Iter, string(), Skipper> text;
  } Config;
  struct Time_ : qi::grammar<Iter, string(), Skipper> {
    Time_() : Time_::base_type(text) {
      using qi::_1;
      using qi::_val;

      /*       text = qi::short_ >> qi::lit("h") >> qi::short_ >> qi::lit("m")
       * >> */
      /*              qi::short_ >> qi::lit("s") >> qi::eol; */
      /*  */
      text = +(qi::char_ - qi::lit("=")) >> qi::lit("=") >>
             +(qi::char_ - qi::lit("=")) >> qi::eol;
    }
    qi::rule<Iter, string(), Skipper> text;
  } Time;

  Skipper
      skipgg = qi::lit(" ") |
               qi::eol - (qi::eol >> (qi::char_ - qi::eol)) |
               qi::lit("/*") >>
                   *(qi::char_ - qi::lit("*/")) >> qi::lit("*/") >> qi::space;

public:
  ConfigParser(const path &confFile);
  ~ConfigParser();
};

class access_denied : public std::exception {
  virtual const char *what() const noexcept override;
};
