use std::collections::VecDeque;

use lib::*;

// another lexing problem, and yet i still dunno how to do them fast
// input: 98 lines of arbitrary ~100ch length

const SAMPLE: &str =
"[({(<(())[]>[[{[]{<()<>>
[(()[<>])]({[<{<<[]>>(
{([(<{}[<>[]}>{[]{[(<()>
(((({<>}<{<{<>}{[]{[]{}
[[<[([]))<([[{}[[()]]]
[{[{({}]{}}([{[{{{}}([]
{<[[]]>}<{[{[{[]{()[[[]
[<(<(<(<{}))><([]([]()
<{([([[(<>()){}]>(<<{{
<{([{{}}[<[[[<>{}]]]>[]]";

fn parse_input(inp: &str) -> Vec<&str> {
    let delim = if inp.len() == SAMPLE.len() {
        "\n"
    } else {
        "\r\n"
    };
    inp.split(delim).collect()
}

fn get_score(right: &char) -> u32 {
    match *right {
        ')' => 3,
        ']' => 57,
        '}' => 1197,
        '>' => 25137,
        _ => panic!("bad right brace")
    }
}

fn valid_chunk(left: &char, right: &char) -> bool {
    if '('.eq(left) && ')'.eq(right)
        || '['.eq(left) && ']'.eq(right)
        || '<'.eq(left) && '>'.eq(right)
        || '{'.eq(left) && '}'.eq(right) {
        return true;
    }
    false
}

fn is_left(left: &char) -> bool {
    // match left {
    //     '(' | '[' | '{' | '<' => true,
    //     _ => false
    // }

    // replaced per clippy
    matches!(*left, '(' | '[' | '{' | '<')
}

fn scan_lines(inp: &str) -> u64 {
    let mut score = 0;
    for line in parse_input(inp) {
        let mut queue = VecDeque::new();
        for brace in line.chars() {
            if is_left(&brace) {
                queue.push_back(brace)
            } else {
                let left = queue.pop_back().expect("not incomplete or corrupt!");
                if !(valid_chunk(&left, &brace)) {
                    // println!("expected {} but got {}", left, brace);
                    score += get_score(&brace) as u64;
                    break; // abandon line
                }
            }
        }
    }

    score
}

// ------------------

// there's no real reason to differentiate here
fn get_score2(left: &char) -> u32 {
    match *left {
        '(' => 1,
        '[' => 2,
        '{' => 3,
        '<' => 4,
        _ => panic!("bad left brace")
    }
}

fn scan_lines2(inp: &str) -> u64 {
    let mut scores = vec![];
    for line in parse_input(inp) {
        let mut linescore = 0;
        let mut queue = VecDeque::new();
        let mut corrupt = false;
        for brace in line.chars() {
            if is_left(&brace) {
                queue.push_back(brace)
            } else {
                let left = queue.pop_back().expect("not incomplete or corrupt!");
                if !(valid_chunk(&left, &brace)) {
                    corrupt = true;
                    break; // abandon line
                }
            }
        }
        if corrupt { continue; }

        // part 2: the rest of the queue, back to front
        for left in queue.iter().rev() {
            linescore = linescore * 5 + get_score2(left) as u64;
        }
        scores.push(linescore);
    }

    // find the median of an always-odd-length list
    scores.sort_unstable();
    scores[scores.len()/2] // always floor
}

fn main() {
    let input = input_string!("input.txt");

    // println!("{}", SAMPLE);
    // println!("{}", scan_lines(SAMPLE));
    // println!("{}", scan_lines(&input));

    println!("{}", scan_lines2(SAMPLE));
    println!("{}", scan_lines2(&input));
}
