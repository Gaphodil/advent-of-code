fn count_increases(inp: &[i32]) -> i32 {
    let mut cur = 0;
    let mut count = 0;
    for depth in inp.iter() {
        if cur.le(depth) {
            count += 1;
        }
        cur = *depth;
    }

    count-1
}

fn count_windows(inp: &[i32]) -> i32 {
    let mut window: Vec<i32> = vec![];
    let mut cur = 0;
    let mut count = 0;
    for (ind, depth) in inp.iter().enumerate() {
        window.push(*depth);
        if ind >= 3 {
            if cur == 0 {
                cur = window[..3].iter().sum();
            }
            window.remove(0);
            let new = window.iter().sum();
            if cur < new {
                count += 1
            }
            cur = new;
        }
    }

    count
}

fn get_input() -> Vec<i32> {
    let data = std::fs::read_to_string("input.txt").unwrap();

    // delimiter = \r\n (not just \n!!!), parse as integers
    let parse = |x: &str| {
        let res = x.parse::<i32>();
        if let Err(e) = res {
            panic!("{}: {}", x, e);
        }
        res.unwrap()
    };
    // data.split('\n').map(parse).collect()
    let seperated = data.split("\r\n");
    // println!("{:?}", seperated);

    seperated.map(parse).collect()
}

fn main() {
    let sample = vec![
        199,
        200, // 1
        208, // 2
        210, // 3
        200,
        207, // 4
        240, // 5
        269, // 6
        260,
        263 // 7
    ];
    // sample outputs: 5 7

    let input = get_input();
    // println!("{:?} {}", input, input.len());

    // println!("{} {}", count_increases(&sample), count_increases(&input));
    println!("{} {}", count_windows(&sample), count_windows(&input));
}
