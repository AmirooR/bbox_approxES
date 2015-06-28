#include "probimage.h"
#include <cstdio>
#include <iostream>
#include <opencv2/opencv.hpp>

#include <string>
#include <sstream>

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
                        ( std::ostringstream() << std::dec << x ) ).str()

using namespace std;
using namespace cv;

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
            r = 192; g = 128; g = 128;
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
            r = 128; b = 64; g = 128;
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

int main( int argc, char * argv[] ){
	if (argc<3){
		printf("Usage: %s compressed_file image_name\n", argv[0] );
        return 1;
	}
	ProbImage im;
	im.decompress( argv[1] );
	//  im.probToEnergy();
    cout<< "Width: "<<im.width()<<", Height: "<<im.height()<<", Depth: "<<im.depth() <<endl;
    float mx = 0.0f;
    int imx = 0;
    
    Mat out(im.height(), im.width(), CV_8UC3);
    for(int j = 0; j < im.height(); j++)
    {
        for(int i=0; i< im.width(); i++)
        {
            mx = 0.0f;
            imx = 0;
            for(int k = 0; k < im.depth(); k++)
            {
                if( mx < im(i,j,k) )
                {
                    mx = im(i,j,k);
                    imx = k;
                }
            }
            int r,g,b;
            get_color(imx, r, g, b);
            out.at<Vec3b>(j,i) = Vec3b(r,g,b);
        }
    }
    string s( argv[2]);
    s = s + string(".png");
    cout<<s<<endl;
    imwrite(s.c_str(),out);
	return 0;
}
