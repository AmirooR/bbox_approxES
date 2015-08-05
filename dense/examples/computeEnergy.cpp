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

#define NO_NORMALIZATION 0
#define MEAN_NORMALIZATION 1
#define PIXEL_NORMALIZATION 2
using namespace std;

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
                        ( std::ostringstream() << std::dec << x ) ).str()
float * classifyCompressedNoLOG( const ProbImage& prob_im, int W, int H, int M , short* map)
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
            /*float prob = prob_im(k%W, k/W, j);
            r[j] = -log( prob + epsilon);
            if( mx < prob )
            {
                mx = prob;
                imx = j;
            }*/
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
}

float * classify( const ProbImage& prob_im, int W, int H, int M , short* map)
{
    return classifyCompressedNoLOG(prob_im, W, H, M, map);
}

struct ColorBGR
{
    int r,g,b;
    ColorBGR(int _b, int _g, int _r)
    {
        r = _r;
        b = _b;
        g = _g;
    }

    bool operator==(const ColorBGR& v)
    {
        if(v.r==r && v.g==g && v.b == b)
            return true;
        else
            return false;
    }
};

int get_class(ColorBGR& v)
{
    if(v == ColorBGR(0,0,128))
        return 0;
    else if(v == ColorBGR(0,128,0))
        return 1;
    else if(v == ColorBGR(0,128,128))
        return 2;
    else if(v == ColorBGR(128,0,0))
        return 3;
    else if(v == ColorBGR(128,128,0))
        return 4;
    else if(v == ColorBGR(128,128,128))
        return 5;
    else if(v == ColorBGR(0,0,192))
        return 6;
    else if(v == ColorBGR(0,128,64))
        return 7;
    else if(v == ColorBGR(0,128,192))
        return 8;
    else if(v == ColorBGR(128,0,64))
        return 9;
    else if(v == ColorBGR(128,0,192))
        return 10;
    else if(v == ColorBGR(128,128,64))
        return 11;
    else if(v == ColorBGR(128,128,192))
        return 12;
    else if(v == ColorBGR(0,64,0))
        return 13;
    else if(v == ColorBGR(0, 64, 128))
        return 14;
    else if(v == ColorBGR(0,192,0))
        return 15;
    else if(v == ColorBGR(128, 64, 128))
        return 16;
    else if(v == ColorBGR(128,192,0))
        return 17;
    else if(v == ColorBGR(128, 192, 128))
        return 18;
    else if(v == ColorBGR(0, 64, 64))
        return 19;
    else if(v == ColorBGR(0, 64, 192))
        return 20;
    else
        return -1;
}




void map_from_labelling( unsigned char* labelling, int N, short* map)
{
    for(int k=0; k<N; k++)
    {
        ColorBGR colorBGR( labelling[3*k+2] ,labelling[3*k+1],labelling[3*k] );
        map[k] = get_class( colorBGR);
    }
}


int main( int argc, char* argv[])
{
    //NOTE: assuming lambda is equal to 1 for multiplicative case
    if (argc<4){
        printf("Usage: %s image compressed_unary labelling-image\n", argv[0] );
        return 1;
    }
    unsigned char *im;
    unsigned char *labelling_image;
    ProbImage prob_im;

    int W,H,N;
    int GH, GW;
    int lW, lH;
    int M = 21;

    DenseCRF2D* crf;

    im = readPPM( argv[1], W, H);
    labelling_image = readPPM( argv[3], lW, lH);
    prob_im.decompress( argv[2]);

    if(!im){
        printf("Failed to load labelling image!\n");
        exit(1);
    }

    N = W*H;
    GH = prob_im.height();
    GW = prob_im.width();
    if(W!=GW || H!=GH)
    {
        printf("ProbImage size doesn't match image!\n");
        exit(1);
    }

    if(W!=lW || H!=lH)
    {
        printf("Labelling image size doesn't match image!\n");
        exit(1);
    }

    short* map = new short[N];
    crf = new DenseCRF2D(W,H,M);
    float* unary = classify( prob_im, W, H, M, map);
    crf->setUnaryEnergy( unary);
    crf->setInitX(unary);
    crf->addPairwiseGaussian( 3,3,5);
    crf->addPairwiseBilateral( 78, 78, 3,3,3, im, 5);
    float* u_result = new float[N];
    float* p_result = new float[N];
    map_from_labelling( labelling_image, N, map);
    crf->unaryEnergy(map, u_result);
    crf->pairwiseEnergy(map, p_result);
    double sum = 0;

    for(int i=0;i<N;i++)
    {
        sum += u_result[i] + p_result[i];
    }
    cout<<sum<<endl;
    delete[] u_result;
    delete[] p_result;
    delete crf;
    delete[] map;
    delete[] unary;
    delete[] im;
    delete[] labelling_image;
    return 0;
}
 
