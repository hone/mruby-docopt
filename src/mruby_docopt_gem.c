#include "mruby.h"
#include "mruby/array.h"
#include "mruby/class.h"
#include "mruby/string.h"
#include <stdlib.h>

mrb_value parse(char* usage, int argc, const char** argv, mrb_state *mrb);

mrb_value mrb_parse(mrb_state *mrb, mrb_value self)
{
  mrb_value usage, argv;
  mrb_get_args(mrb, "SA", &usage, &argv);

  int argc = RARRAY_LEN(argv);
  const char* argvv[argc];
  for(int i = 0; i < argc ; i++ ) {
    mrb_value element = mrb_ary_ref(mrb, argv, i);
    argvv[i] = mrb_str_to_cstr(mrb, element);
  }

  mrb_value options = parse(mrb_str_to_cstr(mrb, usage), argc, argvv, mrb);

  return options;
}

void mrb_mruby_docopt_gem_init(mrb_state* mrb)
{
  struct RClass *module;

  module = mrb_define_module(mrb, "Docopt");
  mrb_define_module_function(mrb, module, "parse", mrb_parse, MRB_ARGS_REQ(2));
}

void mrb_mruby_docopt_gem_final(mrb_state* mrb)
{
}
