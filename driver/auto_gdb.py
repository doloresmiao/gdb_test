import os
import sys
import time
import json
import re
import pexpect

gdb = None
count = 0
def send(*txt):
    global gdb
    global count
    print("send ", count)
    count += 1
    while True:
        #try:
        gdb.sendline(' '.join(txt))
        time.sleep(0.001)
        break
        #except BlockingIOError as e:
        #    print("IO blocked. Retry...")
    return

def recv():
    global gdb
    gdb.expect(r'\(gdb\)')
    text = gdb.before.decode('utf-8')
    ansi_escape = re.compile(r'\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])')
    text = ansi_escape.sub('', text)
    print("text:", text)
    return text

if __name__ == "__main__":
    gdb = pexpect.spawn('gdb')
    gdb.delaybeforesend = None
    gdb.delayafterread = None
    gdb.delayafterclose = None
    gdb.delayafterterminate = None    
    recv()
    send("set", "new-console", "on")
    recv()
    send("file", sys.argv[1])
    recv()
    send("b", "main")
    recv()
    send("r ", sys.argv[2])
    recv()        
    while True:    
        send("s")
        allText = recv()
        endOfProgram = False
        shouldPause = False
        if "exited normally" in allText:
            endOfProgram = True
        if endOfProgram:
            break