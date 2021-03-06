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

/** MACHINE-GENERATED by scripts/IFCImporter/MetaGenerator.py */

#ifndef ASSIMP_BUILD_NO_IFC_IMPORTER

#include "IFCMetaData.h"
#include "Exceptional.h"
#include <sstream>

namespace Assimp {
namespace IFC {
	using namespace STEP;
	using namespace STEP::EXPRESS;

	// ------------------------------------------------------------------------------------------------
	template <typename T, uint64_t N, uint64_t M> aiString ListToString(const ListOf<Lazy<T>,N,M>& list)
	{
		std::stringstream s("");

		for(int i = 0; i < list.size(); i++)
		{
			s << (*list[i]).GetID();

			if (i < (list.size() - 1))
				s << ",";
		}

		return aiString(s.str());
	}


	// ------------------------------------------------------------------------------------------------
	template <typename T, uint64_t N, uint64_t M> aiString ListToString(const ListOf<PrimitiveDataType<T>,N,M>& list)
	{
		std::stringstream s("");

		for(int i = 0; i < list.size(); i++)
		{
			s << list[i];

			if (i < (list.size() - 1))
				s << ",";
		}

		return aiString(s.str());
	}

	// ------------------------------------------------------------------------------------------------
	void AppendSubnodes(aiNode *parent, const std::vector<aiNode *>& subnodes)
	{
		if (subnodes.size() && parent)
		{
			int numCurrentChildren = parent->mNumChildren;
			aiNode **newChildren = new aiNode*[numCurrentChildren + subnodes.size()]();

			if(parent->mChildren)
			{
				// To append to current parent list copy
				// over the old aiNode* pointers first
				int numCurrentChildren = parent->mNumChildren;
				memcpy(newChildren, parent->mChildren, numCurrentChildren * sizeof(aiNode*));

				delete[] parent->mChildren;
				parent->mChildren = NULL;
			}

			BOOST_FOREACH(aiNode* nd, subnodes) {
				newChildren[parent->mNumChildren++] = nd;
				nd->mParent = parent;
			}

			parent->mChildren = newChildren;
		}
	}


// -----------------------------------------------------------------------------------------------------------
template <> aiNode *ExtractMetaData<IfcActorRole>(const aiNode *parent, const IfcActorRole& in, ConversionData& conv)
{
	aiNode *ndnew = new aiNode();

	std::string nodeName = in.GetClassName(); // + "_" + in.GetID();
	ndnew->mName.Set(nodeName);
	ndnew->mTransformation = parent->mTransformation;

    int index = 1;

	aiMetadata *data = new aiMetadata();
	int numProperties = 2;
	if(in.UserDefinedRole) numProperties++;
	if(in.Description) numProperties++;

    data->mNumProperties = numProperties;
    data->mKeys = new aiString[numProperties];
    data->mValues = new aiMetadataEntry[numProperties];

    ndnew->mMetaData = data;
	data->Set(0, "IFC_id", in.GetID());
	data->Set(index++, "Role", aiString(in.Role));
	if (in.UserDefinedRole) data->Set(index++, "UserDefinedRole", aiString(in.UserDefinedRole.Get()));
	if (in.Description) data->Set(index++, "Description", aiString(in.Description.Get()));
	return ndnew;
}
// -----------------------------------------------------------------------------------------------------------
template <> aiNode *ExtractMetaData<IfcApplication>(const aiNode *parent, const IfcApplication& in, ConversionData& conv)
{
	aiNode *ndnew = new aiNode();

	std::string nodeName = in.GetClassName(); // + "_" + in.GetID();
	ndnew->mName.Set(nodeName);
	ndnew->mTransformation = parent->mTransformation;

    int index = 1;

	aiMetadata *data = new aiMetadata();
	int numProperties = 4;

    data->mNumProperties = numProperties;
    data->mKeys = new aiString[numProperties];
    data->mValues = new aiMetadataEntry[numProperties];

    ndnew->mMetaData = data;
	data->Set(0, "IFC_id", in.GetID());
	data->Set(index++, "Version", aiString(in.Version));
	data->Set(index++, "ApplicationFullName", aiString(in.ApplicationFullName));
	data->Set(index++, "ApplicationIdentifier", aiString(in.ApplicationIdentifier));
	std::vector<aiNode *> subnodes;
	const IfcOrganization ApplicationDeveloper = in.ApplicationDeveloper;
    aiNode *nd_ApplicationDeveloper = ExtractMetaData(ndnew, ApplicationDeveloper, conv);
    subnodes.push_back(nd_ApplicationDeveloper);	AppendSubnodes(ndnew, subnodes);
	return ndnew;
}
// -----------------------------------------------------------------------------------------------------------
template <> aiNode *ExtractMetaData<IfcGroup>(const aiNode *parent, const IfcGroup& in, ConversionData& conv)
{
	aiNode *ndnew = new aiNode();

	std::string nodeName = in.GetClassName(); // + "_" + in.GetID();
	ndnew->mName.Set(nodeName);
	ndnew->mTransformation = parent->mTransformation;

    int index = 1;

	aiMetadata *data = new aiMetadata();
	int numProperties = 1;

    data->mNumProperties = numProperties;
    data->mKeys = new aiString[numProperties];
    data->mValues = new aiMetadataEntry[numProperties];

    ndnew->mMetaData = data;
	data->Set(0, "IFC_id", in.GetID());
	return ndnew;
}
// -----------------------------------------------------------------------------------------------------------
template <> aiNode *ExtractMetaData<IfcOrganization>(const aiNode *parent, const IfcOrganization& in, ConversionData& conv)
{
	aiNode *ndnew = new aiNode();

	std::string nodeName = in.GetClassName(); // + "_" + in.GetID();
	ndnew->mName.Set(nodeName);
	ndnew->mTransformation = parent->mTransformation;

    int index = 1;

	aiMetadata *data = new aiMetadata();
	int numProperties = 2;
	if(in.Id) numProperties++;
	if(in.Description) numProperties++;
	if(in.Roles) numProperties++;
	if(in.Addresses) numProperties++;

    data->mNumProperties = numProperties;
    data->mKeys = new aiString[numProperties];
    data->mValues = new aiMetadataEntry[numProperties];

    ndnew->mMetaData = data;
	data->Set(0, "IFC_id", in.GetID());
	if (in.Id) data->Set(index++, "Id", aiString(in.Id.Get()));
	data->Set(index++, "Name", aiString(in.Name));
	if (in.Description) data->Set(index++, "Description", aiString(in.Description.Get()));
	if (in.Roles) data->Set(index++, "Roles", ListToString(in.Roles.Get()));
	if (in.Addresses) data->Set(index++, "Addresses", ListToString(in.Addresses.Get()));
	std::vector<aiNode *> subnodes;
	AppendSubnodes(ndnew, subnodes);
	return ndnew;
}
// -----------------------------------------------------------------------------------------------------------
template <> aiNode *ExtractMetaData<IfcOwnerHistory>(const aiNode *parent, const IfcOwnerHistory& in, ConversionData& conv)
{
	aiNode *ndnew = new aiNode();

	std::string nodeName = in.GetClassName(); // + "_" + in.GetID();
	ndnew->mName.Set(nodeName);
	ndnew->mTransformation = parent->mTransformation;

    int index = 1;

	aiMetadata *data = new aiMetadata();
	int numProperties = 3;
	if(in.State) numProperties++;
	if(in.LastModifiedDate) numProperties++;

    data->mNumProperties = numProperties;
    data->mKeys = new aiString[numProperties];
    data->mValues = new aiMetadataEntry[numProperties];

    ndnew->mMetaData = data;
	data->Set(0, "IFC_id", in.GetID());
	if (in.State) data->Set(index++, "State", aiString(in.State.Get()));
	data->Set(index++, "ChangeAction", aiString(in.ChangeAction));
	if (in.LastModifiedDate) data->Set(index++, "LastModifiedDate", (uint64_t)(in.LastModifiedDate.Get()));
	data->Set(index++, "CreationDate", (uint64_t)(in.CreationDate));
	std::vector<aiNode *> subnodes;
	const IfcPersonAndOrganization OwningUser = in.OwningUser;
    aiNode *nd_OwningUser = ExtractMetaData(ndnew, OwningUser, conv);
    subnodes.push_back(nd_OwningUser);	const IfcApplication OwningApplication = in.OwningApplication;
    aiNode *nd_OwningApplication = ExtractMetaData(ndnew, OwningApplication, conv);
    subnodes.push_back(nd_OwningApplication);	if (in.LastModifyingUser){
		const IfcPersonAndOrganization LastModifyingUser = in.LastModifyingUser.Get();
    aiNode *nd_LastModifyingUser = ExtractMetaData(ndnew, LastModifyingUser, conv);
    subnodes.push_back(nd_LastModifyingUser);	}
	if (in.LastModifyingApplication){
		const IfcApplication LastModifyingApplication = in.LastModifyingApplication.Get();
    aiNode *nd_LastModifyingApplication = ExtractMetaData(ndnew, LastModifyingApplication, conv);
    subnodes.push_back(nd_LastModifyingApplication);	}
	AppendSubnodes(ndnew, subnodes);
	return ndnew;
}
// -----------------------------------------------------------------------------------------------------------
template <> aiNode *ExtractMetaData<IfcPerson>(const aiNode *parent, const IfcPerson& in, ConversionData& conv)
{
	aiNode *ndnew = new aiNode();

	std::string nodeName = in.GetClassName(); // + "_" + in.GetID();
	ndnew->mName.Set(nodeName);
	ndnew->mTransformation = parent->mTransformation;

    int index = 1;

	aiMetadata *data = new aiMetadata();
	int numProperties = 1;
	if(in.Id) numProperties++;
	if(in.FamilyName) numProperties++;
	if(in.GivenName) numProperties++;
	if(in.MiddleNames) numProperties++;
	if(in.PrefixTitles) numProperties++;
	if(in.SuffixTitles) numProperties++;
	if(in.Roles) numProperties++;
	if(in.Addresses) numProperties++;

    data->mNumProperties = numProperties;
    data->mKeys = new aiString[numProperties];
    data->mValues = new aiMetadataEntry[numProperties];

    ndnew->mMetaData = data;
	data->Set(0, "IFC_id", in.GetID());
	if (in.Id) data->Set(index++, "Id", aiString(in.Id.Get()));
	if (in.FamilyName) data->Set(index++, "FamilyName", aiString(in.FamilyName.Get()));
	if (in.GivenName) data->Set(index++, "GivenName", aiString(in.GivenName.Get()));
	if (in.MiddleNames) data->Set(index++, "MiddleNames", ListToString(in.MiddleNames.Get()));
	if (in.PrefixTitles) data->Set(index++, "PrefixTitles", ListToString(in.PrefixTitles.Get()));
	if (in.SuffixTitles) data->Set(index++, "SuffixTitles", ListToString(in.SuffixTitles.Get()));
	if (in.Roles) data->Set(index++, "Roles", ListToString(in.Roles.Get()));
	if (in.Addresses) data->Set(index++, "Addresses", ListToString(in.Addresses.Get()));
	std::vector<aiNode *> subnodes;
	AppendSubnodes(ndnew, subnodes);
	return ndnew;
}
// -----------------------------------------------------------------------------------------------------------
template <> aiNode *ExtractMetaData<IfcPersonAndOrganization>(const aiNode *parent, const IfcPersonAndOrganization& in, ConversionData& conv)
{
	aiNode *ndnew = new aiNode();

	std::string nodeName = in.GetClassName(); // + "_" + in.GetID();
	ndnew->mName.Set(nodeName);
	ndnew->mTransformation = parent->mTransformation;

    int index = 1;

	aiMetadata *data = new aiMetadata();
	int numProperties = 1;
	if(in.Roles) numProperties++;

    data->mNumProperties = numProperties;
    data->mKeys = new aiString[numProperties];
    data->mValues = new aiMetadataEntry[numProperties];

    ndnew->mMetaData = data;
	data->Set(0, "IFC_id", in.GetID());
	if (in.Roles) data->Set(index++, "Roles", ListToString(in.Roles.Get()));
	std::vector<aiNode *> subnodes;
	const IfcPerson ThePerson = in.ThePerson;
    aiNode *nd_ThePerson = ExtractMetaData(ndnew, ThePerson, conv);
    subnodes.push_back(nd_ThePerson);	const IfcOrganization TheOrganization = in.TheOrganization;
    aiNode *nd_TheOrganization = ExtractMetaData(ndnew, TheOrganization, conv);
    subnodes.push_back(nd_TheOrganization);	AppendSubnodes(ndnew, subnodes);
	return ndnew;
}
// -----------------------------------------------------------------------------------------------------------
template <> aiNode *ExtractMetaData<IfcZone>(const aiNode *parent, const IfcZone& in, ConversionData& conv)
{
	aiNode *ndnew = new aiNode();

	std::string nodeName = in.GetClassName(); // + "_" + in.GetID();
	ndnew->mName.Set(nodeName);
	ndnew->mTransformation = parent->mTransformation;

    int index = 1;

	aiMetadata *data = new aiMetadata();
	int numProperties = 1;

    data->mNumProperties = numProperties;
    data->mKeys = new aiString[numProperties];
    data->mValues = new aiMetadataEntry[numProperties];

    ndnew->mMetaData = data;
	data->Set(0, "IFC_id", in.GetID());
	return ndnew;
}

}
}

#endif
