#include "../include/assimp/optimmap.h"
#include "../include/assimp/scene.h"

void aiOptimMap::pushVertexMap(uintptr_t meshPointer, const aiVMap& vMap)
{
	std::map<uintptr_t, std::vector<aiVMap> >::iterator meshMap = vertexMaps.find(meshPointer);
	std::vector<aiVMap> *meshVMap = NULL;

	// Do we already know of the mesh that the child mesh is being merged ?
	if (meshMap == vertexMaps.end())
	{
		meshVMap = new std::vector<aiVMap>();
		meshVMap->push_back(vMap);
		vertexMaps.insert(std::make_pair(meshPointer, *meshVMap));
	} else {
		meshVMap = &(meshMap->second);

		// Does this mesh already exist in the map, for this mesh ?
		//std::vector<aiVMap>::iterator vit = std::find_if(meshVMap->begin(), meshVMap->end(), mapPtrCompare<aiVMap>(vMap.childMesh));

		//if(vit == meshVMap->end())
		/*else*/
		/*if (vit != meshVMap->end())
		{
			std::cout << "Already know about it !!!" << vit->childMesh << "," << vit->startVertexIDX << std::endl;
			//vit->startVertexIDX = vMap.startVertexIDX;
			//vit->endVertexIDX   = vMap.endVertexIDX;
			std::cout << "NEW: " << vMap.childMesh << ", " << vMap.startVertexIDX << std::endl;
		}*/

		//if(vit == meshVMap->end())
		meshVMap->push_back(vMap);
	}
}

void aiOptimMap::pushTriangleMap(uintptr_t meshPointer, const aiTMap& tMap)
{
	std::map<uintptr_t, std::vector<aiTMap> >::iterator meshMap = triangleMaps.find(meshPointer);
	std::vector<aiTMap> *meshTMap = NULL;

	if (meshMap == triangleMaps.end())
	{
		meshTMap = new std::vector<aiTMap>();
		meshTMap->push_back(tMap);
		triangleMaps.insert(std::make_pair(meshPointer, *meshTMap));
	} else {
		meshTMap = &(meshMap->second);

		// If the we already know abouting the mesh that the child is being merged into,
		// does it already know about the child ?
		//std::vector<aiTMap>::iterator tit = std::find_if(meshTMap->begin(), meshTMap->end(), mapPtrCompare<aiTMap>(tMap.childMesh));

		//if(tit == meshTMap->end())
		/*else*/
		/*if (tit != meshTMap->end())
		{
			std::cout << "Already know about it !!!" << tit->childMesh << ", " << tit->startTriangleIDX << ", " << tit->offset << std::endl;
			//tit->startTriangleIDX = tMap.startTriangleIDX;
			//tit->endTriangleIDX   = tMap.endTriangleIDX;
			//tit->offset           = tMap.offset;
			std::cout << "NEW: " << tMap.childMesh << ", " << tMap.startTriangleIDX << ", " << tMap.offset << std::endl;
		}*/

		//if(tit == meshTMap->end())
		meshTMap->push_back(tMap);
	}
}

void aiOptimMap::mergeInto(aiNode *mergingNode)
{
	if (mergingNode->mOptimMap){
		mergeMap.insert(mergingNode->mOptimMap->mergeMap.begin(), mergingNode->mOptimMap->mergeMap.end());
		mergingNode->mOptimMap->mergeMap.clear();

		std::map<uintptr_t, std::vector<aiVMap> >::iterator vertexMapIT;

		for(vertexMapIT = mergingNode->mOptimMap->vertexMaps.begin(); vertexMapIT != mergingNode->mOptimMap->vertexMaps.end(); ++vertexMapIT)
		{
			std::vector<aiVMap> &vMap = vertexMapIT->second;
			std::vector<aiVMap>::iterator vit;

			for(vit = vMap.begin(); vit != vMap.end(); ++vit)
			{
				pushVertexMap(vertexMapIT->first, *vit);
			}

			vertexMapIT->second.clear();
		}

		mergingNode->mOptimMap->vertexMaps.clear();

		std::map<uintptr_t, std::vector<aiTMap> >::iterator triangleMapIT;

		for(triangleMapIT = mergingNode->mOptimMap->triangleMaps.begin(); triangleMapIT != mergingNode->mOptimMap->triangleMaps.end(); ++triangleMapIT)
		{
			std::vector<aiTMap> &tMap = triangleMapIT->second;
			std::vector<aiTMap>::iterator tit;

			for(tit = tMap.begin(); tit != tMap.end(); ++tit)
			{
				pushTriangleMap(triangleMapIT->first, *tit);
			}

			triangleMapIT->second.clear();
		}

		mergeMap.insert(mergingNode->mOptimMap->mergeMap.begin(), mergingNode->mOptimMap->mergeMap.end());
		mergingNode->mOptimMap->triangleMaps.clear();

		delete mergingNode->mOptimMap;
		mergingNode->mOptimMap = NULL;
	}

	mergeMap.insert(reinterpret_cast<uintptr_t>(mergingNode));
}

void aiOptimMap::addMergedVertexMap(aiMesh *mergedMesh, aiMesh *mergingMesh, int from, int num)
{
	uintptr_t meshPointer = reinterpret_cast<uintptr_t>(mergedMesh);
	uintptr_t childPointer = reinterpret_cast<uintptr_t>(mergingMesh);

	aiVMap newVMap;

	newVMap.childMesh      = childPointer;
	newVMap.startVertexIDX = from;
	newVMap.endVertexIDX   = from + num;

	pushVertexMap(meshPointer, newVMap);
}

void aiOptimMap::addMergedTriangleMap(aiMesh *mergedMesh, aiMesh *mergingMesh, int from, int num, int offset)
{
	uintptr_t meshPointer  = reinterpret_cast<uintptr_t>(mergedMesh);
	uintptr_t childPointer = reinterpret_cast<uintptr_t>(mergingMesh);

	aiTMap newTMap;

	newTMap.childMesh        = childPointer;
	newTMap.startTriangleIDX = from;
	newTMap.endTriangleIDX   = from + num;
	newTMap.offset           = offset;

	pushTriangleMap(meshPointer, newTMap);
}


