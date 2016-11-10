#[macro_use]
extern crate mferuby;

#[macro_use]
extern crate lazy_static;

use mferuby::sys;
use std::ffi::CString;

pub mod mruby_docopt;
pub mod mruby_options;

#[no_mangle]
pub extern "C" fn mrb_mruby_docopt_gem_init(mrb: *mut sys::mrb_state) {
    unsafe {
        let docopt_mod = sys::mrb_define_module(mrb, cstr!("Docopt"));
        sys::mrb_define_module_function(mrb, docopt_mod, cstr!("parse"), mruby_docopt::parse as sys::mrb_func_t, sys::MRB_ARGS_REQ(2));

        let options_class = sys::mrb_define_class_under(mrb, docopt_mod, cstr!("Options"), sys::mrb_state_object_class(mrb));
        sys::MRB_SET_INSTANCE_TT(options_class, sys::mrb_vtype::MRB_TT_DATA);
        sys::mrb_define_method(mrb, options_class, cstr!("[]"), mruby_options::access as sys::mrb_func_t, sys::MRB_ARGS_REQ(1));
    }
}

#[no_mangle]
#[allow(unused_variables)]
pub extern "C" fn mrb_mruby_docopt_gem_final(mrb: *mut sys::mrb_state) {
}
