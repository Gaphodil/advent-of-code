use lib::*;

fn find_pt2(inp: &[u32], oxygen: bool) -> u32 {
    let name = if oxygen {"oxygen"} else {"co2"};

    let total = inp.len();
    let mut potential = vec![1; total];

    let end = if total == 12 {5} else {12};
    for power in (0..end).rev() {
        let mut one_count = 0;
        let digit = u32::pow(2, power);
        for (ind, num) in inp.iter().enumerate() {
            // println!("{}", num & digit);
            if potential[ind] == 1 && num & digit == digit { // most of REMAINING numbers
                one_count += 1;
            }
        }

        // let more_ones = one_count as f32 > potential.iter().sum::<i32>() as f32 / 2.0;
        let half_count_potential = potential.iter().sum::<i32>() as f32 / 2.0;
        let more_ones = one_count as f32 >= half_count_potential; // geq because keep 1 for oxy, keep 0 for co2
        for (ind, num) in inp.iter().enumerate() {
            let valid_digit = if oxygen {
                (more_ones && num & digit != digit)
                || (!more_ones && num & digit == digit)
            } else {
                (more_ones && num & digit == digit)
                || (!more_ones && num & digit != digit)
            };
            if potential[ind] == 1 && valid_digit {
                potential[ind] = 0;
            }
        }

        // println!("{}: {:?} {:?}", one_count, potential, inp);
        if potential.iter().sum::<i32>() == 0 {
            panic!("{}: ran out of potential", name);
        } else if potential.iter().sum::<i32>() == 1 {
            break;
        }
    }

    if potential.iter().sum::<i32>() > 1 {
        panic!("{}: too much potential", name);
    }

    for (ind, oxy) in inp.iter().enumerate() {
        if potential[ind] == 1 {
            return *oxy;
        }
    }
    panic!("{}: couldn't find", name);
}

fn get_life_support(inp: &[u32]) -> u32 {
    // technically more efficient to do both together
    // but input is small enough that it doesn't matter
    let oxygen = find_pt2(inp, true);
    let co2 = find_pt2(inp, false);

    println!("{} {}", oxygen, co2);
    oxygen * co2
}

fn get_power(inp: &[u32]) -> u32 {
    let mut gamma = 0;
    let mut epsilon = 0;

    let total = inp.len();

    // input width: sample = 5, input = 12
    let end = if total == 12 {5} else {12};
    for power in 0..end {
        let mut one_count = 0;
        let digit = u32::pow(2, power);
        for num in inp {
            // println!("{}", num & digit);
            if num & digit == digit {
                one_count += 1;
            }
        }
        if one_count >= total / 2 {
            gamma += digit;
        } else {
            epsilon += digit;
        }
    }

    println!("{} {}", gamma, epsilon);
    gamma * epsilon
}

fn main() {
    let parse_binstr_u32 = |s| u32::from_str_radix(s, 2).unwrap();

    let sample = vec![
        "00100",
        "11110",
        "10110",
        "10111",
        "10101",
        "01111",
        "00111",
        "11100",
        "10000",
        "11001",
        "00010",
        "01010",
    ];
    let sample: Vec<u32> = sample.iter().map(|s| parse_binstr_u32(*s)).collect();

    let input = input_string!("input.txt");
    let input = split_lines!(input);
    let input: Vec<u32>  = input.map(parse_binstr_u32).collect();

    // println!("{} {}", get_power(&sample), get_power(&input));
    println!("{} {}", get_life_support(&sample), get_life_support(&input));
}
