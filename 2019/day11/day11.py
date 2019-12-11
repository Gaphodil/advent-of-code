# I SHOULD HAVE KNOWN (this actually didn't edit the intcode any, i suppose)

# all panels are currently black
# input accesses camera, which gives 0 if black, 1 if white
# output:  0/1 to PAINT current panel, then 0/1 to rotate left/right 90deg
#   after turn: immediately move forward
# robot starts facing up

# constantly mi_ed up (_,y) coords being list[y][_]
# apparently visualization is very unfeasible - if starting at center, size > 100 by 100

from enum import Enum
from sys import exit #my x key is broken now, though the rest are workable
# my k key being finicky also cost me a minute at the end,
#   since that was part of my answer

relative_base = 0

# part one (day 11) 
hull_size = 50
hull_center = 4#hull_size//2       #secretly the robot starts at top left for part two
hull_line = ['#']*hull_size # change for part one/two
robot_direction = 0 #up right down left, 0123

hull = []
for i in range(hull_size//2):
    hull.append(hull_line[::])
robot = [hull_center, hull_center]
panels_painted = {}

output_step = 0

# for i in range(len(hull)):
#     line = hull[i][::]
#     line[i] = '@'
#     print(line)

def get_input():
    #global robot
    #global hull
    cur_tile = hull[robot[1]][robot[0]]
    if cur_tile == '.':
        print("cur_tile",cur_tile,"is black")
        return 0
    elif cur_tile == '#':
        print("cur_tile",cur_tile,"is white")
        return 1
    print("bad tile: "+cur_tile)
    exit(-1)
    

def render_output(arg):
    global output_step
    global robot
    global hull
    global panels_painted
    global robot_direction
    if output_step == 0:
        to_paint = '@'
        # get paint
        if arg == 0:
            to_paint = '.'
        elif arg == 1:
            to_paint = '#'
        else:
            print("bad colour output")
            exit(-1)
        
        # do paint
        hull[robot[1]][robot[0]] = to_paint
        panels_painted[robot[0]*100+robot[1]] = 1
        
        print("painted",to_paint,"at:", robot)
        # for line in hull:
        #     print(line)
        # print()
        
        output_step = 1
        
    elif output_step == 1:
        # turn
        if arg == 0:
            robot_direction = (robot_direction-1)%4
        elif arg == 1:
            robot_direction = (robot_direction+1)%4
        else:
            print("bad direction output")
            exit(-1)
            
        # move
        hull2 = []
        for line in hull:
            hull2.append(line[::])
        if robot_direction == 0:
            robot = [robot[0], robot[1]-1]
            hull2[robot[1]][robot[0]] = '^'
        elif robot_direction == 1:
            robot = [robot[0]+1, robot[1]]
            hull2[robot[1]][robot[0]] = '>'
        elif robot_direction == 2:
            robot = [robot[0], robot[1]+1]
            hull2[robot[1]][robot[0]] = 'v'
        elif robot_direction == 3:
            robot = [robot[0]-1, robot[1]]
            hull2[robot[1]][robot[0]] = '<'
        else:
            print("bad direction stored")
            exit(-1)
        
        print("turned and moved:", robot)
        # for line in hull2:
        #     print(line)
        # print()
        
        output_step = 0
        
    else:
        print("render_output bugged")
        exit(-1)
    
    return

# extracted for day 9
# important info missed: memory beyond the initial program starts at 0
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
                print("ram[",arg,"] doesnt exist; returning 0")
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
                print("ram[",rel_ind,"] doesnt exist; returning 0")
                return 0
        return l[rel_ind]
    print("parse_mode failed")
    return -9999

# part one (day 2 and 5)
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
            
            inp = get_input()#2 #int(input())
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
            
            print("opcode 4:\t", arg1)
            render_output(arg1)
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
            print("changing relative_base from",relative_base,"to",relative_base+arg1)
            relative_base = relative_base + arg1
            
            i += 2
            
        elif code%100 == 99:
            print("opcode 99")
            break
            
        else:
            print("something messed up!")
            break
            
    return l[0]
    
if __name__ == "__main__":
    intcode_master = input().split(',')
    for i in range(len(intcode_master)):
        intcode_master[i] = int(intcode_master[i])
    
    intcode = intcode_master[::]
    print(intcode)
    
    # part one (day 9)
    ram = {}
    
    parse_intcode(intcode, ram)
    
    # part one (day 11)
    print(panels_painted.keys(),"\n",len(panels_painted.keys()))
    # part two (day 11)
    for line in hull:
        string = ""
        for letter in line:
            string += letter
        print(string)
            