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
 * \file   tarea04.cpp
 *         Homework for space transformation
 * \author Pablo Alvarado
 * \date   26.05.2012
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

#include <ltiIOImage.h>  // To read/write images from files (jpg, png, and bmp)
#include <ltiViewer2D.h>

#include <ltiLispStreamHandler.h>
#include "ltiBilinearInterpolation.h"
#include "ltiBilinearMapping.h"

// Ensure that the STL streaming is used.
using std::cout;
using std::cerr;
using std::endl;


/*
 * Help 
 */
void usage()  {

  
  cout <<
    "usage: tarea04 [options] [<file>]\n\n"   \
    "       -h|--help        show this help\n"\
    "       <file>           input image" << std::endl;    
}

/*
 * Parse the line command arguments
 * 
 * Set the filename with the given parameter, or an empty string otherwise
 */
void parse(int argc, char*argv[],
           std::string& filename) {

  // We use the standard getopt.h functions here to parse the arguments.
  // Check the documentation of getopt.h for more information on this
  
  // structure for the long options. 
  static struct option lopts[] = {
    {"help",no_argument,0,'h'},
    {0,0,0,0}
  };

  int optionIdx;
  int cc;

  while ((cc = getopt_long(argc, argv, "h", lopts,&optionIdx)) != -1) {
    switch (cc) {
    case 'h':
      usage();
      exit(EXIT_SUCCESS); 
    default:
      cerr << "Option '-" << static_cast<char>(cc) << "' not recognized." 
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

/*
 * Main method
 */
int main(int argc, char* argv[]) {
  
  // check if the use gave some filename 
  std::string filename;
  parse(argc,argv,filename);

  // parameters for our mapping object
  lti::bilinearMapping<lti::rgbaPixel>::parameters mpar;
  static const char* fname = "bilinear.dat";

  // try to read them from disk
  bool write=true;
  std::ifstream in(fname);

  if (in) {
    lti::lispStreamHandler lsh(in);
    write=mpar.read(lsh);
  } 
  if (write) {
    std::ofstream out(fname);
    lti::lispStreamHandler lsh(out);
    mpar.write(lsh);
    out << std::endl;
    out.close();
  }

  // Object to do the mapping
  lti::bilinearMapping<lti::rgbaPixel> mapper(mpar);

  if (!filename.empty()) {
    // we need an object to load images
    lti::ioImage loader;

    // we also need an image object
    lti::image img;

    // load the image
    if (loader.load(filename,img)) {
      // image successfully loaded

      // we need a viewer 
      static lti::viewer2D view(filename);
      view.show(img); // show the image
       
      // a container for the resulting image
      lti::image mapped;
      // perform the mapping
      mapper.apply(img,mapped);

      static lti::viewer2D viewm("Mapped");
      viewm.show(mapped);

      // wait for the user to close the mapped window 
      bool theEnd = false;
      lti::viewer2D::interaction action;
      lti::ipoint pos;

      do {
        viewm.waitInteraction(action,pos); // wait for something to happen
        if (action == lti::viewer2D::Closed) { // window closed?
          theEnd = true; // we are ready here!
        } 
      } while(!theEnd);
      
      return EXIT_SUCCESS;
    }
  }
  return EXIT_FAILURE;
}
