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
		virtual ~NetMsgphysx();
		virtual bool toStream(NetMessengerStreamBuffer& os) const;
		virtual bool fromStream(NetMessengerStreamBuffer& is);
		virtual void onMessageArrived();
		virtual std::string toString() const;


		float pos[3]; // ball position
		float rot[3]; // world rotation
		//float grav[3];
		int pressed[4];
		int startGame;
		std::string winner;
		const int size = 3;
	protected:

	};
}

#endif // AFTR_CONFIG_USE_BOOST
