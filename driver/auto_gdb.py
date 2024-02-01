import os
import sys
import time
import json
import re
import pexpect

gdb = None
count = 0

def recv():
    global gdb
    gdb.expect(r'\(gdb\)')
    text = gdb.before.decode('utf-8')
    ansi_escape = re.compile(r'\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])')
    text = ansi_escape.sub('', text)
    print("text:", text)
    return text

def send(*txt):
    global gdb
    global count
    count += 1
    while True:
        #try:
        gdb.sendline(' '.join(txt))
        time.sleep(0.001)
        break
        #except BlockingIOError as e:
        #    print("IO blocked. Retry...")
    allText = recv()
    return allText

if __name__ == "__main__":
    gdb = pexpect.spawn('gdb')
    gdb.delaybeforesend = None
    gdb.delayafterread = None
    gdb.delayafterclose = None
    gdb.delayafterterminate = None    
    
    send("set", "pagination", "off")
    
    send("set", "disassemble-next-line", "on")
    
    send("file", sys.argv[1])
    
    send("b", "main")
    
    send("r ", sys.argv[2])
            
    while True:    
        allText = send("si")
        endOfProgram = False
        shouldPause = False
        if "exited normally" in allText:
            endOfProgram = True
        if endOfProgram:
            break

        allText = send("x/i $pc")
        allText = send("bt -frame-info location-and-address")
        allText = send("info locals")

        input("continue...")