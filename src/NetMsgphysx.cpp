#include "NetMsgphysx.h"

using namespace Aftr;

NetMsgMacroDefinition(NetMsgphysx);

NetMsgphysx::NetMsgphysx(){}

NetMsgphysx::NetMsgphysx(bool* w, bool* a, bool* s, bool* d){
	for (int i = 0; i < 3; i++){
		this->pos[i] = 0;
		this->rot[i] = 0;
	}
	this->startGame = 0;
	this->winner = "";

	//if (*w)
	//{
	//	this->wPressed = 1;
	//}
	//else {

	//	this->wPressed = 0;
	//}

	//if (*a)
	//{
	//	this->aPressed = 1;
	//}
	//else {

	//	this->aPressed = 0;
	//}

	//if (*s)
	//{
	//	this->sPressed = 1;
	//}
	//else {

	//	this->sPressed = 0;
	//}

	//if (*d)
	//{
	//	this->dPressed = 1;
	//}
	//else {

	//	this->dPressed = 0;
	//}

	this->wPressed = *w;
	this->aPressed = *a;
	this->sPressed = *s;
	this->dPressed = *d;
}

NetMsgphysx::~NetMsgphysx(){}

bool NetMsgphysx::toStream(NetMessengerStreamBuffer& os) const {
	os << this->size;
	os << this->wPressed;
	os << this->aPressed;
	os << this->sPressed;
	os << this->dPressed;
	for (int i = 0; i < 3; i++){
		os << this->pos[i];
		os << this->rot[i];
		os << this->startGame;
		os << this->winner;
	}
	return true;
}

bool NetMsgphysx::fromStream(NetMessengerStreamBuffer& is) {
	is << this->size;
	for (int i = 0; i < 3; i++){
		is >> this->pos[i];
		is >> this->rot[i];
		
	}
	is >> this->startGame;
	is >> this->winner;
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
		//skybox->rotateToIdentity();
		//grass->rotateToIdentity();
		//
		//skybox->rotateAboutRelX(rot[0]);
		//skybox->rotateAboutRelY(rot[1]);
		//skybox->rotateAboutRelZ(rot[2]);
		//
		//grass->rotateAboutRelX(rot[0]);
		//grass->rotateAboutRelY(rot[1]);
		//grass->rotateAboutRelZ(rot[2]);

		ball->setPosition(pos);
	}
}

std::string NetMsgphysx::toString() const {
	std::stringstream ss;
	ss << NetMsg::toString();
	ss << "   Pos:  " << "(" << this->pos[0] << "," << this->pos[1] << "," << this->pos[2] << ")";
	ss << "   Rot:  " << "(" << this->rot[0] << "," << this->rot[1] << "," << this->rot[2] << ")";
	return ss.str();
}