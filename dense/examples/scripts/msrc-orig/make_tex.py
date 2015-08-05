#!/bin/env python
import numpy as np
import cv,cv2
import sys, os


img_dir='/home/amir/src/PMC/DATA/MSRC_ObjCategImageDatabase_v2/Images/'
newgt_dir='/home/amir/src/PMC/DATA/MSRC_ObjCategImageDatabase_v2/Images/'
test_list='/home/amir/src/PMC/DATA/MSRC_ObjCategImageDatabase_v2/Images/Test.txt'

def write_header(tex_file):
    tex_file.write(""" \\documentclass{beamer} \n
        \\mode<presentation>{ \n
        \\usetheme{Madrid} \n
        } \n
        \\usepackage{graphicx} \n
        \\usepackage{booktabs} \n
        \\title[approximateES]{approximateES with dense-crf} \n
        \\author{Amir Rahimi} \n
        \\institute[TU]{\n
        TUD \\\\ \n
                \\medskip \n
                \\textit{noname01.cpp@.com}  \n
                } \n
                \\date{\\today}  \n
                \n
                \\begin{document} \n
                """)

def add_image_frame(tex_file, img_file_path, title=None, text=None):
    tex_file.write("""\\begin{frame}
        \\frametitle{""")
    txt = 'DenseCRF'
    if title:
        txt = title
    tex_file.write(txt.replace('_','\_'))
    tex_file.write("""}
        \\begin{center}""")
    if text:
        tex_file.write(text)
    tex_file.write("""\\end{center}
    \\begin{figure}
    \\includegraphics[scale=0.5]{""")
    tex_file.write(img_file_path)
    tex_file.write("}")
    tex_file.write("""\\end{figure}
    \\end{frame}""")

def write_footer(tex_file):
    tex_file.write("\\end{document}")


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

def get_dense_result(output_file):
    f = open(output_file)
    b = f.read().split('/')
    pr = int(b[0])
    total = float(b[1])
    return pr, total

def get_lambdas(in_dir, best_idx):
    lambda_file = open(in_dir+'lambdas.txt')
    lambdas = lambda_file.readlines()
    lambda_file.close()
    if int(best_idx) == len(lambdas) or int(best_idx) == 0:
        return 'unary\n', 'unary\n'
    return lambdas[int(best_idx)-1], lambdas[int(best_idx)]


tex_file = open('slides.tex','w')
write_header(tex_file)
list_file = open(test_list)
lines = list_file.readlines()
list_file.close()

#file_names = os.listdir(newgt_dir)
for yy in lines:
#    if x.endswith('_GT.bmp'):
        x = yy[:-6]+'_GT.bmp'
        print x
        filename=x[:-7]
        orig_img_file = img_dir+'pngs/'+filename+'.png'
        gt_img_file = newgt_dir+'pngs/'+x[:-4]+'.png'
        dense_crf_img_file = 'RESULTS/'+filename+'/orig/'+filename+'.png'
        dense_pr, dense_total = get_dense_result('RESULTS/'+filename+'/orig/'+filename+'.txt')
        dense_acc = dense_pr/dense_total
        add_image_frame(tex_file, orig_img_file, title=filename, text='Image')
        add_image_frame(tex_file, gt_img_file, title=filename, text='Ground Truth')

        add_image_frame(tex_file, dense_crf_img_file, title=filename, text='Dense-CRF ('+str(dense_acc)+')')
        in_dir = 'RESULTS/'+filename+'/l_0.0_20/'
        best_idx, best_pr, total = get_best(in_dir)
        best_acc = best_pr/total
        l1, l2 = get_lambdas(in_dir, best_idx)
        best_img_file = in_dir+str(best_idx)+'_output.png'
        txt = '$%s \leq \lambda \leq %s$' % (l1[:-1],l2[:-1])
        add_image_frame(tex_file, best_img_file, title=filename, text='aES '+txt+' ('+str(best_acc)+')')

write_footer(tex_file)
tex_file.close()

