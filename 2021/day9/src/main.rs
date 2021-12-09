use std::collections::{HashSet, VecDeque};

use lib::*;

// input is 100x100 square map
// brute force (check adjacent of every tile) = O(10000 * 4) which is. small enough?

// part 2 is... map fill? not map colouring, can't find a name online
// or maybe i'm getting confused and it's a common but somehow unnamed problem (unlikely)

const SAMPLE: &str =
"2199943210
3987894921
9856789892
8767896789
9899965678";

fn parse_line(line: &str) -> Vec<u32> {
    line.chars().map(|c| String::from(c).parse::<u32>().unwrap()).collect()
}

fn parse_input(inp: &str) -> Vec<Vec<u32>> {
    let delim = if inp.len() == SAMPLE.len() {
        "\n"
    } else {
        "\r\n"
    };
    let split = inp.split(delim);
    split.map(parse_line).collect()
}

fn brute_force(inp: &str) -> u32 {
    let matrix = parse_input(inp);

    let ylen = matrix.len();
    let xlen = matrix[0].len();
    let mut total_risk = 0;

    for y in 0..ylen {
        for x in 0..xlen {
            let tile = matrix[y][x];
            // println!("{}: {},{}", tile, x, y);
            if tile == 9 { continue; }
            // top
            if y != 0 {
                let top = matrix[y-1][x];
                if top <= tile { continue; }
            }
            // bot
            if y != ylen-1 {
                let bot = matrix[y+1][x];
                if bot <= tile { continue; }
            }
            // left
            if x != 0 {
                let left = matrix[y][x-1];
                if left <= tile { continue; }
            }
            // right
            if x != xlen-1 {
                let right = matrix[y][x+1];
                if right <= tile { continue; }
            }
            // println!("added");
            total_risk += tile + 1;
        }
    }

    total_risk
}

#[derive(Debug)]
struct PointsOfInterest {
    low_points: HashSet<(usize, usize)>, // each basin has A SINGLE low point
    walls: HashSet<(usize, usize)>,
}

fn brute_force_modified(matrix: &[Vec::<u32>]) -> PointsOfInterest {
    let ylen = matrix.len();
    let xlen = matrix[0].len();
    let mut low_points = HashSet::new();
    let mut walls = HashSet::new();

    for y in 0..ylen {
        for x in 0..xlen {
            let tile = matrix[y][x];
            // println!("{}: {},{}", tile, x, y);
            if tile == 9 {
                walls.insert((x, y));
                continue;
            }
            // top
            if y != 0 {
                let top = matrix[y-1][x];
                if top <= tile { continue; }
            }
            // bot
            if y != ylen-1 {
                let bot = matrix[y+1][x];
                if bot <= tile { continue; }
            }
            // left
            if x != 0 {
                let left = matrix[y][x-1];
                if left <= tile { continue; }
            }
            // right
            if x != xlen-1 {
                let right = matrix[y][x+1];
                if right <= tile { continue; }
            }
            // println!("added");
            low_points.insert((x, y));
        }
    }

    PointsOfInterest { low_points, walls }
}

fn size_basins(inp: &str) -> u32 {
    let matrix = parse_input(inp); // almost forgot i needed this, somehow

    let ylen = matrix.len();
    let xlen = matrix[0].len();
    let poi = brute_force_modified(&matrix);

    // for each point:
    // make a set
    // insert points that aren't walls or oob
    // return set length

    let mut basin_sizes = vec![];
    for point in poi.low_points {
        let mut set = HashSet::new();
        let mut to_search = VecDeque::new();
        to_search.push_back(point);
        // use to_search as a queue that checks against set when pushed to
        while !to_search.is_empty() {
            // println!("{:?}", to_search);
            let spawn = to_search.pop_front().unwrap();
            set.insert(spawn);
            let y = spawn.1;
            let x = spawn.0;
            // intentionally crashed the program here to find out i forgot to swap x and y, a classic
            // should just use assert!s next time
            // let tile = matrix[y][x];
            // top
            if y != 0 {
                let top = (x, y-1);
                if !poi.walls.contains(&top) && !set.contains(&top) && !to_search.contains(&top) {
                    to_search.push_back(top);
                }
            }
            // bot
            if y != ylen-1 {
                let bot = (x, y+1);
                if !poi.walls.contains(&bot) && !set.contains(&bot) && !to_search.contains(&bot) {
                    to_search.push_back(bot);
                }
            }
            // left
            if x != 0 {
                let left = (x-1, y);
                if !poi.walls.contains(&left) && !set.contains(&left) && !to_search.contains(&left) {
                    to_search.push_back(left);
                }
            }
            // right
            if x != xlen-1 {
                let right = (x+1, y);
                if !poi.walls.contains(&right) && !set.contains(&right) && !to_search.contains(&right) {
                    to_search.push_back(right);
                }
            }
        }

        basin_sizes.push(set.len() as u32);
    }

    basin_sizes.sort_unstable();
    println!("{:?}", basin_sizes);
    let len = basin_sizes.len();
    basin_sizes[len-1] * basin_sizes[len-2] * basin_sizes[len-3]
}

fn main() {
    let input = input_string!("input.txt");

    // println!("{:?}", parse_input(SAMPLE));
    // println!("{:?}", brute_force(SAMPLE));
    // println!("{:?}", brute_force(&input));

    // println!("{:?}", brute_force_modified(SAMPLE));
    println!("{:?}", size_basins(SAMPLE));
    println!("{:?}", size_basins(&input));
}
