#include "NetMsgphysx.h"

using namespace Aftr;

NetMsgMacroDefinition(NetMsgphysx);

NetMsgphysx::NetMsgphysx(){
	for (int i = 0; i < size; i++){
		this->pos[i] = 0;
		this->rot[i] = 0;
	}
}

NetMsgphysx::~NetMsgphysx(){}

bool NetMsgphysx::toStream(NetMessengerStreamBuffer& os) const {
	os << this->size;
	for (int i = 0; i < size; i++){
		os << this->pos[i];
		os << this->rot[i];
	}
	return true;
}

bool NetMsgphysx::fromStream(NetMessengerStreamBuffer& is) {
	is << this->size;
	for (int i = 0; i < size; i++){
		is >> this->pos[i];
		is >> this->rot[i];
	}
	return true;
}

void NetMsgphysx::onMessageArrived() {
	WorldContainer* worldLst = ManagerGLView::getGLView()->getWorldContainer();
	if (worldLst == NULL){
		std::cout << "worldlist == null" << std::endl;
	}

	std::cout << worldLst->size() << std::endl;
	WOphysx* cubeObj = (WOphysx*) worldLst->at(3);
	if (cubeObj != NULL){
		cubeObj->setPosition(Vector(pos[0], pos[1], pos[2]));
		cubeObj->addForce(100*PxVec3(rot[0], rot[1], rot[2]));
	}
}

std::string NetMsgphysx::toString() const {
	std::stringstream ss;
	ss << NetMsg::toString();
	ss << "   Payload:  " << "(" << this->pos[0] << "," << this->pos[1] << "," << this->pos[2] << ")";
	return ss.str();
}