#include "../include/assimp/optimmap.h"
#include "../include/assimp/scene.h"

void aiOptimMap::pushMeshMap(uintptr_t meshPointer, const aiMap& map)
{
	//std::cout << "MP: " << meshPointer << " " << meshMaps.size() << std::endl;
	//std::cout << "&" << &meshMaps << std::endl;

	std::unordered_map<uintptr_t, std::vector<aiMap> >::iterator meshMap = meshMaps.find(meshPointer);
	std::vector<aiMap> *meshMapPtr = NULL;

	if (meshMap == meshMaps.end())
	{
		meshMapPtr = new std::vector<aiMap>();
		meshMapPtr->push_back(map);
		meshMaps.insert(std::make_pair(meshPointer, *meshMapPtr))	;
	} else {
		meshMapPtr = &(meshMap->second);

		meshMapPtr->push_back(map);
	}

	//std::cout << "MPII: " << meshPointer << " " << meshMaps.size() << std::endl;
}

void aiOptimMap::mergeInto(aiNode *mergingNode)
{
	if (mergingNode->mOptimMap){
		mergeMap.insert(mergingNode->mOptimMap->mergeMap.begin(), mergingNode->mOptimMap->mergeMap.end());
		mergingNode->mOptimMap->mergeMap.clear();

		std::unordered_map<uintptr_t, std::vector<aiMap> >::iterator meshMapIT;

		for(meshMapIT = mergingNode->mOptimMap->meshMaps.begin(); meshMapIT != mergingNode->mOptimMap->meshMaps.end(); ++meshMapIT)
		{
			std::vector<aiMap> &map = meshMapIT->second;
			std::vector<aiMap>::iterator mit;

			for(mit = map.begin(); mit != map.end(); ++mit)
				pushMeshMap(meshMapIT->first, *mit);

			meshMapIT->second.clear();
		}

		mergingNode->mOptimMap->meshMaps.clear();

		delete mergingNode->mOptimMap;
		mergingNode->mOptimMap = NULL;
	}

	mergeMap.insert(reinterpret_cast<uintptr_t>(mergingNode));
}


void aiOptimMap::addMeshMap(aiMesh *mergedMesh, aiMesh *mergingMesh, aiMaterial *material,
	int vertexFrom, int vertexTo, int triFrom, int triTo,
	const aiVector3D &min, const aiVector3D &max)
{
	uintptr_t meshPointer     = reinterpret_cast<uintptr_t>(mergedMesh);
	uintptr_t childPointer    = reinterpret_cast<uintptr_t>(mergingMesh);
	uintptr_t materialPointer = reinterpret_cast<uintptr_t>(material);

	aiMap newMap;

	newMap.childMesh        = childPointer;
	newMap.material         = materialPointer;
	newMap.startVertexIDX   = vertexFrom;
	newMap.endVertexIDX     = vertexTo;
	newMap.startTriangleIDX = triFrom;
	newMap.endTriangleIDX   = triTo;
	newMap.min              = min;
	newMap.max              = max;

	pushMeshMap(meshPointer, newMap);
}

