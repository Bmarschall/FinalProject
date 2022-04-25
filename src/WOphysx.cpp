#include <WOphysx.h>
#include "PxPhysicsAPI.h"

using namespace physx;

namespace Aftr{
	void WOphysx::setPosition(Vector vec){
		PxTransform t = PxTransform(PxVec3(vec.at(0), vec.at(1), vec.at(2) ));
		this->a->setGlobalPose(t);
	}

	void WOphysx::onCreate(const std::string& path, const Vector& scale, Aftr::MESH_SHADING_TYPE mst, PxPhysics* p, std::string physxInfo){
		WO::onCreate(path, scale, mst);
		physx::PxMaterial* gMaterial = p->createMaterial(0.5f, 0.5f, 0.6f);
		PxShape* shape = p->createShape(PxSphereGeometry(scale[0]*2), *gMaterial, true);
		
		if (physxInfo == "s")
		{
			shape = p->createShape(PxSphereGeometry(scale[0]*5), *gMaterial, true);
		}
		else
		{
			shape = p->createShape(PxBoxGeometry(scale[0]*2, scale[1]*2, scale[2]*2), *gMaterial, true);
		}
		PxTransform t({ 0,0,0 });
		PxRigidDynamic* a = p->createRigidDynamic(t);
		a->attachShape(*shape);
		a->userData = this;
		this->s->addActor(*a);
		this->a = a;
		physx::PxU32 numActors = 0;
		physx::PxActor** actors = this->s->getActiveActors(numActors);
		std::cout << numActors << " actors" << std::endl;
	}

	void WOphysx::addForce(PxVec3 force){
		a->setLinearVelocity(PxVec3(0, 0, 0));
		a->addForce(force, PxForceMode::eIMPULSE);
	}

	void WOphysx::updatePoseFromPhysicsEngine(PxActor* a){
		if (this->a != NULL){
			std::cout << a->getConcreteTypeName() << std::endl;
			PxTransform transform = this->a->getGlobalPose();
			auto m = PxMat44(transform);
			std::cout << m.getPosition().x << "," << m.getPosition().y << "," << m.getPosition().z << std::endl;
			Mat4 m2;
			m2.setMeToIdentity();

			for (int i = 0; i < 16; i++){
				m2[i] = m(i % 4, i / 4);
			}
			this->setPose(m2);
		}
	}

	WOphysx* WOphysx::New(const std::string& modelFileName, const Vector& scale , MESH_SHADING_TYPE shadingType , physx::PxPhysics* p, PxScene* s, std::string physxInfo){
		WOphysx* wo = new WOphysx(s);
		wo->onCreate(modelFileName, scale, shadingType, p,physxInfo);
		return wo;
	}

	WOphysx::WOphysx(PxScene* scene) :IFace(this){ this->s = scene; }
	WOphysx::~WOphysx(){ }
}