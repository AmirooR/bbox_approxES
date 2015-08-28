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
#include "../../approximateES.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <fstream>
//#include "probimage.h"
#include <opencv2/opencv.hpp>

#define NO_NORMALIZATION 0
#define MEAN_NORMALIZATION 1
#define PIXEL_NORMALIZATION 2
using namespace std;
using namespace cv;

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
                        ( std::ostringstream() << std::dec << x ) ).str()

#define INFINITY_UNARY 100000

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


// Store the colors we read, so that we can write them again.
int nColors = 0;
int colors[255];
unsigned int getColor( const unsigned char * c ){
	return c[0] + 256*c[1] + 256*256*c[2];
}
void putColor( unsigned char * c, unsigned int cc ){
	c[0] = cc&0xff; c[1] = (cc>>8)&0xff; c[2] = (cc>>16)&0xff;
}

unsigned char * colorize( short* map, int W, int H ){
	unsigned char * r = new unsigned char[ W*H*3 ];
    for( int k=0; k<W*H; k++ ){
        int c = 0;
        if( map[k] == 1)
        {
            c = 255;
        }
        r[3*k+0] = c;
        r[3*k+1] = c;
        r[3*k+2] = c;

        //putColor( r+3*k, c );
    }
	return r;
}


// Produce a color image from a bunch of labels
/*unsigned char * colorize( short* map, int W, int H ){
	unsigned char * res = new unsigned char[ W*H*3 ];
	for( int k=0; k<W*H; k++ ){
        int r,g,b;
        get_color( map[k], r, g, b);
        res[3*k] = (unsigned char)r;
        res[3*k+1] = (unsigned char)g;
        res[3*k+2] = (unsigned char)b;
		//int c = colors[ map[k] ];
		//putColor( r+3*k, c );
	}
	return res;
}*/



/*float * classifyCompressed( const ProbImage& prob_im, int W, int H, int M , short* map)
{
	float * res = new float[W*H*M];
    float epsilon = 0; //1e-10;
	for( int k=0; k<W*H; k++ )
    {
        float * r = res + k*M;
        float mx = 0.0f;
        int imx = 0;
        for( int j=0; j<M; j++ )
        {
            float prob = prob_im(k%W, k/W, j);
            r[j] = -log( prob + epsilon);
            if( mx < prob )
            {
                mx = prob;
                imx = j;
            }
        }
        map[k] = (short)imx;
    }
	return res;
}*/

/*float * classifyCompressedNoLOG( const ProbImage& prob_im, int W, int H, int M , short* map)
{
	float * res = new float[W*H*M];
    float epsilon = 0; //1e-10;
	for( int k=0; k<W*H; k++ )
    {
        float * r = res + k*M;
        float mx = prob_im(k%W, k/W,0);
        int imx = 0;
        for( int j=0; j<M; j++ )
        {
             float boost_energy = prob_im(k%W, k/W, j);
            r[j] = -boost_energy;
            if( mx < boost_energy)
            {
                mx = boost_energy;
                imx = j;
            }
        }
        map[k] = (short)imx;
    }
	return res;
}*/


/*float * classifyGT_PROB( const ProbImage& prob_im, int W, int H, int M , short* map)
{
    float GT_PROB = 0.5;
	float * res = new float[W*H*M];
    float epsilon = 0; //1e-10;
	for( int k=0; k<W*H; k++ )
    {
        float * r = res + k*M;
        float mx = prob_im(k%W, k/W,0);
        int imx = 0;
        for( int j=0; j<M; j++ )
        {
              float boost_energy = prob_im(k%W, k/W, j);
            r[j] = -boost_energy;
            if( mx < boost_energy)
            {
                mx = boost_energy;
                imx = j;
            }
        }
        for(int j=0; j<M;j++)
            r[j] = -log( (1.0-GT_PROB)/(M-1) );
        r[imx] = -log(GT_PROB);
        map[k] = (short)imx;
    }
	return res;
}*/

/*float * classify( const ProbImage& prob_im, int W, int H, int M , short* map)
{
    return classifyCompressedNoLOG(prob_im, W, H, M, map);
}*/



int main( int argc, char* argv[]){
    if (argc<5){
        printf("Usage: %s image mask outputimg.ppm unary\n", argv[0] );
        return 1;
    }
    const int M = 2;
    int W, H, GW, GH;
    Mat imMat = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    //resize(imMat, imMat, Size(0,0), 0.25, 0.25);

    Mat annoMask = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
    //resize(annoMask, annoMask, Size(0,0), 0.25, 0.25);

    W = imMat.cols;
    H = imMat.rows;
    GW = annoMask.cols;
    GH = annoMask.rows;

    unsigned char *im = new unsigned char[H*W*3];
    for(int r=0; r<H; r++)
    {            
        for(int c=0; c<W; c++)
        {
            Vec3b p = imMat.at<Vec3b>(r,c);
            im[(r*W+c)*3+2] = p[0];
            im[(r*W+c)*3+1] = p[1];
            im[(r*W+c)*3+0] = p[2];
        }
    }
    
    if(!im){
        printf("Failed to load image!\n");
        return 1;
    }

    if(GW != W || GH != H )
    {
        printf("Error reading files!\n");
        return 1;
    }
    //classify( const ProbImage& prob_im, int W, int H, int M , short* map)
    
    short *map = new short[W*H];
    int num_clusters = 10;
    int fg_tr = 255;
    int bg_tr = 64;
    double prob_add = 0;
    // float *unary = FgProbGMM( imMat, annoMask,  num_clusters, fg_tr,  bg_tr, prob_add);
    //
    /* LOADING UNARIES */

    float *unary = new float[M*W*H];
    ifstream myFile (argv[4], ios::in | ios::binary);
    myFile.read((char*)unary, M*H*W*sizeof(float));

    int i = 0;
    for(int r=0; r<imMat.rows; r++)
    {
        for(int c=0; c<imMat.cols; c++)
        {
            unsigned char m = annoMask.at<uchar>(r,c);
            if( m >= fg_tr)
            {
                unary[i] = INFINITY_UNARY;
                unary[i+1] = 0;
            }
            else if ( m <= bg_tr)
            {
                //unary[i] = unary[i] + lambda;
                unary[i] = 0;
                unary[i+1] = INFINITY_UNARY;
            }
            /*else
            {
                unary[i] = unary[i] + lambda;
                unary_out[i+1] = unary_in[i+1];// + lambda;
            }*/
            i+=2;
        }
    }

    //lasso add 0: 1_2_4_53_4_6_1_1  0.960786,
    //-1_1_5_53_4_6_1_-1 0.963329,
    //-2_1_5_53_4_6_1_-1 0.963924,
    //-2_1_5_53_4_6_1_-3 0.964248,
    //-2_1_5_53_4_6_1_-5 0.964275,
    //-2_1_5_53_4_6_1_-7 0.964302,
    //-2_1_5_3_3_4_1_-7 0.970901,
    //-2_1_5_22_5_5_1_-7 0.984423,
    //-2_1_5_21_37_5_1_-7 0.984477,
    //-2_1_5_17_33_5_1_-7 0.985315,
    //-2_1_5_13_35_5_1_-7 0.987695,
    //-2_1_5_7_36_5_1_-7 0.990183,
    //-2_1_5_6_38_5_1_-7 0.990345,
    //
    //
    // Train all:
    //
    // -2_2_32_6_38_32_5_-5  0.933641,
    // -2_2_32_6_38_32_5_2   0.935234,
    //
    //
    // 3_3_10_20_33_6_43_1  0.923417,
    // 3_3_10_20_33_4_43_1  0.924463,
    // 3_3_10_20_33_3_43_1  0.925605,
    // 3_3_10_20_33_12_43_1 0.927005,
    // 3_3_10_20_4_12_43_1  0.928062,
    // 3_3_10_3_16_12_43_1  0.930500,
    // 3_3_5_12_16_12_43_1  0.933023,
    // 3_3_27_12_16_12_43_-2  0.933995,
    // 
    // 2_3_27_12_16_12_43_-2 0.935552,
    // 2_3_27_12_16_12_43_-2 0.935841,
    //
    // 4_3_27_12_14_12_43_-2 0.935852,
    //
    //4_3_17_12_14_12_43_-2 0.936226,
//#pragma omp parallel for //log: 0_1_2_6_2_2
    for(int logl=1; logl<=1;logl++) //0; 21-> 0 *
    {
        float l = logl;///10.0f;//
        //float l = powf(2,logl);
//#pragma omp parallel for
        for(int isr = 43; isr <= 43; isr+=1) // 1 * 
        {
            float expisr = isr;//isr/10.0;//
            //float expisr = powf(2, isr);
//#pragma omp parallel for
            for(int iw = -2; iw <=-2; iw+=1) // -2 *
            {
                float expiw = powf(2, iw);
                //int expiw = iw;

                //#pragma omp parallel for
                for(int gsx = 3; gsx <= 3; gsx+=1) //3; 21-> 1,2 *
                {
                    //float expgsx = powf(2, gsx);
                    int expgsx = gsx;
                    //#pragma omp parallel for
                    for(int w1=18; w1<=18;w1+=1) //5; 21-> 2 * | 17
                    {
                        //float expw1 = powf(2, w1);
                        int expw1 = w1; //powf(2, w1);

                        //#pragma omp parallel for
                        for(int bsx = 12;  bsx <= 12; bsx+=1) //78; 21-> * 6 | 12
                        {
                            //float expbsx = powf(2, bsx);
                            int expbsx = bsx;
                            //#pragma omp parallel for
                            for(int bsr = 14; bsr <= 14; bsr+= 1) //3 <- 7; 21-> * 2| 16
                            {
                                //float expbsr = powf(2, bsr);
                                int expbsr = bsr;//powf(2, bsr);

                                //#pragma omp parallel for
                                for(int w2=12; w2 <=12; w2+= 1) //5;  21-> 5 * 2 | 12
                                {
                                    //float expw2 = powf(2, w2);
                                    int  expw2 = w2;// powf(2, w2);

                                    cout<<"[logl, gsx, w1, bsx, bsr, w, isr,iw] = "<<"["<<logl<<", "<<gsx<<", "<<w1<<", "<<bsx<<", "<<bsr<<", "<<w2<< ", "<< isr<<", "<<iw<<"]"<<endl;
                                    DenseCRF2D crf(W,H,M);
                                    float* l_unary = new float[M*H*W];
                                    for(int idx=0; idx < M*H*W; idx++)
                                        l_unary[idx] = l*unary[idx];
                                    crf.setUnaryEnergy( l_unary);
                                    crf.setInitX(l_unary);
                                    crf.addPairwiseGaussian( expgsx, expgsx, expw1);
                                    crf.addPairwiseGlobalColor( expisr, expisr, expisr, im, expiw);
                                    string out(argv[3]);
                                    string s = string("CROSSVAL-UNCOMPRESSED-COORD/")+SSTR(logl)+string("_")+SSTR(gsx)+string("_")+SSTR(w1)+string("_")+SSTR(bsx)+string("_")+SSTR(bsr)+string("_")+SSTR(w2)+string("_")+SSTR(isr)+string("_")+SSTR(iw)+string("/")+out;
                                    string mkdir_s = string("mkdir -p ")+s;
                                    system(mkdir_s.c_str());
                                    string wrt_s = s+string("/")+out+string(".ppm");
                                    crf.addPairwiseBilateral(expbsx, expbsx, expbsr, expbsr, expbsr, im, expw2);
                                    crf.map(15, map);
                                    unsigned char *res = colorize( map, W, H);
                                    writePPM( wrt_s.c_str(), W, H, res);
                                    delete[] res;
                                    delete[] l_unary;
                                }
                            }
                        }
                    }
                }

            }
        }
    }

    delete[] map;
    delete[] unary;
    delete[] im;
}
