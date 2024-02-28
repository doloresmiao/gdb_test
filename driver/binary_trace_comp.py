import os
import sys
import time
import json
import re
import pexpect
import argparse

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

def TestProgram(name):
    send("file", name)
    
    send("b", "main")
    
    send("r ", *Arguments)
    os.system('cls' if os.name == 'nt' else 'clear')
    prev_inst = ""
    next_command = "si"
    while True:    
        print("-------------------------------")
        allText = send(next_command, display=True)
        next_command = "si"
        endOfProgram = False
        shouldPause = False
        if "exited normally" in allText:
            endOfProgram = True
        if endOfProgram:
            break

        curr_inst = send("x/i $pc")
        allText = send("bt -frame-info location-and-address")
        allText = send("info locals", display=True)
        prt("curr_inst:", curr_inst, level=2)
        if "xmm" in curr_inst:
            traceName = os.path.splitext(os.path.basename(name))[0]
            print("curr_inst:", curr_inst.splitlines()[-1], file=open(traceName + "_trace.txt", "a"))

        if "call" in curr_inst:
            if "_dl_" in curr_inst:
                next_command = "ni"
            else:
                if "<printf>" in curr_inst or "<printf@plt>" in curr_inst:
                    next_command = "ni"
                    rdiText = send("info registers rdi").strip().split()[4]
                    print("rdi:", rdiText)
                    rdiStr = send("p(char*)", rdiText).strip().split("\"")[1]
                    print("rdistr:", rdiStr)
                if args.interactive:
                    p = input("command:")
                    while p.strip() != "":
                        send(p)
                        p = input("command:")

        prev_inst = curr_inst

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

    gdb = pexpect.spawn('gdb')
    gdb.delaybeforesend = None
    gdb.delayafterread = None
    gdb.delayafterclose = None
    gdb.delayafterterminate = None    
    recv(True)
    send("set", "pagination", "off")
    send("set", "print", "asm-demangle", "on")
    send("set", "disassemble-next-line", "on")

    TestProgram(ProgramName + "_o0")
    gdb.close()

    gdb = pexpect.spawn('gdb')
    gdb.delaybeforesend = None
    gdb.delayafterread = None
    gdb.delayafterclose = None
    gdb.delayafterterminate = None    
    recv(True)
    send("set", "pagination", "off")
    send("set", "print", "asm-demangle", "on")
    send("set", "disassemble-next-line", "on")
    TestProgram(ProgramName + "_o3")
    gdb.close()