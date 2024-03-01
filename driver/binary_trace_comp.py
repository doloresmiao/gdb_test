import os
import sys
import time
import json
import re
import pexpect
import argparse
from enum import Enum
from ast import literal_eval 

skipFunctionList = ["pow", "cbrt"]

class FPType(Enum):
    ScalarSingle = 0
    ScalarDouble = 1
    PackedSingle = 2
    PackedDouble = 3
    PackedBitwise = 4

PackedBitwise = set(["pand", "pandn", "por", "pxor"])

gdb = None
count = 0
Verbose = 2
def prt(*args, **kwargs):
    # level 0 - 3: error, warning, info, low-priority info
    # default prt level: info (2)
    global Verbose
    level = 3
    if "level" in kwargs:
        level = kwargs["level"]
        kwargs.pop("level")
    if Verbose >= level:
        print(*args, **kwargs)

def recv(display):
    global gdb
    gdb.expect(r'\(gdb\)')
    text = gdb.before.decode('utf-8')
    ansi_escape = re.compile(r'\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])')
    text = ansi_escape.sub('', text)
    if display:
        prt("text:", text, level=2)
    return text

def send(*txt, **kwargs):
    global gdb
    global count
    count += 1
    display = False
    if "display" in kwargs:
        display = kwargs["display"]

    sendText = ' '.join(txt)
    prt("send:", sendText, level=3)
    gdb.sendline(sendText)
    #time.sleep(0.001)
    allText = recv(display)
    return allText

def PrintAddr(fptype, addr):
    if fptype == FPType.PackedBitwise:
        regText = send("x/4x", str(addr))
        return regText.splitlines()[-1].split(":")[1].strip().replace("\t", " ")
    elif fptype == FPType.ScalarSingle:
        regText = send("x/4f", str(addr))
        return regText.splitlines()[-1].split(":")[1].strip().split()[0]
    elif fptype == FPType.ScalarDouble:
        regText = send("x/2fg", str(addr))
        return regText.splitlines()[-1].split(":")[1].strip().split()[0]     
    elif fptype == FPType.PackedSingle:
        regText = send("x/4f", str(addr))
        return regText.splitlines()[-1].split(":")[1].strip().replace("\t", " ")
    elif fptype == FPType.PackedDouble:
        regText = send("x/2fg", str(addr))
        return regText.splitlines()[-1].split(":")[1].strip().replace("\t", " ")              

def PrintOp(traceName, curr_inst, prev_inst):
    inst = curr_inst.splitlines()[-1].split(":")[-1].strip()

    # extract instruction type
    ins_type = inst.split()[0].strip()
    if ins_type == "call":
        ins_operands = re.split("[<>]", inst)[1].strip()
        print("curr_inst:", ins_type, ins_operands, file=open(traceName + "_trace.txt", "a"))
    else:
        ins_operands = inst.split()[1].strip()
        if ins_type in PackedBitwise:
            ins_fptype = FPType.PackedBitwise
        elif ins_type.endswith("ss"):
            ins_fptype = FPType.ScalarSingle
        elif ins_type.endswith("sd"):
            ins_fptype = FPType.ScalarDouble
        elif ins_type.endswith("ps"):
            if ins_type.startswith("and") or ins_type.startswith("or") or ins_type.startswith("xor"):
                ins_fptype = FPType.PackedBitwise
            else:
                ins_fptype = FPType.PackedSingle
        elif ins_type.endswith("pd"):
            if ins_type.startswith("andp") or ins_type.startswith("orp") or ins_type.startswith("xorp"):
                ins_fptype = FPType.PackedBitwise
            else:
                ins_fptype = FPType.PackedDouble
        else:
            print("new type of instructions:" + ins_type)
        ins_operands = ins_operands.split(",")

        # extract operands
        regs = []
        for op in ins_operands:
            if op.startswith("%xmm"): #register
                regText = send("p", op.replace("%", "$"))
                allsizes = re.split("v8_bfloat16", regText)[1]
                reg = "(none)"
                if ins_fptype == FPType.ScalarSingle:
                    reg = re.split("[{}]", allsizes)[5].split(",")[0]
                elif ins_fptype == FPType.PackedSingle:
                    reg = re.split("[{}]", allsizes)[5]
                elif ins_fptype == FPType.ScalarDouble:
                    reg = re.split("[{}]", allsizes)[7].split(",")[0]
                elif ins_fptype == FPType.PackedDouble:
                    reg = re.split("[{}]", allsizes)[7]
                elif ins_fptype == FPType.PackedBitwise:
                    reg = re.split("uint128", regText)[1].replace(" = ", "").replace("}", "").strip()
            elif "(" in op: # addressing
                if "#" in curr_inst:
                    addr = inst.split("#")[-1].strip()
                    addr = literal_eval(addr)
                    reg = PrintAddr(ins_fptype, addr)
                else:
                    addPtr = re.split("[()]", op)[1].strip().replace("%", "$")
                    regText = send("p/x", addPtr)
                    reg = regText.splitlines()[-1].split("=")[1].strip()
                    addr = literal_eval(reg)
                    offset = literal_eval(re.split("[()]", op)[0].strip())
                    reg = PrintAddr(ins_fptype, addr + offset)
            regs.append(reg)
        print("curr_inst:", ins_type, ins_operands, regs, file=open(traceName + "_trace.txt", "a"))

    # register: print value according to instruction size
    # addressing: get value first, addressing, then print value according to instruction size
    return

def TestProgram(name):
    global gdb
    gdb = pexpect.spawn('gdb')
    gdb.delaybeforesend = None
    gdb.delayafterread = None
    gdb.delayafterclose = None
    gdb.delayafterterminate = None    
    recv(False)
    send("set", "pagination", "off")
    send("set", "print", "asm-demangle", "on")
    send("set", "disassemble-next-line", "on")
    
    print("running ", name)
    send("file", name)
    
    send("b", "main")
    
    send("r ", *Arguments)
    #os.system('cls' if os.name == 'nt' else 'clear')
    traceName = os.path.splitext(os.path.basename(name))[0]
    prev_inst = ""
    next_command = "si"
    while True:    
        #print("-------------------------------")
        allText = send(next_command)
        next_command = "si"
        endOfProgram = False
        shouldPause = False
        if "exited normally" in allText:
            endOfProgram = True
        if endOfProgram:
            break

        curr_inst = send("x/i $pc")
        #allText = send("bt -frame-info location-and-address")
        registerText = send("i", "r")
        print(registerText, file=open(traceName + "_reg.txt", "w"))
        #prt("curr_inst:", curr_inst, level=2)
        if "__libc_start_call_main" in curr_inst:
            break
        if "call" in curr_inst:
            if "_dl_" in curr_inst or "_IO_" in curr_inst:
                next_command = "ni"
            else:
                PrintOp(traceName, curr_inst, prev_inst)
                for func in skipFunctionList:
                    if "<" + func + ">" in curr_inst or "<" + func + "@plt>" in curr_inst:
                        next_command = "ni"
                if "<printf>" in curr_inst or "<printf@plt>" in curr_inst:
                    next_command = "ni"
                    rdiText = send("info registers rdi").strip().split()[4]
                    print("rdi:", rdiText)
                    rdiStr = send("p(char*)", rdiText).strip().split("\"")[1]
                    print("rdistr:", rdiStr)
                if args.interactive:
                    p = input("command:")
                    while p.strip() != "":
                        send(p, display=True)
                        p = input("command:")
        if ("%xmm" in curr_inst) and (not "mov" in curr_inst):
            PrintOp(traceName, curr_inst, prev_inst)

        prev_inst = curr_inst
    
    gdb.close()

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--interactive", dest='interactive', action='store_true', help='interactive mode')
    parser.add_argument("-p", "--program", type=str, help="the program to be tested", required=True)
    parser.add_argument("-a", "--args", nargs='+', help="Program arguments", required=True)
    parser.add_argument("-v", "--verbose", type=int, choices=[0, 1, 2, 3], default=2, help="set output verbosity (0=error, 1=warning, 2=info, 3=low priority info)")
    args = parser.parse_args()
    Verbose = args.verbose
    ProgramName = args.program
    Arguments = args.args
    print("program:", ProgramName, "args:", Arguments)

    TestProgram(ProgramName + "_o0")
    TestProgram(ProgramName + "_o3")
 