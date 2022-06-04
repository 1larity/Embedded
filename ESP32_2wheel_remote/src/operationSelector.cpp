// decode and call bot commands
#include "botfunctions.h"
#include <WebSerial.h>
//ToF sensor support
#include "Adafruit_VL53L0X.h"
//compass support
#include <QMC5883LCompass.h>
#include  "utils.h"

void selectcommand(int commandID,VL53L0X_RangingMeasurementData_t measure, int leftUsound, int rightUsound, QMC5883LCompass compass)
{
	WebSerial.print(commandString);
	WebSerial.print("@");
	WebSerial.println(parameterString);
	int paramInt = parameterString.toInt();
	switch (commandID)
	{
	case 0:
		stop();
		break;
	case 1:
		seek(measure);
		break;
	case 2:
		reverse(paramInt);
		break;
	case 3:
		spinLeft(paramInt);
		break;
	case 4:
		spinRight(paramInt);
		break;
	case 5:
		forwards(paramInt);
		break;
	case 6:
		hokycoky();
		break;
	case 7:
		linedance();
		break;
	case 8:
		range(measure,  leftUsound,  rightUsound,compass);
		break;
	case 9:
		rangeRepeat(measure,  leftUsound,  rightUsound,compass);
		break;
	case 10:
		help();
		break;
	case 11:
		faceNorth(compass);
		break;

	default:
		stop();
		break;
	}
}