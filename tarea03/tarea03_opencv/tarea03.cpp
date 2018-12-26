#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <getopt.h>

using namespace std;
using namespace cv;

int X1,X2,Y1,Y2;
int turn=0; //0 for start, 1 for end, 2 for color
/*
 * Help
 */
void usage()  {


  cout <<
    "usage: ./tarea03 [options] [<file>]\n\n" \
    "       -h        show this help\n"\
    "       <file>           input image" << std::endl;
}

/*
 * Parse the line command arguments
 *
 * Set the filename with the given parameter, or an empty string otherwise
 */
void parse(int argc, char*argv[],std::string& filename) {

  int c;

  // standard getopt.h functions used to parse the arguments.
  // structure for the long options.
  static struct option lopts[] = {
    {"help",no_argument,0,'h'},
    {0,0,0,0}
  };

  int optionIdx;

  while ((c = getopt_long(argc, argv, "h", lopts,&optionIdx)) != -1) {
    switch (c) {
    case 'h':
      usage();
      exit(EXIT_SUCCESS);
    default:
      cerr << "Option '-" << static_cast<char>(c) << "' not recognized."
           << endl;
    }
  }

  // Try to read the image name
  if (optind < argc) {           // if there are still some arguments left...
    filename = argv[optind];  // with the given image file
  } else {
    filename = "";
  }
}




void CallBackFunc(int event, int x, int y, int flags, void* userdata){
  if ( event == EVENT_LBUTTONDOWN ){
    if (turn==0){
      cout<<"\nFirst position: (" << x << ", " << y << ")" << endl;
      //cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
      X1=x;
      Y1=y;
      turn=1;
    }else if (turn==1){
      cout<<"\nSecond position: (" << x << ", " << y << ")" << endl;
      X2=x;
      Y2=y;
      turn=2;
    }

  }
}



void line(Mat& img, Vec3b px,int X1, int Y1, int X2, int Y2){
  int dx, dy, d, incrE, incrNE, x, y, stop, dirX, dirY ,x1,x2,y1,y2;
  Mat orig;

  x1 = Y1;
  y1 = X1;

  x2 = Y2;
  y2 = X2;  

  // origin
  x = x1;
  y = y1;
  
  // the derivative of the line
  dx = x2 - x1;
  dy = y2 - y1;
  
  // the direction in each coordinate
  dirX = (dx > 0) ? 1 : -1;
  dirY = (dy > 0) ? 1 : -1;

  if(dx == 0){
    // Vertical line
    stop = y2 + dirY;
    while(y != stop){
      img.at<Vec3b>(x,y) = px; //Change the pixels 
      y += dirY;
    }
  }
  else if(dy == 0){
    // Horizontal line
    stop = x2 + dirX;
    while(x != stop){
      img.at<Vec3b>(x,y) = px; //Change the pixels 
      x += dirX;
    }
  }
  else if(abs(dx) > abs(dy)){
    // Walk on X 
    stop = x2 + dirX;
    incrE = 2 * dy * dirY;
    incrNE = 2 * (dy * dirY - dx * dirX);
    d = 2 * dy * dirY - dx * dirX;
    while(x != stop){
      img.at<Vec3b>(x,y) = px; //Change the pixels 
      x += dirX;
      if(d <= 0) { 
        d += incrE; 
      }
      else { 
        d += incrNE; 
        y += dirY; 
      }
    }
  }
  else{
    // Walk on Y
    stop = y2 + dirY;
    incrE = 2 * dx * dirX;
    incrNE = 2 * (dx * dirX - dy * dirY);
    d = 2 * dx * dirX - dy * dirY;
    while(y != stop){
      img.at<Vec3b>(x,y) = px; //Change the pixels 
      y += dirY;
      if(d <= 0) { 
        d += incrE; 
      }
      else { 
        d += incrNE; 
        x += dirX; 
      }
    }
  }


}



/*
* Main Function
*/
int main( int argc, char** argv )
{
  // check if the user gave the required arguments
  std::string filename;  
  int colorID; 

  parse(argc,argv,filename);

  if (!filename.empty()) 
  {
    
    Mat img;
    Mat orig;
    Vec3b px;
    img = imread(filename, CV_LOAD_IMAGE_COLOR);   // Read the file    
    orig = imread(filename, CV_LOAD_IMAGE_COLOR);   // Read the file

    if (img.data) 
    {
      // image successfully loaded

      // Asign name to the viewer
      namedWindow(filename,WINDOW_AUTOSIZE);      // Create a window for display.
      imshow(filename,img);     // show the image


       //set the callback function for any mouse event
      setMouseCallback(filename, CallBackFunc, NULL);



      // wait for the user to close the window or to indicate
      bool theEnd = false;

      do {
        char press = (char) waitKey(10);          
        if(turn==2){ //Second point already selected
          //Choose a color for the line
          cout << "\nChoose one color, of the following options: " << endl;
          cout << "ID-Color" << endl;
          cout << "_______" << endl;
          cout << "0-Black" << endl;        
          cout << "1-White" << endl;        
          cout << "2-Red  " << endl;        
          cout << "3-Green" << endl;        
          cout << "4-Blue " << endl;      
          cout << "Enter the color ID: ";  
          std::cin >> colorID;

          switch(colorID)
            {
              case 0: //Black
                px = Vec3b(0,0,0);
                cout << "black";  
                break;
              case 1: //White
                px = Vec3b(255,255,255);
                break;
              case 2: //Red
                px = Vec3b(255,0,0);
                break;
              case 3: //Green
                px = Vec3b(0,255,0);
                break;
              case 4: //Blue
                px = Vec3b(0,0,255);
                break;            
              default: //Black (default)
                px = Vec3b(0,0,0);
                break;
            }

          //Draw the line with Bressenham algorithm     
          line(img,px,X1,Y1,X2,Y2);

          imshow(filename,img);    // show the image
          //img = orig.clone();
          
          /*
          destroyWindow(filename);
          namedWindow(filename,WINDOW_AUTOSIZE);      // Create a window for display.
          imshow(filename,img);     // show the image*/


          turn=0;

        }
        

          //imshow( "Loaded Image", img);     // show the image

          //img = orig.clone();
      } while(!theEnd);

      return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;

  }
  cout << "ERROR: You need to specify an image name as an input argument." << endl;
  return EXIT_FAILURE;
}