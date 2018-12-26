/*
 * Copyright (C) 2009-2015 by Pablo Alvarado
 * 
 * This file is part of the course MP-6123 Digital Image Processing,
 * at the Costa Rica Institute of Technology.
 *
 * The LTI-Lib-2 is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** 
 * \file   tarea03.cpp
 *         Homework for first contact with the LTI-Lib-2
 * \author Esteban Martinez
 * \date   2018
 * revisions ..: $Id: pwc.cpp,v 1.2 2009/06/05 12:59:33 alvarado Exp $
 */

// Standard Headers: from ANSI C and GNU C Library
#include <cstdlib>  // Standard Library for C++
#include <getopt.h> // Functions to parse the command line arguments

// Standard Headers: STL
#include <iostream>
#include <string>
#include <fstream>

// LTI-Lib Headers
#include <ltiObject.h>
#include <ltiMath.h>     // General lti:: math and <cmath> functionality
#include <ltiTimer.h>    // To measure time

#include <ltiIOImage.h> // To read/write images from files (jpg, png, and bmp)
#include <ltiViewer2D.h>

#include <ltiLispStreamHandler.h>

#include <ltiDraw.h>

// Ensure that the STL streaming is used.
using std::cout;
using std::cerr;
using std::endl;



//#include "ltiBresenhamLine.h"

/*
 * Help 
 */
void usage()  {

  
  cout <<
    "usage: tarea03 [options] [<file>]\n\n" \
    "       -h               show this help\n"\
    "       <file>           input image" << std::endl;    
}

/*
 * Parse the line command arguments
 * 
 * Set the filename with the given parameter, or an empty string otherwise
 */
void parse(int argc, char*argv[],std::string& filename) {

  int c;

  // We use the standard getopt.h functions here to parse the arguments.
  // Check the documentation of getopt.h for more information on this
  
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

  // Now try to read the image name
  if (optind < argc) {           // if there are still some arguments left...
    filename = argv[optind];  // with the given image file
  } else {
    filename = "";
  }
}

/**
* Draw a line segment between two given points.
*
* If one or two of the points lie outside the image then this function ...
*
* @param img Image where the line is to be drawn
* @param color Color of the line to be drawn
* @param from Initial point of line segment
* @param end Final point of line segment
*/
template<typename T>
void line(lti::matrix<T>& img,lti::viewer2D &view, lti::rgbaPixel px,const lti::ipoint& from, const lti::ipoint& to){
  int dx, dy, d, incrE, incrNE, x, y, stop, dirX, dirY ,x1,x2,y1,y2;

  x1 = from.y;
  y1 = from.x;

  x2 = to.y;
  y2 = to.x;  

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
      img.at(x,y) = px; //Change the pixels 
      y += dirY;
    }
  }
  else if(dy == 0){
    // Horizontal line
    stop = x2 + dirX;
    while(x != stop){
      img.at(x,y) = px; //Change the pixels 
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
      img.at(x,y) = px; //Change the pixels 
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
      img.at(x,y) = px; //Change the pixels 
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
  printf("asdasda\n");
  view.show(img); // show the image
 
  
}




/*
 * Main method
 */
int main(int argc, char* argv[]) {
  
  // check if the user gave some filename 
  std::string filename;
  parse(argc,argv,filename);

  if (!filename.empty()) {
    // the user just wants to show an image:

    // we need an object to load images
    lti::ioImage loader;

    // we also need an image object
    lti::image img;

    // load the image
    if (loader.load(filename,img)) {
      // image successfully loaded

      // we need a viewer 
      static lti::viewer2D view;

      // and get its parameters
      lti::viewer2D::parameters vpar(view.getParameters());
      vpar.title = filename; // set the image name at the window's title bar
      view.setParameters(vpar);
      view.show(img); // show the image

      //void line(lti::matrix<T>& img, const T& color,const lti::ipoint& from, const lti::ipoint& to){
      lti::ipoint start,end;
       
      // wait for the user to close the window or to indicate
      bool theEnd = false;
      lti::viewer2D::interaction action;
      lti::ipoint pos;
      int colorID;
      int turn=0; //0 for start, 1 for end
      lti::rgbaPixel px;

      do {
        std::cout<<"Select a point in the image..." << std::endl;

        view.waitInteraction(action,pos); // wait for something to happen
        if (action == lti::viewer2D::Closed) { // window closed?
          theEnd = true; // we are ready here!
        }
        else if(action==lti::viewer2D::ButtonReleased){
          if (turn==0){
            start=pos;
            turn=1;
            std::cout<<"\nFirst position: " << start << std::endl;
          }else{
            end=pos;
            turn=0;
            std::cout<<"\nSecond position: " << end << std::endl;

            //As positions are chosen by ltipoint, positions always are going to be inside the image dimensions

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
                  px = lti::rgbaPixel(0,0,0);
                  break;
                case 1: //White
                  px = lti::rgbaPixel(255,255,255);
                  break;
                case 2: //Red
                  px = lti::rgbaPixel(255,0,0);
                  break;
                case 3: //Green
                  px = lti::rgbaPixel(0,255,0);
                  break;
                case 4: //Blue
                  px = lti::rgbaPixel(0,0,255);
                  break;            
                default: //Black (default)
                  px = lti::rgbaPixel(0,0,0);
                  break;
              }

            //Draw the line with Bressenham algorithm            
            line(img,view,px,start,end);

          }
        }
      } while(!theEnd);
      
      
      return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
    
  } else {
    cout << "Try giving a image filename" << endl;
    usage();

    return EXIT_SUCCESS;
  }

  return EXIT_FAILURE;
}