extern crate mferuby;
extern crate docopt;
extern crate libc;

use std::mem;
use mferuby::sys;
use std::ffi::CString;

#[no_mangle]
extern "C" fn free_docopt_result(mrb: *mut sys::mrb_state, map: Box<docopt::ArgvMap>) {}

lazy_static! {
    static ref docopt_option_type: sys::mrb_data_type = sys::mrb_data_type {
        dtype: cstr!("Options"),
        dfree: unsafe { mem::transmute(free_docopt_result as *mut libc::c_void) }
    };
}

#[no_mangle]
#[allow(unused_variables)]
pub extern "C" fn parse(mrb: *mut sys::mrb_state, this: sys::mrb_value) -> sys::mrb_value {
    let mrb_obj = mferuby::Mrb::new(mrb);
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

    println!("RESULT: {:?}", result);

    match result {
        Ok(args) => {
            let args = Box::new(args);
            let obj = mrb_obj.data_object_alloc::<docopt::ArgvMap>(this, args, &docopt_option_type);

            println!("OBJ: {:?}", obj);
            unsafe { sys::mrb_str_new_cstr(mrb, cstr!("hello")) }
        },
        Err(e) => unsafe { println!("ERROR: {:?}", e); sys::nil() },
    }
}
