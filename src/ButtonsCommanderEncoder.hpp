//-------------------------------------------------------------------
#ifndef __buttonsCommanderEncoder_H__
#define __buttonsCommanderEncoder_H__
//-------------------------------------------------------------------

#include "Commanders.h"

class ButtonsCommanderPush;

//-------------------------------------------------------------------

class ButtonsCommanderEncoder : public ButtonsCommanderButton
{
 private:
	int currentValue;
	int startingCurrentValue;
	GPIO_pin_t pin1;
	GPIO_pin_t pin2;
	int mini, maxi;
	int lastEncoded;

 public:
	ButtonsCommanderEncoder();

	inline bool IsAnalog() const { return false; }
	inline int GetPosition() const { return this->currentValue; }
	inline void ResetStartingPosition() { this->currentValue = this->startingCurrentValue; }

	void begin(int inPin1, int inPin2, unsigned long inId, int inStartingCurrentValue = 0, int inMinimum = 0, int inMaximum = 0);
	unsigned long loop();
};

#define ENCODER_BUTTON(name, pin1, pin2, ID) \
	ButtonsCommanderEncoder *name = new ButtonsCommanderEncoder(ID); \
	name->begin(pin1, pin2); \
	macro_buttons.Add(name);

#define ENCODER_BUTTON_VALUE(name, pin1, pin2, ID, start, min, max) \
	ButtonsCommanderEncoder *name = new ButtonsCommanderEncoder(ID, start, min, max); \
	name->begin(pin1, pin2); \
	macro_buttons.Add(name);

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
