//-------------------------------------------------------------------
#ifndef __buttonsCommanderButton_H__
#define __buttonsCommanderButton_H__
//-------------------------------------------------------------------

#include "Commander.hpp"

//-------------------------------------------------------------------
// A button is a hardware device allowing user to act when this device 
// is used. Its id is sent, and the listener of the commander events
// can do the job.
//-------------------------------------------------------------------

class ButtonsCommanderButton
{
 protected:
	static COMMANDERS_EVENT_TYPE eventType;
	static int eventData;

 protected:
	unsigned long Id;

 private:
	ButtonsCommanderButton *pNextButton;

 public:
	ButtonsCommanderButton(unsigned long inId);
	inline unsigned long GetId() const { return this->Id; }
	inline virtual ButtonsCommanderButton* GetFromId(unsigned long inId) { return this; }

	inline virtual bool IsAnalog() const { return false; }
	inline virtual int GetPosition() const { return 0; }

	inline virtual unsigned long loop() { this->eventType = COMMANDERS_EVENT_NONE;  return UNDEFINED_ID; }
	inline virtual void EndLoop() {}

	inline void SetNextButton(ButtonsCommanderButton *inButton) { this->pNextButton = inButton; }
	inline ButtonsCommanderButton *GetNextButton() const { return this->pNextButton; }

	static inline COMMANDERS_EVENT_TYPE GetLastEventType() { return eventType; }
	static inline int GetLastEventData() { return eventData; }
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
