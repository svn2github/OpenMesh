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
//  Implements a writer module for PLY files
//
//=============================================================================


#ifndef __PLYWRITER_HH__
#define __PWRITER_HH__


//=== INCLUDES ================================================================


#include <stdio.h>
#include <string>
#include <fstream>

#include <OpenMesh/Core/System/config.h>
#include <OpenMesh/Core/Utils/SingletonT.hh>
#include <OpenMesh/Core/IO/exporter/BaseExporter.hh>
#include <OpenMesh/Core/IO/writer/BaseWriter.hh>


//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace IO {


//=== IMPLEMENTATION ==========================================================


/** 
    Implementation of the PLY format writer. This class is singleton'ed by 
    SingletonT to PLYWriter.

    currently supported options:
    - VertexColors
    - Binary
    - Binary -> MSB
*/
class _PLYWriter_ : public BaseWriter
{
public:

  _PLYWriter_();

  /// Destructor
  virtual ~_PLYWriter_() {};

  std::string get_description() const { return "PLY polygon file format"; }
  std::string get_extensions() const  { return "ply"; }

  bool write(const std::string&, BaseExporter&, Options) const;

  size_t binary_size(BaseExporter& _be, Options _opt) const;

  enum ValueType {
    Unsupported ,
    ValueTypeFLOAT32, ValueTypeFLOAT,
    ValueTypeUINT8, ValueTypeINT32, ValueTypeINT ,
    ValueTypeUCHAR
  };

private:
  mutable Options options_;

protected:
  void writeValue(ValueType _type, std::fstream& _out, int value) const;
  void writeValue(ValueType _type, std::fstream& _out, unsigned int value) const;
  void writeValue(ValueType _type, std::fstream& _out, float value) const;

  bool write_ascii(std::fstream& _in, BaseExporter&, Options) const;
  bool write_binary(std::fstream& _in, BaseExporter&, Options) const;
};


//== TYPE DEFINITION ==========================================================


/// Declare the single entity of the PLY writer.
extern _PLYWriter_  __PLYWriterInstance;
_PLYWriter_& PLYWriter();


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
#endif
//=============================================================================
