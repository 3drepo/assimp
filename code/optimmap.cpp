#include "../include/assimp/optimmap.h"
#include "../include/assimp/scene.h"

void aiOptimMap::mergeInto(aiNode *mergingNode)
{
	if (mergingNode->mOptimMap){
		mergeMap.insert(mergingNode->mOptimMap->mergeMap.begin(), mergingNode->mOptimMap->mergeMap.end());

		mergingNode->mOptimMap->mergeMap.clear();

		if (mergingNode->mOptimMap->vertexMaps.size())
		{
			vertexMaps.insert(vertexMaps.end(), mergingNode->mOptimMap->vertexMaps.begin(), mergingNode->mOptimMap->vertexMaps.end());
			mergingNode->mOptimMap->vertexMaps.clear();
		}

		if (mergingNode->mOptimMap->triangleMaps.size())
		{
			triangleMaps.insert(triangleMaps.end(), mergingNode->mOptimMap->triangleMaps.begin(), mergingNode->mOptimMap->triangleMaps.end());
			mergingNode->mOptimMap->triangleMaps.clear();
		}

		delete mergingNode->mOptimMap;
		mergingNode->mOptimMap = NULL;
	}

	mergeMap.insert(reinterpret_cast<uintptr_t>(mergingNode));
}

void aiOptimMap::addMergedVertexMap(aiNode *mergingNode, int from, int num)
{
	uintptr_t childPointer = reinterpret_cast<uintptr_t>(mergingNode);
	std::vector<aiVMap>::iterator vit = std::find_if(vertexMaps.begin(), vertexMaps.end(), mapPtrCompare<aiVMap>(childPointer));

	if(vit == vertexMaps.end())
	{
		// If the merging node does not have it's own optimization map
		// just push it into the map
		if (!(mergingNode->mOptimMap && mergingNode->mOptimMap->vertexMaps.size()))
		{
			aiVMap newVMap;

			newVMap.childPointer = childPointer;
			newVMap.startVertexIDX = from;
			newVMap.endVertexIDX = from + num;

			vertexMaps.push_back(newVMap);
		}

		mergeInto(mergingNode);
	} 	
}

void aiOptimMap::addMergedTriangleMap(aiNode *mergingNode, int from, int num, int offset)
{
	uintptr_t childPointer = reinterpret_cast<uintptr_t>(mergingNode);
	std::vector<aiTMap>::iterator tit = std::find_if(triangleMaps.begin(), triangleMaps.end(), mapPtrCompare<aiTMap>(childPointer));

	if(tit == triangleMaps.end())
	{
		if (mergingNode->mOptimMap && mergingNode->mOptimMap->triangleMaps.size())
		{
			// If the merging node has it's own optimization map, then
			// increment the offset ready for merging later
			for(aiTMap &triMap : mergingNode->mOptimMap->triangleMaps)
				triMap.offset += offset;

		} else {
			// Otherwise, start a new one
			aiTMap newTMap;

			newTMap.childPointer = childPointer;
			newTMap.startTriangleIDX = from;
			newTMap.endTriangleIDX = from + num;
			newTMap.offset = offset;

			triangleMaps.push_back(newTMap);
		}

		mergeInto(mergingNode);	
	} 
}

