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
        PxFoundation* f;
        PxPhysics* p;
        PxRigidDynamic* ad = NULL;
        PxRigidStatic* as = NULL;

        static WOphysx* New();
        static WOphysx* New(const std::string& modelFileName, const Vector& scale = Vector(1, 1, 1), MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstAUTO);
        static WOphysx* New(const std::string& modelFileName, const Vector& scale = Vector(1, 1, 1), MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstAUTO, physx::PxPhysics* p = NULL, PxScene* s = NULL, std::string physxInfo = "",PxFoundation* f = NULL);

        virtual ~WOphysx();
        void setPosition(Vector vec);
        void rotateAboutRelX(float deltaRadianAngle);
        void rotateAboutRelY(float deltaRadianAngle);
        void rotateAboutRelZ(float deltaRadianAngle);
        void updatePoseFromPhysicsEngine(PxActor* a);
        void addForce(PxVec3 force);
        void stopForce();
        float  getForce();
        PxVec3 getForces();
        float* vertexListCopy;
        unsigned int* indicesCopy;
        WOphysx* _wo;
    protected:
        WOphysx(PxScene* scene,PxFoundation* found, PxPhysics* phys);
        virtual void onCreate(const std::string& path, const Vector& scale, Aftr::MESH_SHADING_TYPE mst, PxPhysics* p, std::string physxInfo);
        virtual void onCreate(const std::string& path, const Vector& scale, Aftr::MESH_SHADING_TYPE mst, PxPhysics* p);
    };
}