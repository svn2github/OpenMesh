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


//== INCLUDES =================================================================


// OpenMesh
#include <OpenMesh/Core/IO/reader/OBJReader.hh>
#include <OpenMesh/Core/IO/IOManager.hh>
#include <OpenMesh/Core/System/omstream.hh>
#include <OpenMesh/Core/Utils/vector_cast.hh>
#include <OpenMesh/Core/Utils/color_cast.hh>
// STL
#if defined(OM_CC_MIPS)
#  include <ctype.h>
/// \bug Workaround for STLPORT 4.6: isspace seems not to be in namespace std!
#elif defined(_STLPORT_VERSION) && (_STLPORT_VERSION==0x460)
#  include <cctype>
#else
#  include <cctype>
using std::isspace;
#endif

#ifndef WIN32
#include <string.h>
#endif

//=== NAMESPACES ==============================================================


namespace OpenMesh {
namespace IO {


//=== INSTANCIATE =============================================================


_OBJReader_  __OBJReaderInstance;
_OBJReader_& OBJReader() { return __OBJReaderInstance; }


//=== IMPLEMENTATION ==========================================================


_OBJReader_::
_OBJReader_()
{
  IOManager().register_module(this);
}


//-----------------------------------------------------------------------------


bool
_OBJReader_::
read(const std::string& _filename, BaseImporter& _bi, Options& _opt)
{
  std::fstream in( _filename.c_str(), std::ios_base::in );

  if (!in)
  {
    omerr() << "[OBJReader] : cannot not open file "
          << _filename
          << std::endl;
    return false;
  }

  {
#if defined(WIN32)
    std::string::size_type dot = _filename.find_last_of("\\/");
#else
    std::string::size_type dot = _filename.rfind("/");
#endif
    path_ = (dot == std::string::npos)
      ? "./"
      : std::string(_filename.substr(0,dot+1));
  }

  bool result = read(in, _bi, _opt);

  in.close();
  return result;
}


//-----------------------------------------------------------------------------

bool
_OBJReader_::
read_material(std::fstream& _in)
{
  std::string line;
  std::string keyWrd;

  std::string key;
  Material    mat;
  float       f1,f2,f3;
  bool        indef = false;


  mat.cleanup();

  while( _in && !_in.eof() )
  {
    std::getline(_in,line);
    if ( _in.bad() ){
      omerr() << "  Warning! Could not read file properly!\n";
      return false;
    }

    if ( line.empty() )
      continue;

    std::stringstream stream(line);

    stream >> keyWrd;

    if( isspace(line[0]) || line[0] == '#' )
    {
      if (indef && !key.empty() && mat.is_valid())
      {
        materials_[key] = mat;
        mat.cleanup();
      }
    }

    else if (keyWrd == "newmtl") // begin new material definition
    {
      stream >> key;
      indef = true;
    }

    else if (keyWrd == "Kd") // diffuse color
    {
      stream >> f1; stream >> f2; stream >> f3;

      if( !stream.fail() )
        mat.set_Kd(f1,f2,f3);
    }

    else if (keyWrd == "Ka") // ambient color
    {
      stream >> f1; stream >> f2; stream >> f3;

      if( !stream.fail() )
        mat.set_Ka(f1,f2,f3);
    }

    else if (keyWrd == "Ks") // specular color
    {
      stream >> f1; stream >> f2; stream >> f3;

      if( !stream.fail() )
        mat.set_Ks(f1,f2,f3);
    }
#if 0
    else if (keyWrd == "illum") // diffuse/specular shading model
    {
      ; // just skip this
    }

    else if (keyWrd == "Ns") // Shininess [0..200]
    {
      ; // just skip this
    }

    else if (keyWrd == "map_") // map images
    {
      // map_Kd, diffuse map
      // map_Ks, specular map
      // map_Ka, ambient map
      // map_Bump, bump map
      // map_d,  opacity map
      ; // just skip this
    }
#endif

    else if (keyWrd == "Tr") // transparency value
    {
      stream >> f1;

      if( !stream.fail() )
        mat.set_Tr(f1);
    }
    else if (keyWrd == "d") // transparency value
    {
      stream >> f1;

      if( !stream.fail() )
        mat.set_Tr(f1);
    }

    if ( _in && indef && mat.is_valid() && !key.empty())
      materials_[key] = mat;
  }
  return true;
}

//-----------------------------------------------------------------------------


bool
_OBJReader_::
read(std::fstream& _in, BaseImporter& _bi, Options& _opt)
{
  omlog() << "[OBJReader] : read file\n";


  std::string line;
  std::string keyWrd;

  float                  x, y, z, u, v;

  BaseImporter::VHandles vhandles;
  std::vector<Vec3f>     normals;
  std::vector<Vec2f>     texcoords;

  std::vector<Vec2f>     face_texcoords;

  std::map<std::string,Material> materials;
  std::string            matname;


  while( _in && !_in.eof() )
  {
    std::getline(_in,line);
    if ( _in.bad() ){
      omerr() << "  Warning! Could not read file properly!\n";
      return false;
    }

    // Trim Both leading and trailing spaces

    size_t start = line.find_first_not_of(" \t");
    size_t end   = line.find_last_not_of(" \t");

    if(( std::string::npos == start ) || ( std::string::npos == end))
      line = "";
    else
      line = line.substr( start, end-start+1 );

    // comment
    if ( line.size() == 0 || line[0] == '#' || isspace(line[0]) ) {
      continue;
    }

    std::stringstream stream(line);

    stream >> keyWrd;

    // material file
    if (keyWrd == "mtllib")
    {
      std::string matFile;

      stream >> matFile;

      matFile = path_ + matFile;

      omlog() << "Load material file " << matFile << std::endl;

      std::fstream matStream( matFile.c_str(), std::ios_base::in );

      if ( matStream ){

        if ( !read_material( matStream ) )
	        omerr() << "  Warning! Could not read file properly!\n";
        matStream.close();
	      omlog() << "  " << materials_.size() << " materials loaded.\n";

      }else
	      omerr() << "  Warning! Material file '" << matFile << "' not found!\n";
    }

    // usemtl
    else if (keyWrd == "usemtl")
    {
      stream >> matname;
      if (materials_.find(matname)==materials_.end())
      {
        omerr() << "Warning! Material '" << matname
              << "' not defined in material file.\n";
        matname="";
      }
    }

    // vertex
    else if (keyWrd == "v")
    {
      stream >> x; stream >> y; stream >> z;

      if ( !stream.fail() )
        _bi.add_vertex(OpenMesh::Vec3f(x,y,z));
    }

    // texture coord
    else if (keyWrd == "vt")
    {
      stream >> u; stream >> v;

      if ( !stream.fail() ){

        texcoords.push_back(OpenMesh::Vec2f(u, v));
        _opt += Options::VertexTexCoord;

      }else{

        omerr() << "Only single 2D texture coordinate per vertex"
              << "allowed!" << std::endl;
        return false;
      }
    }


    // normal
    else if (keyWrd == "vn")
    {
      stream >> x; stream >> y; stream >> z;

      if ( !stream.fail() ){
        normals.push_back(OpenMesh::Vec3f(x,y,z));
        _opt += Options::VertexNormal;
      }
    }


    // face
    else if (keyWrd == "f")
    {
      int component(0), nV(0);
      int value;

      vhandles.clear();
      face_texcoords.clear();

      // read full line after detecting a face
      std::string faceLine;
      std::getline(stream,faceLine);
      std::stringstream lineData( faceLine );

      // work on the line until nothing left to read
      while ( !lineData.eof() )
      {
        // read one block from the line ( vertex/texCoord/normal )
        std::string vertex;
        lineData >> vertex;

        do{

          //get the component (vertex/texCoord/normal)
          size_t found=vertex.find("/");

          // parts are seperated by '/' So if no '/' found its the last component
          if( found != std::string::npos ){

            // read the index value
            std::stringstream tmp( vertex.substr(0,found) );

            // If we get an empty string this property is undefined in the file
            if ( vertex.substr(0,found).empty() ) {
              // Switch to next field
              vertex = vertex.substr(found+1);

              // Now we are at the next component
              ++component;

              // Skip further processing of this component
              continue;
            }

            // Read current value
            tmp >> value;

            // remove the read part from the string
            vertex = vertex.substr(found+1);

          } else {

            // last component of the vertex, read it.
            std::stringstream tmp( vertex );
            tmp >> value;

            // Clear vertex after finished reading the line
            vertex="";

            // Nothing to read here ( garbage at end of line )
            if ( tmp.fail() ) {
              continue;
            }
          }

          // store the component ( each component is referenced by the index here! )
          switch (component)
          {
            case 0: // vertex
              if ( value < 0 ) {
//                 std::cerr << "Handling negativ vertex index value" << std::endl;
                value = _bi.n_vertices() + value + 1;
              }
              vhandles.push_back(VertexHandle(value-1));
              break;

            case 1: // texture coord
              if ( value < 0 ) {
//                 std::cerr << "Handling negativ texture coordinate index value)" << std::endl;
                value = _bi.n_vertices() + value + 1;
              }
              assert(!vhandles.empty());
              if ( ! texcoords.empty() && (unsigned int)(value-1) < texcoords.size() ) {
                _bi.set_texcoord(vhandles.back(), texcoords[value-1]);
                face_texcoords.push_back( texcoords[value-1] );
              } else {
                omerr() << "Error setting Texture coordinates" << std::endl;
              }

              break;

            case 2: // normal
              if ( value < 0 ) {
//                 std::cerr << "Handling negativ normal index value)" << std::endl;
                value = _bi.n_vertices() + value + 1;
              }
              assert(!vhandles.empty());
              assert((unsigned int)(value-1) < normals.size());
              _bi.set_normal(vhandles.back(), normals[value-1]);
              break;
          }

          // Prepare for reading next component
          ++component;

          // Read until line does not contain any other info
        } while ( !vertex.empty() );

        component = 0;
        nV++;
      }


      size_t n_faces = _bi.n_faces();
      FaceHandle fh = _bi.add_face(vhandles);

      if( !vhandles.empty() )
	_bi.add_face_texcoords( fh, vhandles[0], face_texcoords );

      if ( !matname.empty() && materials_[matname].has_Kd() )
      {
        std::vector<FaceHandle> newfaces;

        for( size_t i=0; i < _bi.n_faces()-n_faces; ++i )
          newfaces.push_back(FaceHandle(n_faces+i));

        Material & mat = materials_[matname];

        Vec3uc fc = color_cast<Vec3uc, Vec3f>(mat.Kd());

        for (std::vector<FaceHandle>::iterator it = newfaces.begin();
                                               it != newfaces.end(); ++it)
          _bi.set_color( *it, fc );

        _opt += Options::FaceColor;
      }
    }
  }

  return true;
}


//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
