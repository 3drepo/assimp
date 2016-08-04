/*
Open Asset Import Library (ASSIMP)
----------------------------------------------------------------------

Copyright (c) 2006-2010, ASSIMP Development Team
All rights reserved.

Redistribution and use of this software in source and binary forms, 
with or without modification, are permitted provided that the 
following conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the ASSIMP team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the ASSIMP Development Team.

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

----------------------------------------------------------------------
*/

/** MACHINE-GENERATED by scripts/IFCImporter/CppGenerator.py */

#ifndef INCLUDED_IFC_METADATA_H
#define INCLUDED_IFC_METADATA_H

#include "IFCLoader.h"
#include "IFCReaderGen.h"
#include "IFCUtil.h"
#include "../include/assimp/scene.h"
#include <vector>

namespace Assimp {
namespace IFC {
	using namespace STEP;
	using namespace STEP::EXPRESS;

	void AppendSubnodes(aiNode *parent, const std::vector<aiNode *>& subnodes);

	template <typename T> aiNode *ExtractMetaData(const aiNode *parent, const T& in, ConversionData& conv);

	template <> aiNode *ExtractMetaData<IfcActorRole>(const aiNode *parent, const IfcActorRole& in, ConversionData& conv);
	template <> aiNode *ExtractMetaData<IfcApplication>(const aiNode *parent, const IfcApplication& in, ConversionData& conv);
	template <> aiNode *ExtractMetaData<IfcGroup>(const aiNode *parent, const IfcGroup& in, ConversionData& conv);
	template <> aiNode *ExtractMetaData<IfcOrganization>(const aiNode *parent, const IfcOrganization& in, ConversionData& conv);
	template <> aiNode *ExtractMetaData<IfcOwnerHistory>(const aiNode *parent, const IfcOwnerHistory& in, ConversionData& conv);
	template <> aiNode *ExtractMetaData<IfcPerson>(const aiNode *parent, const IfcPerson& in, ConversionData& conv);
	template <> aiNode *ExtractMetaData<IfcPersonAndOrganization>(const aiNode *parent, const IfcPersonAndOrganization& in, ConversionData& conv);
	template <> aiNode *ExtractMetaData<IfcZone>(const aiNode *parent, const IfcZone& in, ConversionData& conv);

}
}

#endif // INCLUDED_IFC_METADATA_H