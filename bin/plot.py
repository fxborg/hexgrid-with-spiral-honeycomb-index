#!/usr/bin/python
import sys
import matplotlib.pyplot as plt
color=None
marker='o'
label=''
size=36
all_x=[]
all_y=[]
x=[]
y=[]
color_list=["r", "g", "b", "c", "m", "y", "k", "w"]
marker_list=["o","+","*",".","x","s","d","^","v",">","<","p","h"]

lines=[i.strip() for i in sys.stdin.readlines()]

from pprint import pprint

pprint(lines)

for line in lines:
  vals = line.split(' ');
  
  if len(vals)==2:
    if vals[0]=='marker':
       if vals[1] in marker_list:
         marker=vals[1]
    elif vals[0]=='color':
      if vals[1] in color_list:
        color=vals[1]
    else:
      v0=float(vals[0])
      v1=float(vals[1])
      x.append(v0)
      y.append(v1)
      all_x.append(v0)
      all_y.append(v1)
  elif vals[0]=='$':
    plt.scatter(x,y,s=size,c=color,marker=marker)
    x=[]
    y=[]
"""
if len(all_x)>0 :
  x1=float(min(all_x))
  x2=float(max(all_x))
  y1=float(min(all_y))
  y2=float(max(all_y))
  w=(x2-x1)*0.5
  h=(y2-y1)*0.5
  
  plt.xlim(x1-w,x2+w)
  plt.ylim(y1-h,y2+h)
"""

plt.show()


