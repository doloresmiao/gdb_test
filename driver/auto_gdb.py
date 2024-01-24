import os
import sys
import time
import gdb_ctrl
from gdb_ctrl import SyncGDBCtrl
from gdb_ctrl import GDBCtrl 
import gdb_mi
import json
import re

gdb = None

def send(*txt):
    global gdb
    while True:
        #try:
        gdb.send(' '.join(txt))
        time.sleep(0.001)
        break
        #except BlockingIOError as e:
        #    print("IO blocked. Retry...")
        #    input("enter:")
    return

def recv():
    global gdb
    recvText = ""
    allJsons = []
    while True:
        recvText = str(gdb.recv())
        recvText = recvText.replace("\"", "\\\"").replace("'", "\"").replace("None,", "\"\",")
        recvText = re.sub(r'\"[\n\r\s]+\"', '', recvText)
        #print("recv raw：", recvText)
        if "(gdb)" in recvText:
            #print("recv done:", recvText)
            break
        j = json.loads(recvText)
        #print("recv:", j)
        allJsons.append(j)
    return allJsons

if __name__ == "__main__":
    gdb = SyncGDBCtrl(force_styling=None)
    gdb.spawn(geometry=(200,80))
    send("set", "new-console", "on")
    recv()
    send("file", sys.argv[1])
    recv()
    send("b", "main")
    recv()
    send("r ", sys.argv[2])
    recv()
    while True:
        try:
            send("si")
            allJsons = recv()
            endOfProgram = False
            shouldPause = False
            for j in allJsons:
                if j["type"] == "Notify" and j["class"] == "thread-group-exited":
                    endOfProgram = True
                    break
                if j["type"] == "Exec" and j["class"] == "stopped" and j["reason"] == "end-stepping-range":
                    frame = j["frame"]
                    if "fullname" in frame:
                        print(frame["fullname"], frame["func"], frame["line"])
                    elif "func" in frame:
                        print("(none)", frame["func"], "(none)")
                    else:
                        print("(none) (none) (none)")
                    shouldPause = True
            if endOfProgram:
                break
        except gdb_mi.ParsingError as e:
            print("raw msg:", str(e).split("Original message:\n")[1])
    gdb.shutdown()