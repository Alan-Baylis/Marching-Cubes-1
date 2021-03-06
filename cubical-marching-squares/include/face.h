//////////////////////////////////////////////
/// @file face.h
/// @brief Face class header
///
/// The Cubical Marching Squares (CMS) Project
/// licensed under the 3 clause BSD licence
/// found in LICENCE.md 2015
///
/// @author George Rassovsky
/// (goro.rassovsky@gmail.com)
//////////////////////////////////////////////

#ifndef CMS_FACE_H
#define CMS_FACE_H

#include "cell.h"
#include "vec3.h"
#include "util.h"
#include "strip.h"


namespace cms
{


/// @brief Face State enumerator
/// BRANCH_FACE may also mean EMPTY
enum FaceState 
{ 
  BRANCH_FACE  = 0,
  LEAF_FACE    = 1,
  TRANSIT_FACE = 2
};

/// @struct Face
/// @brief A Face structure stores information about
/// cell faces such as data on the face and it's belonging cell
/// @todo optimise the transitSegs vec of vec structure
///
struct Face
{
  // Face struct typedefs
  typedef std::vector<Strip> StripVec;

  /// @brief Full Contructor
  /// @param int _id - the unique identifier of the face
  /// @param int _cellInd - the unique identifier of it's cell
  /// @param sets all other members to a default value
             Face(int _id, int _cellInd) :
                id(_id)
              , cellInd(_cellInd)
              , skip(true)
              , state(BRANCH_FACE)
              , sharpFeature(false)
              , featurePosition(Vec3(0))
              , twin(NULL)
              , parent(NULL)
              {
                for(int i=0;i<4;++i)
                  children[i] = nullptr;
              }

  /// @brief the unique identifier of the face
  int        id; //todo only used for debug

  /// @brief the unique identifier of the face's cell
  int        cellInd;

  /// @brief a flag denoting whether the face has been taken care of
  bool       skip;

  /// @brief the state enumerator of the face
  FaceState  state;

  /// @brief a flag for face sharp features
  /// todo: to be used
  bool       sharpFeature;

  /// @brief the exact face sharp feature position in 3d space
  /// todo: to be used
  Vec3       featurePosition;

  /// @brief a ptr to the face's twin face
  Face*      twin;

  /// @brief a ptr to the face's parent face
  Face*      parent;

  /// @brief an array of 4 ptrs of the face's child face's
  Face*      children[4];

  /// @brief a vector of strips on that face
  StripVec   strips;

  /// @brief an Array of transitional segments on that face
  /// if it was a transitional face, it would have a twin with more
  /// a more complex set of strips, it stores them here when it has
  /// collected them
  /// @todo: fix the vec of vec
  uintVecVec transitSegs;
};


} //namespace cms

#endif //CMS_FACE_H
