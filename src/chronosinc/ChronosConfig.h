/*
 * ChronosConfig.h
 *
 * Compile-time configuration directives.
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

#ifndef CHRONOS_INTINCLUDES_CHRONOSCONFIG_H_
#define CHRONOS_INTINCLUDES_CHRONOSCONFIG_H_

// CHRONOS_PLATFORM_ARDUINO -- built for Arduino SDK
#define CHRONOS_PLATFORM_ARDUINO

// CHRONOS_CLOCKSOURCE_TIMELIB -- time-keeper is Arduino Time library
#define CHRONOS_CLOCKSOURCE_TIMELIB


// CHRONOS_DEBUG_ENABLE -- only enable this on system's with lotsa ram.
//define CHRONOS_DEBUG_ENABLE

// ENABLE_UTILITY_INCLUDE -- enable include of <utility>, which 
// seems to be missing for 'duino...
//define ENABLE_UTILITY_INCLUDE


#define CHRONOS_VERSION			1
#define CHRONOS_SUBVERSION		2
#define CHRONOS_PATCHLEVEL		0

#endif /* CHRONOS_INTINCLUDES_CHRONOSCONFIG_H_ */
