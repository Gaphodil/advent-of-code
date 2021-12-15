use std::collections::HashMap;

use lib::*;

// the sample solution only goes right and down,
// which is concerning because it's not specified in the problem\

// all maps are square; input is 100x100
// i guess hashmaps could work again?
// one for map, one for lowest of surrounding + current
// but if it curves back around that would need multiple iterations -
// up to like 60...
// but. 60*100*100*4 isn't that bad? so?????

// on the one hand it's 500 now
// on the other it probably does only need to go down/right so

// update: either it doesn't or i screwed up
// doing it in reverse actually lets it reach equilibrium in 60...
// but the answer wasn't changing for a while
// so very inefficient but correct. oh well

const SAMPLE: &str =
"1163751742
1381373672
2136511328
3694931569
7463417111
1319128137
1359912421
3125421639
1293138521
2311944581";

const SAMPLE2: &str =
"11199
99199
11199
19999
11111";

type CaveRisk = HashMap<(usize, usize), u32>;

fn parse_input(inp: &str) -> CaveRisk {
    let delim = if inp.len() == SAMPLE.len() {
        "\n"
    } else {
        "\r\n"
    };
    let lines: Vec<&str> = inp.split(delim).collect();
    let mut map = CaveRisk::new();
    for (y, line) in lines.iter().enumerate() {
        for (x, c) in line.chars().enumerate() {
            map.insert((x, y), c.to_digit(10).unwrap());
        }
    }

    map
}

fn parse_input_5x(inp: &str) -> CaveRisk {
    let delim = if inp.len() == SAMPLE.len() {
        "\n"
    } else {
        "\r\n"
    };
    let lines: Vec<&str> = inp.split(delim).collect();
    let s = lines.len();
    let mut map = CaveRisk::new();
    for (y, line) in lines.iter().enumerate() {
        for (x, c) in line.chars().enumerate() {
            for j in 0..5 {
                for i in 0..5 {
                    let k = (x+i*s, y+j*s);
                    // oh my god it's modulo NINE (sorta)
                    let v = (c.to_digit(10).unwrap() + (i + j) as u32 - 1) % 9 + 1;
                    // println!("{:?}, {:?}", k, v);
                    map.insert(
                        k,
                        v
                    );
                }
            }
        }
    }

    map
}

fn get_lowest_risk(inp: &str) -> u32 {
    let map = parse_input(inp);
    let s = (inp.len() as f64).sqrt() as usize;
    // println!("{}", s);

    let mut total_risks = init_total_risks(s);
    // for i in 0..60 { // estimate
        // let mut changed = false;
        for y in 0..s {
            for x in 0..s {
                let coords = (x, y);
                let ortho = vec![
                    if x>0 {(x-1, y)} else {(s, s)},
                    if x<s-1 {(x+1, y)} else {(s, s)},
                    if y>0 {(x, y-1)} else {(s, s)},
                    if y<s-1 {(x, y+1)} else {(s, s)},
                ];
                for adj in ortho {
                    if let Some(risk) = total_risks.get(&adj) {
                        // println!("{:?} {:?}", coords, adj);
                        let potential_new = map.get(&coords).unwrap() + *risk;
                        if total_risks.get(&coords).unwrap() > &potential_new {
                            // changed = true;
                            total_risks.insert(coords, potential_new);
                        }
                    }
                }
            }
        }

        // if !changed {
        //     break;
        // }
    // }

    *total_risks.get(&(s-1, s-1)).unwrap()
}

fn init_total_risks(len: usize) -> CaveRisk {
    let mut map = CaveRisk::new();
    for j in 0..len {
        for i in 0..len {
            map.insert((i, j), (len * len * 10) as u32);
        }
    }
    map.insert((0, 0), 0);

    map
}

fn get_lowest_risk_5x(inp: &str) -> u32 {
    let map = parse_input_5x(inp);
    let s = (inp.len() as f64).sqrt() as usize;
    let s = s * 5;
    println!("{}", s);

    let mut total_risks = init_total_risks(s);
    for i in 0..60 {
        println!("iter {}", i);
        let mut changed = false;
        for y in 0..s {
            for x in 0..s {
                let coords = (x, y);
                let ortho = vec![
                    if x>0 {(x-1, y)} else {(s, s)},
                    if x<s-1 {(x+1, y)} else {(s, s)},
                    if y>0 {(x, y-1)} else {(s, s)},
                    if y<s-1 {(x, y+1)} else {(s, s)},
                ];
                for adj in ortho {
                    if let Some(risk) = total_risks.get(&adj) {
                        // println!("{:?} {:?}", coords, adj);
                        let potential_new = map.get(&coords).unwrap() + *risk;
                        if total_risks.get(&coords).unwrap() > &potential_new {
                            changed = true;
                            total_risks.insert(coords, potential_new);
                        }
                    }
                }
            }
        }

        for y in (0..s).rev() {
            for x in (0..s).rev() {
                let coords = (x, y);
                let ortho = vec![
                    if x>0 {(x-1, y)} else {(s, s)},
                    if x<s-1 {(x+1, y)} else {(s, s)},
                    if y>0 {(x, y-1)} else {(s, s)},
                    if y<s-1 {(x, y+1)} else {(s, s)},
                ];
                for adj in ortho {
                    if let Some(risk) = total_risks.get(&adj) {
                        // println!("{:?} {:?}", coords, adj);
                        let potential_new = map.get(&coords).unwrap() + *risk;
                        if total_risks.get(&coords).unwrap() > &potential_new {
                            changed = true;
                            total_risks.insert(coords, potential_new);
                        }
                    }
                }
            }
        }
        if !changed {
            break;
        }
    }

    *total_risks.get(&(s-1, s-1)).unwrap()
}

fn main() {
    let input = input_string!("input.txt");

    // println!("{:?}", get_lowest_risk(SAMPLE));
    // println!("{:?}", get_lowest_risk(&input));
    println!("{:?}", get_lowest_risk_5x(SAMPLE));
    println!("{:?}", get_lowest_risk_5x(&input));
}
