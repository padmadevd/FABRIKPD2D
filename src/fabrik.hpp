#ifndef FABRIKPD2D_HPP
#define FABRIKPD2D_HPP

#include <cstdint>
#include <vector>
#include <map>

#include <raylib/raymath.h>

class Fabrik
{
    private:

    class Bone
    {
        private:
        Bone();
        Bone(uint32_t id, Vector2 start, Vector2 end, float minTheta, float maxTheta);
        Bone(uint32_t id, Vector2 start, Vector2 end, float minTheta, float maxTheta, uint32_t parent);

        uint32_t mID;

        Vector2 mStart;
        Vector2 mEnd;

        float mMinTheta;
        float mMaxTheta;

        uint32_t mParent;
        std::vector<uint32_t> mChildren;

        friend class Fabrik;
    };

    public:

    Fabrik();

    uint32_t AddRoot(Vector2 start, Vector2 end);
    uint32_t AddBone(uint32_t parent, Vector2 end, float minTheta, float maxTheta);

    // void SetBoneStart(uint32_t bone, Vector2 start);
    // Vector2 GetBoneStart(uint32_t bone);

    // void SetBoneEnd(uint32_t bone, Vector2 end);
    // Vector2 GetBoneEnd(uint32_t bone);

    // void SetMinTheta(uint32_t bone, float degree);
    // float GetMinTheta(uint32_t bone);

    // void SetMaxTheta(uint32_t bone, float degree);
    // float GetMaxTheta(uint32_t bone);

    std::vector<uint32_t> GetChildren(uint32_t bone);
    uint32_t GetRoot();

    void SetIterationLimit(uint32_t limit);
    uint32_t GetIterationLimit();

    void SetThreshold(float threshold);
    float GetThreshold();

    void Solve(std::vector<uint32_t> effectors, std::vector<Vector2> positions);

    private:

    std::vector<uint32_t> GetDescendants(uint32_t bone, std::map<uint32_t, Vector2> &mp);
    void SolveMultiEnd(uint32_t root, std::vector<uint32_t> effectors, std::vector<Vector2> positions);
    void SolveSingleEnd(uint32_t root, uint32_t effector, Vector2 position);

    std::vector<Bone*> mBones;
    uint32_t mCounter;

    uint32_t mRoot;

    uint32_t mIterationLimit;
    float mThreshold;
};

#endif