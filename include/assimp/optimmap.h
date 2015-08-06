/*
---------------------------------------------------------------------------
Open Asset Import Library (assimp)
---------------------------------------------------------------------------

Copyright (c) 2006-2012, assimp team

All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the following
conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the assimp team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the assimp team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
---------------------------------------------------------------------------
*/

/** @file optimmap.h
 *  @brief Defines the optimization map
 */

#ifndef AI_OPTIMMAP_H_INC
#define AI_OPTIMMAP_H_INC

#include "types.h"
 
#ifdef __cplusplus

#include <vector>
#include <set>
#include <map>
#include <boost/uuid/uuid.hpp>
#include <functional>
#include <iostream>
#include <stdint.h>

#endif

struct aiNode;
struct aiMesh;

// ---------------------------------------------------------------------------
/** @brief Data structure for an Optimization Map
*
*	The optimization map stores the result of the post processing
*	optimization. Such that we can preserve a link between the original
*	scene structure and the new optimized map.
*/
#ifdef __cplusplus

typedef struct {
	uintptr_t childMesh;
	int startVertexIDX;
	int endVertexIDX;
} aiVMap;

typedef struct {
	uintptr_t childMesh;
	int startTriangleIDX;
	int endTriangleIDX;
	int offset;
} aiTMap;

template <class T>
struct mapPtrCompare {
    uintptr_t meshPointer;
    mapPtrCompare(uintptr_t meshPointer) : meshPointer(meshPointer) { }

    bool operator ()(T const& obj) const { return obj.childMesh == meshPointer; }
};	

struct ASSIMP_API aiOptimMap
#else
struct aiOptimMap
#endif
{

#ifdef __cplusplus

public:

	std::set<uintptr_t > mergeMap;

	std::map<uintptr_t, std::vector<aiVMap> > vertexMaps; // Map between mesh id and a set of vertices
	std::map<uintptr_t, std::vector<aiTMap> > triangleMaps; // Map between mesh id and a set of triangles

	std::map<uintptr_t, int> triangleOffset; // Offsets for each of the triangle maps

	void addMergedVertexMap(aiMesh *mergedMesh, aiMesh *mergingMesh, int from, int num);
	void addMergedTriangleMap(aiMesh *mergedMesh, aiMesh *mergingMesh, int from, int num, int offset);
	void mergeInto(aiNode *mergingNode);

private:
	
	void pushTriangleMap(uintptr_t meshPointer, const aiTMap& tMap);
	void pushVertexMap(uintptr_t meshPointer, const aiVMap& vMap);

#endif

};


#endif //!!AI_OPTIMMAP_INC_H
