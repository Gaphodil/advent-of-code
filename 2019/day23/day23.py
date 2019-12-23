# 'f'               - reading from "input.txt"
# 'r' (or not 'f')  - reading from raw input
input_mode = 'f'
# relative_base = 0
relative_bases = [0]*50

# starting 7 minutes late
# rebuild a network
# 50 computers sending packets to each other, each running the input
# (sounds like I can only do part one again)
# have network addresses 0-49, requests its own address w/ single input
# once received, sending and receiving packets (X and Y)
# packets are queued by the recipient and read in order
# format: (dest, X, Y)
# input: if empty, give -1; else, provide X, then Y
# no blocking!! ie. outputs do not wait for packet to be received
# part one:
#   what is Y in FIRST packet sent to 255

# so they're running in unison, then?
# idea: store 50 copies, run until input instruction, save pos AND timestamp
# timestamp and address id requires rewriting parse functions
# outputs also have timestamp attached (read with input to tell if queued or not)
# HOW DID THAT WORK IN ONE TRY 

# part two:
# 255 handled by NAT managing power consumption 
# NAT only tackles the MOST RECENT packet (ignores every previous)
# if the packets list is empty and all computers are getting -1 from input,
#   the network is idle
# if the network is idle, NAT sends most recent NAT packet to address 0,
#   to resume activity
# monitor packets delivered to address 0 - what is first Y sent twice in a row?

# NAT goes in main function (is not a computer!)

# part one
indexes = [0]*50
timestamps = [0]*50
packets = []
out_packet = []
in_packet = []
first_loops = [0]*50
do_block = [1]*50

# part two
NAT_packet = []
NAT_packet_dummy = []
using_dummy = 0
consecutive_idles = [0]*50

def output_func(timestamp, arg):
    # part two
    global NAT_packet
    NAT_len = len(NAT_packet)
    if ((NAT_len > 0) and (NAT_len < 4)):
        NAT_packet.append(arg)
        if NAT_len == 2:
            NAT_packet.append(timestamp)
        return
    
    global NAT_packet_dummy 
    global out_packet
    p_len = len(out_packet)
    if ((p_len == 0) and (arg == 255)):
        if NAT_len != 0:
            if NAT_packet[3] > timestamp:
                NAT_packet_dummy = []
                NAT_packet_dummy.append(arg)
                return
        NAT_packet = [] # oops forgot this very important line
        NAT_packet.append(arg)
        return
    
    global using_dummy
    if using_dummy == 1:
        NAT_packet_dummy.append(arg)
        if len(NAT_packet_dummy) == 3:
            using_dummy = 0
        return
    
    # part one
    global packets
    out_packet.append(arg)
    if p_len == 2:
        out_packet.append(timestamp)
        packets.append(out_packet[::])
        out_packet = []
    
    return

def input_func(timestamp, comp_id):
    global first_loops
    if first_loops[comp_id] == 0:
        first_loops[comp_id] += 1
        return comp_id
    
    global packets  # to pop if completed
    global in_packet # addr, X, Y, timestamp
    global do_block
    p_len = len(in_packet)
    if p_len != 0:
        inp = in_packet[2]
        in_packet = []
        return inp
    for i in range(len(packets)):
        if ((packets[i][0] == comp_id) and (packets[i][3] < timestamp)):
            in_packet = packets.pop(i)
            return in_packet[1]
    if do_block[comp_id] == 1:
        do_block[comp_id] = 0
        return -99999 #pause run
    do_block[comp_id] = 1
    #part two
    global consecutive_idles
    consecutive_idles[comp_id] += 1
    return -1 

def parse_mode(comp_id, l, ram, arg, mode):
    global relative_bases
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
        rel_ind = relative_bases[comp_id]+arg
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

def parse_intcode(comp_id, l, ram):
    global relative_bases
    # part one, day 23
    global indexes
    global timestamps
    i = indexes[comp_id]
    time = timestamps[comp_id];
    if i == -1:
        return
    
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
            arg1 = parse_mode(comp_id, l, ram, arg1, code%10)
            
            code = code // 10
            arg2 = parse_mode(comp_id, l, ram, arg2, code%10)
                
            code = code // 10
            #dest = parse_mode(comp_id, l, ram, dest, code%10)
            
            out = arg2 + arg1
            
            if code%10 == 0:
                if dest >= len(l):
                    ram[dest] = out
                else:
                    l[dest] = out
                #print("stored",out,"in",dest)
            elif code%10 == 2:
                rel_ind = relative_bases[comp_id]+dest
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
            arg1 = parse_mode(comp_id, l, ram, arg1, code%10)
            
            code = code // 10
            arg2 = parse_mode(comp_id, l, ram, arg2, code%10)
            
            out = arg2 * arg1
                
            code = code // 10
            #dest = parse_mode(comp_id, l, ram, dest, code%10)
            if code%10 == 0:
                if dest >= len(l):
                    ram[dest] = out
                else:
                    l[dest] = out
                #print("stored",out,"in",dest)
            elif code%10 == 2:
                rel_ind = relative_bases[comp_id]+dest
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
            #dest = parse_mode(comp_id, l, ram, dest, code%10)
            
            # part one, day 23
            inp = input_func(time, comp_id)
            if inp == -99999: #please don't be part of the program
                timestamps[comp_id] = time
                indexes[comp_id] = i
                break
            
            out = inp
            if code%10 == 0:
                if dest >= len(l):
                    ram[dest] = out
                else:
                    l[dest] = out
                #print("stored",out,"in",dest)
            elif code%10 == 2:
                rel_ind = relative_bases[comp_id]+dest
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
            arg1 = parse_mode(comp_id, l, ram, arg1, code%10)
            
            #print("opcode 4:\t", arg1)
            output_func(time, arg1)
            i += 2
            
        #part two (day 5)
        # jump nonzero (jnz)
        elif code%100 == 5:
            #print("opcode 5")
            arg1 = l[i+1]
            arg2 = l[i+2]
            
            code = code // 100
            arg1 = parse_mode(comp_id, l, ram, arg1, code%10)
            
            code = code // 10
            arg2 = parse_mode(comp_id, l, ram, arg2, code%10)
            
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
            arg1 = parse_mode(comp_id, l, ram, arg1, code%10)
            
            code = code // 10
            arg2 = parse_mode(comp_id, l, ram, arg2, code%10)
            
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
            arg1 = parse_mode(comp_id, l, ram, arg1, code%10)
            
            code = code // 10
            arg2 = parse_mode(comp_id, l, ram, arg2, code%10)
                
            code = code // 10
            #dest = parse_mode(comp_id, l, ram, dest, code%10)
            
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
                rel_ind = relative_bases[comp_id]+dest
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
            arg1 = parse_mode(comp_id, l, ram, arg1, code%10)
            
            code = code // 10
            arg2 = parse_mode(comp_id, l, ram, arg2, code%10)
                
            code = code // 10
            #dest = parse_mode(comp_id, l, ram, dest, code%10)
            
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
                rel_ind = relative_bases[comp_id]+dest
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
            arg1 = parse_mode(comp_id, l, ram, arg1, code%10)
            #print("changing relative_base from",relative_base,"to",relative_base+arg1)
            relative_bases[comp_id] = relative_bases[comp_id] + arg1
            
            i += 2
            
        elif code%100 == 99:
            # part one, day 23
            indexes[comp_id] = -1
            print("opcode 99 for computer", comp_id)
            break
            
        else:
            print("something messed up!")
            break
            
        time += 1
    
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
    
    intcodes = []
    rams = [] # if the program breaks probably need to pass all of rams and get comp_id
    for i in range(50):
        intcodes.append(intcode_master[::])
        rams.append({})
        parse_intcode(i, intcodes[i], rams[i])
    
    last_y = -1
    while True:
        if max(indexes) == -1:
            break
        # for packet in packets:
        #     if packet[0] == 255:
        #         print(packet)
        # print("")
        
        # part two: NAT
        if min(consecutive_idles) > 2:
            if len(packets) == 0:
                # print(NAT_packet)
                consecutive_idles = [0]*50
                this_y = NAT_packet[2]
                
                print("y:", this_y)
                if last_y == this_y:
                    break
                last_y = this_y
                
                packets.append([0, NAT_packet[1], this_y, 0])
                NAT_packet = []
                # print(packets)
                parse_intcode(0, intcodes[0], rams[0])
                # print(packets)
                continue
            # else:
            #     print(packets, NAT_packet)
            #     if packets[0][0] == 51059: # debugged
            #         break
        
        min_timestamp = 9999999
        arg_min = -1
        for i in range(50):
            if ((indexes[i] > -1) and (timestamps[i] < min_timestamp)):
                min_timestamp = timestamps[i]
                arg_min = i
        if arg_min == -1:
            print("timesteps > 10e6")
            break
        
        if min_timestamp%10000 == 0:
            # print(min_timestamp)
        parse_intcode(arg_min, intcodes[arg_min], rams[arg_min])
        