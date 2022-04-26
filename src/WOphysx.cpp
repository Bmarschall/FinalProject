#include <WOphysx.h>
#include "PxPhysicsAPI.h"
#include "Model.h"

using namespace physx;


namespace Aftr{
	void WOphysx::setPosition(Vector vec){
		PxTransform t = PxTransform(PxVec3(vec.at(0), vec.at(1), vec.at(2) ));
		if (as != NULL) { this->as->setGlobalPose(t); };
		if (ad != NULL) { this->ad->setGlobalPose(t); };
		updatePoseFromPhysicsEngine(this->as);
		
	}

	void WOphysx::rotateAboutRelX(float deltaRadianAngle) {
		WO::rotateAboutRelX(deltaRadianAngle);
		
		Mat4 m2 = this->getPose();
		std::cout << m2 << std::endl;
		PxVec4 col0 = PxVec4(m2[0], m2[1], m2[2], m2[3]);
		PxVec4 col1 = PxVec4(m2[4], m2[5], m2[6], m2[7]);
		PxVec4 col2 = PxVec4(m2[8], m2[9], m2[10], m2[11]);
		PxVec4 col3 = PxVec4(m2[12], m2[13], m2[14], m2[15]);
		PxMat44 m = PxMat44(col0,col1,col2,col3);
		std::cout << m.column0.x << "\t" << m.column1.y << "\t" << m.column2.z << "\t" << m.column3.w << std::endl;
		std::cout << m.column0.y << "\t" << m.column1.y << "\t" << m.column2.y << "\t" << m.column3.y << std::endl;
		std::cout << m.column0.z << "\t" << m.column1.z << "\t" << m.column2.z << "\t" << m.column3.z << std::endl;
		std::cout << m.column0.w << "\t" << m.column1.w << "\t" << m.column2.w << "\t" << m.column3.w << std::endl;
		//std::cout << m.getPosition().x << "," << m.getPosition().y << "," << m.getPosition().z << std::endl;
		PxTransform n = PxTransform(m);
		PxVec3 temp = n.p;
		std::cout << col3.x << col3.y << col3.z << std::endl;
		std::cout << m.getPosition().x << m.getPosition().y << m.getPosition().z << std::endl;
		std::cout << temp.x << "\t" << temp.y << "\t" << temp.z << std::endl;
		if (as != NULL) { this->as->setGlobalPose(PxTransform(m)); };
		if (ad != NULL) { this->ad->setGlobalPose(PxTransform(m)); };
		updatePoseFromPhysicsEngine(this->as);

	}
	void WOphysx::rotateAboutRelY(float deltaRadianAngle) {

	}
	void WOphysx::rotateAboutRelZ(float deltaRadianAngle) {

	}

	void WOphysx::onCreate(const std::string& path, const Vector& scale, Aftr::MESH_SHADING_TYPE mst, PxPhysics* p){
		WO::onCreate(path, scale, mst);
		std::cout << "a" << std::endl;
		if (!PxInitExtensions(*p, NULL))
		{
			std::cout << "PxInitExtensions failed!" << std::endl;
			std::cin.get();
		}
		std::cout << "b" << std::endl;
		physx::PxCooking* c = PxCreateCooking(PX_PHYSICS_VERSION, *f, physx::PxCookingParams(physx::PxTolerancesScale()));
		if (!c)
		{
			std::cout << "Cooking error" << std::endl;
			std::cin.get();
		}

		this->upon_async_model_loaded([this,c]()
			{
				//make copy of vertex list and index list
				size_t vertexListSize = this->getModel()->getModelDataShared()->getCompositeVertexList().size();
				std::cout << "c2" << std::endl;
				size_t indexListSize = this->getModel()->getModelDataShared()->getCompositeIndexList().size();
				std::cout << "d" << std::endl;
				this->vertexListCopy = new float[vertexListSize * 3];//might be a better way to do this without making a copy
				this->indicesCopy = new unsigned int[indexListSize];//assuming the composite lists are stored in contiguous memory
				std::cout << "e" << std::endl;
				for (size_t i = 0; i < vertexListSize; i++) {
					this->vertexListCopy[i * 3 + 0] = this->getModel()->getModelDataShared()->getCompositeVertexList().at(i).x;
					this->vertexListCopy[i * 3 + 1] = this->getModel()->getModelDataShared()->getCompositeVertexList().at(i).y;
					this->vertexListCopy[i * 3 + 2] = this->getModel()->getModelDataShared()->getCompositeVertexList().at(i).z;
				}
				std::cout << "f" << std::endl;
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

				PxMaterial* gMaterial = this->p->createMaterial(0.5f, 0.5f, 0.6f);
				PxShape* shape = this->p->createShape(PxTriangleMeshGeometry(mesh), *gMaterial, true);
				PxTransform t({ 0,0,0 });

				as = this->p->createRigidStatic(t);
				bool b = as->attachShape(*shape);

				as->userData = this;
				this->s->addActor(*as);
			});
		

	}

	void WOphysx::onCreate(const std::string& path, const Vector& scale, Aftr::MESH_SHADING_TYPE mst, PxPhysics* p, std::string physxInfo) {
		WO::onCreate(path, scale, mst);
		physx::PxMaterial* gMaterial = p->createMaterial(0.5f, 0.5f, 0.6f);
		PxShape* shape = p->createShape(PxSphereGeometry(scale[0] * 2), *gMaterial, true);

		if (physxInfo == "s") {
			shape = p->createShape(PxSphereGeometry(scale[0] * 5), *gMaterial, true);
		}
		else
		{
			shape = p->createShape(PxBoxGeometry(scale[0] * 2, scale[1] * 2, scale[2] * 2), *gMaterial, true);
		}
		PxTransform t({ 0,0,0 });
		PxRigidDynamic* a = p->createRigidDynamic(t);
		a->attachShape(*shape);
		a->userData = this;
		this->s->addActor(*a);
		this->ad = a;
		physx::PxU32 numActors = 0;
		physx::PxActor** actors = this->s->getActiveActors(numActors);
		std::cout << numActors << " actors" << std::endl;
	}


	void WOphysx::addForce(PxVec3 force){
		ad->setLinearVelocity(PxVec3(0, 0, 0));
		ad->addForce(force, PxForceMode::eIMPULSE);

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
		if (this->ad != NULL){
			//std::cout << a->getConcreteTypeName() << std::endl;
			PxTransform transform = this->ad->getGlobalPose();

			auto m = PxMat44(transform);
			//std::cout << m.getPosition().x << "," << m.getPosition().y << "," << m.getPosition().z << std::endl;
			Mat4 m2;
			m2.setMeToIdentity();

			for (int i = 0; i < 16; i++) {
				m2[i] = m(i % 4, i / 4);
			}
			this->setPose(m2);
		}
		if (this->as != NULL) {
			//std::cout << a->getConcreteTypeName() << std::endl;
			PxTransform transform = this->as->getGlobalPose();
			auto m = PxMat44(transform);
			//std::cout << m.getPosition().x << "," << m.getPosition().y << "," << m.getPosition().z << std::endl;
			Mat4 m2;
			m2.setMeToIdentity();

			for (int i = 0; i < 16; i++) {
				m2[i] = m(i % 4, i / 4);
			}
			this->setPose(m2);
		}
	}


	WOphysx* WOphysx::New(const std::string& modelFileName, const Vector& scale , MESH_SHADING_TYPE shadingType , physx::PxPhysics* p, PxScene* s, std::string physxInfo, PxFoundation* f){
		WOphysx* wo = new WOphysx(s,f,p);
		if (physxInfo == "t") {
			wo->onCreate(modelFileName, scale, shadingType,p);
		}else {

			wo->onCreate(modelFileName, scale, shadingType, p, physxInfo);
		}

		return wo;
	}


	WOphysx::WOphysx(PxScene* scene, PxFoundation* found, physx::PxPhysics* phys) :IFace(this) { this->s = scene; this->f = found; this->p = phys; }
	WOphysx::~WOphysx(){ }

}