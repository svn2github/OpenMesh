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


#define LINE_LEN 4096


//== INCLUDES =================================================================

// OpenMesh
#include <OpenMesh/Core/System/config.h>
#include <OpenMesh/Core/System/omstream.hh>
#include <OpenMesh/Core/IO/reader/PLYReader.hh>
#include <OpenMesh/Core/IO/importer/BaseImporter.hh>
#include <OpenMesh/Core/IO/IOManager.hh>
#include <OpenMesh/Core/Utils/color_cast.hh>
#include <OpenMesh/Core/IO/SR_store.hh>

//STL
#include <fstream>
#include <memory>

#ifndef WIN32
#include <string.h>
#endif

//=== NAMESPACES ==============================================================


namespace OpenMesh {
namespace IO {


//=============================================================================

//=== INSTANCIATE =============================================================


_PLYReader_  __PLYReaderInstance;
_PLYReader_&  PLYReader() { return __PLYReaderInstance; }


//=== IMPLEMENTATION ==========================================================



_PLYReader_::_PLYReader_()
{
  IOManager().register_module(this);
}


//-----------------------------------------------------------------------------


bool
_PLYReader_::read(const std::string& _filename, BaseImporter& _bi,
                  Options& _opt)
{
  std::fstream in(_filename.c_str(), (options_.is_binary() ? std::ios_base::binary | std::ios_base::in
                                                           : std::ios_base::in) );

  if (!in)
  {
    omerr() << "[PLYReader] : cannot not open file "
	  << _filename
	  << std::endl;
    return false;
  }


  bool result = read(in, _bi, _opt);


  in.close();
  return result;
}


//-----------------------------------------------------------------------------


bool
_PLYReader_::read(std::fstream& _in, BaseImporter& _bi, Options& _opt ) const
{
   // filter relevant options for reading
   bool swap = _opt.check( Options::Swap );


   userOptions_ = _opt;

   // build options to be returned
   _opt.clear();

   if (options_.vertex_has_normal() && userOptions_.vertex_has_normal())     _opt += Options::VertexNormal;
   if (options_.vertex_has_texcoord() && userOptions_.vertex_has_texcoord()) _opt += Options::VertexTexCoord;
   if (options_.vertex_has_color() && userOptions_.vertex_has_color())       _opt += Options::VertexColor;
   if (options_.face_has_color() && userOptions_.face_has_color())           _opt += Options::FaceColor;
   if (options_.is_binary())                                                 _opt += Options::Binary;

//    //force user-choice for the alpha value when reading binary
//    if ( options_.is_binary() && userOptions_.color_has_alpha() )
//      options_ += Options::ColorAlpha;

    return (options_.is_binary() ?
 	   read_binary(_in, _bi, swap) :
	   read_ascii(_in, _bi));

}



//-----------------------------------------------------------------------------

bool
_PLYReader_::read_ascii(std::fstream& _in, BaseImporter& _bi) const
{


  omlog() << "[PLYReader] : read ascii file\n";

  // Reparse the header
  if ( !can_u_read(_in) ) {
    omerr() << "[PLYReader] : Unable to parse header\n";
    return false;
  }



  unsigned int            i, j, k, l, idx;
  unsigned int            nV;
  OpenMesh::Vec3f         v;
  std::string             trash;
//   OpenMesh::Vec2f         t;
  OpenMesh::Vec4i         c;
  float                   tmp;
  BaseImporter::VHandles  vhandles;
  VertexHandle            vh;


 _bi.reserve(vertexCount_, 3*vertexCount_, faceCount_);

 if ( vertexDimension_ != 3 ) {
    omerr() << "[PLYReader] : Only vertex dimension 3 is supported." << std::endl;
    return false;
 }

  // read vertices:
  for (i=0; i< vertexCount_ && !_in.eof(); ++i)
  {
    v[0] = 0.0;
    v[1] = 0.0;
    v[2] = 0.0;

    c[0] = 0;
    c[1] = 0;
    c[2] = 0;
    c[3] = 255;

    for ( uint propertyIndex = 0 ; propertyIndex < vertexPropertyCount_; ++propertyIndex ) {
      switch ( vertexPropertyMap_ [ propertyIndex].first ) {
        case XCOORD:
          _in >> v[0];
          break;
        case YCOORD:
          _in >> v[1];
          break;
        case ZCOORD:
          _in >> v[2];
          break;
        case COLORRED:
          if ( vertexPropertyMap_ [ propertyIndex].second == ValueTypeFLOAT32 ){
            _in >> tmp; c[0] = static_cast<OpenMesh::Vec4i::value_type>(tmp * 255.0f);
          }else
            _in >> c[0];
          break;
        case COLORGREEN:
          if ( vertexPropertyMap_ [ propertyIndex].second == ValueTypeFLOAT32 ){
            _in >> tmp; c[1] = static_cast<OpenMesh::Vec4i::value_type>(tmp * 255.0f);
          }else
            _in >> c[1];
          break;
        case COLORBLUE:
          if ( vertexPropertyMap_ [ propertyIndex].second == ValueTypeFLOAT32 ){
            _in >> tmp; c[2] = static_cast<OpenMesh::Vec4i::value_type>(tmp * 255.0f);
          }else
            _in >> c[2];
          break;
        case COLORALPHA:
          if ( vertexPropertyMap_ [ propertyIndex].second == ValueTypeFLOAT32 ){
            _in >> tmp; c[3] = static_cast<OpenMesh::Vec4i::value_type>(tmp * 255.0f);
          }else
            _in >> c[3];
          break;
        default :
          _in >> trash;
          break;
      }
    }

    vh = _bi.add_vertex(v);
    _bi.set_color( vh, Vec4uc( c ) );
  }

  // faces
  // #N <v1> <v2> .. <v(n-1)> [color spec]
  for (i=0; i<faceCount_; ++i)
  {
    // nV = number of Vertices for current face
    _in >> nV;

    if (nV == 3)
    {
      vhandles.resize(3);
      _in >> j;
      _in >> k;
      _in >> l;

      vhandles[0] = VertexHandle(j);
      vhandles[1] = VertexHandle(k);
      vhandles[2] = VertexHandle(l);
    }
    else
    {
      vhandles.clear();
      for (j=0; j<nV; ++j)
      {
         _in >> idx;
         vhandles.push_back(VertexHandle(idx));
      }
    }

    FaceHandle fh = _bi.add_face(vhandles);

  }

  // File was successfully parsed.
  return true;
}


//-----------------------------------------------------------------------------

void _PLYReader_::readValue(ValueType _type , std::fstream& _in, float& _value) const{

  switch (_type) {
    case ValueTypeFLOAT32:
    case ValueTypeFLOAT:
      float32_t tmp;
      restore( _in , tmp, options_.check(Options::MSB) );
      _value = tmp;
      break;
    default :
      _value = 0.0;
      std::cerr << "unsupported conversion type to float: " << _type << std::endl;
      break;
  }
}

void _PLYReader_::readValue(ValueType _type , std::fstream& _in, unsigned int& _value) const{

  int32_t tmp_int32_t;
  uint8_t tmp_uchar;

  switch (_type) {
    case ValueTypeINT:
    case ValueTypeINT32:
      restore( _in , tmp_int32_t, options_.check(Options::MSB) );
      _value = tmp_int32_t;
      break;
    case ValueTypeUCHAR:
      restore( _in , tmp_uchar, options_.check(Options::MSB) );
      _value = tmp_uchar;
      break;
    default :
      _value = 0;
      std::cerr << "unsupported conversion type to int: " << _type << std::endl;
      break;
  }
}

void _PLYReader_::readValue(ValueType _type , std::fstream& _in, int& _value) const{

  int32_t tmp_int32_t;
  uint8_t tmp_uchar;

  switch (_type) {
    case ValueTypeINT:
    case ValueTypeINT32:
      restore( _in , tmp_int32_t, options_.check(Options::MSB) );
      _value = tmp_int32_t;
      break;
    case ValueTypeUCHAR:
      restore( _in , tmp_uchar, options_.check(Options::MSB) );
      _value = tmp_uchar;
      break;
    default :
      _value = 0;
      std::cerr << "unsupported conversion type to int: " << _type << std::endl;
      break;
  }
}

bool
_PLYReader_::read_binary(std::fstream& _in, BaseImporter& _bi, bool /*_swap*/) const
{

  omlog() << "[PLYReader] : read binary file format\n";

  // Reparse the header
  if ( !can_u_read(_in) ) {
    omerr() << "[PLYReader] : Unable to parse header\n";
    return false;
  }

  unsigned int            i, j, k, l, idx;
  unsigned int            nV;
  OpenMesh::Vec3f         v;
  BaseImporter::VHandles  vhandles;
  VertexHandle            vh;
  OpenMesh::Vec4i         c;
  float                   tmp;

  _bi.reserve(vertexCount_, 3*vertexCount_, faceCount_);

  // read vertices:
  for (i=0; i< vertexCount_ && !_in.eof(); ++i)
  {
    v[0] = 0.0;
    v[1] = 0.0;
    v[2] = 0.0;

    c[0] = 0;
    c[1] = 0;
    c[2] = 0;
    c[3] = 255;

    for ( uint propertyIndex = 0 ; propertyIndex < vertexPropertyCount_; ++propertyIndex ) {
      switch ( vertexPropertyMap_ [ propertyIndex].first ) {
        case XCOORD:
          readValue(vertexPropertyMap_ [ propertyIndex].second,_in,v[0]);
          break;
        case YCOORD:
          readValue(vertexPropertyMap_ [ propertyIndex].second,_in,v[1]);
          break;
        case ZCOORD:
          readValue(vertexPropertyMap_ [ propertyIndex].second,_in,v[2]);
          break;
        case COLORRED:
          if ( vertexPropertyMap_ [ propertyIndex].second == ValueTypeFLOAT32 ){
            readValue(vertexPropertyMap_ [ propertyIndex].second,_in,tmp);

            c[0] = static_cast<OpenMesh::Vec4i::value_type>(tmp * 255.0f);
          }else
            readValue(vertexPropertyMap_ [ propertyIndex].second,_in,c[0]);
          break;
        case COLORGREEN:
          if ( vertexPropertyMap_ [ propertyIndex].second == ValueTypeFLOAT32 ){
            readValue(vertexPropertyMap_ [ propertyIndex].second,_in,tmp);
            c[1] = static_cast<OpenMesh::Vec4i::value_type>(tmp * 255.0f);
          }else
            readValue(vertexPropertyMap_ [ propertyIndex].second,_in,c[1]);
          break;
        case COLORBLUE:
          if ( vertexPropertyMap_ [ propertyIndex].second == ValueTypeFLOAT32 ){
            readValue(vertexPropertyMap_ [ propertyIndex].second,_in,tmp);
            c[2] = static_cast<OpenMesh::Vec4i::value_type>(tmp * 255.0f);
          }else
            readValue(vertexPropertyMap_ [ propertyIndex].second,_in,c[2]);
          break;
        case COLORALPHA:
          if ( vertexPropertyMap_ [ propertyIndex].second == ValueTypeFLOAT32 ){
            readValue(vertexPropertyMap_ [ propertyIndex].second,_in,tmp);
            c[3] = static_cast<OpenMesh::Vec4i::value_type>(tmp * 255.0f);
          }else
            readValue(vertexPropertyMap_ [ propertyIndex].second,_in,c[3]);
          break;
        default :
          break;
      }

    }

    vh = _bi.add_vertex(v);
    _bi.set_color( vh, Vec4uc( c ) );
  }

  for (i=0; i<faceCount_; ++i)
  {
    // Read number of vertices for the current face
    readValue(faceIndexType_,_in,nV);

    if (nV == 3)
    {
      vhandles.resize(3);
      readValue(faceEntryType_,_in,j);
      readValue(faceEntryType_,_in,k);
      readValue(faceEntryType_,_in,l);

      vhandles[0] = VertexHandle(j);
      vhandles[1] = VertexHandle(k);
      vhandles[2] = VertexHandle(l);
    }
    else
    {
      vhandles.clear();
      for (j=0; j<nV; ++j)
      {
         readValue(faceEntryType_,_in,idx);
         vhandles.push_back(VertexHandle(idx));
      }
    }

    FaceHandle fh = _bi.add_face(vhandles);
  }

  return true;
}


//-----------------------------------------------------------------------------


bool _PLYReader_::can_u_read(const std::string& _filename) const
{
  // !!! Assuming BaseReader::can_u_parse( std::string& )
  // does not call BaseReader::read_magic()!!!
  if (BaseReader::can_u_read(_filename))
  {
    std::ifstream ifs(_filename.c_str());
    if (ifs.is_open() && can_u_read(ifs))
    {
      ifs.close();
      return true;
    }
  }
  return false;
}


//-----------------------------------------------------------------------------

std::string get_property_name(std::string _string1 , std::string _string2 ) {
  if ( _string1 == "float32" ||
       _string1 == "uint8"   ||
       _string1 == "uchar"   ||
       _string1 == "float"   ||
       _string1 == "int32")
    return _string2;

  if ( _string2 == "float32" ||
       _string2 == "uint8"   ||
       _string2 == "uchar"   ||
       _string2 == "float"   ||
       _string2 == "int32")
    return _string1;

  std::cerr << "Unsupported entry type" << std::endl;
  return "Unsupported";
}

//-----------------------------------------------------------------------------

_PLYReader_::ValueType get_property_type(std::string _string1 , std::string _string2 ) {
  if ( _string1 == "float32" || _string2 == "float32" )
    return _PLYReader_::ValueTypeFLOAT32;
  else if ( _string1 == "uint8"   || _string2 == "float32" )
    return _PLYReader_::ValueTypeUINT8;
  else if ( _string1 == "int32"   || _string2 == "float32" )
    return _PLYReader_::ValueTypeINT32;
  else if ( _string1 == "uchar"   || _string2 == "uchar" )
    return _PLYReader_::ValueTypeUCHAR;
  else if ( _string1 == "float"   || _string2 == "float" )
    return _PLYReader_::ValueTypeFLOAT;

  return _PLYReader_::Unsupported;
}

//-----------------------------------------------------------------------------

bool _PLYReader_::can_u_read(std::istream& _is) const
{
  // Clear per file options
  options_.cleanup();

  // clear vertex property map, will be recreated
  vertexPropertyMap_.clear();
  vertexPropertyCount_ = 0;

  // read 1st line
  std::string line;
  std::getline(_is,line);

  //Check if this file is really a ply format
  if ( line != "PLY" && line != "ply" )
    return false;

//   omlog() << "PLY header found" << std::endl;

  vertexCount_     = 0;
  faceCount_       = 0;
  vertexDimension_ = 0;

  std::string keyword;
  std::string fileType;
  std::string elementName = "";
  std::string propertyType;
  std::string propertyName;
  std::string listIndexType;
  std::string listEntryType;
  float       version;

  _is >> keyword;
  _is >> fileType;
  _is >> version;

  if ( _is.bad() ) {
    omerr() << "Defect PLY header detected" << std::endl;
    return false;
  }

  if ( fileType == "ascii" ) {
    options_ -= Options::Binary;
  } else if ( fileType == "binary_little_endian" ){
    options_ += Options::Binary;
    options_ += Options::LSB;
  } else if ( fileType == "binary_big_endian" ){
    options_ += Options::Binary;
    options_ += Options::MSB;
  } else {
    omerr() << "Unsupported PLY format: " << fileType << std::endl;
    return false;
  }

  unsigned int streamPos = _is.tellg();
  _is >> keyword;
  while ( keyword != "end_header") {

    if ( keyword == "comment" ) {
      std::getline(_is,line);
      omlog() << "PLY header comment : " << line << std::endl;
    } else if ( keyword == "element") {
      _is >> elementName;
      if ( elementName == "vertex" ) {
        _is >> vertexCount_;
      } else if ( elementName == "face" ){
        _is >> faceCount_;
      } else {
        omerr() << "PLY header unsupported element type: " << elementName << std::endl;
      }
    } else if ( keyword == "property") {
      std::string tmp1;
      std::string tmp2;

      // Read first keyword, as it might be a list
      _is >> tmp1;

      if ( tmp1 == "list" ) {
        if ( elementName == "vertex" ) {
          omerr() << "List type not supported for vertices!" << std::endl;
        } else if ( elementName == "face" ) {
          _is >> listIndexType;
          _is >> listEntryType;
          _is >> propertyName;

          if ( listIndexType == "uint8" ) {
            faceIndexType_ = ValueTypeUINT8;
          } else if ( listIndexType == "uchar" ) {
            faceIndexType_ = ValueTypeUCHAR;
          } else {
            omerr() << "Unsupported Index type for face list: " << listIndexType << std::endl;
          }

          if ( listEntryType == "int32"  ) {
            faceEntryType_ = ValueTypeINT32;
          } else if ( listEntryType == "int"  ) {
            faceEntryType_ = ValueTypeINT;
          } else {
            omerr() << "Unsupported Entry type for face list: " << listEntryType << std::endl;
          }

        }
      } else {
        // as this is not a list property, read second value of property
        _is >> tmp2;

        if ( elementName == "vertex" ) {
          // Extract name and type of property
          // As the order seems to be different in some files, autodetect it.
          ValueType valueType = get_property_type(tmp1,tmp2);
          propertyName        = get_property_name(tmp1,tmp2);

          if ( propertyName == "x" ) {
            std::pair< VertexProperty, ValueType>  entry(XCOORD,valueType);
            vertexPropertyMap_[vertexPropertyCount_] = entry;
            vertexDimension_++;
          } else if ( propertyName == "y" ) {
            std::pair< VertexProperty, ValueType>  entry(YCOORD,valueType);
            vertexPropertyMap_[vertexPropertyCount_] = entry;
            vertexDimension_++;
          } else if ( propertyName == "z" ) {
            std::pair< VertexProperty, ValueType>  entry(ZCOORD,valueType);
            vertexPropertyMap_[vertexPropertyCount_] = entry;
            vertexDimension_++;
          } else if ( propertyName == "red" ) {
            std::pair< VertexProperty, ValueType>  entry(COLORRED,valueType);
            vertexPropertyMap_[vertexPropertyCount_] = entry;
            options_ += Options::VertexColor;
          } else if ( propertyName == "green" ) {
            std::pair< VertexProperty, ValueType>  entry(COLORGREEN,valueType);
            vertexPropertyMap_[vertexPropertyCount_] = entry;
            options_ += Options::VertexColor;
          } else if ( propertyName == "blue" ) {
            std::pair< VertexProperty, ValueType>  entry(COLORBLUE,valueType);
            vertexPropertyMap_[vertexPropertyCount_] = entry;
            options_ += Options::VertexColor;
          } else if ( propertyName == "alpha" ) {
            std::pair< VertexProperty, ValueType>  entry(COLORALPHA,valueType);
            vertexPropertyMap_[vertexPropertyCount_] = entry;
            options_ += Options::VertexColor;
            options_ += Options::ColorAlpha;
          } else {
            std::cerr << "Unsupported property : " << propertyName << std::endl;
          }

          vertexPropertyCount_++;

        } else if ( elementName == "face" ) {
          omerr() << "Properties not supported for faces " << std::endl;
        }

      }

    } else {
      omlog() << "Unsupported keyword : " << keyword << std::endl;
    }

    streamPos = _is.tellg();
    _is >> keyword;
    if ( _is.bad() ) {
      omerr() << "Error while reading PLY file header" << std::endl;
      return false;
    }
  }

  // As the binary data is directy after the end_header keyword
  // and the stream removes too many bytes, seek back to the right position
  if ( options_.is_binary() ) {
    _is.seekg(streamPos + 12);
  }

  return true;
}


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
