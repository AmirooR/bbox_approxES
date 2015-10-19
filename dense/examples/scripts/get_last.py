#! /usr/bin/env python

import sys, os

in_dir = sys.argv[1]
a = os.listdir(in_dir)
last_pr = -1
total = -1
last_idx = -1
for x in a:
    if x.startswith('best'):
        pass;#os.remove(in_dir+x)
    elif x.endswith('_output.txt'):
#        print x
        idx = int(x.split('_')[0])
        if idx > last_idx:
            last_idx = idx


f = open(in_dir+str(last_idx)+'_output.txt')
b = f.read().split('/')
pr = int(b[0])
total = float(b[1])
f.close()
print pr/total#str(best_pr) +  '/' + str(total)
