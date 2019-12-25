# 'f'               - reading from "input.txt"
# 'r' (or not 'f')  - reading from raw input
input_mode = 'f'
relative_base = 0

# output values are ASCII (chr(output)) text
# 35 = #, 46 = ., 10 = \n
# input is ASCII text commands (ord(input)):
#     north, south, east, west
#     take itemname
#     drop itemname
#     inv
# all ended with \n

# part one
from itertools import combinations
out_stream = []
in_stream = []
in_iterator = 0

# after testing:
dangerous = ["infinite loop", "photons", \
    "giant electromagnet", "molten lava", "escape pod"]
goal = "Security Checkpoint"
# requires a specific combination of items to proceed (west)
poss_dir = "Doors here lead:\n"
poss_items = "Items here:\n"
want_input = "Command?\n"
look_inv = "Items in your inventory:\n"
# following lines begin with "- ", items = str[2:]
last_out = ""

# or i could just do it all manually
items = ["candy cane","wreath","hypercube","food ration","weather machine",\
    "space law space brochure","prime number","astrolabe"]
# all but [candy cane, food ration] is too light - both required
# dropping two of other items - still too heavy
# whatever just do the speedrun to door and auto from there
walkthrough = """north
take wreath
east
east
east
take weather machine
west
west
west
south
south
south
take candy cane
north
west
take prime number
west
take astrolabe
east
east
north
east
take food ration
south
east
south
take hypercube
east
take space law space brochure
north
"""
in_stream = list(walkthrough)

dropped = []
to_drop = 3 # manually tried the previous 012
to_try_all = []
def solve_puzzle():
    global dropped
    global to_drop
    global to_try_all
    global in_stream
    
    steps = ""
    while len(dropped) != 0:
        steps += "take " + dropped.pop() + '\n'
    
    if len(to_try_all) == 0:
        to_try_all = list(combinations(items, to_drop))
        to_drop += 1
        print(to_try_all)
        
    to_try = to_try_all.pop()
    for item in to_try:
        steps += "drop " + item + '\n'
        dropped.append(item)
    
    steps += "west\n"
    print(steps)
    in_stream = list(steps)

#answer: 2415919488

def output_func(arg):
    global out_stream
    global last_out
    charg = chr(arg)
    out_stream.append(charg)
    if charg == '\n':
        last_out = ''.join(out_stream)
        print(last_out)
        if goal in last_out:
            solve_puzzle()
        out_stream = []
    # print(out_stream)
    return

def input_func():
    global in_stream
    global in_iterator
    if len(in_stream) == 0:
        in_stream = list(input())
        in_stream.append('\n')
    
    arg = in_stream[in_iterator]
    if in_iterator == len(in_stream)-1:
        in_stream = []
        in_iterator = 0
    else:
        in_iterator += 1 
    return ord(arg)

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
    