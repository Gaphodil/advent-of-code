use std::collections::HashMap;

use lib::*;

// initial thoughts: there's a way to do this with memoizing,
// and there's a way to just do it mathematically,
// and there's a way to do this with a hashmap by days remaining
// which lets me borrow a lot of yesterday's code
// input length: 300

const SAMPLE: &str = "3,4,3,1,2";

fn count_fish(inp: &str, days: i32) -> u64 { // part2: changed return type
    // init
    let mut fish_map = HashMap::new();
    for fish in parse_input(inp) {
        let of_an_age = fish_map.entry(fish).or_insert(0);
        *of_an_age += 1;
    }

    // cycle
    for day in 0..days {
        // move 0->temp
        // then temp->8->(7,temp)->6->..0, right to left
        let temp = *(fish_map.entry(0).or_insert(0));
        for age in 1..7 {
            let age_count = *(fish_map.entry(age).or_insert(0));
            fish_map.insert(age-1, age_count); // should always exist
        }
        let seven = *(fish_map.entry(7).or_insert(0));
        fish_map.insert(6, seven + temp);
        let eight = *(fish_map.entry(8).or_insert(0));
        fish_map.insert(7, eight);
        fish_map.insert(8, temp);
        // println!("{:?}", fish_map);
    }

    // count
    fish_map.into_values().sum()
}

fn parse_input(inp: &str) -> Vec<i32> {
    inp.split(',').map(parse_to_dec).collect()
}

fn main() {
    let input = input_string!("input.txt");
    // let input = split_lines!(input);

    // println!("{:?}", parse_input(SAMPLE).len());
    // println!("{:?}", parse_input(&input).len());
    println!("{:?}", count_fish(SAMPLE, 256));
    println!("{:?}", count_fish(&input, 256));
}
