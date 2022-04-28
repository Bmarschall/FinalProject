#include "NetMsgphysx.h"

using namespace Aftr;

NetMsgMacroDefinition(NetMsgphysx);

NetMsgphysx::NetMsgphysx(){
	for (int i = 0; i < size; i++){
		this->pos[i] = 0;
		this->rot[i] = 0;
	}
	this->startGame = 0;
	this->winner = "";
}

NetMsgphysx::~NetMsgphysx(){}

bool NetMsgphysx::toStream(NetMessengerStreamBuffer& os) const {
	os << this->size;
	for (int i = 0; i < size; i++){
		os << this->pos[i];
		os << this->rot[i];
		os << this->startGame;
		os << this->winner;
	}
	return true;
}

bool NetMsgphysx::fromStream(NetMessengerStreamBuffer& is) {
	is << this->size;
	for (int i = 0; i < size; i++){
		is >> this->pos[i];
		is >> this->rot[i];
		is >> this->startGame;
		is >> this->winner;
	}
	return true;
}

void NetMsgphysx::onMessageArrived() {
	WorldContainer* worldLst = ManagerGLView::getGLView()->getWorldContainer();
	if (worldLst == NULL){
		std::cout << "worldlist == null" << std::endl;
	}

	WOphysx* skybox = (WOphysx*) worldLst->at(1);
	WOphysx* grass = (WOphysx*) worldLst->at(2);
	WOphysx* ball = (WOphysx*) worldLst->at(4);
	if (skybox != NULL && grass != NULL){
		skybox->rotateToIdentity();
		grass->rotateToIdentity();

		skybox->rotateAboutRelX(rot[0]);
		skybox->rotateAboutRelY(rot[1]);
		skybox->rotateAboutRelZ(rot[2]);

		grass->rotateAboutRelX(rot[0]);
		grass->rotateAboutRelY(rot[1]);
		grass->rotateAboutRelZ(rot[2]);

		ball->setPosition((pos[0], pos[1], pos[2]));
	}
}

std::string NetMsgphysx::toString() const {
	std::stringstream ss;
	ss << NetMsg::toString();
	ss << "   Pos:  " << "(" << this->pos[0] << "," << this->pos[1] << "," << this->pos[2] << ")";
	ss << "   Rot:  " << "(" << this->rot[0] << "," << this->rot[1] << "," << this->rot[2] << ")";
	return ss.str();
}