/*
 * timesourceTimelib.h
 * A few defines that leverage the Arduino Time library for
 * time-keeping and epoch <-> elements conversions.
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

#ifndef CHRONOS_INTINCLUDES_PLATFORM_TIMESOURCETIMELIB_H_
#define CHRONOS_INTINCLUDES_PLATFORM_TIMESOURCETIMELIB_H_

#define DATETIME_GET_CURRENT_EPOCH()		::now()


#define DATETIME_SET_CURRENT_TIME(year, month, day, hours, minutes, seconds) \
	::setTime(hours, minutes, secs, day, month, year);


#define DATETIME_CONVERT_EPOCH_INTO_TIMELEMENTS(epoch, elements) \
	::breakTime(epoch, elements);

#define DATETIME_CONVERT_TIMELEMENTS_TO_EPOCH(elements) \
		::makeTime(elements);


/*
#define DATETIME_CONVERT_EPOCH_INTO_TIMELEMENTS(epoch, elements) \
	Chronos::Calculator::breakTime(epoch, elements);

#define DATETIME_CONVERT_TIMELEMENTS_TO_EPOCH(elements) \
		Chronos::Calculator::makeTime(elements);
*/
#endif /* CHRONOS_INTINCLUDES_PLATFORM_TIMESOURCETIMELIB_H_ */
