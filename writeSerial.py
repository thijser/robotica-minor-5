import sys
from bbio import *

Serial2.begin(9600)
for arg in sys.argv:
    Serial2.write(arg)
  
