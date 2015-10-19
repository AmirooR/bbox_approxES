#! /usr/bin/env python

import sys, os

img_dir='/home/amir/Downloads/data_GT/'
newgt_dir='/home/amir/Downloads/boundary_GT/'
mask_dir='/home/amir/Downloads/boundary_GT_lasso/'
unary_dir='unary/'
RESULTS_dir='RESULTS/'
file_list = '/home/amir/Desktop/Test25.txt'

def get_lambda_0(in_dir):
    #in_dir = sys.argv[1]
    lambda_file = open(in_dir+'lambdas.txt')
    lambdas = lambda_file.readlines()
    lambda_file.close()
    idx = 0
    for x in lambdas:
        l = float(x)
        if l>= 0:
            return idx
        idx = idx+1


def get_last_idx(in_dir):
    a = os.listdir(in_dir)
    last_pr = -1
    total = -1
    last_idx = -1
    for x in a:
        if x.startswith('best'):
            pass;
        elif x.endswith('_output.txt'):
            idx = int(x.split('_')[0])
            if idx > last_idx:
                last_idx = idx
    return last_idx

file_list_f = open(file_list)
file_names = file_list_f.readlines()
file_list_f.close()
for x in file_names:
        print x
        x = x.strip()
        filename=x[:-4]
        orig_img_file = img_dir+x
        unary_file = unary_dir+filename+'_0.unary'
        #orig_labelling_file = RESULTS_dir+ filename+'/orig/'+filename+'.ppm'
        es_result_dir = RESULTS_dir+filename+'/l_-0.5_0.5/'
        lambda_0_idx = get_lambda_0(es_result_dir)
        last_idx = get_last_idx(es_result_dir)
        orig_labelling_file = es_result_dir+str(last_idx)+'_output.ppm'
        mask_file = mask_dir+filename+'.bmp'
        command1 = './computeEnergy '+orig_img_file+' '+unary_file+' '+ mask_file+' '+ orig_labelling_file + ' >> output.txt'
        os.system(command1)
        command2 = './computeEnergy '+orig_img_file+' '+unary_file+' '+ mask_file+' '+es_result_dir+str(lambda_0_idx)+'_output.ppm >> output_es.txt'
        os.system(command2)
        #print command2
