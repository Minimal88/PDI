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
 * \file   ltiBilinearMapping.h
 *         Contains the class lti::bilinearMapping, to perform a complex
 *         valued bilinear geometrical transformation of an image
 * 
 * \author Pablo Alvarado
 * \date   09.06.2012
 *
 * revisions ..: $Id: ltiBilinearMapping.h,v $
 */

#ifndef _LTI_BILINEAR_MAPPING_H_
#define _LTI_BILINEAR_MAPPING_H_

#include "ltiMatrix.h"
#include "ltiComplex.h"
#include "ltiGeometricTransform.h"

namespace lti {

  /**
   * Class bilinearMapping
   *
   * This class performs a geometrical transformation of an image by means of
   * a bilinear mapping.
   *
   * Assume the coordinates of the input image are \f$x\f$ and \f$y\f$.  Assume
   * a complex valued variable \f$z=x+iy\f$, is transformed by the bilinear
   * mapping
   * \f[
   *   w=\dfrac{az+b}{cz+d} 
   * \f]
   * where the real part of \f$w\f$ will be the horizontal coordinate of the
   * destination pixel and the imaginary part of \f$w\f$  will be the 
   * vertical coordinate of the destination image.
   *
   * This class makes the geometrical transformation following the
   * geometricTransform interface, which implies the use of an interpolation
   * functor.
   *
   * @see bilinearMapping::parameters.
   *
   */
  template<typename T, template<class> class I = bilinearInterpolation> 
  class bilinearMapping : public geometricTransform<T,I> {
  public:
    /**
     * The parameters for the class bilinearMapping
     */
    class parameters : public geometricTransform<T,I>::parameters {
    public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      ~parameters();

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * Returns the complete name of the parameters class.
       */
      virtual const std::string& name() const;

      /**
       * Returns a pointer to a clone of the parameters.
       */
      virtual parameters* clone() const;

      /**
       * Returns a pointer to a new instance of the parameters.
       */
      virtual parameters* newInstance() const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Coefficient \f$a\f$ of the mapping
       *
       * \f[
       *   w=\dfrac{az+b}{cz+d} 
       * \f]
       *
       * Default value: 2.1 + i2.1
       */
      fcomplex a;

      /**
       * Coefficient \f$b\f$ of the mapping
       *
       * \f[
       *   w=\dfrac{az+b}{cz+d} 
       * \f]
       *
       * Default value: 0
       */
      fcomplex b;

      /**
       * Coefficient \f$c\f$ of the mapping
       *
       * \f[
       *   w=\dfrac{az+b}{cz+d} 
       * \f]
       *
       * Default value: 0.003
       */
      fcomplex c;

      /**
       * Coefficient \f$d\f$ of the mapping
       *
       * \f[
       *   w=\dfrac{az+b}{cz+d} 
       * \f]
       *
       * Default value: 1+i
       */
      fcomplex d;

    };

    /**
     * Default constructor
     */
    bilinearMapping();

    /**
     * Construct a functor using the given parameters
     */
    bilinearMapping(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    bilinearMapping(const bilinearMapping<T,I>& other);

    /**
     * Destructor
     */
    virtual ~bilinearMapping();

    /**
     * Transforms the given image as matrix and leaves the result in the
     * same container.
     *
     * @param srcdest matrix<T> with the source data. The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(matrix<T>& srcdest) const;

    /**
     * Transforms the src image as matrix and leaves the result in the
     * dest container.
     *
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<T>& src, matrix<T>& dest) const;

    /**
     * Transform geometrically the given image and leave the result on the
     * same container.
     *
     * If the parameters specify to AdjustDimensions, then the offset
     * value will contain the relative position of the \a srcdest
     * origin with respect to the original image coordinate system.
     * To all other resize policies, the value of offset is set to
     * (0,0).
     *
     * @param srcdest matrix<T> with the source data. The result
     *                will be left here too.
     * @param offset position of the origin of the result with respect to the
     *               coordinate system of the original image.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(matrix<T>& srcdest,
                       fpoint& offset) const;

    /**
     * Transform geometrically the source image and leave the result on the
     * destination container.
     *
     *
     * If the parameters specify to AdjustDimensions, then the offset
     * value will contain the relative position of the \a srcdest
     * origin with respect to the original image coordinate system.
     * To all other resize policies, the value of offset is set to
     * (0,0).
     *
     * @param src matrix<value_type> with the source data.
     * @param dest matrix<value_type> where the result will be left.
     * @param offset position of the origin of the result with respect to the
     *               coordinate system of the original image.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<T>& src, 
                             matrix<T>& dest,
                             fpoint& offset) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    bilinearMapping& copy(const bilinearMapping& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    bilinearMapping& operator=(const bilinearMapping& other);

    /**
     * Returns the complete name of the functor class
     */
    virtual const std::string& name() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual bilinearMapping* clone() const;

    /**
     * Returns a pointer to a new instance of this functor.
     */
    virtual bilinearMapping* newInstance() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update functor state on a parameters change
     */
    virtual bool updateParameters();

  protected:
    /**
     * Shadow of the parameters
     */
    fcomplex a_,b_,c_,d_;
    
    /**
     * Shadow of the parameters for the inverse transformation
     */
    fcomplex ai_,bi_,ci_,di_;
  
    /**
     * Forward transformation 
     */
    fcomplex forwards(const fcomplex& z) const;

    /**
     * Forward transformation 
     */
    fcomplex backwards(const fcomplex& z) const;
  };
}

#include "ltiBilinearMapping_template.h"

#endif

