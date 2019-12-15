# 'f'               - reading from "input.txt"
# 'r' (or not 'f')  - reading from raw input
input_mode = 'f'

# in loop:
# accept movement command via input
# send movement command to drone
# wait for it to finish
# output report of status

# movement commands:
#     n=1 s=2 w=3 e=4 (nesw = 1423)
#     always the same direction

# statuses:
#     0: hit wall, no change in pos
#     1: success!
#     2: ultra success (system reached)!

# part one
from sys import exit
from math import floor, ceil

space = {(0,0): "D"}    #dict(tuple pos, char)
weights = {(0,0): 0}    #level of wanting to not revisit
visited = {(0,0)}       #set(tuple pos)
x_pos = 0
y_pos = 0
last_dir = 0
ox_gen = -1
o_ygen = -1

dirs = {"N":1, "S":2, "W":3, "E":4}

def in_dir():
    x = x_pos
    y = y_pos
    if last_dir == dirs["N"]:
        y += 1
    elif last_dir == dirs["S"]:
        y -= 1
    elif last_dir == dirs["W"]:
        x -= 1
    elif last_dir == dirs["E"]:
        x += 1
    else:
        print("error with last_dir")
    return (x, y)
    
def get_dir(pos):
    dx = pos[0] - x_pos
    dy = pos[1] - y_pos
    if dy == 1: return dirs["N"]
    if dy == -1: return dirs["S"]
    if dx == -1: return dirs["W"]
    if dx == 1: return dirs["E"]
    print("get_dir failed\n  pos =", pos, "\n  x_pos y_pos =", x_pos, y_pos)
    exit()
            
def draw_local():
    width = 5
    line = "-"*width
    print(line)
    for i in range(y_pos-floor(width/2), y_pos+ceil(width/2)):
        line = ""
        for j in range(x_pos-floor(width/2), x_pos+ceil(width/2)):
            try:
                line += space[(j,i)]
            except KeyError:
                line += ' '
        print(line)

def draw_full():
    min_x = 99
    max_x = -99
    min_y = 99
    max_y = -99
    for key in space.keys():
        x, y = key[0], key[1]
        if x < min_x: min_x = x
        if x > max_x: max_x = x
        if y < min_y: min_y = y
        if y > max_y: max_y = y
    
    print("-----")
    for i in range(min_y, max_y+1):
        line = ""
        for j in range(min_x, max_x+1):
            if (j,i) == (0,0):
                line += "0"
            elif (j,i) == (ox_gen, o_ygen):
                line += "*"
            else:
                try:
                    line += space[(j,i)]
                except KeyError:
                    line += ' '
        print(line)

import queue
def bfs(start, end):
    fifo = queue.Queue()
    deltas = {  (0+1, 0),(0, 0+1),\
                (0-1, 0),(0, 0-1)}
    fifo.put(start)
    cur = start
    bfs_visited = {start}
    depths = {start: 0}
    while cur != end:
        #part two
        if fifo.empty():
            print(end,"not found (oxygen propagated)")
            break
        cur = fifo.get()
        bfs_visited.add(cur)
        for (dx, dy) in deltas:
            child = (cur[0]+dx, cur[1]+dy)
            if space[child] != '#' and child not in bfs_visited:
                fifo.put(child)
                depths[child] = depths[cur] + 1
        
    return depths[cur]

def output_func(arg):
    global space, weights, visited
    loc = in_dir()
    visited.add(loc)
    if arg == 0:
        space[loc] = "#"
        weights[loc] = -9999
    elif arg == 1 or arg == 2:
        global x_pos, y_pos
        space[(x_pos, y_pos)] = "."
        try: 
            weights[(x_pos, y_pos)] -= 1 
        except KeyError:
            weights[(x_pos, y_pos)] = -1
        space[loc] = "D"
        x_pos, y_pos = loc[0], loc[1]
    else:
        print("error with output value")
    
    # draw_local()
    
    if arg == 2:
        global ox_gen, o_ygen
        # print("oxygen found at ("+str(x_pos)+", "+str(y_pos)+")")
        ox_gen = x_pos
        o_ygen = y_pos
        # draw_full()
        # exit()
    
    
    return

def input_func():
    global last_dir
    neighbours = {  (x_pos+1, y_pos),(x_pos, y_pos+1),\
                    (x_pos-1, y_pos),(x_pos, y_pos-1)}
    for loc in neighbours:
        if loc not in visited:
            last_dir = get_dir(loc)
            return last_dir
            
    # if all attempted:
    max_weight = -15
    max_dir = -1
    for loc in neighbours:
        weight = weights[loc]
        if weight > max_weight:
            max_weight = weight
            max_dir = get_dir(loc)
    
    last_dir = max_dir
    return max_dir

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
            print("opcode 99")
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
    intcode = intcode_master[::]
    ram = {}
    parse_intcode(intcode, ram)
    
    # moved from inside inp/out functions
    draw_full()
    print(bfs((0,0),(ox_gen,o_ygen)))
    
    # part two
    print(bfs((ox_gen, o_ygen),(-1000,-1000)))