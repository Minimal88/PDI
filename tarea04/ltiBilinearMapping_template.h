/*
 * Copyright (C) 2012
 * Pablo Alvarado
 *
 * This file is part of the LTI-Computer Vision Library 2 (LTI-Lib-2)
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
 * \file   ltiBilinearMapping.cpp
 *         Contains the class lti::bilinearMapping, to perform a complex
 *         valued bilinear geometrical transformation of an image
 * 
 * \author Pablo Alvarado
 * \date   09.06.2012
 *
 * revisions ..: $Id: ltiBilinearMapping.cpp,v $
 */

#include "ltiBilinearMapping.h"

namespace lti {
  // --------------------------------------------------
  // bilinearMapping<T,I>::parameters
  // --------------------------------------------------

  // default constructor
  template <typename T, template<class> class I>
  bilinearMapping<T,I>::parameters::parameters()
    : geometricTransform<T,I>::parameters() {
    
    a = fcomplex(2.1f,2.1f);
    b = fcomplex(0.0f);
    c = fcomplex(0.003f);
    d = fcomplex(1.0f,1.0f);
  }

  // copy constructor
  template <typename T, template<class> class I>
  bilinearMapping<T,I>::parameters::parameters(const parameters& other)
    : geometricTransform<T,I>::parameters() {
    copy(other);
  }

  // destructor
  template <typename T, template<class> class I>
  bilinearMapping<T,I>::parameters::~parameters() {
  }

  // copy member

  template <typename T, template<class> class I>
  typename bilinearMapping<T,I>::parameters&
  bilinearMapping<T,I>::parameters::copy(const parameters& other) {
    geometricTransform<T,I>::parameters::copy(other);
   
    c = other.c;
    a = other.a;
    b = other.b;
    d = other.d;

    return *this;
  }

  // alias for copy method
  template <typename T, template<class> class I>
  typename bilinearMapping<T,I>::parameters&
  bilinearMapping<T,I>::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // class name
  template <typename T, template<class> class I>
  const std::string& bilinearMapping<T,I>::parameters::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone method
  template <typename T, template<class> class I>
  typename bilinearMapping<T,I>::parameters*
  bilinearMapping<T,I>::parameters::clone() const {
    return new parameters(*this);
  }

  // new instance
  template <typename T, template<class> class I>
  typename bilinearMapping<T,I>::parameters*
  bilinearMapping<T,I>::parameters::newInstance() const {
    return new parameters();
  }

  /*
   * Write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was successful
   */
  template <typename T, template<class> class I>
  bool bilinearMapping<T,I>::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    bool bb = true;
    if (complete) {
      bb = handler.writeBegin();
    }

    if (bb) {
      
      lti::write(handler,"c",c);
      lti::write(handler,"a",a);
      lti::write(handler,"b",b);
      lti::write(handler,"d",d);
    }

    bb = bb && geometricTransform<T,I>::parameters::write(handler,false);

    if (complete) {
      bb = bb && handler.writeEnd();
    }

    return bb;
  }

  /*
   * Read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if read was successful
   */
  template <typename T, template<class> class I>
  bool bilinearMapping<T,I>::parameters::read(ioHandler& handler,
                                              const bool complete)  {
    bool bb = true;
    if (complete) {
      bb = handler.readBegin();
    }

    if (bb) {
      
      lti::read(handler,"c",c);
      lti::read(handler,"a",a);
      lti::read(handler,"b",b);
      lti::read(handler,"d",d);
    }

    bb = bb && geometricTransform<T,I>::parameters::read(handler,false);

    if (complete) {
      bb = bb && handler.readEnd();
    }

    return bb;
  }

  // --------------------------------------------------
  // bilinearMapping
  // --------------------------------------------------

  // default constructor
  template <typename T, template<class> class I>
  bilinearMapping<T,I>::bilinearMapping()
    : geometricTransform<T,I>() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    this->setParameters(defaultParameters);
  }

  // default constructor
  template <typename T, template<class> class I>
  bilinearMapping<T,I>::bilinearMapping(const parameters& par)
    : geometricTransform<T,I>() {

    // set the given parameters
    this->setParameters(par);
  }


  // copy constructor
  template <typename T, template<class> class I>
  bilinearMapping<T,I>::bilinearMapping(const bilinearMapping& other)
    : geometricTransform<T,I>() {
    copy(other);
  }

  // destructor
  template <typename T, template<class> class I>
  bilinearMapping<T,I>::~bilinearMapping() {
  }

  // copy member
  template <typename T, template<class> class I>
  bilinearMapping<T,I>& 
  bilinearMapping<T,I>::copy(const bilinearMapping& other) {
    geometricTransform<T,I>::copy(other);

    return (*this);
  }

  // alias for copy member
  template <typename T, template<class> class I>
  bilinearMapping<T,I>&
  bilinearMapping<T,I>::operator=(const bilinearMapping& other) {
    return (copy(other));
  }

  // class name
  template <typename T, template<class> class I>
  const std::string& bilinearMapping<T,I>::name() const {
    _LTI_RETURN_CLASS_NAME
  }

  // clone member
  template <typename T, template<class> class I>
  bilinearMapping<T,I>* bilinearMapping<T,I>::clone() const {
    return new bilinearMapping(*this);
  }

  // create a new instance
  template <typename T, template<class> class I>
  bilinearMapping<T,I>* bilinearMapping<T,I>::newInstance() const {
    return new bilinearMapping();
  }

  // return parameters
  template <typename T, template<class> class I>
  const typename bilinearMapping<T,I>::parameters&
  bilinearMapping<T,I>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(name());
    }
    return *par;
  }

  template <typename T, template<class> class I>
  bool bilinearMapping<T,I>::updateParameters() {
    const parameters& par = getParameters();

    a_=par.a;
    b_=par.b;
    c_=par.c;
    d_=par.d;

    fcomplex det = b_*c_ - a_*d_;

    if (abs(det)<std::numeric_limits<float>::epsilon()) {

      a_=ai_=0.0f;
      b_=bi_=0.0f;
      c_=ci_=0.0f;
      d_=di_=0.0f;
      
      return false;
    }

    ai_=-d_;
    bi_= b_;
    ci_= c_;
    di_=-a_;
  
    return true;
  }

  // -------------------------------------------------------------------
  // The apply() member functions
  // -------------------------------------------------------------------

  // On copy apply for type matrix<T>!
  template <typename T, template<class> class I>
  bool bilinearMapping<T,I>::apply(const matrix<T>& src,
                                   matrix<T>& dest,
                                   fpoint& offset) const {

    // TODO: Put your code here.
    //       the 'offset' should contain at the end a necessary shift
    //       to make the output image just fit into the dest container

    // Dummy implementation just copies the src image
    dest.copy(src);

    return true;
  }

  // On copy apply for type matrix<T>!
  template <typename T, template<class> class I>
  bool bilinearMapping<T,I>::apply(const matrix<T>& src,
                                   matrix<T>& dest) const {
    fpoint offset(0.0f,0.0f);
    return apply(src,dest,offset);
  }
  
  // On place apply for type matrix<T>!
  template <typename T, template<class> class I>
  bool bilinearMapping<T,I>::apply(matrix<T>& srcdest) const {
    // just use the parent class implementation 
    return geometricTransform<T,I>::apply(srcdest);
  }
  
  template <typename T, template<class> class I>
  bool bilinearMapping<T,I>::apply(matrix<T>& srcdest,fpoint& offset) const {
    // just use the parent class implementation 
    return geometricTransform<T,I>::apply(srcdest,offset);
  }

  template <typename T, template<class> class I>
  fcomplex bilinearMapping<T,I>::forwards(const fcomplex& z) const {

    // TODO: Put your code here
    //       Dummy implementation just returns the input z
    return z;
  }

  template <typename T, template<class> class I>
  fcomplex bilinearMapping<T,I>::backwards(const fcomplex& z) const {

    // TODO: Put your code here
    //       Dummy implementation just returns the input z
    return z;
  }


}

