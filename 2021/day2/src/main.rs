use lib::*;

fn do_instr(x: &mut i64, z: &mut i64, instr: &str) {
    // println!("inst: {}", instr);
    let mut split = instr.split(' ');
    let dir = split.next().unwrap();
    let length = split.next().unwrap();
    let length = length.parse::<i64>().unwrap();

    if dir.eq("forward") {
        *x += length;
    } else if dir.eq("down") {
        *z += length;
    } else if dir.eq("up") {
        *z -= length;
    } else {
        panic!("invalid direction {}", dir);
    }
}

fn do_movement(commands: &[&str]) -> i64 {
    let mut x_lvl = 0;
    let mut z_lvl = 0;
    for instr in commands {
        do_instr(&mut x_lvl, &mut z_lvl, instr);
    }

    println!("{} {}", x_lvl, z_lvl);
    x_lvl * z_lvl
}

fn do_instr_with_aim(x: &mut i64, z: &mut i64, aim: &mut i64, instr: &str) {
    let mut split = instr.split(' ');
    let dir = split.next().unwrap();
    let length = split.next().unwrap();
    let length = length.parse::<i64>().unwrap();

    if dir.eq("forward") {
        *x += length;
        *z += length * *aim;
    } else if dir.eq("down") {
        *aim += length;
    } else if dir.eq("up") {
        *aim -= length;
    } else {
        panic!("invalid direction {}", dir);
    }
}

fn do_movement_with_aim(commands: &[&str]) -> i64 {
    let mut x_lvl = 0;
    let mut z_lvl = 0;
    let mut aim = 0;
    for instr in commands {
        do_instr_with_aim(&mut x_lvl, &mut z_lvl, &mut aim, instr);
    }

    println!("{} {}", x_lvl, z_lvl);
    x_lvl * z_lvl
}

fn main() {
    let sample = vec![
        "forward 5",
        "down 5",
        "forward 8",
        "up 3",
        "down 8",
        "forward 2"
    ];
    let input = input_string!("input.txt");
    let input = split_lines!(input);
    // println!("{:?}", split.collect::<Vec<_>>());
    let input = input.collect::<Vec<&str>>();

    // println!("{} {}", do_movement(&sample), do_movement(&input));
    println!("{} {}", do_movement_with_aim(&sample), do_movement_with_aim(&input));
}
