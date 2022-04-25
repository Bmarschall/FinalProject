#pragma once
#include "AftrConfig.h"
#include "WO.h"
#include "PxPhysicsAPI.h"
#include "Mat4.h"

using namespace physx;

namespace Aftr {
    class WOphysx : public WO {
    public:
        PxScene* s;
        PxRigidDynamic* a = NULL;

        static WOphysx* New();
        static WOphysx* New(const std::string& modelFileName, const Vector& scale = Vector(1, 1, 1), MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstAUTO);
        static WOphysx* New(const std::string& modelFileName, const Vector& scale = Vector(1, 1, 1), MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstAUTO, physx::PxPhysics* p = NULL, PxScene* s = NULL, std::string physxInfo = "");

        virtual ~WOphysx();
        void setPosition(Vector vec);
        void updatePoseFromPhysicsEngine(PxActor* a);
        void addForce(PxVec3 force);
    protected:
        WOphysx(PxScene* scene);
        virtual void onCreate(const std::string& path, const Vector& scale, Aftr::MESH_SHADING_TYPE mst, PxPhysics* p, std::string physxInfo);

    };
}