use lib::*;

// lower/upper bounds provided
// brute force SEEMED unlikely but wow
// i still really REALLY underestimate how fast computers can do 1M calculations

// then for part 2 i somehow confused this with fibonacci

const SAMPLE: &str = "16,1,2,0,4,2,7,1,2,14";

// a testament to overthinking
fn find_local_minima(inp: &str) -> i32 {
    // let crabs = commas_to_ints(inp);
    // let mut pos_l = (crabs.len() / 3) as i32;
    // let mut pos_r = 2 * pos_l;

    // let mut val_l = fuel_cost(&crabs, pos_l);
    // let mut val_r = fuel_cost(&crabs, pos_r);

    // while (pos_r - pos_l) > 1 {
    //     let guess = pos_l + (pos_r - pos_l) / 2;
    //     let val_guess = fuel_cost(&crabs, guess);

    //     // l < guess -> l=0, r=old l
    //     // l > guess > r -> uhhh
    // }

    0
}

fn fuel_cost(crabs: &[i32], pos: i32) -> i32 {
    let mut cost = 0;
    for crab in crabs {
        cost += (*crab - pos).abs();
    }
    cost
}

// literally no idea where to start so i looked it up
// what's a triangular number???
fn fuel_cost_2(crabs: &[i32], pos: i32) -> i32 {
    let mut cost = 0;
    for crab in crabs {
        let n = (*crab - pos).abs();
        cost += n * (n+1) / 2;
    }
    cost
}

fn brute_force(inp: &str) -> i32 {
    let crabs = commas_to_ints(inp);
    let mut costs: Vec<i32> = Vec::new();

    let max = crabs.iter().max().unwrap();
    let min = crabs.iter().min().unwrap();

    for pos in *min..*max+1 {
        // costs.push(fuel_cost(&crabs, pos));
        costs.push(fuel_cost_2(&crabs, pos));
    }
    // println!("{:?}", costs);

    *costs.iter().min().unwrap()
}

fn main() {
    let input = input_string!("input.txt");

    // println!("{:?}", commas_to_ints(SAMPLE).len());
    // println!("{:?}", commas_to_ints(&input).len());

    println!("{:?}", brute_force(SAMPLE)); // implies a single minima?
    println!("{:?}", brute_force(&input));
}
