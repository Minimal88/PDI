#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"


#include "opencv2/core/core.hpp"




#include <stdio.h>
#include <iostream>
#include <stdint.h>


char key;
float brightness, contrast, saturation;
cv::VideoCapture cap(0);

void help() {
  printf("Keys");
  printf("  ? : Print this help\n");
  printf("  p : Print this help\n");
  printf("  x : Exit program\n");
  
  

  printf("  h (or HomeKey) : Increase brightness\n");
  printf("  f (or EndKey) : Decrease brightness\n");

  printf("  PgUpKey : Increase contrast\n");
  printf("  PgDownKey : Decrease contrast\n");

  printf("  Left mouse button : Increase saturation\n");
  printf("  Right mouse button : Decrease saturation\n");
}

void usage() {
  printf("Usage: tarea02 [options] [-h]\n");
  printf("Capture images with camera and display them.\n");
  printf("  -h show this help.\n");
  printf("  -c show original in color\n");
  help();
}

void parseArgs(int argc, char*argv[]) {
  
  // check each argument of the command line
  for (int i=1; i<argc; i++) {
    if (*argv[i] == '-') {
      switch (argv[i][1]) {
      case 'h':
        usage();
        exit(EXIT_SUCCESS);
        break;      
      default:
        break;
      }
    } 
  }
}


static void mouseHandler(int event,int x,int y, int flags,void* param){
    if (event==4){  //EVENT_LBUTTONUP       
        //INcrease saturation
        saturation = cap.get(CV_CAP_PROP_SATURATION);
        cap.set(CV_CAP_PROP_SATURATION,saturation+0.1);
        printf("Saturation: %f\n",saturation);
    }else if (event==6){  //EVENT_MBUTTONUP
        //Decrease saturation
        saturation = cap.get(CV_CAP_PROP_SATURATION);
        cap.set(CV_CAP_PROP_SATURATION,saturation-0.1);
        printf("Saturation: %f\n",saturation);
    }
   
}
     








int main( int argc, char* argv[] ) {

    parseArgs(argc,argv);

    cv::namedWindow( "Video Footage", cv::WINDOW_AUTOSIZE );
    
    if(!cap.isOpened())
        return -1;

    
    cv::Mat frame;



    for(;;) 
    {
        cap >> frame;
        cv::imshow( "Video Footage", frame );
        cv::setMouseCallback("Video Footage", mouseHandler, &frame);
        key = cv::waitKey(33);
        switch(key){
            case 80:    //Increase Brigtness
            case 104:{
                brightness = cap.get(CV_CAP_PROP_BRIGHTNESS);
                cap.set(CV_CAP_PROP_BRIGHTNESS,brightness+0.1);
                printf("Brightness: %f\n",brightness);
            }break;
            case 87:    //Decrease Brigtness
            case 102:{
                brightness = cap.get(CV_CAP_PROP_BRIGHTNESS);
                cap.set(CV_CAP_PROP_BRIGHTNESS,brightness-0.1);
                printf("Brightness: %f\n",brightness);
            }break;


            case 85:{ //Increase contrast
                contrast = cap.get(CV_CAP_PROP_CONTRAST);
                cap.set(CV_CAP_PROP_CONTRAST,contrast+0.1);
                printf("Contrast: %f\n",contrast);
            }break;
            case 86:{  //Decrease contrast
                contrast = cap.get(CV_CAP_PROP_CONTRAST);
                cap.set(CV_CAP_PROP_CONTRAST,contrast-0.1);
                printf("Contrast: %f\n",contrast);
            }break;
        }
        if(key == 120 ){break;} //exit


    }
return 0;
}


// Keys
//   ? : Print this help
//   p : Print this help
//   x : Exit program
//   - : Decrease space filter size
//   + : Increase space filter size
//   a : Decrease past time consideration
//   s : Increase past time consideration
//   l : Reload camera parameters
//   h (or HomeKey) : Increase brightness
//   f (or EndKey) : Decrease brightness
//   PgUpKey : Increase contrast
//   PgDownKey : Decrease contrast
//   Scroll Up : Increase saturation
//   Scroll Down : Decrease saturation