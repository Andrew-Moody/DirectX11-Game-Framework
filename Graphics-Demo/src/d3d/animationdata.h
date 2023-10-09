#pragma once

#include <DirectXMath.h>

#include <vector>
#include <string>
#include <map>

struct aiAnimation;

namespace d3d
{
	struct BoneAnimation;


	class AnimationData
	{
	public:

		void interpolateBone(float timeInSeconds,
			const std::string& boneName,
			DirectX::XMFLOAT4X4& transform) const;

		AnimationData(const aiAnimation* animation);

	private:

		// The animation data for a single bone
		struct BoneAnimation
		{
			// Keyframes for postion, rotation, and scale can't be stored together in
			// a structure since they don't have the same number of keyframes
			// Keyframes are stored in order of increasing time to allow easy searching
			std::vector<std::pair<double, DirectX::XMFLOAT3>> PositionKeys;
			std::vector<std::pair<double, DirectX::XMFLOAT4>> RotationKeys;
			std::vector<std::pair<double, DirectX::XMFLOAT3>> ScalingKeys;
		};


		void interpolateRotation(float timeInTicks, const BoneAnimation& boneAnim, DirectX::XMFLOAT4& quaternion) const;

		void interpolatePosition(float timeInTicks, const BoneAnimation& boneAnim, DirectX::XMFLOAT3& position) const;

		double m_ticksPerSecond{};
		double m_ticksDuration{};

		// Ideally use an index rather than a string eventually
		std::map<std::string, BoneAnimation> m_keyFrames;
	};
}
