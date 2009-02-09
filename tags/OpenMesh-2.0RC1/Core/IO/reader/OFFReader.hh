/*===========================================================================*\
 *                                                                           *
 *                               OpenMesh                                    *
 *        Copyright (C) 2003 by Computer Graphics Group, RWTH Aachen         *
 *                           www.openmesh.org                                *
 *                                                                           *
 *---------------------------------------------------------------------------* 
 *                                                                           *
 *                                License                                    *
 *                                                                           *
 *  This library is free software; you can redistribute it and/or modify it  *
 *  under the terms of the GNU Lesser General Public License as published    *
 *  by the Free Software Foundation, version 2.1.                            *
 *                                                                           *
 *  This library is distributed in the hope that it will be useful, but      *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of               *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU        *
 *  Lesser General Public License for more details.                          *
 *                                                                           *
 *  You should have received a copy of the GNU Lesser General Public         *
 *  License along with this library; if not, write to the Free Software      *
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                *
 *                                                                           *
\*===========================================================================*/


//=============================================================================
//
//  Implements a reader module for OFF files
//
//=============================================================================


#ifndef __OFFREADER_HH__
#define __OFFREADER_HH__


//=== INCLUDES ================================================================


#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>

#include <OpenMesh/Core/System/config.h>
#include <OpenMesh/Core/Utils/SingletonT.hh>
#include <OpenMesh/Core/IO/reader/BaseReader.hh>

#ifndef WIN32
#include <string.h>
#endif

//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace IO {


//== FORWARDS =================================================================


class BaseImporter;


//== IMPLEMENTATION ===========================================================


/** 
    Implementation of the OFF format reader. This class is singleton'ed by 
    SingletonT to OFFReader.

    By passing Options to the read function you can manipulate the reading behavoir.
    The following options can be set:

    VertexNormal
    VertexColor
    VertexTexCoord
    FaceColor
    ColorAlpha [only when reading binary]

    These options define if the corresponding data should be read (if available)
    or if it should be omitted.

    After execution of the read function. The options object contains information about
    what was actually read.

    e.g. if VertexNormal was true when the read function was called, but the file
    did not contain vertex normals then it is false afterwards.

    When reading a binary off with Color Flag in the header it is assumed that all vertices
    and faces have colors in the format "int int int".
    If ColorAlpha is set the format "int int int int" is assumed.

*/

class _OFFReader_ : public BaseReader
{
public:

  _OFFReader_();

  std::string get_description() const { return "Object File Format"; }
  std::string get_extensions()  const { return "off"; }
  std::string get_magic()       const { return "OFF"; }
   
  bool read(const std::string& _filename, 
	    BaseImporter& _bi, 
	    Options& _opt);

  bool can_u_read(const std::string& _filename) const;



private:

  bool can_u_read(std::istream& _is) const;

  bool read(std::fstream& _in, BaseImporter& _bi, Options& _opt ) const;
  bool read_ascii(std::fstream& _in, BaseImporter& _bi) const;
  bool read_binary(std::fstream& _in, BaseImporter& _bi, bool swap) const;
   
  void readValue(std::fstream& _in, float& _value) const;
  void readValue(std::fstream& _in, int& _value) const;
  void readValue(std::fstream& _in, unsigned int& _value) const;

  int getColorType(std::string & _line, bool _texCoordsAvailable) const;
  
  //available options for reading
  mutable Options options_;
  //options that the user wants to read
  mutable Options userOptions_;
};


//== TYPE DEFINITION ==========================================================


/// Declare the single entity of the OFF reader
extern _OFFReader_  __OFFReaderInstance;
_OFFReader_&  OFFReader();


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
#endif
//=============================================================================
