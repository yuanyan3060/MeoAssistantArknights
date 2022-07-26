// build.rs

use std::env;

fn main() {
    println!("cargo:rustc-link-search=native={}", env::var("CARGO_MANIFEST_DIR").unwrap());
    println!("cargo:rustc-link-lib=MeoAssistant");
}