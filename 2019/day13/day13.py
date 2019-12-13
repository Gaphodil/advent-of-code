# still need to make this properly

relative_base = 0

# day 13:
# output is provided in triplicate: x (key still broken) y tileid
# x from left, y from top, tileid = is:
tileids =  {0: ' ',\
            1: '|',\
            2: 'x',\
            3: '-',\
            4: '*'}
# 1 indestructible, 2 breakable, 3 same as 1 but horizontal, 
# 4 diagonally-moving bouncy ball

x = 0
y = 0
tileid = 0
output_num = 0

game_map = {}

total_x = set()
total_y = set()

paddle = 0
drawing = []
# moved for part two
def render_output():
    global total_x
    global total_y
    global game_map
    global tileids
    global paddle
    
    x_dim = max(total_x)+1
    y_dim = max(total_y)+1
    
    line = [' ']*(x_dim)
    global drawing
    drawing = []
    for i in range(y_dim):
        drawing.append(line[::])
    
    # block_count = 0
    
    for key in game_map.keys():
        x = key // 100
        y = key %  100
        tile = game_map[key]
        assist_tiles = {'>','!','<'}
        if drawing[y][x] not in assist_tiles :
            drawing[y][x] = tileids[tile]
        # if tile == 2:
            # block_count += 1
        # assist mode!!
        if tile == 4:
            drawing[y_dim-1][x-1] = '>'
            drawing[y_dim-1][x]   = '!'
            drawing[y_dim-1][x+1] = '<'
        # "ai" tracking info!!!
        if tile == 3:
            paddle = (y,x)
    
    for line in drawing:
        print(''.join(line))
    return

# part two
def get_input():
    render_output()
    # asd = input()
    # if asd == 'a':
    #     return -1
    # if asd == 'd':
    #     return 1
        
    # FINE i'll write a simple ai, gave it a few honest attempts but wowza
    # 'but i don't wanna do it the "correct" way'
    global paddle
    global drawing
    beneath = drawing[paddle[0]+1][paddle[1]]
    if beneath == '<':
        return -1
    if beneath == '>':
        return 1
    
    return 0
    
def set_tile(arg):
    global output_num
    global x
    global y
    global total_x
    global total_y
    global tileid
    
    if output_num == 0:
        x = arg
        if x != -1 and x not in total_x:
            total_x.add(x)
        output_num += 1
    elif output_num == 1:
        y = arg
        if y not in total_y:
            total_y.add(y)
        output_num += 1
    elif output_num == 2:
        output_num = 0
        if x == -1 and y == 0:
            print(arg)
            return
        tileid = arg
        global game_map
        game_map[100*x + y] = tileid
    
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
            set_tile(arg1)
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
    f = open('input.txt', 'r')
    intcode_master = f.read().split(',')
    f.close()
    
    for i in range(len(intcode_master)):
        intcode_master[i] = int(intcode_master[i])
    
    intcode = intcode_master[::]
    print(intcode)
    
    # part one (day 9)
    ram = {}
    
    # part two (day 13)
    intcode[0] = 2
    
    parse_intcode(intcode, ram)
    
    # part one (day 13) moved

    # print(block_count)