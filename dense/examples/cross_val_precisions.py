#!/bin/env python
import numpy as np
import cv,cv2
import sys, os

img_dir='/home/amir/src/PMC/DATA/MSRC_ObjCategImageDatabase_v2/Images/'
newgt_dir='/home/amir/src/PMC/DATA/newgt/'
cross_val_result_dir='CROSSVAL-UNCOMPRESSED-COORD/'
k=5

def get_dense_result(output_file):
    f = open(output_file)
    b = f.read().split('/')
    f.close()
    pr = int(b[0])
    total = float(b[1])
    return pr, int(total)

def get_train_test_list(fold):
    train_list = []
    test_list = []
    for i in range(0,k):
        list_file_name = "%sfold%d.txt" % (newgt_dir, i+1)
        list_file = open(list_file_name)
        files = list_file.readlines()
        if i==fold:
            test_list.extend(files)
        else:
            train_list.extend(files)
        list_file.close()
    return train_list, test_list

res_file = open('cross_val_results.txt','w')

for fold in range(0,k):
    print 'fold %d' % fold
    train_list, test_list = get_train_test_list(fold)
    best_score = 0.
    best_param_set = None
    for param_set in os.listdir(cross_val_result_dir):
        if os.path.isdir(cross_val_result_dir+param_set):
            #print 'parameter_set: %s' % param_set
            sum_pr = 0
            sum_total = 0
            for x in train_list:
                filename=x[:-8]
                score_file = cross_val_result_dir+param_set+'/'+filename+'/'+filename+'.txt'
                pr, total = get_dense_result(score_file)
                sum_pr = sum_pr + pr
                sum_total = sum_total+total
            score = sum_pr/float(sum_total)
            print '\tscore: %f' % score
            if score > best_score:
                best_score = score
                best_param_set = param_set

    print 'best_score: %f, param_set: %s' % (best_score, best_param_set)
    print '\nTesting'
    sum_pr = 0
    sum_total = 0
    for x in test_list:
        filename=x[:-8]
        score_file = cross_val_result_dir+best_param_set+'/'+filename+'/'+filename+'.txt'
        pr, total = get_dense_result(score_file)
        sum_pr = sum_pr+pr
        sum_total = sum_total + total
    score = sum_pr/float(sum_total)
    wrt_str = '%f %s\n' % (score, best_param_set)
    print 'test score: %f' % score
    res_file.write(wrt_str)
res_file.close()
