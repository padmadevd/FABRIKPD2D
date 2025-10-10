#include "fabrik.hpp"

#include <cmath>
#include <cstdint>
#include <vector>

#include "raylib/raymath.h"

Fabrik::Bone::Bone()
    : mID(0), mStart{0, 0}, mEnd{0, 0}, mMinTheta(0), mMaxTheta(0), mParent(0), mChildren()
{
}

Fabrik::Bone::Bone(uint32_t id, Vector2 start, Vector2 end, float minTheta, float maxTheta)
    : mID(id), mStart(start), mEnd(end), mMinTheta(minTheta), mMaxTheta(maxTheta), mParent(0), mChildren()
{
}

Fabrik::Bone::Bone(uint32_t id, Vector2 start, Vector2 end, float minTheta, float maxTheta, uint32_t parent)
    : mID(id), mStart(start), mEnd(end), mMinTheta(minTheta), mMaxTheta(maxTheta), mParent(parent), mChildren()
{
}

Fabrik::Fabrik()
    : mBones(), mCounter(0), mRoot(0), mIterationLimit(20), mThreshold(1.f)
{
    mCounter++;
    mBones.push_back(nullptr);
}

uint32_t Fabrik::AddRoot(Vector2 start, Vector2 end)
{
    if(mRoot != 0)
    {
        return 0;
    }

    Bone *bone = new Bone(mCounter++, start, end, -180, 180, 0);
    mBones.push_back(bone);

    mRoot = 1;

    return bone->mID;
}

uint32_t Fabrik::AddBone(uint32_t parent, Vector2 end, float minTheta, float maxTheta)
{
    if(mRoot == 0 || parent >= mCounter || parent < 1)
    {
        return 0;
    }

    float theta = RAD2DEG*Vector2Angle(Vector2Normalize(mBones[parent]->mEnd-mBones[parent]->mStart), Vector2Normalize(end-mBones[parent]->mEnd));

    if(theta < minTheta || theta > maxTheta)
    {
        theta = ((theta < minTheta) ? minTheta : maxTheta);
        theta += RAD2DEG*Vector2Angle(Vector2{1, 0}, Vector2Normalize(mBones[parent]->mEnd-mBones[parent]->mStart));

        float length = Vector2Distance(end, mBones[parent]->mEnd);
        end = mBones[parent]->mEnd + Vector2Rotate(Vector2{1, 0}, DEG2RAD*theta)*length;
    }

    Bone *bone = new Bone(mCounter++, mBones[parent]->mEnd, end, minTheta, maxTheta, parent);
    mBones.push_back(bone);

    mBones[parent]->mChildren.push_back(bone->mID);
    return bone->mID;
}

// void Fabrik::SetBoneStart(uint32_t bone, Vector2 start)
// {
//     if(bone < 1 || bone >= mCounter)
//     {
//         return;
//     }

//     mBones[bone]->mStart = start;
//     if(mBones[bone]->mParent != 0)
//     {
//         mBones[mBones[bone]->mParent]->mEnd = start;
//         for(uint32_t child : mBones[mBones[bone]->mParent]->mChildren)
//         {
//             mBones[child]->mStart = start;
//         }
//     }
// }

// Vector2 Fabrik::GetBoneStart(uint32_t bone)
// {
//     if(bone < 1 || bone >= mCounter)
//     {
//         return Vector2{0, 0};
//     }
//     return mBones[bone]->mStart;
// }

// void Fabrik::SetBoneEnd(uint32_t bone, Vector2 end)
// {
//     if(bone < 1 || bone >= mCounter)
//     {
//         return;
//     }

//     if(mBones[bone]->mParent != 0)
//     {
//         uint32_t parent = mBones[bone]->mParent;
//         float theta = RAD2DEG*Vector2Angle(Vector2Normalize(mBones[parent]->mEnd-mBones[parent]->mStart), Vector2Normalize(end-mBones[bone]->mStart));
//         if(theta < mBones[bone]->mMinTheta || theta > mBones[bone]->mMaxTheta)
//         {
//             theta = ((theta < mBones[bone]->mMinTheta) ? mBones[bone]->mMinTheta :  mBones[bone]->mMaxTheta);
//             theta += RAD2DEG*Vector2Angle(Vector2{1, 0}, Vector2Normalize(mBones[parent]->mEnd-mBones[parent]->mStart));

//             float length = Vector2Distance(end, mBones[bone]->mStart);
//             end = mBones[bone]->mStart + Vector2Rotate(Vector2{1, 0}, DEG2RAD*theta)*length;
//         }
//     }

//     mBones[bone]->mEnd = end;
//     for(uint32_t child : mBones[bone]->mChildren)
//     {
//         mBones[child]->mStart = end;
//     }
// }

// Vector2 Fabrik::GetBoneEnd(uint32_t bone)
// {
//     if(bone < 1 || bone >= mCounter)
//     {
//         return Vector2{0, 0};
//     }
//     return mBones[bone]->mEnd;
// }

// void Fabrik::SetMinTheta(uint32_t bone, float degree)
// {
//     if(bone < 1 || bone >= mCounter)
//     {
//         return;
//     }
//     mBones[bone]->mMinTheta = degree;
// }
// float Fabrik::GetMinTheta(uint32_t bone)
// {
//     if(bone < 1 || bone >= mCounter)
//     {
//         return 0;
//     }
//     return mBones[bone]->mMinTheta;
// }

// void Fabrik::SetMaxTheta(uint32_t bone, float degree)
// {
//     if(bone < 1 || bone >= mCounter)
//     {
//         return;
//     }
//     mBones[bone]->mMaxTheta = degree;
// }
// float Fabrik::GetMaxTheta(uint32_t bone)
// {
//     if(bone < 1 || bone >= mCounter)
//     {
//         return 0;
//     }
//     return mBones[bone]->mMaxTheta;
// }

std::vector<uint32_t> Fabrik::GetChildren(uint32_t bone)
{
    if(bone < 1 || bone >= mCounter)
    {
        return {};
    }
    return mBones[bone]->mChildren;
}
uint32_t Fabrik::GetRoot()
{
    return mRoot;
}

void Fabrik::SetIterationLimit(uint32_t limit)
{
    mIterationLimit = limit;
}
uint32_t Fabrik::GetIterationLimit()
{
    return mIterationLimit;
}

void Fabrik::SetThreshold(float threshold)
{
    mThreshold = threshold;
}
float Fabrik::GetThreshold()
{
    return mThreshold;
}

void Fabrik::Solve(std::vector<uint32_t> effectors, std::vector<Vector2> positions)
{
    if(mRoot == 0)
    {
        return;
    }

    std::map<uint32_t, Vector2> mp;
    for(int i = 0; i < effectors.size(); i++)
    {
        mp[effectors[i]] = positions[i];
    }

    if(mp.find(mRoot) != mp.end())
    {
        mBones[mRoot]->mStart = mp[mRoot];
    }

    std::vector<uint32_t> queueA;
    std::vector<uint32_t> queueB;

    queueA.push_back(mRoot);

    while(!queueA.empty())
    {
        for(uint32_t bone : queueA)
        {
            std::vector<uint32_t> children = GetDescendants(bone, mp);
            std::vector<Vector2> childPositions;
            for(uint32_t child : children)
            {
                queueB.push_back(child);
                childPositions.push_back(mp[child]);
            }
            SolveMultiEnd(bone, children, childPositions);
        }
        queueA = queueB;
        queueB.clear();
    }
}

std::vector<uint32_t> Fabrik::GetDescendants(uint32_t bone, std::map<uint32_t, Vector2> &mp)
{
    std::vector<uint32_t> children;

    mp.erase(bone);

    std::vector<uint32_t> boneStack;
    std::vector<uint32_t> childStack;
    boneStack.push_back(bone);
    childStack.push_back(0);

    while(!boneStack.empty())
    {
        uint32_t currBone = boneStack.back();
        uint32_t childIndex = childStack.back();

        if(mp.find(currBone) != mp.end())
        {
            children.push_back(currBone);
            boneStack.pop_back();
            childStack.pop_back();
        }
        else if(childIndex >= mBones[currBone]->mChildren.size())
        {
            boneStack.pop_back();
            childStack.pop_back();
        }
        else
        {
            childStack.back() = childIndex+1;
            boneStack.push_back(mBones[currBone]->mChildren[childIndex]);
            childStack.push_back(0);
        }
    }

    return children;
}

void Fabrik::SolveMultiEnd(uint32_t root, std::vector<uint32_t> effectors, std::vector<Vector2> positions)
{
    if(effectors.size() == 1)
    {
        return SolveSingleEnd(root, effectors[0], positions[0]);
    }

    
}

void Fabrik::SolveSingleEnd(uint32_t root, uint32_t effector, Vector2 position)
{
    
}