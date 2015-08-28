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
#include <opencv2/opencv.hpp>
#include <fstream>

#define NO_NORMALIZATION 0
#define MEAN_NORMALIZATION 1
#define PIXEL_NORMALIZATION 2

#define INFINITY_UNARY 100000
using namespace std;
using namespace cv;

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
                        ( std::ostringstream() << std::dec << x ) ).str()


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
    if(v == ColorBGR(255,255,255))
        return 1;
    else if(v == ColorBGR(0, 0, 0))
        return 0;
    else
        cout<<"Warning: non 0-1 class"<<endl;
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

void define_unaries( float* unary_in, float* unary_out, Mat imMat, Mat annoMask, int fg_tr, int bg_tr, float lambda = 0)
{
    int i = 0;
    for(int r=0; r<imMat.rows; r++)
    {
        for(int c=0; c<imMat.cols; c++)
        {
            unsigned char m = annoMask.at<uchar>(r,c);
            if( m >= fg_tr)
            {
                unary_out[i] = INFINITY_UNARY;
                unary_out[i+1] = 0;
            }
            else if ( m <= bg_tr)
            {
                //unary[i] = unary[i] + lambda;
                unary_out[i] = 0;
                unary_out[i+1] = INFINITY_UNARY;
            }
            else
            {
                unary_out[i] = unary_in[i] + lambda;
                unary_out[i+1] = unary_in[i+1];// + lambda;
            }
            i+=2;
        }
    }
}


int main( int argc, char* argv[])
{
    //NOTE: assuming lambda is equal to 0 for additive case
    if (argc<5){
        printf("Usage: %s image unary mask labelling-image\n", argv[0] );
        return 1;
    }
    unsigned char *im;
    unsigned char *labelling_image;
    float* unary;

    int W,H,N;
    int GH, GW;
    int lW, lH;
    int M = 2;

    DenseCRF2D* crf;

    Mat imMat = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    Mat annoMask = imread(argv[3], CV_LOAD_IMAGE_GRAYSCALE);
    labelling_image = readPPM( argv[4], lW, lH);

    W = imMat.cols;
    H = imMat.rows;
    GW = annoMask.cols;
    GH = annoMask.rows;

    if (W!=GW || H!=GH)
    {
        printf("Annotation size doesn't match image!\n");
        exit(1);
    }

    N = W*H;
    
    im = new unsigned char[H*W*3];
   
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
        printf("Failed to load labelling image!\n");
        exit(1);
    }
    N = W*H;
    short* map = new short[N];
    crf = new DenseCRF2D(W,H,M);

    unary = new float[M*N];

    ifstream myFile (argv[2], ios::in | ios::binary);
    myFile.read((char*)unary, M*N*sizeof(float));
    
    //define unaries: they are -P now!     
    define_unaries( unary, unary, imMat, annoMask, 255, 64, 0);

    crf->setUnaryEnergy( unary);
    crf->setInitX(unary);
        
    
    crf->addPairwiseGaussian( 3.f, 3.f, 18.f );
    crf->addPairwiseBilateral( 12.f, 12.f, 14.f, 14.f , 14.f, im, 12.f );
    crf->addPairwiseGlobalColor( 43.f, 43.f, 43.f, im, 0.25f);

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
 
