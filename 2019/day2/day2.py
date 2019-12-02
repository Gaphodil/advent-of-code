# opcodes:
# 1 -> pos1 pos2 out -> pos1 + pos2
# 2 -> pos1 pos2 out -> pos1 * pos2
# 99 -> end
# syntax: opcode par1 par2 par3, opcode par1 par2 par3, opcode...

# part one
def parse_intcode(l):
    for i in range(0,len(l),4):
        #print(i)
        if l[i] == 1:
            l[l[i+3]] = l[l[i+2]] + l[l[i+1]]
        elif l[i] == 2:
            l[l[i+3]] = l[l[i+2]] * l[l[i+1]]
        elif l[i] == 99:
            break
        else:
            print("something messed up!")
    return l[0]
    
def program_alarm(l, noun, verb):
    l[1] = noun #part one: 12
    l[2] = verb #part one: 2
    return

if __name__ == "__main__":
    intcode_master = input().split(',')
    
    # part one
    """
    intcode = intcode_master[::]
    #print(intcode)
    for i in range(len(intcode)):
        intcode[i] = int(intcode[i])
    #print(intcode)
    program_alarm(intcode)
    #print(intcode)
    parse_intcode(intcode)
    print(intcode[0])
    """
    
    for noun in range(100):
        for verb in range(100):
            intcode = intcode_master[::]
            for i in range(len(intcode)):
                intcode[i] = int(intcode[i])
            program_alarm(intcode, noun, verb)
            out = parse_intcode(intcode)
            print(noun,verb,out)
            if out == 19690720:
                print("!!!!!!!!!")
                break
        if out == 19690720:
            break