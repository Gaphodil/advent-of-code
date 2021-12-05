#[macro_export]
macro_rules! input_string {
    ($file:expr) => {
        std::fs::read_to_string($file).unwrap()
    };
}

#[macro_export]
macro_rules! split_lines {
    ($input:expr) => {
        // delimiter = \r\n (not just \n!!!)
        $input.split("\r\n")
    };
}

// assume well-formed input
pub fn parse_to_dec(s: &str) -> i32 {
    s.parse::<i32>().unwrap()
}