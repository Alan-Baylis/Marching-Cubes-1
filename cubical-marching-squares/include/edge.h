//////////////////////////////////////////////
/// @file edge.h
/// @brief Edge class header
///
/// The Cubical Marching Squares (CMS) Project
/// licensed under the 3 clause BSD licence
/// found in LICENCE.md 2015
///
/// @author George Rassovsky
/// (goro.rassovsky@gmail.com)
//////////////////////////////////////////////

#ifndef CMS_EDGE_H
#define CMS_EDGE_H

#include "vec3.h"
#include "index3d.h"


namespace cms
{

/// @class Edge
/// @brief Edge class storing vertex index
///
class Edge
{
public:
  /// @brief Constructor
  Edge();

  /// @brief Destructor
  ~Edge();

  /// @brief Setting a vertex index on the edge
  void setVertIndex(int i_vertIndex);

  /// @brief Getting a read-only vertex index from the edge
  const int& getVertIndex() const;

  /// @brief setting the bool flag, for occupied edge
  void setEmpty(bool i_empty);

  /// @brief a bool flag denoting whether the edge has data
  const bool& empty() const;

  /// @brief a 3D index of the edge block to which that edge belongs
  Index3D m_block;

  /// @brief the direction this edge is facing on the edgeblock
  /// 0 - x, 1 - y, 2 - z
  int m_dir;

private:
  /// @brief the empty boolean flag
  bool m_empty;

  /// @brief the vertex index stored on this edge if applicable
  int m_vertIndex;
};



/// @struct EdgeBlock
/// a structure holding 3 edges
/// with ordering x y z - right up front
struct EdgeBlock
{
  /// @brief Empty ctor
  EdgeBlock() :
    m_empty(true), m_edgeInds(-1)
  {}

  /// @brief Ctor with bool
  EdgeBlock(bool empty) :
    m_empty(empty), m_edgeInds(-1)
  {}

  /// @brief Ctor with edges
  EdgeBlock(int right, int up, int front) :
    m_empty(false), m_edgeInds(right, up, front)
  {}

  /// @ Full Ctor
  EdgeBlock(bool empty, int right, int up, int front) :
    m_empty(empty), m_edgeInds(right, up, front)
  {}

  /// @brief Overloaded [] operator
  int& operator []( const int& _i )
  {
    return (&m_edgeInds.m_x)[_i];
  }

  // Members
  /// @brief the empty bool flag
  bool m_empty;

  /// @brief the 3d edgeblock index in the edge array
  Index3D m_edgeInds;
};

} //namespace cms

#endif //CMS_EDGE_H
