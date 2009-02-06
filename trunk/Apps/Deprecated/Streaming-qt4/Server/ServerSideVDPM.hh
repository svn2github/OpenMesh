#ifndef OPENMESH_APP_SERVERSIDEVDPM_HH
#define OPENMESH_APP_SERVERSIDEVDPM_HH

#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <OpenMesh/Tools/VDPM/VHierarchy.hh>
#include <vector>
#include <string>
#include <string.h>


using OpenMesh::VDPM::VHierarchy;
using OpenMesh::VDPM::VHierarchyNodeHandleContainer;


class ServerSideVDPM
{
private:

  char  name_[256];

  std::vector<OpenMesh::Vec3f>  points_;        // points related to this pm
  std::vector<OpenMesh::Vec3ui> triangles_;     // base mesh triangles
  VHierarchy                    vhierarchy_;

  unsigned int  n_base_vertices_;
  unsigned int  n_base_faces_;
  unsigned int  n_details_;
  unsigned char tree_id_bits_;  

public:

  ServerSideVDPM()  { clear(); }
  void clear();

  const char* name() const { return name_; }

  void get_name(char _name[256])            { strcpy(_name, name_); }
  void set_name(const char _name[256])      { strcpy(name_, _name); }


  std::string
  basename(const std::string& _f)
  {
    std::string::size_type dot = _f.rfind("/");
    if (dot == std::string::npos)
      return std::string(_f);
    return std::string(_f.substr(dot+1, _f.length()-(dot+1)));
  }


  bool is_same_name(const char _name[256])  
  {     
    return (basename( name_ ) == basename( _name ));
  }

  OpenMesh::VertexHandle add_vertex(const OpenMesh::Vec3f &p);
  OpenMesh::FaceHandle add_face(const unsigned int _triangle[3]);

  const OpenMesh::Vec3f& point(OpenMesh::VertexHandle _vertex_handle) const
  {
    return  points_[_vertex_handle.idx()];
  }

  OpenMesh::Vec3f& point(OpenMesh::VertexHandle _vertex_handle)
  {
    return  points_[_vertex_handle.idx()];
  }

  const VHierarchy& vhierarchy() const      { return  vhierarchy_; }
  VHierarchy& vhierarchy()                  { return  vhierarchy_; }

  void vhierarchy_roots(VHierarchyNodeHandleContainer &roots) const;
  
  unsigned int n_base_vertices() const      { return  n_base_vertices_; }
  unsigned int n_base_faces() const         { return  n_base_faces_; }
  unsigned int n_details() const            { return  n_details_; }  

  bool open_vd_prog_mesh(const char *_filename);
};

std::ostream& operator << ( std::ostream& _os, const ServerSideVDPM& _o );


typedef std::vector<ServerSideVDPM>   ServerSideVDPMContainer;
typedef std::list<ServerSideVDPM>     ServerSideVDPMList;

#endif //OPENMESH_APP_SERVERSIDEVDPM_HH defined

