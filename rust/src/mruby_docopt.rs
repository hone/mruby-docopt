extern crate mferuby;
extern crate docopt;

use std::mem;
use mferuby::sys;
use std::ffi::CString;

#[no_mangle]
#[allow(unused_variables)]
pub extern "C" fn parse(mrb: *mut sys::mrb_state, this: sys::mrb_value) -> sys::mrb_value {
    let mut usage: sys::mrb_value = unsafe { mem::uninitialized() };
    let mut argv: sys::mrb_value = unsafe { mem::uninitialized() };
    unsafe { sys::mrb_get_args(mrb, cstr!("SA"), &mut usage, &mut argv); }

    let rust_usage = mferuby::mruby_str_to_rust_string(usage).unwrap();

    let argc = unsafe { sys::RARRAY_LEN(argv) };
    let mut vec_args: Vec<String> = vec![];
    for i in 0..argc {
        let element = unsafe { sys::mrb_ary_ref(mrb, argv, i) };
        vec_args.push(mferuby::mruby_str_to_rust_string(element).unwrap());
    }

    let result = docopt::Docopt::new(rust_usage)
        .and_then(|d| d.help(false).argv(vec_args.into_iter()).parse());
    match result {
        Ok(args) => {
            println!("{:?}", args);
            unsafe { sys::mrb_str_new_cstr(mrb, cstr!("hello")) }
        },
        Err(e) => unsafe { println!("{:?}", e); sys::nil() },
    }
}
