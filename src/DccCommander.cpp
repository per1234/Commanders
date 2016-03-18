/*************************************************************
project: <Universal Accessory Decoder>
author: <Thierry PARIS>
description: <Dcc Commander>
*************************************************************/

#ifndef NO_DCCCOMMANDER
#include "DccCommander.hpp"

#ifdef VISUALSTUDIO
DCC_Decoder DCC_Decoder::DCCInstance;
#endif

unsigned long DccCommander::LastDccId = UNDEFINED_ID;
GPIO_pin_t DccCommander::dccStatusLedPin;
boolean DccCommander::UseRawDccAddresses;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DCC accessory packet handler 
//
void DccCommander::DccAccessoryDecoderPacket(int address, boolean activate, byte data)
{
	int realAddress = address;

	if (!DccCommander::UseRawDccAddresses)
	{
		realAddress -= 1;
		realAddress *= 4;
		realAddress += 1;
		realAddress += (data & 0x06) >> 1;
		data = data % 2;
	}

#ifdef DEBUG_VERBOSE_MODE
	Serial.print(F("Dcc packet found : Address : "));
	Serial.print(realAddress);
	Serial.print(F(" / "));
	Serial.print(data, DEC);
	Serial.print(F(" / "));
	Serial.println(activate, DEC);
#endif

	if (DccCommander::func_AccPacket)
		(DccCommander::func_AccPacket)(realAddress, activate, data);
	else
	{
		Commander::RaiseEvent(DCCINT(realAddress, data), COMMANDERS_EVENT_SELECTED, 0);

		DccCommander::LastDccId = DCCINT(realAddress, data);

		/*
		for (int i = 0; i < Accessories::AccessoriesFullList.AccessoriesAddCounter; i++)
		Accessories::AccessoriesFullList.pAccessoriesFullList[i]->DCCToggle(realAddress, data);
		for (int i = 0; i < AccessoryGroup::StaticData.AccessoryGroupAddCounter; i++)
		AccessoryGroup::StaticData.pAccessoryGroupFullList[i]->DCCToggle(realAddress, data);
		*/
	}
}

#ifdef DEBUG_MODE
#define CHECK(val, text)	CheckIndex(val, F(text))
#else
#define CHECK(val, text)
#endif

#ifdef DEBUG_MODE
void DccCommander::CheckIndex(unsigned char inIndex, const __FlashStringHelper *inFunc)
{
}
#endif

void DccCommander::Setup(int i, int j, int k, boolean inUseRawDccAddresses)
{
	DCC.SetupDecoder(i, j, k);
	DccCommander::UseRawDccAddresses = inUseRawDccAddresses;
}

bool status = false;
void StatusBlink_handler()
{
	digitalWrite2f(DccCommander::dccStatusLedPin, status ? HIGH : LOW);
	status = !status;
}

void DccCommander::SetStatusLedPin(int inLedPin)
{
	DccCommander::dccStatusLedPin = Arduino_to_GPIO_pin(inLedPin);
	pinMode2f(DccCommander::dccStatusLedPin, OUTPUT);

	DCC.SetInterruptMonitor(StatusBlink_handler);
}

void DccCommander::PriorityLoop()
{
	DccCommander::Loop();
}

#define MINTIME	2
#ifdef DEBUG_MODE
#ifdef DEBUG_VERBOSE_MODE
static int countLoop = 0;
#endif
#endif

static unsigned long start = 0;

#define ELAPSEDTIME	((unsigned long) -2)

CommanderEvent DccCommander::Loop()
{
	if (start == 0)
		start = millis();

	DCC.loop();
#ifdef DEBUG_MODE
#ifdef DEBUG_VERBOSE_MODE
	countLoop++;
#endif
#endif

	if (millis() - start > MINTIME)
	{
#ifdef DEBUG_MODE
#ifdef DEBUG_VERBOSE_MODE
		Serial.print("DCC commander : ");
		Serial.println(countLoop, DEC);
		countLoop = 0;
#endif
#endif
		start = 0;
		unsigned long last = LastDccId;
		LastDccId = UNDEFINED_ID;
		return CommanderEvent(last, COMMANDERS_EVENT_SELECTED, 0);
	}

	return EmptyEvent;
}

DccAccDecoderPacket DccCommander::func_AccPacket = NULL;

void DccCommander::SetAccessoryDecoderPacketHandler(DccAccDecoderPacket func)
{
	DccCommander::func_AccPacket = func;
}

#endif
