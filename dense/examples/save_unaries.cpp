/*
    Copyright (c) 2011, Philipp Krähenbühl
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the Stanford University nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Philipp Krähenbühl ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Philipp Krähenbühl BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "densecrf.h"
#include <cstdio>
#include <cmath>
#include "util.h"
#include <iostream>
#include <fstream>
#include "../../approximateES.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstring>
//#include "probimage.h"
#include <opencv2/opencv.hpp>

#define NO_NORMALIZATION 0
#define MEAN_NORMALIZATION 1
#define PIXEL_NORMALIZATION 2
using namespace std;
using namespace cv;

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
                        ( std::ostringstream() << std::dec << x ) ).str()


float* FgProbGMM(Mat im, Mat fgMask, int num_clusters = 10, int fg_tr = 128, int bg_tr = 64, double prob_add = 0.0)
{
    Mat patterns(0, 0, CV_32F);
    Mat bg_patterns(0, 0, CV_32F);
    
    for(int r=0; r<im.rows; r++)
    {
        for(int c=0; c<im.cols; c++)
        {
            Vec3b p = im.at<Vec3b>(r,c);
            float cb = p[0]/255.0f;
            float cg = p[1]/255.0f;
            float cr = p[2]/255.0f;
            Mat pat = (cv::Mat_<float>(1,3)<< cr,cg,cb);
            unsigned char m = fgMask.at<uchar>(r,c);
            if( m == fg_tr)
            {
                patterns.push_back(pat);
            }
            else if ( m == bg_tr)
            {
                bg_patterns.push_back(pat);
            }
        }
    }

    const int cov_mat_type = cv::EM::COV_MAT_GENERIC;
    cv::TermCriteria term(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 1500, 1e-4);
    cv::EM gmm(num_clusters, cov_mat_type, term);
    cout << "#fg samples: " << patterns.rows << endl;
    cv::Mat labels, posterior, logLikelihood;

    cv::EM bg_gmm(num_clusters, cov_mat_type, term);
    cout << "#bg samples: " << bg_patterns.rows << endl;
    cv::Mat bg_labels, bg_posterior, bg_logLikelihood;


    cout << "Training Foreground GMM... " << flush;
    gmm.train(patterns, logLikelihood, labels, posterior);
    cout << "Done!" << endl;

    cout << "Training Background GMM... " << flush;
    bg_gmm.train(bg_patterns, bg_logLikelihood, bg_labels, bg_posterior);
    cout << "Done!" << endl;


    //Mat output(im.rows, im.cols, CV_64FC1);
    float* output = new float[im.rows*im.cols*2];

    int i = 0;
    double max = 0;
    for(int r=0; r<im.rows; r++)
    {
        for(int c=0; c<im.cols; c++)
        {
            Vec3b p = im.at<Vec3b>(r,c);
            float cb = p[0]/255.0f;
            float cg = p[1]/255.0f;
            float cr = p[2]/255.0f;
            Mat pat = (cv::Mat_<float>(1,3)<< cr,cg,cb);
            Vec2d fg_logp = gmm.predict(pat);
            Vec2d bg_logp = bg_gmm.predict(pat);

            float fg_prob = (exp(fg_logp[0])+prob_add)/(exp(fg_logp[0])+exp(bg_logp[0])+2*prob_add);
            //output.at<double>(r,c) = fg_prob;
            output[ (r*im.cols+c)*2 + 0] = -(1.0f-fg_prob);
            output[ (r*im.cols+c)*2 + 1] = -(fg_prob);
        }
    }

    /*Mat output_g(im.rows, im.cols, CV_8U);

    for(int r=0; r<im.rows; r++)
    {
        for(int c=0; c<im.cols; c++)
        {
            output_g.at<uchar>(r,c) = saturate_cast<uchar>( ( output.at<double>(r,c))*255.0 );
        }
    }*/

    return output;

}


int main( int argc, char* argv[]){
    if (argc<4){
        printf("Usage: %s image mask name\n", argv[0] );
        return 1;
    }
    const int M = 2;
    int W, H, GW, GH;
    Mat imMat = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    //resize(imMat, imMat, Size(0,0), 0.25, 0.25);

    Mat annoMask = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
    string name(argv[3]);
    //resize(annoMask, annoMask, Size(0,0), 0.25, 0.25);

    W = imMat.cols;
    H = imMat.rows;
    GW = annoMask.cols;
    GH = annoMask.rows;

    if(GW != W || GH != H )
    {
        printf("Error reading files!\n");
        return 1;
    }
    //classify( const ProbImage& prob_im, int W, int H, int M , short* map)
    int num_clusters = 10;
    int fg_tr = 255;
    int bg_tr = 64;
    double prob_add = 0;
    float *unary = FgProbGMM( imMat, annoMask,  num_clusters, fg_tr,  bg_tr, prob_add);
    string unary_file = name + string(".unary");
    ofstream myFile (unary_file.c_str(), ios::out | ios::binary);
    myFile.write( (char*)unary, sizeof(float)*M*W*H);

    Mat output_g(imMat.rows, imMat.cols, CV_8U);

    int i = 1;
    for(int r=0; r<imMat.rows; r++)
    {
        for(int c=0; c<imMat.cols; c++)
        {
            output_g.at<uchar>(r,c) = saturate_cast<uchar>( unary[i] * -255.0 );
            i+=2;
        }
    }

    string probability_file = name + string(".png");
    imwrite(probability_file.c_str(), output_g);

    delete[] unary;
    return 0;
}
