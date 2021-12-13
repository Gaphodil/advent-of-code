use std::collections::HashSet;

use lib::*;

// input: coords upwards of 1200
// fold instructions LOOK only on halfway points but might as well be safe
// fold along y=b: y<b -> (x,y); y>b -> (x, b-(y-b))
// fold along x=a: x<a -> (x,y); x>a -> (a-(x-a), y)
// idea: set of points, return new set after fold

const SAMPLE: &str =
"6,10
0,14
9,10
0,3
10,4
4,11
6,0
6,12
4,1
0,13
10,12
3,4
3,0
8,4
1,10
2,14
8,10
9,0

fold along y=7
fold along x=5";

#[derive(Debug, PartialEq)]
enum FoldAxis {
    X, Y
}

#[derive(Debug)]
struct Fold {
    dimension: FoldAxis,
    location: u32,
}

impl Fold {
    fn new(line: &str) -> Self {
        let mut split = line.split(' ').last().unwrap().split('=');
        Fold {
            dimension: match split.next().unwrap() {
                "x" => FoldAxis::X,
                "y" => FoldAxis::Y,
                _ => panic!("invalid fold dimension")
            },
            location: split.next().unwrap().parse::<u32>().unwrap()
        }
    }
}

fn parse_points(inp: &str, delim: &str) -> HashSet<(u32, u32)> {
    let mut set = HashSet::new();
    for line in inp.split(delim).collect::<Vec<&str>>() {
        let mut split = line.split(',');
        let x = split.next().unwrap().parse::<u32>().unwrap();
        let y = split.next().unwrap().parse::<u32>().unwrap();
        set.insert((x,y));
    }

    set
}

fn parse_fold(inp: &str, delim: &str) -> Vec<Fold> {
    inp.split(delim).map(Fold::new).collect()
}

fn parse_input(inp: &str) -> (HashSet<(u32,u32)>, Vec<Fold>) {
    let delim = if inp.len() == SAMPLE.len() {
        "\n"
    } else {
        "\r\n"
    };
    let double_delim = format!("{}{}", delim, delim);
    let mut split = inp.split(double_delim.as_str());
    let points = parse_points(split.next().unwrap(), delim);
    let folds = parse_fold(split.next().unwrap(), delim);

    (points, folds)
}

fn do_one_fold(inp: &str) -> usize {
    let (mut paper, folds) = parse_input(inp);
    let instr = &folds[0];

    let mut folded = paper.clone();
    if instr.dimension == FoldAxis::X {
        for point in paper {
            if point.0 > instr.location {
                let new_x = 2*instr.location - point.0;
                folded.remove(&point);
                folded.insert((new_x, point.1));
            }
        }
    } else {
        for point in paper {
            if point.1 > instr.location {
                let new_y = 2*instr.location - point.1;
                folded.remove(&point);
                folded.insert((point.0, new_y));
            }
        }
    }
    paper = folded;

    paper.len()
}

fn do_all_folds(inp: &str) -> HashSet<(u32, u32)> {
    let (mut paper, folds) = parse_input(inp);
    for instr in &folds {
        let mut folded = paper.clone();
        if instr.dimension == FoldAxis::X {
            for point in paper {
                if point.0 > instr.location {
                    let new_x = 2*instr.location - point.0;
                    folded.remove(&point);
                    folded.insert((new_x, point.1));
                }
            }
        } else {
            for point in paper {
                if point.1 > instr.location {
                    let new_y = 2*instr.location - point.1;
                    folded.remove(&point);
                    folded.insert((point.0, new_y));
                }
            }
        }
        paper = folded;
    }
    paper
}

// input suggests 40x6 in size
fn display_result(inp: &str) {
    let paper = do_all_folds(inp);
    for y in 0..6 {
        for x in 0..40 {
            print!("{}",
                if paper.contains(&(x as u32, y as u32)) {"#"}
                else {"."}
            );
        }
        println!();
    }
}

fn main() {
    let input = input_string!("input.txt");

    // println!("{:?}", parse_input(SAMPLE));
    // println!("{:?}", parse_input(&input));

    println!("{:?}", do_one_fold(SAMPLE));
    println!("{:?}", do_one_fold(&input));

    display_result(&input);
}
