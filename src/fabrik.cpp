#include "fabrik.hpp"

#include "raylib/raylib.h"
#include "raylib/raymath.h"

#include <cstdint>
#include <map>
#include <vector>

Fabrik::Bone::Bone()
    : mID(0), mLength(0), mTheta(0), mMinTheta(-180), mMaxTheta(180), mPrev(0), mNext(0)
{
}

Fabrik::Bone::Bone(uint32_t id, float length, float theta, float minTheta, float maxTheta)
    : mID(id), mLength(0), mTheta(), mMinTheta(minTheta), mMaxTheta(maxTheta),mPrev(0), mNext(0)
{
}

Fabrik::Bone::Bone(uint32_t id, float length, float theta, float minTheta, float maxTheta, uint32_t prev)
    : mID(id), mLength(length), mTheta(theta), mMinTheta(minTheta), mMaxTheta(maxTheta), mPrev(prev), mNext(0)
{
}

Fabrik::Fabrik()
    : mBones(), mBasePosition{0, 0}, mBaseTheta(0), mIterationLimit(20), mIterationThreshold(0.1f), mThreshold(1.f)
{
    mBones.push_back(Bone());
}

uint32_t Fabrik::AddRoot(Vector2 start, Vector2 end)
{
    if(mBones.size() > 1)
    {
        return 0;
    }

    mBasePosition = start;

    Bone bone;
    bone.mID = mBones.size();
    bone.mLength = Vector2Distance(start, end);
    bone.mTheta = RAD2DEG*Vector2Angle(Vector2{1, 0}, Vector2Normalize(end-start))-mBaseTheta;
    mBones.push_back(bone);
    return bone.mID;
}

uint32_t Fabrik::AddBone(Vector2 end)
{
    if(mBones.size() <= 1)
    {
        return 0;
    }

    uint32_t last = mBones.back().mID;

    Bone bone;
    bone.mID = mBones.size();

    Vector2 start = GetBoneEnd(last);
    bone.mLength = Vector2Distance(start, end);
    bone.mTheta = RAD2DEG*Vector2Angle(Vector2{1, 0}, Vector2Normalize(end-start))-GetThetaGlobal(last);

    mBones[last].mNext = bone.mID;
    bone.mPrev = last;

    mBones.push_back(bone);
    return bone.mID;
}

uint32_t Fabrik::GetPrevBone(uint32_t bone)
{
    if(bone < 1 || bone >= mBones.size())
    {
        return 0;
    }
    return mBones[bone].mPrev;
}
uint32_t Fabrik::GetNextBone(uint32_t bone)
{
    if(bone < 1 || bone >= mBones.size())
    {
        return 0;
    }
    return mBones[bone].mNext;
}
uint32_t Fabrik::GetRoot()
{
    if(mBones.size() == 0)
    {
        return 0;
    }
    return 1;
}

Vector2 Fabrik::GetBasePosition()
{
    return mBasePosition;
}
void Fabrik::SetBasePosition(Vector2 position)
{
    mBasePosition = position;
}

float Fabrik::GetBaseTheta()
{
    return mBaseTheta;
}
void Fabrik::SetBaseTheta(float theta)
{
    mBaseTheta = theta;
}

float Fabrik::GetTheta(uint32_t bone)
{
    if(bone < 1 || bone >= mBones.size())
    {
        return 0;
    }
    return mBones[bone].mTheta;
}
void Fabrik::SetTheta(uint32_t bone, float theta)
{
    if(bone < 1 || bone >= mBones.size())
    {
        return;
    }
    if(theta < mBones[bone].mMinTheta)
    {
        theta = mBones[bone].mMinTheta;
    }
    if(theta > mBones[bone].mMaxTheta)
    {
        theta = mBones[bone].mMaxTheta;
    }
    mBones[bone].mTheta = theta;
}

float Fabrik::GetLength(uint32_t bone)
{
    if(bone < 1 || bone >= mBones.size())
    {
        return 0;
    }
    return mBones[bone].mLength;
}
void Fabrik::SetLength(uint32_t bone, float length)
{
    if(bone < 1 || bone >= mBones.size())
    {
        return;
    }
    mBones[bone].mLength = length;
}

Vector2 Fabrik::GetBoneStart(uint32_t bone)
{
    if(bone < 1 || bone >= mBones.size())
    {
        return Vector2{0, 0};
    }

    Vector2 position = mBasePosition;
    float thetaGlobal = mBaseTheta;
    uint32_t curr = 1;
    while(curr < bone)
    {
        float length = mBones[curr].mLength;
        float theta = mBones[curr].mTheta;
        thetaGlobal += theta;
        position += Vector2Rotate(Vector2{1, 0}, thetaGlobal*DEG2RAD)*length;
        curr++;
    }

    return position;
}

Vector2 Fabrik::GetBoneEnd(uint32_t bone)
{
    if(bone < 1 || bone >= mBones.size())
    {
        return Vector2{0, 0};
    }

    Vector2 position = mBasePosition;
    float theta = mBaseTheta;
    uint32_t curr = 1;
    while(curr <= bone)
    {
        theta += mBones[curr].mTheta;
        position += Vector2Rotate(Vector2{1, 0}, theta*DEG2RAD)*mBones[curr].mLength;
        curr++;
    }

    return position;
}

float Fabrik::GetThetaGlobal(uint32_t bone)
{
    if(bone < 1 || bone >= mBones.size())
    {
        return mBaseTheta;
    }

    float theta = mBaseTheta;
    uint32_t curr = 1;
    while(curr <= bone)
    {
        theta += mBones[curr].mTheta;
        curr++;
    }
    return theta;
}

void Fabrik::SetMinTheta(uint32_t bone, float theta)
{
    if(bone < 1 || bone >= mBones.size())
    {
        return;
    }
    if(mBones[bone].mTheta < theta)
    {
        mBones[bone].mTheta = theta;
    }
    mBones[bone].mMinTheta = theta;
}
float Fabrik::GetMinTheta(uint32_t bone)
{
    if(bone < 1 || bone >= mBones.size())
    {
        return 0;
    }
    return mBones[bone].mMinTheta;
}

void Fabrik::SetMaxTheta(uint32_t bone, float theta)
{
    if(bone < 1 || bone >= mBones.size())
    {
        return;
    }
    if(mBones[bone].mTheta > theta)
    {
        mBones[bone].mTheta = theta;
    }
    mBones[bone].mMaxTheta = theta;
}
float Fabrik::GetMaxTheta(uint32_t bone)
{
    if(bone < 1 || bone >= mBones.size())
    {
        return 0;
    }
    return mBones[bone].mMaxTheta;
}

void Fabrik::SetIterationLimit(uint32_t limit)
{
    mIterationLimit = limit;
}
uint32_t Fabrik::GetIterationLimit()
{
    return mIterationLimit;
}

void Fabrik::SetIterationThreshold(float threshold)
{
    mIterationThreshold = threshold;
}
float Fabrik::GetIterationThreshold()
{
    return mIterationThreshold;
}

void Fabrik::SetThreshold(float threshold)
{
    mThreshold = threshold;
}
float Fabrik::GetThreshold()
{
    return mThreshold;
}

void Fabrik::Solve(std::vector<uint32_t> effectors, std::vector<Vector2> targets, std::vector<bool> fixed)
{
    if(mBones.size() <= 2)
    {
        return;
    }

    std::map<uint32_t, Vector2> mpTarget;
    std::map<uint32_t, bool> mpFixed;
    for(int i = 0; i < effectors.size(); i++)
    {
        mpTarget[effectors[i]] = targets[i];
        mpFixed[effectors[i]] = fixed[i];
    }
    if(mpTarget.find(1) != mpTarget.end())
    {
        mBasePosition = mpTarget[1];
    }

    uint32_t base = 1;
    uint32_t curr = 2;
    while(curr < mBones.size())
    {
        if(mpTarget.find(curr) != mpTarget.end())
        {
            SolveSingleEnd(base, curr, mpTarget[curr]);
            if(mpFixed[curr])
            {
                base = curr;
            }
        }
        ++curr;
    }
}

void Fabrik::SolveSingleEnd(uint32_t base, uint32_t effector, Vector2 target)
{
    uint32_t numberOfNodes = effector-base+1;
    Vector2 baseStart = GetBoneStart(base);
    float baseTheta = GetThetaGlobal(base-1);

    std::vector<Vector2> positions(numberOfNodes);
    std::vector<float> lengths(numberOfNodes);
    {
        Vector2 start = baseStart;
        float thetaGlobal = baseTheta;

        uint32_t curr = base;
        int i = 0;
        while(curr <= effector)
        {
            positions[i] = start;
            lengths[i] = mBones[curr].mLength;

            thetaGlobal += mBones[curr].mTheta;
            start += Vector2Rotate(Vector2{1, 0}, DEG2RAD*thetaGlobal)*mBones[curr].mLength;
            ++curr;
            ++i;
        }
    }

    Vector2 prevEffectorStart = target;
    uint32_t iterations = 0;

    while((Vector2Distance(positions[numberOfNodes-1], target) > mThreshold) && (Vector2Distance(positions[numberOfNodes-1], prevEffectorStart) > mIterationThreshold) && (iterations < mIterationLimit))
    {
        prevEffectorStart = positions[numberOfNodes-1];

        // FORWARD REACHING
        int i = numberOfNodes-1;
        uint32_t curr = effector;
        positions[i] = target;
        --curr;
        --i;
        while(i >= 0)
        {
            float r = Vector2Distance(positions[i], positions[i+1]);
            float lambda = lengths[i]/r;
            positions[i] = positions[i+1]*(1-lambda) + positions[i]*(lambda);

            if(i < numberOfNodes-2)
            {
                Vector2 a = positions[i+1]-positions[i];
                Vector2 b = positions[i+2]-positions[i+1];
                float theta = RAD2DEG*Vector2Angle(a, b);

                if(theta < mBones[curr].mMinTheta)
                {
                    positions[i] = positions[i+1]+Vector2Rotate(Vector2Normalize(Vector2Invert(b)), -mBones[curr].mMinTheta)*lengths[i];
                }
                else if(theta > mBones[curr].mMaxTheta)
                {
                    positions[i] = positions[i+1]+Vector2Rotate(Vector2Normalize(Vector2Invert(b)), -mBones[curr].mMaxTheta)*lengths[i];
                }
            }

            --curr;
            --i;
        }

        // BACKWARD REACHING
        i = 0;
        curr = base;
        positions[i] = baseStart;
        while(i < numberOfNodes-1)
        {
            float r = Vector2Distance(positions[i], positions[i+1]);
            float lambda = lengths[i]/r;
            positions[i+1] = positions[i]*(1-lambda) + positions[i+1]*(lambda);

            if(i > 0)
            {
                Vector2 a = positions[i]-positions[i-1];
                Vector2 b = positions[i+1]-positions[i];
                float theta = RAD2DEG*Vector2Angle(a, b);

                if(theta < mBones[curr].mMinTheta)
                {
                    positions[i+1] = positions[i]+Vector2Rotate(Vector2Normalize(a), mBones[curr].mMinTheta)*lengths[i];
                }
                else if(theta > mBones[curr].mMaxTheta)
                {
                    positions[i+1] = positions[i]+Vector2Rotate(Vector2Normalize(a), mBones[curr].mMaxTheta)*lengths[i];
                }
            }

            ++i;
            ++curr;
        }

        ++iterations;
    }

    // FOR REMAINING NODES AFTER EFFECTOR
    uint32_t numberOfRemain = mBones.back().mID-effector+1 + 1; // additional joint for end of last node
    std::vector<Vector2> positionsRemain(numberOfRemain);
    std::vector<float> lengthsRemain(numberOfRemain);
    {
        Vector2 start = GetBoneStart(effector);
        float thetaGlobal = GetThetaGlobal(effector-1);

        uint32_t curr = effector;
        int i = 0;
        while(curr <= mBones.back().mID)
        {
            positionsRemain[i] = start;
            lengthsRemain[i] = mBones[curr].mLength;

            thetaGlobal += mBones[curr].mTheta;
            start += Vector2Rotate(Vector2{1, 0}, DEG2RAD*thetaGlobal)*mBones[curr].mLength;
            ++curr;
            ++i;
        }
        positionsRemain[i] = start; // end of last node

        // BACKWARD REACHING ONCE
        i = 0;
        curr = effector;
        positionsRemain[i] = positions.back();
        while(i < numberOfRemain-1)
        {
            float r = Vector2Distance(positionsRemain[i], positionsRemain[i+1]);
            float lambda = lengthsRemain[i]/r;
            positionsRemain[i+1] = positionsRemain[i]*(1-lambda) + positionsRemain[i+1]*(lambda);

            if(curr > 1)
            {
                Vector2 a;
                if(i > 0)
                {
                    a = positionsRemain[i]-positionsRemain[i-1];
                }
                else if(numberOfNodes > 1)
                {
                    a = positionsRemain[i]-positions[numberOfNodes-2];
                }
                else
                {
                    a = positionsRemain[i]-GetBoneStart(curr-1);
                }
                Vector2 b = positionsRemain[i+1]-positionsRemain[i];
                float theta = RAD2DEG*Vector2Angle(a, b);

                if(theta < mBones[curr].mMinTheta)
                {
                    positionsRemain[i+1] = positionsRemain[i]+Vector2Rotate(Vector2Normalize(a), mBones[curr].mMinTheta)*lengthsRemain[i];
                }
                else if(theta > mBones[curr].mMaxTheta)
                {
                    positionsRemain[i+1] = positionsRemain[i]+Vector2Rotate(Vector2Normalize(a), mBones[curr].mMaxTheta)*lengthsRemain[i];
                }
            }

            ++i;
            ++curr;
        }
    }

    {
        // FOR NODES IN ACTION
        Vector2 start = baseStart;
        float thetaGlobal = baseTheta;

        uint32_t curr = base;
        int i = 1;
        while(i < numberOfNodes)
        {
            Vector2 end = positions[i];

            float theta = RAD2DEG*Vector2Angle(Vector2{1, 0}, Vector2Normalize(end-start))-thetaGlobal;
            mBones[curr].mTheta = theta;

            thetaGlobal += theta;
            start = end;
            ++curr;
            ++i;
        }
    }

    {
        // FOR REMAINING NODES
        Vector2 start = GetBoneStart(effector);
        float thetaGlobal = GetThetaGlobal(effector-1);

        uint32_t curr = effector;
        int i = 1;
        while(i < numberOfRemain)
        {
            Vector2 end = positionsRemain[i];

            float theta = RAD2DEG*Vector2Angle(Vector2{1, 0}, Vector2Normalize(end-start))-thetaGlobal;
            mBones[curr].mTheta = theta;

            thetaGlobal += theta;
            start = end;
            ++curr;
            ++i;
        }
    }
}