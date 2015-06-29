#! /usr/bin/env python

import sys, os

in_dir = sys.argv[1]
a = os.listdir(in_dir)
best_pr = -1
total = -1
best_name = ''
for x in a:
#    if x.startswith('best'):
#        os.remove(in_dir+x)
    if x.endswith('_output.txt'):
        print x
        f = open(in_dir+x)
        b = f.read().split('/')
        pr = int(b[0])
        total = float(b[1])
        if best_pr < pr:
            best_pr = pr
            best_name = x
        f.close()
ff = open(in_dir+'best_'+best_name[:-4]+'.txt','w')
ff.write(str(best_pr)+'/'+str(total)+'\n')
ff.close()
