// visualize as side-view with -y going down
// target area is inclusive

fn try_vel(xvel: i32, yvel: i32, target: (i32,i32,i32,i32)) -> bool {
    let (left, down, right, up) = target;
    let mut xpos = 0;
    let mut ypos = 0;

    let mut xvel = xvel;
    let mut yvel = yvel;
    let mut max_ypos = down;

    while xpos < right && ypos > down {
        // do movement
        xpos += xvel;
        ypos += yvel;
        max_ypos = i32::max(ypos, max_ypos);
        // if landed: success!
        if (xpos >= left && xpos <= right) && (ypos >= down && ypos <= up) {
            // println!("{}, {}", xpos, ypos);
            // println!("{}", max_ypos);
            return true;
        }
        // change velocity to drag/gravity
        xvel += match xvel {
            v if v > 0 => -1,
            v if v < 0 => 1,
            v if v == 0 => 0,
            _ => panic!()
        };
        yvel -= 1;
    }

    false
}

fn find_max_yvel(target: (i32,i32,i32,i32)) -> i32 {
    let min_xvel = 21; // found manually by subtracting factorials from input
    let mut yvel = 1;
    loop {
        if try_vel(min_xvel, yvel, target) { break; }
        yvel += 1;
    }
    loop {
        // if !try_vel(min_xvel, yvel, target) { break; }
        print!("{} ", yvel);
        try_vel(min_xvel, yvel, target);
        yvel += 1;
        if yvel > 200 { break; }
    }
    yvel
}

// part 2
fn brute_force(target: (i32,i32,i32,i32)) -> u32 {
    let (left, down, right, up) = target;
    let min_xvel = if left == 20 {
        6
    } else { 21 };

    let mut valid_vel = 0;
    for xvel in min_xvel..right+1 {
        for yvel in down..200 {
            if try_vel(xvel, yvel, target) {
                valid_vel += 1;
            }
        }
    }

    valid_vel
}

fn main() {
    // let input = "target area: x=211..232, y=-124..-69";
    let input = (211, -124, 232, -69); // l d r u
    // let sample = "target area: x=20..30, y=-10..-5";
    let sample = (20,-10,30,-5);

    // println!("{:?}", parse_input(&input));
    // println!("{:?}", try_vel(7, 2, sample));
    // println!("{:?}", try_vel(6, 3, sample));
    // println!("{:?}", try_vel(9, 0, sample));
    // println!("{:?}", try_vel(17, 4, sample)); // does not match
    // println!("{:?}", try_vel(6, 9, sample)); // highest y
    // println!("{:?}", try_vel(7, 9, sample)); // this should also work?

    // println!("{:?}", find_max_yvel(input));
    println!("{:?}", brute_force(sample));
    println!("{:?}", brute_force(input));
}
