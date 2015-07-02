#include "probimage.h"
#include <cstdio>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "util.h"

#include <string>
#include <sstream>

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
                        ( std::ostringstream() << std::dec << x ) ).str()

using namespace std;
using namespace cv;


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
            out.at<Vec3b>(j,i) = Vec3b(b,g,r);
        }
    }
    string s( argv[2]);
    s = s + string(".png");
    cout<<s<<endl;
    imwrite(s.c_str(),out);
	return 0;
}
