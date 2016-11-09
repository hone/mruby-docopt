extern crate mferuby;

use mferuby::sys;
use mferuby::libc::c_void;
use std::ffi::CString;

#[no_mangle]
#[allow(unused_variables)]
pub extern "C" fn argv_map_free(mrb: *mut sys::mrb_state, p: *mut c_void) {
}

// let foo = sys::mrb_data_type {
//         struct_name: cstr!("ArgvMap"),
//         dfree: argv_map_free as *mut c_void,
//     };

#[no_mangle]
#[allow(unused_variables)]
pub extern "C" fn access(mrb: *mut sys::mrb_state, this: sys::mrb_value) -> sys::mrb_value {
    unsafe { sys::nil() }
}
