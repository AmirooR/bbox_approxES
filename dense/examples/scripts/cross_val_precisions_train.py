#!/bin/env python
import numpy as np
import cv,cv2
import sys, os

cross_val_result_dir='CROSSVAL-UNCOMPRESSED-COORD/'
training_file = '/home/amir/Desktop/Test25.txt'
def get_dense_result(output_file):
    f = open(output_file)
    b = f.read().split('/')
    f.close()
    pr = int(b[0])
    total = float(b[1])
    return pr, int(total)

def get_train_list(train_file):
    f = open(train_file)
    files = f.readlines()
    f.close()
    return files


#filename = sys.argv[1]

best_score = 0
for param_set in os.listdir(cross_val_result_dir):
        if os.path.isdir(cross_val_result_dir+param_set):
            #print 'parameter_set: %s' % param_set
            sum_pr = 0
            sum_total = 0
            train_list = get_train_list(training_file)
            for x in train_list:
                filename=x[:-5]
                print filename
                score_file = cross_val_result_dir+param_set+'/'+filename+'/'+filename+'.txt'
                print score_file
                if os.path.isfile(score_file):
                    pr, total = get_dense_result(score_file)
                    #score = pr/float(total)
                    sum_pr = sum_pr + pr
                    sum_total = sum_total+total
            print sum_pr
            print sum_total
            score = sum_pr/float(sum_total)
            if score > best_score:
                best_score = score
                best_param_set = param_set

print 'best_score: %f, param_set: %s' % (best_score, best_param_set)

