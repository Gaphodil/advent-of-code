# no idea how to take multiple inputs if i'm passing a file as input
# probably should've made the main input a string, and have opcode 3 call input()

# part one (day 7)
from itertools import permutations
global_output = -1

# part one (day 2 and 5 and 7)
# (day 7) intup is a tuple for the input
# i is the starting index for resuming an amplifier
def parse_intcode(l, intup, i=0):
    #i = 0
    inp_x = 0
    while (i < len(l)):
        #print(i,l[i])
        code = l[i]
        
        if code%100 == 1:
            print("opcode 1")
            arg1 = l[i+1]
            arg2 = l[i+2]
            dest = l[i+3]
            
            code = code // 100
            if code%10 == 0:
                arg1 = l[arg1]
            
            code = code // 10
            if code%10 == 0:
                arg2 = l[arg2]
            
            l[dest] = arg2 + arg1
            i += 4
            
        elif code%100 == 2:
            print("opcode 2")
            arg1 = l[i+1]
            arg2 = l[i+2]
            dest = l[i+3]
            
            code = code // 100
            if code%10 == 0:
                arg1 = l[arg1]
            
            code = code // 10
            if code%10 == 0:
                arg2 = l[arg2]
            
            l[dest] = arg2 * arg1
            i += 4
            
        # some part one (day 7)
        elif code%100 == 3:
            print("opcode 3")
            dest = l[i+1]
            # part two (day 7)
            if inp_x >= len(intup):
                print("held on input")
                return i
            inp = intup[inp_x]#5 #int(input())
            l[dest] = inp
            print("input was:",inp)
            inp_x += 1
            i += 2
            
        elif code%100 == 4:
            arg1 = l[i+1]
            code = code // 100
            if code%10 == 0:
                arg1 = l[arg1]
            print("opcode 4")#:\t", arg1)
            global global_output
            global_output = arg1
            print(arg1,"moved into global_output")
            i += 2
            
        #part two (day 5)
        elif code%100 == 5:
            print("opcode 5")
            arg1 = l[i+1]
            arg2 = l[i+2]
            
            code = code // 100
            if code%10 == 0:
                arg1 = l[arg1]
            
            code = code // 10
            if code%10 == 0:
                arg2 = l[arg2]
            
            if arg1 != 0:
                i = arg2
            else:
                i += 3
                
        elif code%100 == 6:
            print("opcode 6")
            arg1 = l[i+1]
            arg2 = l[i+2]
            
            code = code // 100
            if code%10 == 0:
                arg1 = l[arg1]
            
            code = code // 10
            if code%10 == 0:
                arg2 = l[arg2]
            
            if arg1 == 0:
                i = arg2
            else:
                i += 3
                
        elif code%100 == 7:
            print("opcode 7")
            arg1 = l[i+1]
            arg2 = l[i+2]
            dest = l[i+3]
            
            code = code // 100
            if code%10 == 0:
                arg1 = l[arg1]
            
            code = code // 10
            if code%10 == 0:
                arg2 = l[arg2]
                
            if arg1 < arg2:
                l[dest] = 1
            else:
                l[dest] = 0
            
            i += 4
            
        elif code%100 == 8:
            print("opcode 8")
            arg1 = l[i+1]
            arg2 = l[i+2]
            dest = l[i+3]
            
            code = code // 100
            if code%10 == 0:
                arg1 = l[arg1]
            
            code = code // 10
            if code%10 == 0:
                arg2 = l[arg2]
            
            if arg1 == arg2:
                l[dest] = 1
            else:
                l[dest] = 0
             
            i += 4
            
        elif code%100 == 99:
            print("opcode 99")
            break
            
        else:
            print("something messed up!")
            break
            
    return -1#l[0]
    
if __name__ == "__main__":
    intcode_master = input().split(',')
    # wow i should've done this one first huh!! (day 7)
    for i in range(len(intcode_master)):
        intcode_master[i] = int(intcode_master[i])
                
    # part one (day 2)
    #intcode = intcode_master[::]
    #print(intcode)
    #for i in range(len(intcode)):
    #    intcode[i] = int(intcode[i])
    #print(intcode)
    #parse_intcode(intcode)
    
    # part one (day 7)
    perms = permutations(range(5))
    #print(perms)
    max_out = -9999999
    max_tup = 0
    
    #part two (day 7)
    perms2 = permutations(range(5,10))
    for tup in perms2:
        print("TRYING",tup)
        intermedi_out = 0
        # part one
        """for i in range(5):
            inp = (tup[i], intermedi_out) 
            
            intcode = intcode_master[::]
            for i in range(len(intcode)):
                intcode[i] = int(intcode[i])
            #print(intcode)
            
            parse_intcode(intcode, inp)
            intermedi_out = global_output
        """
        # part two
        intcodes = []
        # loop one
        amp_index = [0,0,0,0,0]
        for i in range(5):
            intcodes.append(intcode_master[::])
            amp_index[i] = parse_intcode(intcodes[i], (tup[i], intermedi_out))
            intermedi_out = global_output
        while True:
            for i in range(5):
                amp_index[i] = parse_intcode(intcodes[i], \
                    (intermedi_out,), amp_index[i])
                intermedi_out = global_output
            if amp_index[4] == -1:
                break
        
        if intermedi_out > max_out:
            max_out = intermedi_out
            max_tup = tup
    print(max_out, max_tup)
        