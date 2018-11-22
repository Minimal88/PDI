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
 * \file   tarea02.cpp
 *         Homework for first contact with the LTI-Lib-2
 * \author Pablo Alvarado
 * \date   26.05.2012
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


#include "ltiConvolution.h"
#include "ltiGaussKernels.h"
#include "ltiLispStreamHandler.h"

#include "ltiPoint.h"

#include "ltiV4l2.h"
//#include "ltiViewer2D.h" // The normal viewer

// Ensure that the STL streaming is used.
using std::cout;
using std::cerr;
using std::endl;



using std::cout;
using std::cerr;
using std::endl;

bool color = false;

static const char* confFile = "parameters.lsp";

/*
 * Help 
 */
void help() {
  cout << "Keys" << endl;
  cout << "  ? : Print this help\n";
  cout << "  p : Print this help\n";
  cout << "  x : Exit program\n";
  cout << "  - : Decrease space filter size\n";
  cout << "  + : Increase space filter size\n";
  cout << "  a : Decrease past time consideration\n";
  cout << "  s : Increase past time consideration\n";
  cout << "  l : Reload camera parameters\n";

  cout << "  h (or HomeKey) : Increase brightness\n";
  cout << "  f (or EndKey) : Decrease brightness\n";

  cout << "  PgUpKey : Increase contrast\n";
  cout << "  PgDownKey : Decrease contrast\n";


  cout << "  Scroll Up : Increase saturation\n";
  cout << "  Scroll Down : Decrease saturation\n";
}

void usage() {
  cout << "Usage: tarea02 [options] [-h]" << endl;
  cout << "Capture images with the v4l2 camera and display them.\n";
  cout << "  -h show this help." << endl;
  cout << "  -c show original in color" << endl;
  help();
}

/*
 * Parse the line command arguments
 */
void parseArgs(int argc, char*argv[]) {
  
  // check each argument of the command line
  for (int i=1; i<argc; i++) {
    if (*argv[i] == '-') {
      switch (argv[i][1]) {
      case 'h':
        usage();
        exit(EXIT_SUCCESS);
        break;
      case 'c':
        color = true;
        break;
      default:
        break;
      }
    } 
  }
}


//int parameter -> 0: brightness, 1: contrast, 2: saturation 
//int updown -> 0: up, 1:down
lti::v4l2::parameters set_cam_parameters(int paramater, int updown){  
  // reload the camera parameters and try to set them

  
  lti::v4l2::parameters param, param2,param3;
  
  std::ifstream in(confFile);  

  //std::ofstream out(confFile);  


  lti::lispStreamHandler lsh,lsh_in;
  
  lsh.use(in);
  if (param.read(lsh)) {
    cout<<"\n"<<param.hue<<std::endl;

    //Hardware default (from GTC)
    
    param2=param;
    lti::lispStreamHandler lsh_out;
    switch(paramater){
      case 0:{
        if(updown){param2.brightness = param2.brightness - 1;}
        else{param2.brightness = param2.brightness + 1;}
        cout << "Brightness: "<<param2.brightness<<std::endl;        
      } break;
      case 1:{
        if(updown){param2.contrast = param2.contrast - 1;}
        else{param2.contrast = param2.contrast + 1;}
        cout << "Contrast: "<<param2.contrast<<std::endl;
      }break;
      case 2:{
        if(updown){param2.saturation = param2.saturation - 1;}
        else{param2.saturation = param2.saturation + 1;}
        cout << "Saturation: "<<param2.saturation<<std::endl;
      }break;
    }
    //param2.hue = 0;
    //param2.saturation = 64;
    //param2.contrast = 32;
    //param2.brightness = param2.brightness + 1;
    //param2.resolution = lti::ipoint(640,360);
    in.close();
    
    std::ofstream out(confFile);  
    lsh_out.use(out);
    param2.write(lsh_out);
    

    out.close();


    lsh_in.use(in);
    param3.read(lsh_in);    

    
    cout << "ok\nSetting camera parameters...";
    // if (cam.setParameters(param3)) {

    //   cout <<"ok" << std::endl;
    // } else {
    //   cout <<"failed: " << cam.getStatusString() << std::endl;
    // }
  } else {
    cout << "failed: " << lsh.getStatusString() << std::endl;
  }


  return param;
}

/*
 * Main method
 */
int main(int argc, char* argv[]) {
    
  parseArgs(argc,argv);

  

  lti::v4l2::parameters param,param2,param3;
 
  // try to read the configuration file
  std::ifstream in(confFile);
  bool write=true;
  if (in) {
    lti::lispStreamHandler lsh;
    lsh.use(in);
    write=!param.read(lsh);
  }
  if (write) {
    // something went wrong reading, write a new configuration file
    std::ofstream out(confFile);
    lti::lispStreamHandler lsh;
    lsh.use(out);
    param.write(lsh);
  }
  
  // create the detector with the user specified configuration
  lti::v4l2 cam(param);

  lti::image img;
  lti::channel x,last,y,s;

  int kSize = 1; // spatial filter
  float coef = 0.0f; // temporal filter
  
  
  lti::gaussKernel2D<float> gk(kSize);
  lti::convolution::parameters cp;
  cp.setKernel(gk);
  cp.boundaryType = lti::Constant;
  lti::convolution filter(cp);

  lti::viewer2D::interaction action,lastAction;
  lti::viewer2D::parameters vPar;
  vPar.mappingType = lti::viewer2DPainter::Optimal;
  vPar.title = "Filtered";


  static lti::viewer2D view(vPar);

  static lti::viewer2D orig("Original");

  vPar.title = "Subtracted";
  vPar.mappingType = lti::viewer2DPainter::OffsetSlope;
  vPar.offset=0.5;
  vPar.slope =0.75;

  lti::viewer2D subs(vPar);
  lti::ipoint pos;
  bool ok,update(false);
  

  if (!cam.apply(img)) {
    cout << "Error: " << cam.getStatusString() << endl;
    exit(EXIT_FAILURE);
  }
  last.assign(img.size(),0.0f);


  do {
    ok = cam.apply(img);
    orig.getLastAction(action,pos);
    
    if (ok && color) {  //shows original webcam 
      orig.show(img);
    }
    x.castFrom(img);
    filter.apply(x);
      
    
    y.addScaled(1.0f-coef,x,coef,last);
    view.show(y); //shows filtered webcam 
    y.swap(last);

    view.getLastAction(action,pos);



    if (action.action == lti::viewer2D::KeyPressed)  { //Obtains the 
      switch(action.key) {
      case 'x':
      case 'X':
        ok = false;
        break;
      case lti::viewer2D::RightKey:
      case '+':
        kSize = lti::min(kSize + 2,65);        
        update = true;
        break;
      case lti::viewer2D::LeftKey:
      case '-': {
        kSize = lti::max(1,kSize-2);
        update = true;
        break;

        
        
      } break;


      case lti::viewer2D::UpKey:
      case 'a':
      case 'A':
        // decrease filter coefficient  value
        coef = lti::max(-1.f,coef-0.01f);
        cout << "Coefficient: " << coef << endl;
        break;
      case lti::viewer2D::DownKey:
      case 's':
      case 'S':
      // increase filter coefficient  value
        coef = lti::min(1.0f,coef+0.01f);
        cout << "Coefficient: " << coef << endl;
        break;

      case lti::viewer2D::HomeKey:
      case 'h':
      case 'H':{  //Increase brightness
        cout << "Updating camera parameters! " << endl;
        param = set_cam_parameters(0,0);        
        cam.setParameters(param);
      }break;

      case lti::viewer2D::EndKey:
      case 'f':
      case 'F':{  //Decrease brightness
        cout << "Updating camera parameters! " << endl;
        param = set_cam_parameters(0,1);        
        cam.setParameters(param);
      }break;
      
      case lti::viewer2D::PageUpKey:{  //Increase contrast
        cout << "Updating camera parameters! " << endl;
        set_cam_parameters(1,0);        
      }break;
      
      case lti::viewer2D::PageDownKey:{  //Decrease contrast
        cout << "Updating camera parameters! " << endl;
        set_cam_parameters(1,1);        
      }break;





      case 'l': {
        // reload the camera parameters and try to set them
        std::ifstream in(confFile);
        cout << "Reloading camera parameters...";
        if (in) {
          lti::lispStreamHandler lsh;
          lsh.use(in);
          if (param.read(lsh)) {
            cout << "ok\nSetting camera parameters...";
            if (cam.setParameters(param)) {
              cout <<"ok" << std::endl;
            } else {
              cout <<"failed: " << cam.getStatusString() << std::endl;
            }
          } else {
            cout << "failed: " << lsh.getStatusString() << std::endl;
          }
        } else {
          cout <<"failed: could not open file '"<< confFile<<"'"<< std::endl;
        }


      } break;



      case '?':
      case 'p':{
        help();
        }break;
      default:
        cout << "<" << action.key << ">" << std::endl;
      }

      if (update) {
        cout << "Kernel: " << kSize << endl;
        gk.generate(kSize,lti::sqr((kSize+2.0f)/6.0f));
        filter.setKernel(gk);
        update=false;
      }
      
    }

    else if(action.action == lti::viewer2D::ButtonPressed){
      switch(action.key) {
        case lti::viewer2D::WheelUp:{ //Increase saturation 
          cout << "Updating camera parameters! " << endl;
          set_cam_parameters(2,0);        
        }break;

        case lti::viewer2D::WheelDown:{ //Decrease saturation 
          cout << "Updating camera parameters! " << endl;
          set_cam_parameters(2,1);        
        }break;
      }
      
    }
    lastAction = action;
  } while(ok);


  return EXIT_SUCCESS;





/*










  //!!!!!!!!!!!!!!PARTE ORIGINAL DE LA TAREA!!!!!!!!!!!!!!
  // check if the user gave some filename 
  std::string filename;
  //parse(argc,argv,filename);

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
       
      // wait for the user to close the window or to indicate
      bool theEnd = false;
      lti::viewer2D::interaction action;
      lti::ipoint pos;

      do {
        view.waitInteraction(action,pos); // wait for something to happen
        if (action == lti::viewer2D::Closed) { // window closed?
          theEnd = true; // we are ready here!
        } 
      } while(!theEnd);
      
      
      return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
    
  } else {

    // -----------------------------------
    // -----------------------------------
    //        PUT YOUR CODE HERE!!!    
    // -----------------------------------
    // -----------------------------------
    cout << "Not done yet!  Try giving a image name" << endl;
    usage();

    return EXIT_SUCCESS;
  }

  return EXIT_FAILURE;

  */
}
