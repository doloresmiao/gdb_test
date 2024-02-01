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
    display = True
    if "display" in kwargs:
        display = kwargs["display"]

    sendText = ' '.join(txt)
    prt("send:", sendText, level=3)
    gdb.sendline(sendText)
    #time.sleep(0.001)
    allText = recv(display)
    return allText

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
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
    #send("set", "disassemble-next-line", "on")
    
    send("file", ProgramName)
    
    send("b", "main")
    
    send("r ", *Arguments)
    os.system('cls' if os.name == 'nt' else 'clear')
    prev_inst = ""
    next_command = "si"
    while True:    
        allText = send(next_command)
        next_command = "si"
        endOfProgram = False
        shouldPause = False
        if "exited normally" in allText:
            endOfProgram = True
        if endOfProgram:
            break

        curr_inst = send("x/i $pc", display=False)
        allText = send("bt -frame-info location-and-address")
        allText = send("info locals")
        prt("curr_inst:", curr_inst, level=2)

        if "call" in curr_inst:
            if "<printf>" in curr_inst:
                next_command = "ni"
            p = input("command:")
            while p.strip() != "":
                send(p)
                p = input("command:")

        prev_inst = curr_inst