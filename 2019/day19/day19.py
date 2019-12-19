# 'f'               - reading from "input.txt"
# 'r' (or not 'f')  - reading from raw input
input_mode = 'f'
relative_base = 0

#started several hours late, decided to take a break
#unfortunately it was one i could possibly do reasonably quick

#two input instructions: requests X and Y position; domain: nonzero integers
#will output: stationary (0) or being pulled (1)
#provided image: beam is top left of (0,0) and aims down-right (positive)
#eg. in 10x10 area points affected are 27
#part one: what is affected in 50x50? (0-49)
#   bruteforced in ~30 minutes of work
#part two: what is min distance to fit a 100x100 square into the beam
#   specifically, find top left corner of said square, and give 10000*x + y
#   uhhhhhhhh
#   evaluating previous output by sight:
#       width (bottleneck) of beam:       dist from left      dist from top
#                                 1                   0                   0
#                                 2                   3                   11
#                                 3                   5                   20
#                                 4                   7                   28
#                                 5                   8                   33
#                                 6                   10                  41
#                                 7                   12                  49
# estimated location of width = 100: 
#    x = 180, y = 50//7*100
# estimated location of width = 100 and a square exists:
#    x = 180 + 12 + 14 (roughly how many y will displace), y = 50//*100 + 93 + 100 (idk)
# personal leaderboard suggests 90 minutes after completing part one (lot of simple bugs in square-finder)

# added in part two to generalize
depth = 200
# x_offset = 180 + 12+14 # 12 from experiment 1
# y_offset = 50//7*100 + 93+100 # 93 from experiment 1
x_offset = 218  # from experiment 2
y_offset = 902  # from experiment 2

# part one
line = ['.']*depth
area = []
for i in range(depth):
    area.append(line[::])
# print(area)
a = x_offset
b = y_offset
swap = 0
affected = 0

def output_func(arg):
    global a
    global b
    global area
    global affected
    if arg == 1:
        area[b-y_offset][a-x_offset] = '#'
        affected += 1
    a += 1
    if a == x_offset + depth:
        a = x_offset
        b += 1
    
    return

def input_func():
    global swap
    if swap == 0:
        swap = 1
        return a
    if swap == 1:
        swap = 0
        return b
    print("input error")
    return -1

def render_area():
    for y in range(depth):
        s = ""
        for x in range(depth):
            s += area[y][x]
        print(s)
    return
    
def scan_for_square():
    start_x = 0
    start_y = 0
    counts = [area[y].count('#') for y in range(depth)]
    indexes = [area[y].index('#') for y in range(depth)]
    print(counts)
    print(indexes)
    success = 1
    for y in range(100):
        for x in range(depth-90):
            success = 0
            new_counts = [area[j][x:].count('#') for j in range(y,100+y)]
            new_indexes = [area[j][x:].index('#') for j in range(y,100+y)]
            # print(x,y,new_counts,new_indexes)
            if new_counts[0] < 100: #checking one every line after strictly has >= this line
                success = 0
                break
            
            try:
                new_indexes.index(0)
            except ValueError:
                continue
            
            try:
                if new_indexes.index(1) != -1:
                    continue
            except ValueError:
                pass
            
            success = 1
            start_x = x
            start_y = y
            break
        if success != 0:
            break

    print(x_offset, y_offset)
    print("+",start_x, start_y)
    print ("=",start_x+x_offset, start_y+y_offset)
                
      
def parse_mode(l, ram, arg, mode):
    global relative_base
    if mode == 0:   #position mode
        if arg < 0:
            print("negative index")
            return -1
        if arg >= len(l):
            try:
                return ram[arg]
            except:
                # print("ram[",arg,"] doesnt exist; returning 0")
                return 0
        return l[arg]
    elif mode == 1: #immediate mode
        return arg
    elif mode == 2: #relative mode
        rel_ind = relative_base+arg
        if rel_ind < 0:
            print("negative index")
            return -1
        if rel_ind >= len(l):
            try:
                return ram[rel_ind]
            except:
                # print("ram[",rel_ind,"] doesnt exist; returning 0")
                return 0
        return l[rel_ind]
    print("parse_mode failed")
    return -9999

def parse_intcode(l, ram):
    global relative_base
    i = 0;
    while (i < len(l)):
        #print(i,l[i])
        code = l[i]
        
        # add
        if code%100 == 1:
            #print("opcode 1")
            arg1 = l[i+1]
            arg2 = l[i+2]
            dest = l[i+3]
            
            code = code // 100
            arg1 = parse_mode(l, ram, arg1, code%10)
            
            code = code // 10
            arg2 = parse_mode(l, ram, arg2, code%10)
                
            code = code // 10
            #dest = parse_mode(l, ram, dest, code%10)
            
            out = arg2 + arg1
            
            if code%10 == 0:
                if dest >= len(l):
                    ram[dest] = out
                else:
                    l[dest] = out
                #print("stored",out,"in",dest)
            elif code%10 == 2:
                rel_ind = relative_base+dest
                if rel_ind >= len(l):
                    ram[rel_ind] = out
                else:
                    l[rel_ind] = out
                #print("stored",out,"in",rel_ind)
            else:
                print("opcode 1 failed")
            
            i += 4
        
        # multiply
        elif code%100 == 2:
            #print("opcode 2")
            arg1 = l[i+1]
            arg2 = l[i+2]
            dest = l[i+3]
            
            code = code // 100
            arg1 = parse_mode(l, ram, arg1, code%10)
            
            code = code // 10
            arg2 = parse_mode(l, ram, arg2, code%10)
            
            out = arg2 * arg1
                
            code = code // 10
            #dest = parse_mode(l, ram, dest, code%10)
            if code%10 == 0:
                if dest >= len(l):
                    ram[dest] = out
                else:
                    l[dest] = out
                #print("stored",out,"in",dest)
            elif code%10 == 2:
                rel_ind = relative_base+dest
                if rel_ind >= len(l): # this (and lines like it) were bugged in day 9
                    ram[rel_ind] = out
                else:
                    l[rel_ind] = out
                #print("stored",out,"in",rel_ind)
            else:
                print("opcode 2 failed")
            
            i += 4
        
        # input
        elif code%100 == 3:
            #print("opcode 3")
            dest = l[i+1]
            
            code = code // 100
            #dest = parse_mode(l, ram, dest, code%10)
            
            inp = input_func()
            out = inp
            if code%10 == 0:
                if dest >= len(l):
                    ram[dest] = out
                else:
                    l[dest] = out
                #print("stored",out,"in",dest)
            elif code%10 == 2:
                rel_ind = relative_base+dest
                if rel_ind >= len(l):
                    ram[rel_ind] = out
                else:
                    l[rel_ind] = out
                #print("stored",out,"in",rel_ind)
            else:
                print("opcode 3 failed")
            
            i += 2
        
        # output
        elif code%100 == 4:
            arg1 = l[i+1]
            
            code = code // 100
            arg1 = parse_mode(l, ram, arg1, code%10)
            
            #print("opcode 4:\t", arg1)
            output_func(arg1)
            i += 2
            
        #part two (day 5)
        # jump nonzero (jnz)
        elif code%100 == 5:
            #print("opcode 5")
            arg1 = l[i+1]
            arg2 = l[i+2]
            
            code = code // 100
            arg1 = parse_mode(l, ram, arg1, code%10)
            
            code = code // 10
            arg2 = parse_mode(l, ram, arg2, code%10)
            
            if arg1 != 0:
                i = arg2
            else:
                i += 3
        
        # jump zero (jz)
        elif code%100 == 6:
            #print("opcode 6")
            arg1 = l[i+1]
            arg2 = l[i+2]
            
            code = code // 100
            arg1 = parse_mode(l, ram, arg1, code%10)
            
            code = code // 10
            arg2 = parse_mode(l, ram, arg2, code%10)
            
            if arg1 == 0:
                i = arg2
            else:
                i += 3
        
        # eval less than
        elif code%100 == 7:
            #print("opcode 7")
            arg1 = l[i+1]
            arg2 = l[i+2]
            dest = l[i+3]
            
            code = code // 100
            arg1 = parse_mode(l, ram, arg1, code%10)
            
            code = code // 10
            arg2 = parse_mode(l, ram, arg2, code%10)
                
            code = code // 10
            #dest = parse_mode(l, ram, dest, code%10)
            
            if arg1 < arg2:
                out = 1
            else:
                out = 0
                
            if code%10 == 0:
                if dest >= len(l):
                    ram[dest] = out
                else:
                    l[dest] = out
                #print("stored",out,"in",dest)
            elif code%10 == 2:
                rel_ind = relative_base+dest
                if rel_ind >= len(l):
                    ram[rel_ind] = out
                else:
                    l[rel_ind] = out
                #print("stored",out,"in",rel_ind)
            else:
                print("opcode 7 failed")
            
            i += 4
        
        # eval equal
        elif code%100 == 8:
            #print("opcode 8")
            arg1 = l[i+1]
            arg2 = l[i+2]
            dest = l[i+3]
            
            code = code // 100
            arg1 = parse_mode(l, ram, arg1, code%10)
            
            code = code // 10
            arg2 = parse_mode(l, ram, arg2, code%10)
                
            code = code // 10
            #dest = parse_mode(l, ram, dest, code%10)
            
            if arg1 == arg2:
                out = 1
            else:
                out = 0
            
            if code%10 == 0:
                if dest >= len(l):
                    ram[dest] = out
                else:
                    l[dest] = out
                #print("stored",out,"in",dest)
            elif code%10 == 2:
                rel_ind = relative_base+dest
                if rel_ind >= len(l):
                    ram[rel_ind] = out
                else:
                    l[rel_ind] = out
                #print("stored",out,"in",rel_ind)
            else:
                print("opcode 8 failed")
            
            i += 4
            
        # part 1 (day 9)
        # offset relative base
        elif code%100 == 9:
            #print("opcode 9")
            arg1 = l[i+1]
            
            code = code // 100
            arg1 = parse_mode(l, ram, arg1, code%10)
            #print("changing relative_base from",relative_base,"to",relative_base+arg1)
            relative_base = relative_base + arg1
            
            i += 2
            
        elif code%100 == 99:
            #print("opcode 99")
            break
            
        else:
            print("something messed up!")
            break
    
    #return l[0]

if __name__ == "__main__":
    if input_mode == 'f':
        f = open('input.txt', 'r')
        intcode_master = f.read().split(',')
        f.close()
    else:
        intcode_master = input().split(',')
    
    for i in range(len(intcode_master)):
        intcode_master[i] = int(intcode_master[i])
    
    # main level edits can be made here!
    
    for i in range(depth**2):
        intcode = intcode_master[::]
        ram = {}
        parse_intcode(intcode, ram)
        
    render_area()
    # print(affected)
    
    scan_for_square()
    