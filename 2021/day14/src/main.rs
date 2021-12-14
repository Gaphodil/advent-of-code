use std::collections::HashMap;

use lib::*;

// apparently vec max size = 4GB so brute force may not work
// similar vein to previous few, do logic on original and overwrite with edited clone

// input: 100 rules, 20char orig str
// brute force: O(2^n) where n is number of steps
// 1024 for part 1 isn't so bad but i'm worried

// well, it was bad because string ops in rust are kinda rough (intentionally)
// and yep, 2^40
// ... ah, store as (pair, count_in_polymer)
// then for total letters for every pair, add the 1st and last from the template
// then halve each count
// if AB -> C is matched, then set AB to 0 and duplicate to AC and CB

const SAMPLE: &str =
"NNCB

CH -> B
HH -> N
CB -> H
NH -> C
HB -> C
HC -> B
HN -> C
NN -> C
BH -> H
NC -> B
NB -> B
BN -> B
BB -> N
BC -> B
CC -> N
CN -> C";

// #[derive(Debug)]
// struct PairRule {
//     pattern: String,
//     output: char,
// }

// impl PairRule {
//     fn new(line: &str) -> Self {
//         let mut split = line.split(" -> ");
//         Self {
//             pattern: split.next().unwrap().to_owned(),
//             output: split.next().unwrap().chars().next().unwrap(),
//         }
//     }
// }

// fn parse_pairs(inp: &str, delim: &str) -> Vec<PairRule> {
//     inp.split(delim).map(PairRule::new).collect()
// }

fn parse_pairs(inp: &str, delim: &str) -> HashMap<String, char> {
    let lines: Vec<&str> = inp.split(delim).collect();
    let mut map = HashMap::new();
    for line in lines {
        let mut split = line.split(" -> ");
        map.insert(
            split.next().unwrap().to_owned(),
            split.next().unwrap().chars().next().unwrap()
        );
    }
    map
}

fn parse_input(inp: &str) -> (String, HashMap<String, char>) {
    let delim = if inp.len() == SAMPLE.len() {
        "\n"
    } else {
        "\r\n"
    };
    let double_delim = format!("{}{}", delim, delim);
    let mut split = inp.split(double_delim.as_str());
    let init = split.next().unwrap();
    let pairs = parse_pairs(split.next().unwrap(), delim);

    (init.to_owned(), pairs)
}

fn brute_force(inp: &str, steps: u32) -> u64 {
    let (mut polymer, rules) = parse_input(inp);
    let mut polymer_bytes = polymer.as_bytes().to_vec();
    for step in 0..steps {
        // println!("{}", String::from_utf8(polymer_bytes.clone()).unwrap());
        let mut edited = polymer_bytes.clone();
        let mut offset = 0;
        for second in 1..polymer_bytes.len() {
            let pair = String::from_utf8(polymer_bytes[second-1..second+1].to_vec()).unwrap();

            if let Some(add) = rules.get(&pair) {
                edited.insert(second + offset, *add as u8);
                // println!("{:?} -> {:?}", pair, add);
                offset += 1;
            }
        }
        polymer_bytes = edited;
    }
    polymer = String::from_utf8(polymer_bytes).unwrap();

    high_sub_low(frequencies(&polymer))
}

// not sure what the alternative is to not using a reference here
fn high_sub_low(map: HashMap<char, u64>) -> u64 {
    let mut vals: Vec<u64> = map.into_values().collect();
    vals.sort_unstable();
    vals.last().unwrap() - vals.first().unwrap()
}

fn frequencies(polymer: &str) -> HashMap<char, u64> {
    let mut map = HashMap::new();
    for c in polymer.chars() {
        let mut e = map.entry(c).or_insert(0);
        *e += 1;
    }
    map
}

fn not_brute_force(inp: &str, steps: u32) -> u64 {
    let (polymer, rules) = parse_input(inp);
    let mut orig_map = pair_map_from_str(&polymer);
    let rules = rules_to_two_pairs(rules);
    for step in 0..steps {
        let mut edited = orig_map.clone();
        for (pair, count) in orig_map {
            if count > 0 {
                if let Some(pairs) = rules.get(&pair) {
                    let epair = edited.entry(pair).or_insert(0);
                    if *epair < count { panic!() }
                    *epair -= count;

                    let es1 = edited.entry(pairs.0.to_owned()).or_insert(0);
                    *es1 += count;

                    let es2 = edited.entry(pairs.1.to_owned()).or_insert(0);
                    *es2 += count;
                }
            }
        }
        orig_map = edited;
    }
    high_sub_low(frequencies_from_map(orig_map, &polymer))
}

fn pair_map_from_str(template: &str) -> HashMap<String, u64> {
    let bytes = template.as_bytes().to_vec();
    let mut map = HashMap::new();
    for second in 1..bytes.len() {
        let pair = String::from_utf8(bytes[second-1..second+1].to_vec()).unwrap();
        let e = map.entry(pair).or_insert(0);
        *e += 1;
    }
    map
}

fn rules_to_two_pairs(rules: HashMap<String, char>) -> HashMap<String, (String, String)> {
    let mut map = HashMap::new();
    for (pair, c) in rules {
        let mut pchars = pair.chars();
        let c1 = pchars.next().unwrap();
        let c2 = pchars.next().unwrap();

        let s1: String = vec![c1, c].iter().collect();
        let s2: String = vec![c, c2].iter().collect();
        map.insert(pair, (s1, s2));
    }
    map
}

fn frequencies_from_map(polymer: HashMap<String, u64>, orig: &str) -> HashMap<char, u64> {
    let mut map = HashMap::new();
    for (p, v) in polymer {
        let mut pchars = p.chars();
        let mut e = map.entry(pchars.next().unwrap()).or_insert(0);
        *e += v;
        e = map.entry(pchars.next().unwrap()).or_insert(0);
        *e += v;
    }
    let mut ochars = orig.chars();
    let mut e = map.entry(ochars.next().unwrap()).or_insert(0);
    *e += 1;
    e = map.entry(ochars.last().unwrap()).or_insert(0);
    *e += 1;

    let mut halved_map = HashMap::new();
    for (c, v) in &map {
        halved_map.insert(*c, v/2);
    }
    halved_map
}

fn main() {
    let input = input_string!("input.txt");

    // println!("{:?}", parse_input(SAMPLE));
    // println!("{:?}", parse_input(&input));

    // println!("{:?}", brute_force(SAMPLE, 10));
    // println!("{:?}", brute_force(&input, 10));

    // println!("{:?}", not_brute_force(SAMPLE, 40));
    println!("{:?}", not_brute_force(&input, 40));
}
