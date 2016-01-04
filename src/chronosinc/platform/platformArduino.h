/*
 * platformArduino.h
 *
 *  http://flyingcarsandstuff.com/projects/chronos
 *  Created on: Dec 20, 2015
 *      Author: Pat Deegan
 *      Part of the Chronos library project
 *      Copyright (C) 2015 Pat Deegan, http://psychogenic.com
 * 
 *  This file is part of the Chronos embedded datetime/calendar library.
 * 
 *     Chronos is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU Lesser Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     Chronos is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU Lesser Public License for more details.
 * 
 *    You should have received a copy of the GNU Lesser Public License
 *    along with Chronos.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CHRONOS_INTINCLUDES_PLATFORM_PLATFORMARDUINO_H_
#define CHRONOS_INTINCLUDES_PLATFORM_PLATFORMARDUINO_H_

#include "../ChronosConfig.h"


#define CHRONOS_DEBUG_SERIAL_DEVICE		Serial

#ifdef CHRONOS_DEBUG_ENABLE
#define CHRONOS_DEBUG_OUT(...)		CHRONOS_DEBUG_SERIAL_DEVICE.print(__VA_ARGS__)
#define CHRONOS_DEBUG_OUTLN(...)	CHRONOS_DEBUG_SERIAL_DEVICE.println(__VA_ARGS__)
#define CHRONOS_DEBUG_OUTINT(v)		CHRONOS_DEBUG_SERIAL_DEVICE.print((int)(v))
#define CHRONOS_DEBUG_OUTHEX(v)		CHRONOS_DEBUG_SERIAL_DEVICE.print((int)(v), HEX)

#endif


#define CHRONOS_DELAY_MS(t)		delay(t)


#endif /* CHRONOS_INTINCLUDES_PLATFORM_PLATFORMARDUINO_H_ */
