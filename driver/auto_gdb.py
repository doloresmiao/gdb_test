import os
import sys
import time
from gdb_ctrl import SyncGDBCtrl
from gdb_ctrl import GDBCtrl 

if __name__ == "__main__":
    gdb = SyncGDBCtrl(force_styling=None)
    gdb.spawn()
    gdb.shutdown()