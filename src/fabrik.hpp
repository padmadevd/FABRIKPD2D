#ifndef FABRIKPD2D_HPP
#define FABRIKPD2D_HPP

#include <cstdint>
#include <vector>

#include <raylib/raylib.h>
#include <raylib/raymath.h>

class Fabrik
{
    private:

    class Bone
    {
        private:

        Bone();
        Bone(uint32_t id, float length, float theta, float minTheta, float maxTheta);
        Bone(uint32_t id, float length, float theta, float minTheta, float maxTheta, uint32_t prev);

        uint32_t mID;

        float mLength;
        float mTheta;

        float mMinTheta;
        float mMaxTheta;

        uint32_t mPrev;
        uint32_t mNext;

        friend class Fabrik;
    };

    public:

    Fabrik();

    uint32_t AddRoot(Vector2 start, Vector2 end);
    uint32_t AddBone(Vector2 end);

    uint32_t GetPrevBone(uint32_t bone);
    uint32_t GetNextBone(uint32_t bone);
    uint32_t GetRoot();

    Vector2 GetBasePosition();
    void SetBasePosition(Vector2 position);

    float GetBaseTheta();
    void SetBaseTheta(float theta);

    float GetTheta(uint32_t bone);
    void SetTheta(uint32_t bone, float theta);

    float GetLength(uint32_t bone);
    void SetLength(uint32_t bone, float length);

    Vector2 GetBoneStart(uint32_t bone);
    Vector2 GetBoneEnd(uint32_t bone);

    float GetThetaGlobal(uint32_t bone);

    void SetMinTheta(uint32_t bone, float theta);
    float GetMinTheta(uint32_t bone);

    void SetMaxTheta(uint32_t bone, float theta);
    float GetMaxTheta(uint32_t bone);

    void SetIterationLimit(uint32_t limit);
    uint32_t GetIterationLimit();

    void SetIterationThreshold(float threshold);
    float GetIterationThreshold();

    void SetThreshold(float threshold);
    float GetThreshold();

    void Solve(std::vector<uint32_t> effectors, std::vector<Vector2> targets, std::vector<bool> fixed);

    private:

    void SolveSingleEnd(uint32_t base, uint32_t effector, Vector2 target);

    std::vector<Bone> mBones;
    Vector2 mBasePosition;
    float mBaseTheta;

    uint32_t mIterationLimit;
    float mIterationThreshold;
    float mThreshold;
};

#endif