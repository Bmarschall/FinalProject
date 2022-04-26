#include <WOphysx.h>
#include "PxPhysicsAPI.h"

using namespace physx;

namespace Aftr{
	void WOphysx::setPosition(Vector vec){
		PxTransform t = PxTransform(PxVec3(vec.at(0), vec.at(1), vec.at(2) ));
		this->a->setGlobalPose(t);
	}

	void WOphysx::onCreate(const std::string& path, const Vector& scale, Aftr::MESH_SHADING_TYPE mst){
		WO::onCreate(path, scale, mst);

		//make copy of vertex list and index list
		size_t vertexListSize = this->getModel()->getModelDataShared()->getCompositeVertexList().size();
		size_t indexListSize = this->getModel()->getModelDataShared()->getCompositeIndexList().size();

		this->vertexListCopy = new float[vertexListSize * 3];//might be a better way to do this without making a copy
		this->indicesCopy = new unsigned int[indexListSize];//assuming the composite lists are stored in contiguous memory

		for (size_t i = 0; i < vertexListSize; i++){
			this->vertexListCopy[i * 3 + 0] = this->getModel()->getModelDataShared()->getCompositeVertexList().at(i).x;
			this->vertexListCopy[i * 3 + 1] = this->getModel()->getModelDataShared()->getCompositeVertexList().at(i).y;
			this->vertexListCopy[i * 3 + 2] = this->getModel()->getModelDataShared()->getCompositeVertexList().at(i).z;
		}
		for (size_t i = 0; i < indexListSize; i++)
			this->indicesCopy[i] = this->getModel()->getModelDataShared()->getCompositeIndexList().at(i);

		PxTriangleMeshDesc meshDesc;
		meshDesc.points.count = vertexListSize;
		meshDesc.points.stride = sizeof(float) * 3;//tightly packaged
		meshDesc.points.data = this->vertexListCopy;

		meshDesc.triangles.count = indexListSize / 3;
		meshDesc.triangles.stride = 3 * sizeof(unsigned int);//aside about index lists here
		meshDesc.triangles.data = this->indicesCopy;

		PxDefaultMemoryOutputStream writeBuffer;
		PxTriangleMeshCookingResult::Enum result;
		bool status = c->cookTriangleMesh(meshDesc, writeBuffer, &result);
		if (!status)
		{
			std::cout << "Failed to create Triangular mesh" << std::endl;
			std::cin.get();
		}
		PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
		physx::PxTriangleMesh* mesh = this->p->createTriangleMesh(readBuffer);

		PxMaterial* gMaterial = p->createMaterial(0.5f, 0.5f, 0.6f);
		PxShape* shape = p->createShape(PxTriangleMeshGeometry(mesh), *gMaterial, true);
		PxTransform t({ 0,0,0 });

		a = p->createRigidStatic(t);
		bool b = a->attachShape(*shape);

		a->userData = this;
		this->s->addActor(*a);
	}

	void WOphysx::onCreate(const std::string& path, const Vector& scale, Aftr::MESH_SHADING_TYPE mst, PxPhysics* p, std::string physxInfo){
		WO::onCreate(path, scale, mst);
		physx::PxMaterial* gMaterial = p->createMaterial(0.5f, 0.5f, 0.6f);
		PxShape* shape = p->createShape(PxSphereGeometry(scale[0]*2), *gMaterial, true);
		
		if (physxInfo == "s"){
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
	void WOphysx::stopForce() {
		a->setLinearVelocity(PxVec3(0, 0, 0));
	}

	float WOphysx::getForce() {
		PxVec3 temp = a->getLinearVelocity();
		int volume = 0;
		for (size_t i = 0; i < 3; i++)
		{
			volume += temp[i];
		}
		return volume;
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
		if (physxInfo == "t") {
			wo->onCreate(modelFileName, scale, shadingType);
		}else {
			wo->onCreate(modelFileName, scale, shadingType, p, physxInfo);
		}
		
		return wo;
	}

	WOphysx::WOphysx(PxScene* scene) :IFace(this){ this->s = scene; }
	WOphysx::~WOphysx(){ }
}