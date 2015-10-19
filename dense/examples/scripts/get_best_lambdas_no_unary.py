#! /usr/bin/env python

import sys, os
def get_last(in_dir):
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
    return last_idx

def get_best_es(in_dir, last_idx=-1):
    a = os.listdir(in_dir)
    best_pr = -1
    total = -1
    best_name = ''
    best_idx = -1
    for x in a:
        if x.startswith('best'):
#        os.remove(in_dir+x)
            pass;
        elif x.endswith('_output.txt'):
#        print x
            idx = int(x.split('_')[0])
            if(idx != last_idx and idx != 0 ):
                f = open(in_dir+x)
                b = f.read().split('/')
                pr = int(b[0])
                total = float(b[1])
                if best_pr < pr:
                    best_pr = pr
                    best_name = x
                    best_idx = idx
                f.close()
    return best_idx, best_pr, total, best_name

def get_best(in_dir):
        a = os.listdir(in_dir)
        best_pr = -1
        total = -1
        best_name = ''
        best_idx = -1
        for x in a:
            if x.startswith('best'):
                pass;#los.remove(in_dir+x)
            elif x.endswith('_output.txt'):
                f = open(in_dir+x)
                b = f.read().split('/')
                pr = int(b[0])
                total = float(b[1])
                if best_pr < pr:
                    best_pr = pr
                    best_name = x
                    best_idx = x.split('_')[0]
                f.close()
        return best_idx, best_pr, total

def get_lambdas(in_dir, best_idx):
    lambda_file = open(in_dir+'lambdas.txt')
    lambdas = lambda_file.readlines()
    lambda_file.close()
    if int(best_idx) == len(lambdas) or int(best_idx) == 0:
        return 'unary', 'unary'
    return lambdas[int(best_idx)-1], lambdas[int(best_idx)]



in_dir = sys.argv[1]
last_idx = get_last(in_dir)
best_idx, best_pr, total, best_name = get_best_es(in_dir, last_idx)
l1, l2 = get_lambdas(in_dir, best_idx)
print l1[:-1]+'\t'+l2[:-1]
