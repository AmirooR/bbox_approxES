#!/bin/env python
import numpy as np
import cv,cv2
import sys, os


img_dir='/home/amir/Downloads/data_GT/'
newgt_dir='/home/amir/Downloads/boundary_GT/'

#\\geometry{paperwidth=100mm,paperheight=75mm}\n

def write_header(tex_file):
    tex_file.write(""" \\documentclass[8pt]{beamer} \n
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
    \\includegraphics[width=3cm, height=4cm, keepaspectratio]{""")
    tex_file.write(img_file_path)
    tex_file.write("}")
    tex_file.write("""\\end{figure}
    \\end{frame}""")

def add_multicolumn_image_frame(tex_file, img1, img2, acc2, img3, acc3, title=None, text=None):
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
    tex_file.write("""\\end{center}""")
    tex_file.write("""\\begin{columns}[t, totalwidth=\\textwidth]
    \\column{0.33\\linewidth}""")
    tex_file.write("""
    \\begin{figure}
    \\includegraphics[width=3cm, height=4cm, keepaspectratio]{""")
    tex_file.write(img1)
    tex_file.write("}")
    tex_file.write("""\\caption{Input}\\end{figure}""")

    tex_file.write("""\\column{0.33\\linewidth}""")
    tex_file.write("""
    \\begin{figure}
    \\includegraphics[width=3cm, height=4cm, keepaspectratio]{""")
    tex_file.write(img2)
    tex_file.write("}")
    tex_file.write("""\\caption{DRF ("""+acc2+""")}\\end{figure}""")

    tex_file.write("""\\
    \\column{0.33\\linewidth}""")
    tex_file.write("""
    \\begin{figure}
    \\includegraphics[width=3cm, height=4cm, keepaspectratio]{""")
    tex_file.write(img3)
    tex_file.write("}")
    tex_file.write("""\\caption{ES ("""+acc3+""")}\\end{figure}""")

    tex_file.write("""\\end{columns}""")
    tex_file.write("""\\end{frame}""")



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
            if(idx != last_idx ):
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

sorted_file = open('/home/amir/Desktop/Test25.txt')
file_names = sorted_file.readlines()
sorted_file.close()

i=0
#file_names = os.listdir(newgt_dir)
for y in file_names:
    x = y.strip()
    print x
    filename=x[:-4]
    orig_img_file = img_dir+'pngs/'+filename+'.png'
    in_dir = 'RESULTS/'+filename+'/l_-0.5_0.5/'
    last_idx = get_last(in_dir)
    dense_crf_img_file = in_dir+'mask_drf.png'
    dense_pr, dense_total = get_dense_result(in_dir+str(last_idx)+'_output.txt')
    dense_acc = dense_pr/dense_total
    best_idx, best_pr, total, best_name = get_best_es(in_dir, last_idx)
    best_acc = best_pr/total
    l1, l2 = get_lambdas(in_dir, best_idx)
    #best_img_ppm = in_dir+str(best_idx)+'_output.ppm'
    #img_pp = cv2.imread(best_img_ppm)
    best_img_file = in_dir+'mask_es.png'#in_dir+str(best_idx)+'_output.png'
    #cv2.imwrite(best_img_file, img_pp)
    txt = '$%s \leq \lambda \leq %s$' % (l1[:-1],l2[:-1])
    add_multicolumn_image_frame(tex_file, orig_img_file, dense_crf_img_file, str(dense_acc), best_img_file, str(best_acc), title=filename, text=txt)
    i = i+1

write_footer(tex_file)
tex_file.close()

