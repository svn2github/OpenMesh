#ifndef FILL_PROPS_HH
#define FILL_PROPS_HH

#include <OpenMesh/Core/Utils/Property.hh>
#include "int2roman.hh"


template <typename Mesh>
bool 
fill_props( Mesh& _m, OpenMesh::VPropHandleT<float> _ph, bool _check=false)
{
  float  v;
  static float a[9] = { 1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f, 7.7f, 8.8f, 9.9f };

  for(typename Mesh::VertexIter it=_m.vertices_begin(); 
      it != _m.vertices_end(); ++it)
  {
    v = a[it.handle().idx()%9];
    if ( _check && !(_m.property( _ph, it ) == v) )
      return false;
    else  
      _m.property( _ph, it ) = v;
  }
  return true;
}


template <typename Mesh>
bool 
fill_props( Mesh& _m, OpenMesh::EPropHandleT<bool> _ph, bool _check=false )
{
  size_t n;
  bool   v;
  for( typename Mesh::EdgeIter it=_m.edges_begin();
       it != _m.edges_end(); ++it)
  {
    n = it.handle().idx();
    v = ((n&(n-1))==0); // true for 0,1,2,4,8,..

    if (_check && _m.property( _ph, it ) != v)
    {
      std::cout << "    eprop_bool: " << n << " -> " 
                << _m.property(_ph, it ) << " != " << v << std::endl;
      return false;
    }
    else
    {
      _m.property( _ph, it ) = v;
      std::cout << "    eprop_bool: " << n << " -> " << v << std::endl;
    }
  }
  return true;
}



template <typename Mesh>
bool 
fill_props(Mesh& _m, OpenMesh::FPropHandleT<std::string> _ph, bool _check=false)
{
  int  n;
  for( typename Mesh::FaceIter it=_m.faces_begin();
       it != _m.faces_end(); ++it)
  {
    n = it.handle().idx();
    _m.property( _ph, it ) = int2roman(++n);
  }
  return true;
}


template <typename Mesh, typename T>
bool 
fill_props( Mesh& _m, OpenMesh::HPropHandleT<T> _ph, bool _check=false)
{
  int  n;
  T    v;  
  static float a[9] = { 1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f, 7.7f, 8.8f, 9.9f };
  static float b[9] = { 2.2f, 3.3f, 4.4f, 5.5f, 6.6f, 7.7f, 8.8f, 9.9f, 1.1f };
  static float c[9] = { 3.3f, 4.4f, 5.5f, 6.6f, 7.7f, 8.8f, 9.9f, 1.1f, 2.2f };
  static float d[9] = { 4.4f, 5.5f, 6.6f, 7.7f, 8.8f, 9.9f, 1.1f, 2.2f, 3.3f };
  static double values[9] = { 0.1, 0.02, 0.003, 0.0004, 0.00005, 0.000006,
                              0.0000007, 0.00000008, 0.000000009 };

  for( typename Mesh::HalfedgeIter it=_m.halfedges_begin();
       it != _m.halfedges_end(); ++it)
  {
    n = it.handle().idx();

//    v = it.handle().idx()+1; // ival
//    v = values[n%9];         // dval
    v = ((n&(n-1))==0);      // bval
    v.vec4fval[0] = a[n%9];
    v.vec4fval[1] = b[n%9];
    v.vec4fval[2] = c[n%9];
    v.vec4fval[3] = d[n%9];

    if ( _check && _m.property( _ph, it ) != v )
      return false;
    else
      _m.property( _ph, it ) = v;
  }
  return true;
}

template <typename Mesh, typename T>
bool 
fill_props( Mesh& _m, OpenMesh::MPropHandleT<T> _ph, bool _check=false)
{
  size_t idx;
  for( typename Mesh::FaceIter it=_m.faces_begin(); it != _m.faces_end(); ++it)
  {
    idx = it.handle().idx();
    if ( _check && _m.property( _ph )[int2roman(idx+1)] != idx )
      return false;
    else
      _m.property( _ph )[int2roman(idx+1)] = idx;
  }
  return true;
}


#endif
