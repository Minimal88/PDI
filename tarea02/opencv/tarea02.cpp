//tut2_dispVideo.cpp

#include "opencv2/opencv.hpp"

#include <stdio.h>

char key;

int main( int, char** ) {

    cv::namedWindow( "Video Footage", cv::WINDOW_AUTOSIZE );
    cv::VideoCapture cap(0);
    if(!cap.isOpened())
        return -1;

    
    cv::Mat frame;
    for(;;) 
    {
        cap >> frame;
        cv::imshow( "Video Footage", frame );
        key = cv::waitKey(33);
        if(key >= 0 ){
            printf("key: %i\n",key );
            break;
        }else{
            printf("mmmm si no se como está la vara\n");
        }
    }
return 0;
}