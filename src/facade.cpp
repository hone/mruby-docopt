#include "docopt.h"
#include "facade.h"

#include <iostream>
#include <map>

pair docopt_value_to_pair(std::string name, const docopt::value& value) {
  if (value.isString()) {
    pair res{
        name.c_str(),
        type_enum::STRING
    };
    res.str = value.asString().c_str();

    return res;
  } else if (value.isBool()) {
    pair res{
        name.c_str(),
        type_enum::BOOL
    };
    res.b = value.asBool() ? 1 : 0;

    return res;
  } else if (value.isLong()) {
    pair res{
        name.c_str(),
        type_enum::LONG
    };
    res.l = value.asLong();

    return res;
  } else if (value.isStringList()) {
    pair res{
        name.c_str(),
        type_enum::STRINGLIST
    };
    auto list = value.asStringList();
    res.str_l.size = list.size();
    res.str_l.strings = (const char**)malloc(list.size() * sizeof(char*));
    for(int i = 0 ; i < list.size() ; i++) {
      res.str_l.strings[i] = list[i].c_str();
    }
    return res;
  }
  pair res{
    name.c_str(),
    type_enum::EMPTY
  };
  res.b = 0;
  return res;
}

extern "C" list parse(char* usage, int argc, const char** argv) {
    std::map<std::string, docopt::value> args
        = docopt::docopt(
                usage,
                { argv + 1, argv + argc },
                false
            );

    list options;
    options.pairs = (pair*)malloc(args.size() * sizeof(pair));
    options.length = args.size();
    int idx = 0;
    for(auto const& arg : args) {
        options.pairs[idx] = 
            docopt_value_to_pair(arg.first, arg.second);
        idx++;
    }
    return options;
}

