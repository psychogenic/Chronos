/*
 * platform.h
 *
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

#ifndef CHRONOS_INTINCLUDES_PLATFORM_PLATFORM_H_
#define CHRONOS_INTINCLUDES_PLATFORM_PLATFORM_H_

#include "../ChronosConfig.h"


#ifdef CHRONOS_PLATFORM_ARDUINO
#include "../../chronosinc/platform/platformArduino.h"
#else
#error "You MUST specify a supported CHRONOS_PLATFORM_* in ChronosConfig.h"
#endif

#ifndef CHRONOS_DEBUG_OUT
#define CHRONOS_DEBUG_OUT(...)
#define CHRONOS_DEBUG_OUTLN(...)
#define CHRONOS_DEBUG_OUTINT(v)
#define CHRONOS_DEBUG_OUTHEX(v)
#endif

#ifndef CHRONOS_DELAY_MS
#define CHRONOS_DELAY_MS(t)
#endif

#ifdef ENABLE_UTILITY_INCLUDE
#ifdef __cplusplus
#if (__cplusplus > 201100L)
#include <utility>
#define PLATFORM_SUPPORTS_RVAL_MOVE
#endif
#endif
#endif

#endif /* CHRONOS_INTINCLUDES_PLATFORM_PLATFORM_H_ */
