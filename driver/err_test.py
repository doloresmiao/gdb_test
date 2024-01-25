import sys
import os
import pexpect
import asyncio

c = pexpect.spawn('gdb')
c.delaybeforesend = None
c.delayafterread = None
c.delayafterclose = None
c.delayafterterminate = None

c.expect(r'\(gdb\)')
print("before:", c.before.decode('utf-8'))
count = 0
c.sendline('r')
while True:
    print("count:", count)
    c.expect(r'\(gdb\) ')
    print("before:", c.before.decode('utf-8')) 
    c.sendline('r')            
    count += 1