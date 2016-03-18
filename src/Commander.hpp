//-------------------------------------------------------------------
#ifndef __commander_H__
#define __commander_H__
//-------------------------------------------------------------------

#define  GPIO2_PREFER_SPEED    1
#ifdef VISUALSTUDIO
#include "../VStudio/arduino2.hpp"
#else
#include "arduino2.hpp"
#endif

enum COMMANDERS_EVENT_TYPE
{
	COMMANDERS_EVENT_NONE = 0,			// Should never appear
	COMMANDERS_EVENT_SELECTED = 1,		// If a push button or similar is pressed
	COMMANDERS_EVENT_ABSOLUTEMOVE = 2,	// If a potentiometer or similar is moved
	COMMANDERS_EVENT_RELATIVEMOVE = 3	// If an encoder or similar is moved
};

struct CommanderEvent
{
	unsigned long ID;
	COMMANDERS_EVENT_TYPE Event;
	int Data;

	inline CommanderEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData) {
		this->ID = inId;
		this->Event = inEvent;
		this->Data = inData;
	}
};

extern const CommanderEvent EmptyEvent;

typedef void(*CommandersEventHandlerFunction)(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData);

#ifndef UNDEFINED_ID
#define UNDEFINED_ID	((unsigned long)-1)
#endif

class Commander
{
	public:
		unsigned long Id;
		Commander *pNextCommander;

		// Start of the linked list of all commanders. Each commander have the address of its follower or NULL !
		static Commander *pFirstCommander;
		static CommandersEventHandlerFunction EventHandler;

	public:
		inline Commander() { AddCommander(this); pNextCommander = 0; }
		
		virtual void Setup() {}
		inline virtual CommanderEvent Loop() { return EmptyEvent; }
		inline virtual void PriorityLoop() { }
		void CommanderPriorityLoop();
		static void RaiseEvent(unsigned long inId, COMMANDERS_EVENT_TYPE inEvent, int inData);

		static CommanderEvent Loops();

	private:
		static void AddCommander(Commander *inCommander);

#ifdef DEBUG_MODE
	public:
		void CheckIndex(byte inIndex, const __FlashStringHelper *infunc);
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
