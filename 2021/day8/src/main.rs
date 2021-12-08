/// lost interest in part 2, did not finish

use lib::*;

const SAMPLELINE: &str =
"acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab | cdfeb fcadb cdfeb cdbaf";

const SAMPLE: &str =
"be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb | fdgacbe cefdb cefbgd gcbe
edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec | fcgedb cgb dgebacf gc
fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef | cg cg fdcagb cbg
fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega | efabcd cedba gadfec cb
aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga | gecf egdcabf bgf bfgea
fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf | gebdcfa ecba ca fadegcb
dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf | cefg dcbef fcge gbcadfe
bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd | ed bcgafe cdgba cbgef
egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg | gbdfcae bgc cg cgb
gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc | fgae cfgab fg bagce";

#[derive(Debug)]
struct DigitDisplay {
    a: bool,
    b: bool,
    c: bool,
    d: bool,
    e: bool,
    f: bool,
    g: bool,
}

impl DigitDisplay {
    fn new(inp: &str) -> Self {
        let chars = inp.chars();
        let mut bools = vec![false; 7];
        for c in chars {
            match c {
                'a' => bools[0] = true,
                'b' => bools[1] = true,
                'c' => bools[2] = true,
                'd' => bools[3] = true,
                'e' => bools[4] = true,
                'f' => bools[5] = true,
                'g' => bools[6] = true,
                _ => panic!("illegal character"),
            }
        }
        DigitDisplay {
            a: bools[0],
            b: bools[1],
            c: bools[2],
            d: bools[3],
            e: bools[4],
            f: bools[5],
            g: bools[6],
        }
    }

    // i originally decided to do nested if statements by segment in order of frequency
    // which, lmao
    // also completely ignored part 1 to do this instead which means i read the problem wrong
    fn get_val(&self) -> i32 {
        if self.is_8() { return 8; }
        if self.is_0() { return 0; }
        if self.is_9() { return 9; }
        if self.is_6() { return 6; }
        if self.is_2() { return 2; }
        if self.is_3() { return 3; }
        if self.is_5() { return 5; }
        if self.is_4() { return 4; }
        if self.is_7() { return 7; }
        if self.is_1() { return 1; }
        -1
    }

    fn get_segment_count(&self) -> i32 {
        let mut count = 0;
        if self.a { count += 1; }
        if self.b { count += 1; }
        if self.c { count += 1; }
        if self.d { count += 1; }
        if self.e { count += 1; }
        if self.f { count += 1; }
        if self.g { count += 1; }
        count
    }

    // these are NOT exact and caller must do them in a proper order
    // also made these before part 2 i've been owned so hard
    // and THEN i completely failed to read part 2 again,
    // destroyed by not reading, twice in the same problem!!!
    fn is_0(&self) -> bool {
        // self.a && self.b && self.c && self.e && self.f && self.g
        self.d && self.e && self.a && self.g && self.b && self.c
    }

    fn is_1(&self) -> bool {
        // self.c && self.f
        self.a && self.b
    }

    fn is_2(&self) -> bool {
        // self.a && self.c && self.d && self.e && self.g
        self.d && self.a && self.f && self.g && self.c
    }

    fn is_3(&self) -> bool {
        // self.a && self.c && self.d && self.f && self.g
        self.d && self.a && self.f && self.b && self.c
    }

    fn is_4(&self) -> bool {
        // self.b && self.c && self.d && self.f
        self.e && self.a && self.f && self.b
    }

    fn is_5(&self) -> bool {
        // self.a && self.b && self.d && self.f && self.g
        self.d && self.e && self.f && self.b && self.c
    }

    fn is_6(&self) -> bool {
        // self.a && self.b && self.d && self.e && self.f && self.g
        self.d && self.e && self.f && self.g && self.b && self.c
    }

    fn is_7(&self) -> bool {
        // self.a && self.c && self.f
        self.d && self.a && self.b
    }

    fn is_8(&self) -> bool {
        // self.a && self.b && self.c && self.d && self.e && self.f && self.g
        self.d && self.e && self.a && self.f && self.g && self.b && self.c
    }

    fn is_9(&self) -> bool {
        // self.a && self.b && self.c && self.d && self.f && self.g
        self.d && self.e && self.a && self.f && self.b && self.c
    }
}

impl std::fmt::Display for DigitDisplay {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, ".{}.\n{}.{}\n.{}.\n{}.{}\n.{}.\n\n",
            if self.a {'a'} else {'.'},
            if self.b {'b'} else {'.'},
            if self.c {'c'} else {'.'},
            if self.d {'d'} else {'.'},
            if self.e {'e'} else {'.'},
            if self.f {'f'} else {'.'},
            if self.g {'g'} else {'.'},
        )
    }
}

#[derive(Debug)]
struct Entry {
    patterns: Vec<DigitDisplay>,
    output: Vec<DigitDisplay>,
}

impl std::fmt::Display for Entry {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let mut patterns = String::new();
        let mut output = String::new();
        for d in &self.patterns {
            patterns.push_str(&format!("{}", d));
        }
        for d in &self.output {
            output.push_str(&format!("{}", d));
        }
        write!(f, "{}---\n\n{}", patterns, output)
    }
}

#[derive(Debug)]
enum Segment {
    a, b, c, d, e, f, g, None
}

impl Default for Segment {
    fn default() -> Self {
        Segment::None
    }
}

impl DigitDisplay {
    fn find_missing_in(&self, other: &DigitDisplay) -> Segment {
        if self.a || !other.a { return Segment::a; }
        if self.b || !other.b { return Segment::b; }
        if self.c || !other.c { return Segment::c; }
        if self.d || !other.d { return Segment::d; }
        if self.e || !other.e { return Segment::e; }
        if self.f || !other.f { return Segment::f; }
        if self.g || !other.g { return Segment::g; }
        Segment::None
    }
}

#[derive(Debug, Default)]
struct DigitDecoder {
    top: Segment,
    upl: Segment,
    upr: Segment,
    mid: Segment,
    dol: Segment,
    dor: Segment,
    bot: Segment,
}

fn find_digit_by_segment_count(patterns: &[DigitDisplay], count: i32) -> &DigitDisplay {
    for d in patterns {
        if d.get_segment_count() == count { return &d; }
    }
    panic!();
}

impl DigitDecoder {
    fn new(patterns: &[DigitDisplay]) -> Self {
        let mut decoder = DigitDecoder::default();

        let one = find_digit_by_segment_count(patterns, 2);
        let seven = find_digit_by_segment_count(patterns, 3);
        let four = find_digit_by_segment_count(patterns, 4);
        let eight = find_digit_by_segment_count(patterns, 8);

        // here's where i wish i assigned the segments to a digit in a vector
        decoder.top = seven.find_missing_in(one);

        decoder
    }
}

fn parse_line(line: &str) -> Entry {
    let split: Vec<&str> = line.split(" | ").collect();
    let patterns: Vec<DigitDisplay> = split[0]
        .split_whitespace()
        .map(DigitDisplay::new)
        .collect();
    let output: Vec<DigitDisplay> = split[1]
        .split_whitespace()
        .map(DigitDisplay::new)
        .collect();
    Entry { patterns, output }
}

fn parse_input(inp: &str) -> Vec<Entry> {
    let delim = if inp.len() == SAMPLE.len() {
        "\n"
    } else {
        "\r\n"
    };
    inp.split(delim).map(parse_line).collect()
}

fn count_output_1478(inp: &str) -> u32 {
    let entries = parse_input(inp);
    let mut matched = 0;
    for entry in entries {
        for digit in entry.output {
            // let val = digit.get_val();
            // println!("{}", val);
            // if val == 1 || val == 4 || val == 7 || val == 8 {
            //     count += 1;
            // }
            let count = digit.get_segment_count();
            if count == 2 || count == 4 || count == 3 || count == 7 {
                matched += 1
            }
        }
    }
    matched
}

fn get_component_value(digits: &[DigitDisplay]) -> u64 {
    let mut value = 0;
    for digit in digits {
        let val = digit.get_val();
        if val == -1 {
            eprintln!("{:?}", digit);
            panic!();
        }
        value = value * 10 + val as u64;
    }
    value
}

fn sum_outputs(inp: &str) -> u64 {
    let entries = parse_input(inp);
    let mut sum = 0;
    for entry in entries {
        sum += get_component_value(&entry.output);
    }
    sum
}

fn main() {
    let input = input_string!("input.txt");

    // println!("{}", parse_line(SAMPLELINE));
    // println!("{:?}", count_output_1478(SAMPLE));
    // println!("{:?}", count_output_1478(&input));

    // println!("{}", get_component_value(&parse_line(SAMPLELINE).output));
    println!("{:?}", sum_outputs(SAMPLE));
    println!("{:?}", sum_outputs(&input));
}
