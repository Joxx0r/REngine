#include "stdafx.h"
#include "RevModelLoader.h"
#include "RevModel.h"
#include <cimport.h>
#include <scene.h>

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
#include "d3dcompiler.h"
#include <memory>
#include <fstream>

#define USE_ASSIMP 1
//path file exist
#include <codecvt>
#include <locale>

#include "RevEngineExecutionFunctions.h"
#include "RevEngineRetrievalFunctions.h"
#include "Shlwapi.h"
#include "../DXSampleHelper.h"
#include "../Microsoft/RevDDSTextureLoader.h"

void LoadTexture(const std::wstring& path, struct ID3D12Resource** resourceToEndUpAt)
{
	std::vector<D3D12_SUBRESOURCE_DATA> subResources;
	std::unique_ptr<uint8_t[]> ddsData;
	ThrowIfFailed(LoadDDSTextureFromFile(
        RevEngineRetrievalFunctions::GetDevice(),
        path.c_str(),
        resourceToEndUpAt,
        ddsData,
        subResources));
}

std::wstring GetMaterialPath(aiMaterial* material, aiTextureType type, std::wstring basePath)
{
#if USE_ASSIMP
	aiString str;
	material->GetTexture(type, 0, &str);
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring element = converter.from_bytes(str.C_Str());
	std::wstring data = element.substr(2, element.length() - 1);
	std::wstring pathFile = basePath;
	std::wstring otherString = pathFile.substr(0, pathFile.find_last_of('/') + 1);
	otherString += data;
	return otherString;
#else
	std::string returnString;
	return returnString;
#endif
}


void CreateBaseVertex(
		const aiMesh* mesh, 
		UINT vertIndex, 
		RevVertexPosTexNormBiTan* outVertex)
{
	aiVector3D vert = mesh->mVertices[vertIndex];
	outVertex->m_position = XMFLOAT3(vert.x, vert.y, vert.z);
	for (UINT texCordIndex = 0; texCordIndex < mesh->mNumUVComponents[0]; texCordIndex++)
	{
		outVertex->m_tex.x = (float)mesh->mTextureCoords[0][vertIndex].x;
		outVertex->m_tex.y = 1 - (float)mesh->mTextureCoords[0][vertIndex].y;
	}

	aiVector3D normal = mesh->mNormals[vertIndex];
	outVertex->m_normal = XMFLOAT3(normal.x, normal.y, normal.z);

	aiVector3D biNormal = mesh->mBitangents[vertIndex];
	outVertex->m_binormal = XMFLOAT3(biNormal.x, biNormal.y, biNormal.z);

	aiVector3D tangent = mesh->mTangents[vertIndex];
	outVertex->m_tangent = XMFLOAT3(tangent.x, tangent.y, tangent.z);
}


void LoadIndecies(const aiMesh* mesh, std::vector<UINT> &indices)
{
	for (UINT vertIndex = 0; vertIndex < mesh->mNumFaces; vertIndex++)
	{
		aiFace* face = &mesh->mFaces[vertIndex];
		for (UINT faceIndex = 0; faceIndex < face->mNumIndices; faceIndex++)
		{
			indices.push_back(
				face->mIndices[faceIndex]);
		}
	}
}

aiNode* FindNodeRecursive(aiNode* node, const char* name)
{
	if (strcmp(node->mName.C_Str(), name) == 0)
	{
		return node;
	}

	for (UINT index = 0; index < node->mNumChildren; index++)
	{
		aiNode* childNode = node->mChildren[index];
		if (aiNode* returnNode = FindNodeRecursive(childNode, name))
		{
			return returnNode;
		}
	}

	return nullptr;
}

void LoadTexturePaths(
		const aiMesh* mesh, 
		const struct aiScene* scene, 
		RevTextureCollection& outData,
		const std::wstring& path)
{
#if USE_ASSIMP
	outData.m_numTextures = 0;
	UINT numMaterial = mesh->mMaterialIndex;
	if (mesh->mMaterialIndex >= 0)
	{
		outData.m_numTextures = 4;
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		outData.m_textures.push_back(RevTexture(GetMaterialPath(material, aiTextureType_DIFFUSE, path), RevTextureType::Diffuse));
		outData.m_textures.push_back(RevTexture(GetMaterialPath(material, aiTextureType_DIFFUSE, path), RevTextureType::Normal));
		outData.m_textures.push_back(RevTexture(GetMaterialPath(material, aiTextureType_DIFFUSE, path), RevTextureType::Substance));
		outData.m_textures.push_back(RevTexture(GetMaterialPath(material, aiTextureType_DIFFUSE, path), RevTextureType::RoughnessAOEmissive));
	}
#endif
}

void LoadIndecies(const aiMesh* mesh, UINT** indices, UINT& outNumIndicies)
{
	outNumIndicies = 0;
	for (UINT vertIndex = 0; vertIndex < mesh->mNumFaces; vertIndex++)
	{
		aiFace* face = &mesh->mFaces[vertIndex];
		for (UINT faceIndex = 0; faceIndex < face->mNumIndices; faceIndex++)
		{
			outNumIndicies++;
		}
	}

	*indices = new UINT[outNumIndicies];

	UINT indiciesIndex = 0;
	for (UINT vertIndex = 0; vertIndex < mesh->mNumFaces; vertIndex++)
	{
		aiFace* face = &mesh->mFaces[vertIndex];
		for (UINT faceIndex = 0; faceIndex < face->mNumIndices; faceIndex++)
		{
			(*indices)[indiciesIndex] = face->mIndices[faceIndex];
			indiciesIndex++;
		}
	}
}
/*
void LoadAnimatedModel(const struct aiScene* scene, RevModel* newModel, const char* path)
{
	RevAnimatedNodelInitializationData initializationData;

	for (UINT meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
	{
		const aiMesh* mesh = scene->mMeshes[meshIndex];
		if (mesh)
		{
			initializationData.m_nVertexes = mesh->mNumVertices;
			initializationData.vertices = new RevAnimatedVertex[initializationData.m_nVertexes];
			for (UINT vertIndex = 0; vertIndex < mesh->mNumVertices; vertIndex++)
			{
				RevAnimatedVertex vertexToUse = {};
				CreateBaseVertex(mesh, vertIndex, &vertexToUse);
				initializationData.vertices[vertIndex] = vertexToUse;
			}

			LoadIndecies(mesh, &initializationData.indices, initializationData.m_nIndices);
			LoadTexturePaths(mesh, scene, initializationData.m_textureInitialization, path);

			initializationData.m_nBones = mesh->mNumBones;
			initializationData.bones = new RevBone[initializationData.m_nBones];
			for (UINT boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++)
			{
				aiBone* bone = mesh->mBones[boneIndex];
				RevBone boneData = {};

				strcpy(boneData.m_name , bone->mName.C_Str());
				aiMatrix4x4 boneM = bone->mOffsetMatrix.Transpose();
				boneData.m_offsetMatrix.Load(&boneM.a1);
				for (UINT influenceIndex = 0; influenceIndex < bone->mNumWeights; influenceIndex++)
				{
					aiVertexWeight weight = bone->mWeights[influenceIndex];
					RevAnimatedVertex* animatedVertex = (RevAnimatedVertex*)&initializationData.vertices[weight.mVertexId];
					UINT indexToUse = UINT8_MAX;
					for (UINT boneInfluenceIndex = 0; boneInfluenceIndex < REV_MAX_INFLUENCES_PER_BONE; boneInfluenceIndex++)
					{
						if (animatedVertex->m_boneInfluences[boneInfluenceIndex] == UINT_MAX)
						{
							indexToUse = boneInfluenceIndex;
							break;
						}
					}

					float weightScale = weight.mWeight;

					if (indexToUse == UINT8_MAX)
					{
						float minimumWeightFound = weightScale;
						for (UINT boneInfluenceIndex = 0; boneInfluenceIndex < REV_MAX_INFLUENCES_PER_BONE; boneInfluenceIndex++)
						{
							const float& weightToCheck = animatedVertex->m_boneWeights[boneInfluenceIndex];
							if (weightToCheck < minimumWeightFound)
							{
								minimumWeightFound = weightToCheck;
								indexToUse = boneInfluenceIndex;
							}
						}
					}

					if (indexToUse != UINT8_MAX)
					{
						animatedVertex->m_boneWeights[indexToUse] = weightScale;
						animatedVertex->m_boneInfluences[indexToUse] = boneIndex;
					}
				}

				initializationData.bones[boneIndex] = boneData;
			}

			for (UINT boneIndex = 0; boneIndex < initializationData.m_nBones; boneIndex++)
			{
				RevBone& bone = initializationData.bones[boneIndex];

				aiNode* node = FindNodeRecursive(scene->mRootNode, bone.m_name);

				UINT parentIndex = UINT8_MAX;
				if (node && node->mParent)
				{
					for (UINT parentSearchIndex = 0; parentSearchIndex < initializationData.m_nBones; parentSearchIndex++)
					{
						RevBone& parentBone = initializationData.bones[parentSearchIndex];
						if (strcmp(parentBone.m_name, node->mParent->mName.C_Str()) == 0)
						{
							parentIndex = parentSearchIndex;
							break;
						}
					}
				}
				bone.m_parentIndex = parentIndex;
			}
		}
	}

	for (UINT animationIndex = 0; animationIndex < scene->mNumAnimations; animationIndex++)
	{
		const aiAnimation* animation = scene->mAnimations[animationIndex];
		RevAnimationInstanceData animationModelInstanceData = {};
		animationModelInstanceData.m_name = animation->mName.C_Str();
		
		animationModelInstanceData.m_duration = (float)animation->mDuration;
		assert(animation->mNumChannels != 0);
		for (UINT channelIndex = 0; channelIndex < animation->mNumChannels; channelIndex++)
		{
			aiNodeAnim* channel = animation->mChannels[channelIndex];
			assert(channel);

			RevAnimationChannel animationChannel = {};
			animationChannel.m_nodeName = channel->mNodeName.C_Str();

			assert(channel->mNumPositionKeys == channel->mNumRotationKeys
				&& channel->mNumPositionKeys == channel->mNumScalingKeys);
			animationChannel.m_keyFrames = std::vector<RevKeyFrame>(channel->mNumPositionKeys);

			animationChannel.m_boneIndex = UINT8_MAX;
			for (UINT boneIndex = 0; boneIndex < initializationData.m_nBones; boneIndex++)
			{
				const RevBone& bone = initializationData.bones[boneIndex];
				if (strcmp(bone.m_name, animationChannel.m_nodeName.c_str()) == 0)
				{
					animationChannel.m_boneIndex = boneIndex;
					break;
				}
			}

			for (UINT keyFrameIndex = 0; keyFrameIndex < channel->mNumPositionKeys; keyFrameIndex++)
			{
				RevKeyFrame newKeyFrame = {};
				aiVectorKey locationKey = channel->mPositionKeys[keyFrameIndex];
				aiQuatKey rotationKey = channel->mRotationKeys[keyFrameIndex];
				aiVectorKey scaleKey = channel->mScalingKeys[keyFrameIndex];

				assert(locationKey.mTime == rotationKey.mTime
					&& locationKey.mTime == scaleKey.mTime);

				newKeyFrame.m_time = (float)locationKey.mTime;
				newKeyFrame.m_rotation = RevQuat(rotationKey.mValue.x, rotationKey.mValue.y, rotationKey.mValue.z, rotationKey.mValue.w);
				newKeyFrame.m_location = RevVector3(locationKey.mValue.x, locationKey.mValue.y, locationKey.mValue.z);
				newKeyFrame.m_scale = RevVector3(scaleKey.mValue.x, scaleKey.mValue.y, scaleKey.mValue.z);
				
				animationChannel.m_keyFrames[keyFrameIndex] = newKeyFrame;
			}

			animationModelInstanceData.m_channels.push_back(animationChannel);
		}

		initializationData.instanceData.push_back(animationModelInstanceData);
	}
	
	for (UINT boneIndex = 0; boneIndex < initializationData.m_nBones; boneIndex++)
	{
		RevBone& bone = initializationData.bones[boneIndex];

		bone.m_channelIndex = UINT_MAX;
		for (UINT instanceIndex = 0; instanceIndex< initializationData.instanceData.size(); instanceIndex++)
		{
			RevAnimationInstanceData& instance = initializationData.instanceData[instanceIndex];
			for (UINT channelIndex = 0; channelIndex < instance.m_channels.size(); channelIndex++)
			{
				RevAnimationChannel& channel = instance.m_channels[channelIndex];
				if (channel.m_boneIndex == boneIndex)
				{
					bone.m_channelIndex = channelIndex;
					break;
				}
			}
		}
	}
	//readjust vertex weights to be normalized
	for (UINT32 index = 0; index < initializationData.m_nVertexes; index++)
	{
		RevAnimatedVertex& v = initializationData.vertices[index];
		float sum = 0.0f;
		for (UINT i = 0; i < ARRAYSIZE(v.m_boneInfluences); i++)
		{
			if (v.m_boneInfluences[i] == UINT8_MAX)
			{
				v.m_boneInfluences[i] = 0;
				v.m_boneWeights[i] = 0.0f;
			}

			sum += v.m_boneWeights[i];
		}

		if (sum != 0.0f && sum != 1.0f)
		{
			for (UINT i = 0; i < ARRAYSIZE(v.m_boneInfluences); i++)
			{
				v.m_boneWeights[i] /= sum;
			}
		}
	}
	
	RevEngineFunctions::CreateAnimatedModelGeometry(
		initializationData, 
		newModel->m_modelData);

	{
		RevArchiveSaver saver;
		std::string modelPath(path);
		modelPath = modelPath.substr(0, modelPath.find_last_of('.'));
		modelPath.append("_MODEL.rrev");
		std::fstream fstream;
		fstream.open(modelPath.c_str(), std::ios_base::out | std::ios_base::binary);
		{
			saver << (UINT8)RevModelType::Animated;
			initializationData.Serialize(saver);
			fstream << saver.Tell();
			fstream.write((const char*)saver.m_byteArray, saver.Tell());
			fstream.close();
		}
	}


}
*/
void LoadNormalModel(const struct aiScene* scene, RevModelData& outModelData, const std::wstring& path)
{
#if USE_ASSIMP
	bool foundFile = false;
	for (UINT meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
	{
		const aiMesh* mesh = scene->mMeshes[meshIndex];
		if (mesh)
		{
			outModelData.m_staticVertexes.reserve(mesh->mNumVertices);
			for (UINT vertIndex = 0; vertIndex < mesh->mNumVertices; vertIndex++)
			{
				RevVertexPosTexNormBiTan staticVert = {};
				CreateBaseVertex(mesh, vertIndex, &staticVert);
				outModelData.m_staticVertexes.push_back(staticVert);
			}

			LoadIndecies(mesh, outModelData.m_indices);
			LoadTexturePaths(mesh, scene, outModelData.m_textures, path);
			for (auto& texturePath : outModelData.m_textures.m_textures)
			{
				LoadTexture(texturePath.m_path, &texturePath.m_resource);
			}
		}
	}

	/*{
		RevArchiveSaver saver;

		std::string modelPath(path);
		modelPath = modelPath.substr(0, modelPath.find_last_of('.'));
		modelPath.append("_MODEL.rrev");
		std::fstream fstream;
		fstream.open(modelPath.c_str(), std::ios_base::out | std::ios_base::binary);
		{
			saver << (UINT8)RevModelType::Normal;
			initializationData.Serialize(saver);
			fstream << saver.Tell();
			fstream.write((const char*)saver.m_byteArray, saver.Tell());
			fstream.close();
		}
	}*/
#endif
}



RevModel* RevModelLoader::LoadModel(const std::wstring& path)
{
	ID3D12GraphicsCommandList* commandList = RevEngineRetrievalFunctions::GetCommandList();
	ThrowIfFailed(commandList->Reset(RevEngineRetrievalFunctions::GetCommandAllocator(), nullptr));

	RevModel* newModel = new RevModel();
	RevModelData modelData = {};

	RevEModelType type = RevEModelType::ModelStatic;

	bool loadedBinary = false;
	std::wstring modelPath(path);
	modelPath = modelPath.substr(0, modelPath.find_last_of('.'));
	modelPath.append(L"_MODEL.rrev");

#if DO_LOAING_BINARY
	if (PathFileExists(modelPath.c_str()))
	{
		std::fstream fstream;
		fstream.open(modelPath.c_str(), std::ios_base::in | std::ios_base::binary);
		if (fstream.is_open())
		{
			UINT size = 0;
			fstream >> size;

			RevArchiveLoader loader(size);
			fstream.read((char*)loader.m_byteArray, size);
			loader << type;
			if (type == RevModelType::Animated)
			{
				RevAnimatedNodelInitializationData initData = {};
				initData.Serialize(loader);
				RevEngineFunctions::CreateAnimatedModelGeometry(
					initData,
					newModel->m_modelData);
			}
			else
			{
				RevNormalModelInitializationData initData = {};
				initData.Serialize(loader);
				RevEngineFunctions::CreateNormalModelGeometry(
					initData,
					newModel->m_modelData);
			}

			fstream.close();
			loader.Delete();
		}
	}
	else
#endif
	{
#if USE_ASSIMP
		char output[256];
		const WCHAR* wc =path.c_str();
		sprintf_s(&output[0], 256, "%ws", wc);
		const struct aiScene* scene = aiImportFile(output, 0);
		assert(scene);
		modelData.m_type = scene->HasAnimations() ? RevEModelType::ModelAnimated : RevEModelType::ModelStatic;
		if (modelData.m_type == RevEModelType::ModelStatic)
		{
			LoadNormalModel(scene, modelData, path);
		}
		else
		{
		//	LoadAnimatedModel(scene, newModel, path);
		}
#else
		assert(0 && "Not using assimp and dont have proepr context");
#endif
	}

	newModel->Initialize(modelData,  4);
	ThrowIfFailed(commandList->Close());
	ID3D12CommandList* cmdsLists[] = { commandList  };
	RevEngineRetrievalFunctions::GetCommandQueue()->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	// Wait until initialization is complete.
	RevEngineExecutionFunctions::FlushCommandQueue();
	return newModel;
}



RevModelData RevModelLoader::CreateModelDataFromFile(const std::wstring& path)
{
	RevModelData modelData = {};

	RevEModelType type = RevEModelType::ModelStatic;
	bool loadedBinary = false;
	std::wstring modelPath(path);
	modelPath = modelPath.substr(0, modelPath.find_last_of('.'));
	modelPath.append(L"_MODEL.rrev");

#if DO_LOAING_BINARY
	if (PathFileExists(modelPath.c_str()))
	{
		std::fstream fstream;
		fstream.open(modelPath.c_str(), std::ios_base::in | std::ios_base::binary);
		if (fstream.is_open())
		{
			UINT size = 0;
			fstream >> size;

			RevArchiveLoader loader(size);
			fstream.read((char*)loader.m_byteArray, size);
			loader << type;
			if (type == RevModelType::Animated)
			{
				RevAnimatedNodelInitializationData initData = {};
				initData.Serialize(loader);
				RevEngineFunctions::CreateAnimatedModelGeometry(
                    initData,
                    newModel->m_modelData);
			}
			else
			{
				RevNormalModelInitializationData initData = {};
				initData.Serialize(loader);
				RevEngineFunctions::CreateNormalModelGeometry(
                    initData,
                    newModel->m_modelData);
			}

			fstream.close();
			loader.Delete();
		}
	}
	else
#endif
	{
#if USE_ASSIMP
		char output[256];
		const WCHAR* wc =path.c_str();
		sprintf_s(&output[0], 256, "%ws", wc);
		const struct aiScene* scene = aiImportFile(output, 0);
		assert(scene);
		modelData.m_type = scene->HasAnimations() ? RevEModelType::ModelAnimated : RevEModelType::ModelStatic;
		if (modelData.m_type == RevEModelType::ModelStatic)
		{
			LoadNormalModel(scene, modelData, path);
		}
		else
		{
			//	LoadAnimatedModel(scene, newModel, path);
		}


		
#else
		assert(0 && "Not using assimp and dont have proepr context");
#endif
	}
	return modelData;
}

