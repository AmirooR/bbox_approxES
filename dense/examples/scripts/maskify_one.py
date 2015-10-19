#! /usr/bin/env python

import sys, os
import numpy as np
import cv, cv2

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

in_dir = sys.argv[1]
in_img = sys.argv[2]

last_idx = get_last(in_dir)
best_es_idx, best_pr, total, best_name = get_best_es(in_dir, last_idx)

img = cv2.imread(in_img)
mask_drf = cv2.imread(in_dir+str(last_idx)+'_output.ppm')
mask_es  = cv2.imread(in_dir+str(best_es_idx)+'_output.ppm')

img_drf = 255*np.ones(img.shape, dtype=np.uint8)
img_es  = 255*np.ones(img.shape, dtype=np.uint8)

img_drf[np.where(mask_drf > 0)] = img[np.where(mask_drf>0)]
img_es[np.where(mask_es > 0)] =  img[np.where(mask_es>0)]

cv2.imwrite(in_dir+'mask_drf.png', img_drf)
cv2.imwrite(in_dir+'mask_es.png', img_es)
