
namespace CGAL {

/*!
\ingroup PkgPolyhedron

The class `Polyhedron_items_3` is a model of the `PolyhedronItems_3` 
concept. It provides definitions for vertices with points, halfedges, 
and faces with plane equations. The polyhedron traits class must 
provide the respective types for the point and the plane equation. 
Vertices and facets both contain a halfedge handle to an incident 
halfedge. 

\models \ref ::PolyhedronItems_3 

\sa \ref ::CGAL::Polyhedron_3<Traits> 
\sa \ref ::CGAL::Polyhedron_min_items_3 
\sa \ref ::CGAL::HalfedgeDS_min_items 
\sa \ref ::CGAL::HalfedgeDS_items_2 

Example 
-------------- 

The following example program defines a new face class based on the 
`CGAL::HalfedgeDS_face_base` and adds a new color member variable. 
The new face class is used to replace the face definition in the 
`CGAL::Polyhedron_items_3` class. The main function illustrates the 
access to the new member variable. 
See also the `PolyhedronItems_3` concept for another illustrative 
example. 

\cgalexample{polyhedron_prog_color.cpp} 

*/

class Polyhedron_items_3 {
public:

/// \name Types in `Polyhedron_items_3::Face_wrapper<Refs,Traits>::Vertex`
/// @{

`Polyhedron_items_3::Vertex_wrapper<Refs,Traits>::Vertex` 
/*! 

*/ 
typedef Traits::Point_3 Point; 

`Polyhedron_items_3::Vertex_wrapper<Refs,Traits>::Vertex` 
/*! 

*/ 
typedef CGAL::Tag_true Supports_vertex_point; 

/// @} 

/// \name Types in `Polyhedron_items_3::Face_wrapper<Refs,Traits>::Face` 
/// @{

/*! 

*/ 
typedef Traits::Plane_3 Plane; 

/*! 

*/ 
typedef CGAL::Tag_true Supports_face_plane; 

/// @} 

/// \name Creation 
/// @{

/*! 
default constructor. 
*/ 
Polyhedron_items_3(); 

/// @}

}; /* end Polyhedron_items_3 */
} /* end namespace CGAL */
