# no idea how to take multiple inputs if i'm passing a file as input
# probably should've made the main input a string, and have opcode 3 call input()

# part one (day 2 and 5)
def parse_intcode(l):
    i = 0;
    while (i < len(l)):
        print(i,l[i])
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
            
        elif code%100 == 3:
            print("opcode 3")
            dest = l[i+1]
            inp = 5 #int(input())
            l[dest] = inp
            i += 2
            
        elif code%100 == 4:
            arg1 = l[i+1]
            code = code // 100
            if code%10 == 0:
                arg1 = l[arg1]
            print("opcode 4:\t", arg1)
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
            
    return l[0]
    
if __name__ == "__main__":
    intcode_master = input().split(',')
    
    # part one (day 2)
    intcode = intcode_master[::]
    #print(intcode)
    for i in range(len(intcode)):
        intcode[i] = int(intcode[i])
    #print(intcode)
    parse_intcode(intcode)
            