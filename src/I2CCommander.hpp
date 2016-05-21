//-------------------------------------------------------------------
#ifndef __i2cCommander_H__
#define __i2cCommander_H__
//-------------------------------------------------------------------

#include "Commanders.h"

#ifndef NO_I2CCOMMANDER
#ifdef VISUALSTUDIO
	#include "../VStudio/Wire.hpp"
#else
	#include <Wire.h>
#endif

#define I2CCommander I2CCommanderClass::GetCurrent()

/*-------------------------------------------------------------------

Only one receiver can be defined inside a UAD session. This is why 
the I2CSlaveId and LastEventId are defined in static, and initialized
by the static method begin().

-------------------------------------------------------------------*/

class I2CCommanderClass : Commander
{
	public:
		static uint8_t I2CSlaveId;
		static unsigned long LastEventId;

		inline I2CCommanderClass() : Commander() {}
		unsigned long loop();

		static void begin(uint8_t inI2CSlaveID);

	public:
		static I2CCommanderClass *pI2cCommander;
		static inline I2CCommanderClass &GetCurrent()
		{
			if (pI2cCommander == NULL)
				pI2cCommander = new I2CCommanderClass();

			return *(I2CCommanderClass::pI2cCommander);
		}
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
