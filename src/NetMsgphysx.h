#pragma once
#include "WO.h"
#include "WOphysx.h"
#include "WorldContainer.h"
#include "NetMsg.h"
#include "GLViewFinalProject.h"
#include "ManagerSerializableNetMsgMap.h"
#include "NetMessengerStreamBuffer.h"
#include <string>
#ifdef AFTR_CONFIG_USE_BOOST

namespace Aftr
{

	class NetMsgphysx : public NetMsg
	{
	public:
		NetMsgMacroDeclaration(NetMsgphysx);

		NetMsgphysx();
		NetMsgphysx(bool* w, bool* a, bool* s, bool* d);
		virtual ~NetMsgphysx();
		virtual bool toStream(NetMessengerStreamBuffer& os) const;
		virtual bool fromStream(NetMessengerStreamBuffer& is);
		virtual void onMessageArrived();
		virtual std::string toString() const;

		int wPressed = 0;
		int aPressed = 0;
		int sPressed = 0;
		int dPressed = 0;

		float pos[3]; // ball position
		float rot[3]; // world rotation
		int startGame;
		std::string winner;
		const int size = 3;
	protected:

	};
}

#endif // AFTR_CONFIG_USE_BOOST
