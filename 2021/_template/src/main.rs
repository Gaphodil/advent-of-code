use lib::*;

const SAMPLE: &str =
"";

fn parse_input(inp: &str) -> Vec<&str> {
    let delim = if inp.len() == SAMPLE.len() {
        "\n"
    } else {
        "\r\n"
    };
    inp.split(delim).collect()
}

fn main() {
    let input = input_string!("input.txt");
    // let input = split_lines!(input);
    let input = parse_input(input);
}
