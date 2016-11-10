extern crate mferuby;
extern crate docopt;

use mferuby::sys;
use mferuby::libc::c_void;
use std::ffi::CString;
use std::mem;

lazy_static! {
    pub static ref docopt_option_type: sys::mrb_data_type = sys::mrb_data_type {
        dtype: cstr!("Options"),
        dfree: unsafe { mem::transmute(free_docopt_result as *mut c_void) }
    };
}

#[no_mangle]
extern "C" fn free_docopt_result(mrb: *mut sys::mrb_state, map: Box<docopt::ArgvMap>) {}

#[no_mangle]
#[allow(unused_variables)]
pub extern "C" fn access(mrb: *mut sys::mrb_state, this: sys::mrb_value) -> sys::mrb_value {
    let datap = unsafe {
        sys::mrb_data_get_ptr(mrb, this, &docopt_option_type as &sys::mrb_data_type)
    };
    let map: &docopt::ArgvMap = unsafe { mem::transmute(datap) };
    let mut key: sys::mrb_value = unsafe { mem::uninitialized() };
    unsafe { sys::mrb_get_args(mrb, cstr!("S"), &mut key); }
    let rust_key = mferuby::mruby_str_to_rust_string(key).unwrap();
    unsafe {
        match map.find(&rust_key) {
            None => sys::nil(),
            Some(value) => match *value {
                docopt::Value::Switch(value) => {
                    if value {
                        sys::mrb_true()
                    } else {
                        sys::mrb_false()
                    }
                },
                _ => sys::nil(),
            },
        }
    }
}
