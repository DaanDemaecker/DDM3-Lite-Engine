// FbxLoader.h
// This class will handle the loading of fbx files

#ifndef FbxLoaderIncluded
#define FbxLoaderIncluded

// File includes
#include "../../../Includes/FbxSdkIncludes.h"
#include "../../../DataTypes/Structs.h"

// Standard library includes
#include <vector>
#include <unordered_map>
#include <memory>

namespace DDM3
{
	class FbxLoader
	{
	public:
		FbxLoader();

		~FbxLoader();

		// Load in a fbx file given a file path
		// Parameters:
		//     - path: The path to the model file
		//     - vertices: The vector that will be used to store the vertices
		//     - indices: The vector that will be used to store the indices
		void LoadFbxModel(const std::string& path, std::vector<DDM3::Vertex>& vertices, std::vector<uint32_t>& indices);

		// Load in animations given a file path
		// Parameters:
		//     - path: The path to the file
		//     - animationClips: The vector that will be used to store the animationc lips
		void LoadAnimations(const std::string& path, std::vector<std::unique_ptr<AnimationClip>>& animationClips);
	private:

		FbxManager* m_pFbxManager{};

		struct fbxTexturedInfo
		{
			bool textured{ false };
			int uvIndex{};
			FbxStringList uvSets{};
			int uvSetsCount{};
		};

		struct fbxBoneInfo
		{
			glm::vec4 boneIndices{};
			glm::vec4 boneWeights{};
			int boneAmount{};

			void AddBone(float boneIndex, float boneWeight)
			{
				if (boneAmount >= 4)return;

				boneIndices[boneAmount] = boneIndex;
				boneWeights[boneAmount] = boneWeight;

				boneAmount++;
			}
		};

		struct fbxSkinnedInfo
		{
			bool isSkinned{ false };
			FbxSkin* pSkin{ nullptr };
			std::vector<fbxBoneInfo> boneInfos{};
		};

		void ConvertMesh(FbxMesh* pMesh,
			std::unordered_map<DDM3::Vertex, uint32_t>& uniqueVertices, std::vector<DDM3::Vertex>& vertices,
			std::vector<uint32_t>& indices, int& baseUvIndex);

		void HandleFbxVertex(FbxMesh* pMesh, FbxVector4* controlPoints, int polygonIndex, int inPolygonPosition,
			std::unordered_map<DDM3::Vertex, uint32_t>& uniqueVerticesint,
			fbxTexturedInfo& textureInfo, fbxSkinnedInfo& skinnedInfo,
			std::vector<DDM3::Vertex>& vertices, std::vector<uint32_t>& indices);


		void SetupTangents(std::vector<DDM3::Vertex>& vertices, std::vector<uint32_t>& indices);

		void SetupSkin(fbxSkinnedInfo& skinnedInfo, int controlPointAmount);

		FbxScene* LoadScene(const std::string& path);
	};
}


#endif // !FbxLoaderIncluded
