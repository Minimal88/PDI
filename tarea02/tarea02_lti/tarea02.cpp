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

#include <ltiV4l2.h>
#include <ltiIOImage.h> // To read/write images from files (jpg, png, and bmp)
#include <ltiViewer2D.h>

#include <ltiLispStreamHandler.h>

// Ensure that the STL streaming is used.
using std::cout;
using std::cerr;
using std::endl;


static const char* confFile = "parameters.lsp";  

/*
 * Help
 */
void usage()  {


  cout <<
    "usage: tarea02 [options] \n\n" \
    "  -h        show this help\n"\
    "  x:        Exit program"<< std::endl;

    cout << "  l :        Reload camera parameters\n";
  

    cout << "  up button :        Increase brightness\n";
    cout << "  down button :        Decrease brightness\n";

    cout << "  PgUpKey :        Increase contrast\n";
    cout << "  PgDownKey :        Decrease contrast\n";


    cout << "  Scroll Up :        Increase saturation\n";
    cout << "  Scroll Down :        Decrease saturation\n";
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
      default:
        break;
      }
    } 
  }
}

/*
 * Main program
 */
int main(int argc, char* argv[]) {

  // check if the user gave some filename
  
  
  parseArgs(argc,argv);

    
  //Camera parameters  
  lti::v4l2::parameters param;

  //Try to read the configuration file
  std::ifstream in(confFile);
  bool write = true;
  
  if (in) 
  {
    lti::lispStreamHandler lsh;
    lsh.use(in);
    write =! param.read(lsh);
  }

  if (write) 
  {
    // something went wrong reading, write a new configuration file
    std::ofstream out(confFile);
    lti::lispStreamHandler lsh;
    lsh.use(out);
    param.write(lsh);
  }

  //Create the detector with the user specified configuration
  lti::v4l2 cam(param);
  lti::image img;
  lti::viewer2D::parameters vPar;
  static lti::viewer2D orig(vPar);

  if (!cam.apply(img)) 
  {
    cout << "Error: " << cam.getStatusString() << endl;
    exit(EXIT_FAILURE);
  }

  lti::viewer2D::interaction action;
  lti::ipoint pos;

  //Initial values for image features
  float contrast = 0.5;
  float brightness = 1;
  float saturation = 0.5;

  //Control flags
  bool cam_enable = true;
  bool update = false;
  
  //Main Loop
  while(cam_enable)
  {
    cam_enable = cam.apply(img);
    orig.show(img);

    orig.getLastAction(action,pos);
    if (action.action == lti::viewer2D::KeyPressed 
        || action.action == lti::viewer2D::ButtonPressed)  
    {
      switch(action.key)
      {          

        
        case lti::viewer2D::UpKey:  //Increase brightness
          brightness = lti::min(1.0f, brightness + 0.1f);
          cout << "Increasing Brightness to: " << brightness<< endl;
          update = true;
          break;
        case lti::viewer2D::DownKey: //Decrease brightness 
          brightness = lti::max(0.0f, brightness - 0.1f);
          cout << "Decreasing Brightness to: "<< brightness << endl;
          update = true;
          break;



        case lti::viewer2D::PageUpKey: //Increase Contrast
          contrast = lti::min(1.0f, contrast + 0.1f);
          cout << "Increasing Contrast to: " << contrast << endl;
          update = true;
          break;          
        case lti::viewer2D::PageDownKey: //Decrease Contrast 
          contrast = lti::max(0.0f, contrast - 0.1f);
          cout << "Decreasing Contrast to: " << contrast << endl;          
          update = true;
          break;
        

        
        case lti::viewer2D::WheelUp: //Increase Saturation
          saturation = lti::min(1.0f, saturation + 0.1f);
          cout << "Increasing Saturation to: " << saturation << endl;
          update = true;
          break;
        case lti::viewer2D::WheelDown:  //Decrease Saturation
          saturation = lti::max(0.0f, saturation - 0.1f);
          cout << "Decreasing Saturation to: " << saturation << endl;
          update = true;
          break;

        case 'x':
          std::cout << "Good bye" << endl;
            cam_enable = false;
            break;

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
      }
    }

    //Updating the image
    if (update){
      cam.setContrast(lti::FeatureManual, contrast);
      cam.setSaturation(lti::FeatureManual,saturation);
      cam.setBrightness(lti::FeatureManual, brightness);
      update = false;
    }
  }

  //Exiting program
  std::ofstream out(confFile);
  lti::lispStreamHandler lsh;
  lsh.use(out);
  cam.dumpParameters(param);
  param.write(lsh);

  //****************************************************

  return EXIT_SUCCESS;
  

  return EXIT_FAILURE;
}
