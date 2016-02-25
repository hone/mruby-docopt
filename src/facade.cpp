#include "docopt.h"

#include "mruby.h"
#include "mruby/array.h"
#include "mruby/hash.h"
#include "mruby/string.h"

#include <iostream>
#include <map>

mrb_value docopt_value_to_mrb_value(const docopt::value& value, mrb_state *mrb) {
  if (value.isString()) {
    return mrb_str_new_cstr(mrb, value.asString().c_str());
  } else if (value.isBool()) {
    return mrb_bool_value(value.asBool() ? 1 : 0);
  } else if (value.isLong()) {
    return mrb_fixnum_value(value.asLong());
  } else if (value.isStringList()) {
    mrb_value res = mrb_ary_new(mrb);
    for(const auto& str: value.asStringList()){
      mrb_ary_push(mrb, res, mrb_str_new_cstr(mrb, str.c_str()));
    }
    return res;
  }
  return mrb_nil_value();
}

extern "C" mrb_value parse(char* usage, int argc, const char** argv, mrb_state *mrb) {
    mrb_value options = mrb_hash_new(mrb);

    if (argc <= 0) return options;

    std::map<std::string, docopt::value> args
        = docopt::docopt(
                usage,
                { argv + 1, argv + argc },
                false
            );

    for(auto const& arg : args) {
        mrb_value value = docopt_value_to_mrb_value(arg.second, mrb);
        mrb_hash_set(
          mrb,
          options,
          mrb_str_new_cstr(mrb, arg.first.c_str()),
          value
        );
    }
    return options;
}

