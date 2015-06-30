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
#include "probimage.h"
//#include <opencv2/opencv.hpp>

#define NO_NORMALIZATION 0
#define MEAN_NORMALIZATION 1
#define PIXEL_NORMALIZATION 2
using namespace std;
//using namespace cv;

inline void  get_color(int c, int& r, int& g, int& b)
{
    r = 0;
    g = 0;
    b = 0;
    switch(c)
    {
        case 0:
            r = 128;
            break;
        case 1:
            g = 128;
            break;
        case 2:
            r = 128; g=128;
            break;
        case 3:
            b = 128;
            break;
        case 4:
            g = 128; b = 128;
            break;
        case 5:
            r = 128; b = 128; g = 128;
            break;
        case 6:
            r = 192;
            break;
        case 7:
            r = 64; g = 128;
            break;
        case 8:
            r = 192; g = 128;
            break;
        case 9:
            r = 64; b = 128;
            break;
        case 10:
            r = 192; b = 128;
            break;
        case 11:
            r = 64; g = 128; b = 128;
            break;
        case 12:
            r = 192; g = 128; b = 128;
            break;
        case 13:
            g = 64;
            break;
        case 14:
            r = 128; g = 64;
            break;
        case 15:
            g = 192;
            break;
        case 16:
            r = 128; g = 64; b = 128;
            break;
        case 17:
            g = 192; b = 128;
            break;
        case 18:
            r = 128; g = 192; b = 128;
            break;

        case 19:
            r = 64; g = 64;
            break;
        case 20:
            r = 192; g = 64;
            break;
    }
}

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
                        ( std::ostringstream() << std::dec << x ) ).str()


// Store the colors we read, so that we can write them again.
int nColors = 0;
int colors[255];
unsigned int getColor( const unsigned char * c ){
	return c[0] + 256*c[1] + 256*256*c[2];
}
void putColor( unsigned char * c, unsigned int cc ){
	c[0] = cc&0xff; c[1] = (cc>>8)&0xff; c[2] = (cc>>16)&0xff;
}
// Produce a color image from a bunch of labels
unsigned char * colorize( short* map, int W, int H ){
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
}



float * classify( const ProbImage& prob_im, int W, int H, int M , short* map)
{
	float * res = new float[W*H*M];
    float epsilon = 1e-10;
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
}





int main( int argc, char* argv[]){
	if (argc<4){
		printf("Usage: %s image compressed_unary outputimg.ppm\n", argv[0] );
		return 1;
	}
    const int M = 21;
    int W, H, GW, GH;
    ProbImage prob_im;
    prob_im.decompress(argv[2]);
    GW = prob_im.width();
    GH = prob_im.height();
    unsigned char *im = readPPM( argv[1], W, H);
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
    float *unary = classify( prob_im, W, H, M, map);

    DenseCRF2D crf(W,H,M);
    crf.setUnaryEnergy( unary);
    crf.setInitX(unary);
    //double gsx = 3.f, double gsy = 3.f, double gw=3.f,
    //double bsx = 60.f, double bsy = 60.f, double bsr=20.f, double bsg=20.f, double bsb=20.f, double bw=10.f
    crf.addPairwiseGaussian( 1, 1, 1);
    //crf->addPairwiseBilateral( bsx, bsy, bsr, bsg, bsb, im, bw );

    crf.addPairwiseBilateral(80., 80., 13., 13., 13., im, 80);
    crf.map(15, map);
    unsigned char *res = colorize( map, W, H);

    writePPM( argv[3], W, H, res);

    delete[] map;
    delete[] res;
    delete[] unary;
    delete[] im;
}
