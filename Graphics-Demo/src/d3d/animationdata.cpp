#include "animationdata.h"

#include "mathutil.h"

#include <assimp/scene.h>

#include <iostream>

namespace d3d
{
	using namespace DirectX;


	void AnimationData::interpolateBone(float timeInSeconds,
		const std::string& boneName,
		XMFLOAT4X4& transform) const
	{
		float timeInTicks = fmod((timeInSeconds * m_ticksPerSecond), m_ticksDuration);

		// get a reference to the desired bone
		auto iter = m_keyFrames.find(boneName);
		if (iter == m_keyFrames.end())
		{
			return;
		}

		const BoneAnimation& boneAnimation = iter->second;

		XMFLOAT3 scale{ 1.0f, 1.0f, 1.0f };
		XMFLOAT3 rotationOrigin{ 0.0f, 0.0f, 0.0f };
		XMFLOAT4 quaternion{};
		XMFLOAT3 position{};
		
		interpolateRotation(timeInTicks, boneAnimation, quaternion);
		interpolatePosition(timeInTicks, boneAnimation, position);

		XMMATRIX matrix = XMMatrixAffineTransformation(
			XMLoadFloat3(&scale),
			XMLoadFloat3(&rotationOrigin),
			XMLoadFloat4(&quaternion),
			XMLoadFloat3(&position)
		);

		XMStoreFloat4x4(&transform, matrix);


		/*std::cout << "Interpolated transform for bone: " << boneName << " at time: " << timeInSeconds << '\n';
		printMatrix(transform);
		printDecomp(transform);
		std::cout << '\n';*/

		if (boneName == "Root")
		{
			std::cout << "Interpolated transform for bone: " << boneName << " at time: " << timeInSeconds << ", tick: " << timeInTicks << '\n';
			//printMatrix(transform);
			printDecomp(transform);
			std::cout << '\n';
		}
	}


	AnimationData::AnimationData(const aiAnimation* animation)
	{
		if (!animation)
		{
			return;
		}

		m_ticksPerSecond = animation->mTicksPerSecond;
		m_ticksDuration = animation->mDuration;

		std::cout << "TicksPerSecond: " << m_ticksPerSecond << ", Duration in ticks" << m_ticksDuration << '\n';

		for (size_t boneIdx = 0; boneIdx < animation->mNumChannels; ++boneIdx)
		{
			const aiNodeAnim* nodeAnim = animation->mChannels[boneIdx];

			BoneAnimation& boneAnim = m_keyFrames[nodeAnim->mNodeName.C_Str()];

			for (size_t i = 0; i < nodeAnim->mNumPositionKeys; ++i)
			{
				const aiVectorKey& key = nodeAnim->mPositionKeys[i];

				boneAnim.PositionKeys.push_back(
					std::make_pair(key.mTime, XMFLOAT3(key.mValue.x, key.mValue.y, key.mValue.z))
				);


				/*if (i == 0)
				{
					std::cout << nodeAnim->mNodeName.C_Str() << " Position Key "
						<< ", x: " << key.mValue.x << ", y: " << key.mValue.y << ", z: " << key.mValue.z << '\n';
				}*/
			}


			for (size_t i = 0; i < nodeAnim->mNumRotationKeys; ++i)
			{
				const aiQuatKey& key = nodeAnim->mRotationKeys[i];
				
				/*if (i == 0)
				{
					std::cout << nodeAnim->mNodeName.C_Str() << " Quaternion Key " << "w: " << key.mValue.w
						<< ", x: " << key.mValue.x << ", y: " << key.mValue.y << ", z: " << key.mValue.z << '\n';
				}*/
				

				boneAnim.RotationKeys.push_back(
					std::make_pair(key.mTime, XMFLOAT4(key.mValue.x, key.mValue.y, key.mValue.z, key.mValue.w))
					//std::make_pair(key.mTime, XMFLOAT4(key.mValue.w, key.mValue.x, key.mValue.y, key.mValue.z))
				);


				// Based on this test it seems X, Y, Z, W is the correct order
				// to convert aiQuaternion to XMFLOAT4

				if (i == 0)
				{
					//std::cout << "Rotation Matrix from aiQuatKey.GetMatrix()" << '\n';
					//printRotationMatrix(key.mValue.GetMatrix());
					//std::cout << '\n';

					//XMFLOAT4 quat(key.mValue.w, key.mValue.x, key.mValue.y, key.mValue.z);

					XMFLOAT4 quat(key.mValue.x, key.mValue.y, key.mValue.z, key.mValue.w);
					XMMATRIX rotation = XMMatrixRotationQuaternion(XMLoadFloat4(&quat));
					XMFLOAT4X4 rot;
					XMStoreFloat4x4(&rot, rotation);

					std::cout << "Rotation Matrix from XMRotationQuaternion" << '\n';
					printMatrix(rot);
					printDecomp(rot);
					std::cout << '\n';
				}
			}


			for (size_t i = 0; i < nodeAnim->mNumScalingKeys; ++i)
			{
				const aiVectorKey& key = nodeAnim->mScalingKeys[i];

				boneAnim.ScalingKeys.push_back(
					std::make_pair(key.mTime, XMFLOAT3(key.mValue.x, key.mValue.y, key.mValue.z))
				);
			}

			
			std::cout << "Loading Keys for bone: " << nodeAnim->mNodeName.C_Str() << '\n';
			std::cout << "Position keys loaded: " << boneAnim.PositionKeys.size() << '\n';
			std::cout << "Rotation keys loaded: " << boneAnim.RotationKeys.size() << '\n';
			std::cout << "Scaling keys loaded: " << boneAnim.ScalingKeys.size() << '\n';
			std::cout << '\n';
		}

		std::cout << '\n';
	}


	void AnimationData::interpolateRotation(float timeInTicks, const BoneAnimation& boneAnim, DirectX::XMFLOAT4& quaternion) const
	{
		// if there is only one key the value is constant
		// though if there is only one key and its time is
		// zero theres no need to handle it as a special case
		// since the search will immediatly halt
		/*if (boneAnimation.PositionKeys.size() == 1)
		{
			boneAnimation.PositionKeys[0].second;
		}*/

		// Find the two keys that straddle the target time
		auto beforeKey = boneAnim.RotationKeys.begin();
		auto afterKey = boneAnim.RotationKeys.end();

		while (beforeKey < afterKey - 1)
		{
			auto guess = beforeKey + (afterKey - beforeKey) / 2;

			if (guess->first < timeInTicks)
			{
				beforeKey = guess;
			}
			else if (guess->first > timeInTicks)
			{
				afterKey = guess;
			}
			else
			{
				beforeKey = guess;
				afterKey = beforeKey + 1;
			}
		}


		if (beforeKey->first == timeInTicks ||
			afterKey == boneAnim.RotationKeys.end())
		{
			// Before key is the exact time or is the last/only key
			// so no interpolation is needed;

			quaternion = beforeKey->second;
		}
		else
		{
			// Interpolate between before and after rotation

			float normalizedTime = (timeInTicks - beforeKey->first) / (afterKey->first - beforeKey->first);

			XMVECTOR beforeQuat = XMLoadFloat4(&beforeKey->second);
			XMVECTOR afterQuat = XMLoadFloat4(&afterKey->second);

			XMStoreFloat4(&quaternion, XMQuaternionSlerp(beforeQuat, afterQuat, normalizedTime));
		}
	}


	void AnimationData::interpolatePosition(float timeInTicks, const BoneAnimation& boneAnim, XMFLOAT3& position) const
	{
		// Find the two keys that straddle the target time

		auto beforeKey = boneAnim.PositionKeys.begin();
		auto afterKey = boneAnim.PositionKeys.end();

		while (beforeKey < afterKey - 1)
		{
			auto guess = beforeKey + (afterKey - beforeKey) / 2;

			if (guess->first < timeInTicks)
			{
				beforeKey = guess;
			}
			else if (guess->first > timeInTicks)
			{
				afterKey = guess;
			}
			else
			{
				beforeKey = guess;
				afterKey = beforeKey + 1;
			}
		}


		if (beforeKey->first == timeInTicks ||
			afterKey == boneAnim.PositionKeys.end())
		{
			// Before key is the exact time or is the last/only key
			// so no interpolation is needed;
			position = beforeKey->second;
		}
		else
		{
			// Use previous key rather than interpolate
			//position = beforeKey->second;


			// Interpolation was causing severe bobbing up and down
			// Had used normalized time vs timeInTicks resulting in extrapolation
			//XMStoreFloat3(&position, XMVectorLerp(beforePos, afterPos, timeInTicks));
			
			// Interpolate between before and after position
			float normalizedTime = (timeInTicks - beforeKey->first) / (afterKey->first - beforeKey->first);

			XMVECTOR beforePos = XMLoadFloat3(&beforeKey->second);
			XMVECTOR afterPos = XMLoadFloat3(&afterKey->second);

			XMStoreFloat3(&position, XMVectorLerp(beforePos, afterPos, normalizedTime));

			
		}
	}
}
