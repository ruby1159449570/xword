#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
#/Users/moonpie/xword/script.py Mon07Sep2020 {fcG}

from datetime import datetime
import math
import random
#from matplotlib import pyplot as plt

current_time = datetime.now()

print(current_time)
r1 = random.randint(0, 100)  
print(r1) # Random integer where 0 <= r

pi = 4.0 * math.atan(1.0)
print(pi)

print(dir(math))
print(dir(datetime))
print(dir(random))
#print(dir(matplotlib))
