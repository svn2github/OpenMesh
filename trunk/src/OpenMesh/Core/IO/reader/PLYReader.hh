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


#ifndef __PLYREADER_HH__
#define __PLYREADER_HH__


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
    Implementation of the PLY format reader. This class is singleton'ed by
    SingletonT to OFFReader.

*/

class _PLYReader_ : public BaseReader
{
public:

  _PLYReader_();

  std::string get_description() const { return "PLY polygon file format"; }
  std::string get_extensions()  const { return "ply"; }
  std::string get_magic()       const { return "PLY"; }

  bool read(const std::string& _filename,
	    BaseImporter& _bi,
	    Options& _opt);

  bool can_u_read(const std::string& _filename) const;

  enum ValueType {
    Unsupported ,
    ValueTypeFLOAT32, ValueTypeFLOAT,
    ValueTypeUINT8, ValueTypeINT32, ValueTypeINT ,
    ValueTypeUCHAR
  };

private:

  bool can_u_read(std::istream& _is) const;

  bool read(std::fstream& _in, BaseImporter& _bi, Options& _opt ) const;
  bool read_ascii(std::fstream& _in, BaseImporter& _bi) const;
  bool read_binary(std::fstream& _in, BaseImporter& _bi, bool swap) const;

  float readToFloatValue(ValueType _type , std::fstream& _in) const;

  void readValue(ValueType _type , std::fstream& _in, float& _value) const;
  void readValue(ValueType _type , std::fstream& _in, unsigned int& _value) const;
  void readValue(ValueType _type , std::fstream& _in, int& _value) const;

  //available options for reading
  mutable Options options_;
  //options that the user wants to read
  mutable Options userOptions_;

  mutable unsigned int vertexCount_;
  mutable unsigned int faceCount_;

  mutable ValueType vertexType_;
  mutable uint vertexDimension_;

  mutable ValueType faceIndexType_;
  mutable ValueType faceEntryType_;

  enum VertexProperty {
    XCOORD,YCOORD,ZCOORD,
    TEXX,TEXY,
    COLORRED,COLORGREEN,COLORBLUE,COLORALPHA
  };


  // number of vertex properties
  mutable unsigned int vertexPropertyCount_;
  mutable std::map< int , std::pair< VertexProperty, ValueType> > vertexPropertyMap_;

};


//== TYPE DEFINITION ==========================================================


/// Declare the single entity of the PLY reader
extern _PLYReader_  __PLYReaderInstance;
_PLYReader_&  PLYReader();


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
#endif
//=============================================================================
