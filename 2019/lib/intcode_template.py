# 'f'               - reading from "input.txt"
# 'r' (or not 'f')  - reading from raw input
input_mode = 'r'
relative_base = 0

def output_func(arg):
    # insert code here!
    return

def input_func():
    # insert code here!
    return

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
    