/*
 * test.cpp
 * Performance testing, used for library dev/testing.
 *
 *  http://flyingcarsandstuff.com/projects/chronos
 *  Created on: Jan 2, 2016
 *      Author: Pat Deegan
 *      Part of the Chronos library project
 *      Copyright (C) 2016 Pat Deegan, http://psychogenic.com
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

#ifndef DATECHRONOS_INTINCLUDES_TEST_CPP_
#define DATECHRONOS_INTINCLUDES_TEST_CPP_


//define DATETIME_TEST_ENABLE

#ifdef DATETIME_TEST_ENABLE
#include "../chronosinc/timeExtInc.h"
uint32_t runTest(uint16_t numTimes);
#endif


#endif /* DATECHRONOS_INTINCLUDES_TEST_CPP_ */
