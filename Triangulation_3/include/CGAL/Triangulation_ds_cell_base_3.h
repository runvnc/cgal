// Copyright (c) 1999-2005  INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
// Author(s)     : Monique Teillaud <Monique.Teillaud@sophia.inria.fr>
//                 Sylvain Pion

// cell of a triangulation data structure of any dimension <=3

#ifndef CGAL_TRIANGULATION_DS_CELL_BASE_3_H
#define CGAL_TRIANGULATION_DS_CELL_BASE_3_H

#include <CGAL/basic.h>
#include <CGAL/triangulation_assertions.h>
#include <CGAL/internal/Dummy_tds_3.h>

#ifdef CGAL_LINKED_WITH_TBB
# include <tbb/atomic.h>
# include <boost/type_traits/is_base_of.hpp>
#endif

namespace CGAL {
  
// Without erase counter
template <bool Use_erase_counter, typename Concurrency_tag>
class Triangulation_ds_cell_base_3_base
{
};

// Specialized version (with erase counter)
template <typename Concurrency_tag>
class Triangulation_ds_cell_base_3_base<true, Concurrency_tag>
{
public:
  // Erase counter (cf. Compact_container)
  unsigned int get_erase_counter() const
  {
    return this->m_erase_counter;
  }
  void set_erase_counter(unsigned int c)
  {
	  this->m_erase_counter = c;
  }
  void increment_erase_counter()
  {
    ++this->m_erase_counter;
  }
  
protected:
  
#ifdef CGAL_LINKED_WITH_TBB
  typedef typename boost::mpl::if_c<
    boost::is_base_of<Parallel_tag, Concurrency_tag>::value,
    tbb::atomic<unsigned int>,
    unsigned int>::type             Erase_counter_type;
#else
  typedef unsigned int              Erase_counter_type;
#endif
  Erase_counter_type                m_erase_counter;

};



template < typename TDS = void >
class Triangulation_ds_cell_base_3
: public Triangulation_ds_cell_base_3_base<
    TDS::Cell_container_strategy::Uses_erase_counter,
    typename TDS::Concurrency_tag>
{
public:
  typedef TDS                           Triangulation_data_structure;
  typedef typename TDS::Vertex_handle   Vertex_handle;
  typedef typename TDS::Cell_handle     Cell_handle;
  typedef typename TDS::Vertex          Vertex;
  typedef typename TDS::Cell            Cell;
  typedef typename TDS::Cell_data       TDS_data;

  template <typename TDS2>
  struct Rebind_TDS { typedef Triangulation_ds_cell_base_3<TDS2> Other; };

  Triangulation_ds_cell_base_3() 
  {
#ifdef SHOW_REMAINING_BAD_ELEMENT_IN_RED
    mark = -1;
    mark2 = -1;
#endif
  }

  Triangulation_ds_cell_base_3(Vertex_handle v0, Vertex_handle v1,
                               Vertex_handle v2, Vertex_handle v3)
#ifndef CGAL_CFG_NO_CPP0X_UNIFIED_INITIALIZATION_SYNTAX
    : V{v0, v1, v2, v3}
  {
#ifdef SHOW_REMAINING_BAD_ELEMENT_IN_RED
    mark = -1;
    mark2 = -1;
#endif
  }
#else
  {
    set_vertices(v0, v1, v2, v3);
#ifdef SHOW_REMAINING_BAD_ELEMENT_IN_RED
    mark = -1;
    mark2 = -1;
#endif
  }
#endif

  Triangulation_ds_cell_base_3(Vertex_handle v0, Vertex_handle v1,
                               Vertex_handle v2, Vertex_handle v3,
                               Cell_handle   n0, Cell_handle   n1,
                               Cell_handle   n2, Cell_handle   n3)
#ifndef CGAL_CFG_NO_CPP0X_UNIFIED_INITIALIZATION_SYNTAX
    : N((Cell_handle[4]) {n0, n1, n2, n3}), V((Vertex_handle[4]) {v0, v1, v2, v3} ) {}
#else
  {
    set_neighbors(n0, n1, n2, n3);
    set_vertices(v0, v1, v2, v3);
#ifdef SHOW_REMAINING_BAD_ELEMENT_IN_RED
    mark = -1;
    mark2 = -1;
#endif
  }
#endif

  // ACCESS FUNCTIONS

  Vertex_handle vertex(int i) const
  {
    CGAL_triangulation_precondition( i >= 0 && i <= 3 );
    return V[i];
  }

  bool has_vertex(Vertex_handle v) const
  {
    return (V[0] == v) || (V[1] == v) || (V[2]== v) || (V[3]== v);
  }

  bool has_vertex(Vertex_handle v, int & i) const
    {
      if (v == V[0]) { i = 0; return true; }
      if (v == V[1]) { i = 1; return true; }
      if (v == V[2]) { i = 2; return true; }
      if (v == V[3]) { i = 3; return true; }
      return false;
    }

  int index(Vertex_handle v) const
  {
    if (v == V[0]) { return 0; }
    if (v == V[1]) { return 1; }
    if (v == V[2]) { return 2; }
    CGAL_triangulation_assertion( v == V[3] );
    return 3;
  }

  Cell_handle neighbor(int i) const
  {
    CGAL_triangulation_precondition( i >= 0 && i <= 3);
    return N[i];
  }

  bool has_neighbor(Cell_handle n) const
  {
    return (N[0] == n) || (N[1] == n) || (N[2] == n) || (N[3] == n);
  }

  bool has_neighbor(Cell_handle n, int & i) const
  {
    if(n == N[0]){ i = 0; return true; }
    if(n == N[1]){ i = 1; return true; }
    if(n == N[2]){ i = 2; return true; }
    if(n == N[3]){ i = 3; return true; }
    return false;
  }

  int index(Cell_handle n) const
  {
    if (n == N[0]) return 0;
    if (n == N[1]) return 1;
    if (n == N[2]) return 2;
    CGAL_triangulation_assertion( n == N[3] );
    return 3;
  }

  // SETTING

  void set_vertex(int i, Vertex_handle v)
  {
    CGAL_triangulation_precondition( i >= 0 && i <= 3);
    V[i] = v;
  }

  void set_neighbor(int i, Cell_handle n)
  {
    CGAL_triangulation_precondition( i >= 0 && i <= 3);
    CGAL_triangulation_precondition( this != &*n );
    N[i] = n;
  }

  void set_vertices()
  {
    V[0] = V[1] = V[2] = V[3] = Vertex_handle();
  }

  void set_vertices(Vertex_handle v0, Vertex_handle v1,
                    Vertex_handle v2, Vertex_handle v3)
  {
    V[0] = v0;
    V[1] = v1;
    V[2] = v2;
    V[3] = v3;
  }

  void set_neighbors()
  {
    N[0] = N[1] = N[2] = N[3] = Cell_handle();
  }

  void set_neighbors(Cell_handle n0, Cell_handle n1,
                     Cell_handle n2, Cell_handle n3)
  {
    CGAL_triangulation_precondition( this != &*n0 );
    CGAL_triangulation_precondition( this != &*n1 );
    CGAL_triangulation_precondition( this != &*n2 );
    CGAL_triangulation_precondition( this != &*n3 );
    N[0] = n0;
    N[1] = n1;
    N[2] = n2;
    N[3] = n3;
  }

  // CHECKING

  // the following trivial is_valid allows
  // the user of derived cell base classes
  // to add their own purpose checking
  bool is_valid(bool = false, int = 0) const
  { return true; }

  // This is here in the *ds*_cell_base to ease its use as default
  // template parameter, so that the .dual() functions of Delaunay_3
  // still work.
  template < typename Traits >
  typename Traits::Point_3
  circumcenter(const Traits& gt) const
  {
    return gt.construct_circumcenter_3_object()(this->vertex(0)->point(),
                                                this->vertex(1)->point(),
                                                this->vertex(2)->point(),
                                                this->vertex(3)->point());
  }

  // For use by Compact_container.
  void * for_compact_container() const { return N[0].for_compact_container(); }
  void * & for_compact_container()     { return N[0].for_compact_container(); }

  // TDS internal data access functions.
        TDS_data& tds_data()       { return _tds_data; }
  const TDS_data& tds_data() const { return _tds_data; }
  
#ifdef SHOW_REMAINING_BAD_ELEMENT_IN_RED
  int mark;
  int mark2;
#endif

private:

  Cell_handle   N[4];
  Vertex_handle V[4];
  TDS_data      _tds_data;
};

template < class TDS >
inline
std::istream&
operator>>(std::istream &is, Triangulation_ds_cell_base_3<TDS> &)
  // non combinatorial information. Default = nothing
{
  return is;
}

template < class TDS >
inline
std::ostream&
operator<<(std::ostream &os, const Triangulation_ds_cell_base_3<TDS> &)
  // non combinatorial information. Default = nothing
{
  return os;
}

// Specialization for void.
template <>
class Triangulation_ds_cell_base_3<void>
{
public:
  typedef internal::Dummy_tds_3                         Triangulation_data_structure;
  typedef Triangulation_data_structure::Vertex_handle   Vertex_handle;
  typedef Triangulation_data_structure::Cell_handle     Cell_handle;
  template <typename TDS2>
  struct Rebind_TDS { typedef Triangulation_ds_cell_base_3<TDS2> Other; };
};

} //namespace CGAL

#endif // CGAL_TRIANGULATION_DS_CELL_BASE_3_H
