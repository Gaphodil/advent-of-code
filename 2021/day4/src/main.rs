use lib::*;

const SAMPLE: &str =
"7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1

22 13 17 11  0
 8  2 23  4 24
21  9 14 16  7
 6 10  3 18  5
 1 12 20 15 19

 3 15  0  2 22
 9 18 13 17  5
19  8  7 25 23
20 11 10 24  4
14 21 16 12  6

14 21 17 24  4
10 16 15  9 19
18  8 23 26 20
22 11 13  6  5
 2  0 12  3  7";

fn contains_only_true(vec: &[bool]) -> bool {
    vec.iter().map(|b| if *b {1} else {0}).sum::<usize>() == vec.len()
}

#[derive(Debug)]
struct BingoBoard {
    board: Vec<Vec<i32>>, // vector of rows; [x][y]
    marked: Vec<Vec<bool>>,
}

impl BingoBoard {
    fn new(board: Vec<Vec<i32>>) -> Self {
        BingoBoard {
            board,
            marked: vec![vec![false; 5]; 5],
        }
    }

    fn process_call(&mut self, num: i32) -> bool {
        for (xpos, row) in self.board.iter().enumerate() {
            for (ypos, val) in row.iter().enumerate() {
                if num.eq(val) {
                    self.marked[xpos][ypos] = true;
                    return true;
                }
            }
        }
        false
    }

    fn has_bingo(&self) -> bool {
        // sum rows, manual col/diag
        for row in &self.marked {
            if contains_only_true(row) {
                return true;
            }
        }
        for col in 0..4 {
            let mut winner = true;
            for row in &self.marked {
                winner = winner && row[col];
            }
            if winner {
                return true;
            }
        }
        // oh does NOT check diagonals
        // self.has_diagonal()
        false
    }

    fn has_diagonal(&self) -> bool {
        (self.marked[0][0] && self.marked[1][1] && self.marked[2][2] && self.marked[3][3] && self.marked[4][4])
        || (self.marked[0][4] && self.marked[1][3] && self.marked[2][2] && self.marked[3][1] && self.marked[4][0])
    }

    fn sum_unmarked(&self) -> i32 {
        let mut sum = 0;
        for (xpos, row) in self.board.iter().enumerate() {
            for (ypos, val) in row.iter().enumerate() {
                if !self.marked[xpos][ypos] {
                    sum += val;
                }
            }
        }
        sum
    }
}

#[derive(Debug)]
struct BingoGame {
    choices: Vec<i32>,
    boards: Vec<BingoBoard>,
}

impl BingoGame {
    fn run_game(&mut self) -> i32 {
        for call in &self.choices {
            println!("Calling {}!", call);

            let mut marks = 0;
            for board in &mut self.boards {
                if board.process_call(*call) {
                    marks += 1;
                }
            }
            println!("{}/{} boards marked", marks, self.boards.len());

            for board in &self.boards {
                if board.has_bingo() {
                    println!("Found a winner! {:?}", board);
                    let unmarked_sum = board.sum_unmarked();
                    println!("Unmarked sum: {}", unmarked_sum);
                    println!("Product: {}", unmarked_sum * call);

                    return unmarked_sum * call;
                }
            }
            println!("No winner yet, on to the next number!\n");
        }
        panic!("No winner?!?!");
    }

    // part 2
    fn run_long_game(&mut self) -> i32 {
        let total_boards = self.boards.len();
        let mut winner_checklist = vec![false; total_boards];

        for call in &self.choices {
            println!("Calling {}!", call);

            let mut marks = 0;
            for board in &mut self.boards {
                if board.process_call(*call) {
                    marks += 1;
                }
            }
            println!("{}/{} boards marked", marks, total_boards);

            for (ind, board) in self.boards.iter().enumerate() {
                if !winner_checklist[ind] && board.has_bingo() {
                    println!("Found a new winner!");
                    winner_checklist[ind] = true;

                    if contains_only_true(&winner_checklist) {
                        println!("Final winner found! {:?}", board);
                        let unmarked_sum = board.sum_unmarked();
                        println!("Unmarked sum: {}", unmarked_sum);
                        println!("Product: {}", unmarked_sum * call);

                        return unmarked_sum * call;
                    }
                }
            }
            println!("No final winner yet, on to the next number!\n");
        }
        panic!("No last winner?!?!");
    }
}

fn parse_input(inp: &str) -> BingoGame {
    let delim = if inp.len() > SAMPLE.len() {
        "\r\n"
    } else {
        "\n"
    };
    let parse_to_dec = |s: &str| s.parse::<i32>().unwrap();
    let parse_board_row = |s: &str| s.split_whitespace().map(parse_to_dec).collect();
    let parse_board = |s: &str| {
        BingoBoard::new(s.split(delim).map(parse_board_row).collect())
    };

    let split: Vec<&str> = inp.split(&delim.repeat(2)).collect();
    let choices = split[0].split(',').map(parse_to_dec).collect();
    let boards = split[1..].iter().map(|s| parse_board(*s)).collect();
    BingoGame { choices, boards }
}

fn main() {
    let input = input_string!("input.txt");
    // let input = split_lines!(input);

    // println!("{} {}", parse_input(SAMPLE).run_game(), parse_input(&input).run_game());
    println!("{} {}", parse_input(SAMPLE).run_long_game(), parse_input(&input).run_long_game());
}
