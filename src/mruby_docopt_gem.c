#include "mruby.h"
#include "mruby/array.h"
#include "mruby/class.h"
#include "mruby/hash.h"
#include "mruby/string.h"
#include "facade.h"
#include <stdlib.h>

list parse(char* usage, int argc, const char** argv);

mrb_value mrb_parse(mrb_state *mrb, mrb_value self)
{
  mrb_value usage, argv;
  mrb_value options = mrb_hash_new(mrb);
  mrb_get_args(mrb, "SA", &usage, &argv);

  int argc = RARRAY_LEN(argv);
  const char* argvv[argc];
  for(int i = 0; i < argc ; i++ ) {
    mrb_value element = mrb_ary_ref(mrb, argv, i);
    argvv[i] = mrb_str_to_cstr(mrb, element); 
  }
  list c_options = parse(mrb_str_to_cstr(mrb, usage), argc, argvv);
  for(int i = 0 ; i < c_options.length ; i++){
    struct pair_struct element = c_options.pairs[i];
    mrb_value value;

    switch(element.type) {
      case STRING:
        value = mrb_str_new_cstr(mrb, element.str);
        break;
      case BOOL:
        value = mrb_bool_value(element.b);
        break;
      case LONG:
        value = mrb_fixnum_value(element.b);
        break;
      case EMPTY:
        value = mrb_nil_value();
      case STRINGLIST:
        value = mrb_ary_new(mrb);
        for(int i = 0; i < element.str_l.size; i++) {
          mrb_ary_push(mrb, value, mrb_str_new_cstr(mrb, element.str_l.strings[i]));
        }
        break;
    }
    mrb_hash_set(mrb, options, mrb_str_new_cstr(mrb, element.key), value);
  }
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
