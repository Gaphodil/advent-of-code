use lib::*;

const SAMPLE: &str =
"5483143223
2745854711
5264556173
6141336146
6357385478
4167524645
2176841721
6882881134
4846848554
5283751526";

fn parse_line(line: &str) -> Vec<Octopus> {
    line.chars().map(|s|
        Octopus::new(String::from(s).parse::<u32>().unwrap())
    ).collect()
}

fn parse_input(inp: &str) -> Vec<Vec<Octopus>> {
    let delim = if inp.len() == SAMPLE.len() {
        "\n"
    } else {
        "\r\n"
    };
    inp.split(delim).map(parse_line).collect()
}

#[derive(Debug, Clone, Copy)]
struct Octopus {
    energy: u32,
    flash: bool,
}

impl Octopus {
    fn new(energy: u32) -> Self {
        Octopus {
            energy,
            flash: false
        }
    }
}

// in-place
fn increase_all(deez: &mut [Vec<Octopus>]) {
    for row in deez.iter_mut() {
        for po in row.iter_mut() {
            po.energy += 1;
        }
    }
}

// not in-place
fn flash_all(deez: &[Vec<Octopus>]) -> (Vec<Vec<Octopus>>, u32) {
    let mut changed = true;
    let mut old_deez = deez.to_owned();
    let mut flash_count = 0;
    while changed {
        changed = false;
        // almost forgot a rudimentary game of life sim works on a copy for each step
        let mut new_deez = old_deez.to_owned();
        for (y, row) in old_deez.iter().enumerate() {
            for (x, po) in row.iter().enumerate() {
                if po.energy > 9 && !po.flash {
                    flash_one(x, y, new_deez.as_mut());
                    changed = true;
                    flash_count += 1;
                }
            }
        }
        old_deez = new_deez.clone();
    }

    (old_deez, flash_count)
}

// assume input energy is above 9
fn flash_one(x: usize, y: usize, deez: &mut [Vec<Octopus>]) {
    let coords = vec![
        if x > 0 && y > 0 {(x-1, y-1)} else {(10, 10)},
        if x > 0 {(x-1, y)} else {(10, 10)},
        if x > 0 {(x-1, y+1)} else {(10, 10)},
        if y > 0 {(x, y-1)} else {(10, 10)},
        (x, y+1),
        if y > 0 {(x+1, y-1)} else {(10, 10)},
        (x+1, y),
        (x+1, y+1),
    ];
    for adj in coords {
        if adj.0 >= 10 || adj.1 >= 10 { continue; }
        deez[adj.1][adj.0].energy += 1;
    }
    deez[y][x].flash = true;
}

// in-place
fn resolve_flashes(deez: &mut [Vec<Octopus>]) {
    for row in deez.iter_mut() {
        for po in row.iter_mut() {
            if po.flash {
                po.energy = 0;
                po.flash = false;
            }
        }
    }
}

fn do_steps(inp: &str, count: u32) -> u32 {
    let mut deez = parse_input(inp);
    let mut flash_total = 0;
    for step in 0..count {
        // println!("Step {}", step+1);

        increase_all(&mut deez);
        let ret = flash_all(&deez);
        deez = ret.0.clone();
        flash_total += ret.1;
        resolve_flashes(&mut deez);

        // println!("{:?}", deez);
    }

    flash_total
}

fn do_steps_until_sync(inp: &str) -> u32 {
    let mut deez = parse_input(inp);
    let mut curstep = 0;
    loop {
        curstep += 1;

        increase_all(&mut deez);
        let ret = flash_all(&deez);
        deez = ret.0.clone();
        // flash_total += ret.1;
        resolve_flashes(&mut deez);

        if synched(&deez) { break; }
    }

    curstep
}

fn synched(deez: &[Vec<Octopus>]) -> bool {
    for row in deez {
        for po in row {
            if po.energy > 0 { return false; }
        }
    }
    true
}

fn main() {
    let input = input_string!("input.txt");

    // println!("{}", do_steps(SAMPLE, 10));
    // println!("{}", do_steps(SAMPLE, 100));
    // println!("{}", do_steps(&input, 100));

    println!("{}", do_steps_until_sync(SAMPLE));
    println!("{}", do_steps_until_sync(&input));
}

// octopodes nuts
