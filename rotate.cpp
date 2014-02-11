
#include <sstream>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "cv.h"

using namespace cv;
using namespace std;


void rotateImage(const Mat &img_in, Mat &img, double rotx, double roty, double rotz)
{

//conversion degrees -> radiants
    rotx = rotx*CV_PI/180.;
    roty = roty*CV_PI/180.;
    rotz = rotz*CV_PI/180.;

    int f = 8; // this is also configurable, f=2 should be about 50mm focal length
    
    double w = (double)img_in.cols;
    double h = (double)img_in.rows;
    
    float cx = cosf(rotx), sx = sinf(rotx);
    float cy = cosf(roty), sy = sinf(roty);
    float cz = cosf(rotz), sz = sinf(rotz);

    float roto[3][2] = { // last column not needed, our vector has z=0
	{ cz * cy, cz * sy * sx - sz * cx },
	{ sz * cy, sz * sy * sx + cz * cx },
	{ -sy, cy * sx }
    };

    float pt[4][2] = {{ -w / 2, -h / 2 }, { w / 2, -h / 2 }, { w / 2, h / 2 }, { -w / 2, h / 2 }};
    float ptt[4][2];
    for (int i = 0; i < 4; i++) {
	float pz = pt[i][0] * roto[2][0] + pt[i][1] * roto[2][1];
	ptt[i][0] = w / 2 + 400 + (pt[i][0] * roto[0][0] + pt[i][1] * roto[0][1]) * f * h / (f * h + pz);
	ptt[i][1] = h / 2 + 240 + (pt[i][0] * roto[1][0] + pt[i][1] * roto[1][1]) * f * h / (f * h + pz);
    }

    cv::Mat in_pt = (cv::Mat_<float>(4, 2) << 0, 0, w, 0, w, h, 0, h);
    cv::Mat out_pt = (cv::Mat_<float>(4, 2) << ptt[0][0], ptt[0][1],
	ptt[1][0], ptt[1][1], ptt[2][0], ptt[2][1], ptt[3][0], ptt[3][1]);

    cv::Mat transform = cv::getPerspectiveTransform(in_pt, out_pt);

    img = img_in.clone();
    cv::warpPerspective(img_in, img, transform, Size (1280,720), INTER_LANCZOS4);
}
  
int main(int argc, char **argv)
{
  
    std::stringstream filename;  
    cv::Mat image_src, image_dst, image_final, mask;
    image_src = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file
    if(! image_src.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    if (argc != 9) {
        cout <<  "Syntax: rotate <img file><x><x><y><y><z><z><step>" << std::endl ;
        return -1;
    } 
        
    for (int x=atoi(argv[2]);  x <= atoi(argv[3]); x+=atoi(argv[8]) ) {
      for (int y=atoi(argv[4]); y <= atoi(argv[5]); y+=atoi(argv[8])) {
	for (int z=atoi(argv[6]); z <= atoi(argv[7]); z+=atoi(argv[8])){
  
	  rotateImage (image_src, image_dst, x, y, z);

	  mask.setTo(Scalar (0,0,0));
	  image_final = image_dst.clone();
	  image_final.setTo(Scalar (0,255,0));
	  cv::inRange (image_dst,
		      Scalar(0, 0, 0), 
		      Scalar(40, 40, 40), //		      Scalar(0, 255, 255),  //quitar blancos tambien
		      mask
           );
	  cv::bitwise_not (mask, mask);
	  //cvtColor (mask,mask,CV_BGR2RGB);
	  cvtColor (image_final,image_final,CV_BGR2RGB);
	  image_dst.copyTo (image_final, mask);
	  imshow ("Salida", image_final);
	  
	  filename << argv[1] << "_conv_" << x << "_" << y << "_" << z << ".jpg";  
//	  imwrite(filename.str(), image_final);
	  filename.str("");
	  waitKey(1);
	}
      }
    }     

  
}
