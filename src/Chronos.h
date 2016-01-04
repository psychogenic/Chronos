/*
 * Chronos.h
 *
 * http://flyingcarsandstuff.com/projects/chronos/
 *
 * Chronos is a library that allows you to manipulate date-times
 * easily in order to perform scheduling, setup calendars and
 * determine points in time that are somehow of interest.
 *
 * The major elements are:
 *
 * 	DateTimes/Spans -- an OO date-time object that can be manipulated in many ways.
 * 	   See the DateTime example, or the DateTime.h header.
 *
 * 	Marks -- sets of calendar point events (e.g. "every Sunday at 15h00").
 *     See the PointEvents and Calendar examples for usage.
 *
 *  Events/Calendar -- actual events with fixed or repeating datetimes of specific
 *  duration, and the scheduling functions needed to find/manipulate events of interest.
 *     See the Calendar example for usage, or the Calendar.h header.
 *
 *
 * The library is object oriented and attempts to allow for intuitive
 * interaction with dates and times, while being designed with
 * embedded systems in mind.  As such, the design's primary focus
 * is on keeping a small footprint especially in terms of memory
 * usage.
 *
 * Chronos is not meant to handle time *keeping*--an external library
 * such as the Arduino Time library does that job--but is easily
 * adapted to leverage external time-keepers (basically anything
 * that can answer the question "what is the time now?")
 * and provide all its functionality in a manner that is
 * relatively independent of the method used to track time.
 *
 *
 * CAVEATS:
 * 		- this library is based on Unix epoch time calculations
 * 		- the granularity is _seconds_ -- nothing smaller is handled
 * 		- the epoch starts in 1970 -- no dates before that handled
 * 		- the epoch (in seconds) is currently held in a 32-bit variable, so
 * 		  upper-most date is somewhere in 2106 (see DateTime::endOfTime()).
 * 		- the main concern is embedded system *memory*, so when a trade-off
 * 		  needs to be made between CPU cycles and RAM, RAM usually wins
 * 		  at the cost of more computing.
 *
 *
 *
 * All the details of the API are described on the project site at
 * http://flyingcarsandstuff.com/projects/chronos/ and there are
 * usage examples (coded for Arduino SDK) in the examples/ directory.
 *
 *
 *  Created on: Dec 21, 2015
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

#ifndef CHRONOS_MAIN_INCLUDE_H_
#define CHRONOS_MAIN_INCLUDE_H_


#include "chronosinc/DateTime.h"
#include "chronosinc/marks/marks.h"
#include "chronosinc/schedule/ScheduledEvent.h"
#include "chronosinc/schedule/Calendar.h"
#include "chronosinc/test.h"


#endif /* CHRONOS_MAIN_INCLUDE_H_ */
