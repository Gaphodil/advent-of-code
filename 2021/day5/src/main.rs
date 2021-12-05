use std::{collections::HashMap, cmp::{min, max}};

use lib::*;

#[derive(Debug, Clone, Copy)]
#[derive(PartialEq, Eq, Hash)]
struct Point(i32, i32);

impl Point {
    fn new(s: &str) -> Self {
        let coords: Vec<i32> = s.split(',').map(parse_to_dec).collect();
        Point { 0: coords[0], 1: coords[1] }
    }
}

#[derive(Debug)]
#[derive(PartialEq, Eq, Hash)]
struct LineSegment {
    start: Point,
    end: Point,
}

impl LineSegment {
    fn new(line: &str) -> Self {
        let points = line.split(" -> ");
        let points: Vec<Point> = points.map(Point::new).collect();
        LineSegment { start: points[0], end: points[1] }
    }

    fn is_horizontal(&self) -> bool {
        self.start.1 == self.end.1
    }

    fn is_vertical(&self) -> bool {
        self.start.0 == self.end.0
    }

    // seperate diagonals for ease of calculation
    fn is_ul_dr(&self) -> bool {
        // x2-x1 == y2-y1; works both directions
        (self.end.0 - self.start.0) == (self.end.1 - self.start.1)
    }

    fn is_ur_dl(&self) -> bool {
        // x2-x1 == -(y2-y1)
        (self.end.0 - self.start.0) == -(self.end.1 - self.start.1)
    }

    // based on https://stackoverflow.com/questions/45282970
    fn points_in_line(&self) -> Vec<Point> {
        if self.is_horizontal() { // same y, different x y
            let start = min(self.start.0, self.end.0);
            let end = max(self.start.0, self.end.0);
            return (start .. end+1).map(
                |x| Point { 0: x, 1: self.start.1 }
            ).collect();
        }
        if self.is_vertical() { // same x, different y
            let start = min(self.start.1, self.end.1);
            let end = max(self.start.1, self.end.1);
            return (start .. end+1).map(
                |y| Point { 0: self.start.0, 1: y }
            ).collect();
        }
        // println!("{:?} -> {:?} not orthogonal", self.start, self.end);
        // vec![]

        // part 2
        // addition working while negative does NOT mean the range works with negative
        let dist = self.end.0 - self.start.0; // e.g. 2,2 -> 0,0
        let start = min(0, dist);
        let end = max(0, dist);
        if self.is_ul_dr() {
            return (start .. end+1).map(
                |d| Point {0: self.start.0 + d, 1: self.start.1 + d} // 2+(-x), 2+(-x)
            ).collect();
        }
        if self.is_ur_dl() {
            return (start .. end+1).map(
                |d| Point {0: self.start.0 + d, 1: self.start.1 - d} // 2+(-x), 0-(-x)
            ).collect();
        }
        println!("{:?} -> {:?} not 8-dir??", self.start, self.end);
        vec![]
    }
}

fn find_overlap(inp: &str) -> usize {
    let segments = parse_input(inp);
    let mut cloudmap = HashMap::new();

    for seg in segments {
        // println!("\n{:?}", seg);
        for point in seg.points_in_line() {
            // println!("{:?}", point);
            let count = cloudmap.entry(point).or_insert(0);
            *count += 1;
        }
    }

    cloudmap.into_values().filter(|i| *i > 1).count()
}

fn parse_input(inp: &str) -> Vec<LineSegment> {
    let lines = if inp.len() > SAMPLE.len() {
        split_lines!(inp)
    } else {
        inp.split("\n")
    };

    lines.map(LineSegment::new).collect()
}

const SAMPLE: &str =
"0,9 -> 5,9
8,0 -> 0,8
9,4 -> 3,4
2,2 -> 2,1
7,0 -> 7,4
6,4 -> 2,0
0,9 -> 2,9
3,4 -> 1,4
0,0 -> 8,8
5,5 -> 8,2";

fn main() {
    // input dimensions max at 999 - probably too high to brute force
    // only 500 line segments
    let input = input_string!("input.txt");

    // println!("{:?} {:?}", parse_input(SAMPLE), parse_input(&input));
    println!("{:?}", find_overlap(SAMPLE));
    println!("{:?}", find_overlap(&input));
}
