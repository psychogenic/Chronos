/*
 * Weekly.cpp
 *
 *  http://flyingcarsandstuff.com/projects/chronos
 *  Created on: Dec 18, 2015
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

#include "chronosinc/platform/platform.h"
#include "chronosinc/marks/Weekly.h"
namespace Chronos {
namespace Mark {

Weekly::Weekly(WeekDay d, Hours h, Minutes m, Seconds s) :
		Chronos::Mark::Event(), strict_time(true), wday(d), hour(h), minute(m), sec(s) {



}
Weekly::Weekly(WeekDay d) :
		Chronos::Mark::Event(), strict_time(false), wday(d), hour(0), minute(0), sec(0) {

}

Weekly::Weekly(Chronos::Weekday::Day d) :
		Chronos::Mark::Event(), strict_time(false), wday((WeekDay)d), hour(0), minute(0), sec(0)
{

}

Weekly::Weekly(Chronos::Weekday::Day d, Hours h, Minutes m, Seconds s) :
		Chronos::Mark::Event(), strict_time(true), wday((WeekDay)d), hour(h), minute(m), sec(s) {


}
Weekly::~Weekly()
{

}
Event * Weekly::clone()  const
{
	Weekly * theClone = new Weekly(wday, hour, minute, sec);
	theClone->strict_time = strict_time;
	return theClone;
}



DateTime Weekly::next(const DateTime & dt) const {

	DateTime theNext(applyTo(dt, Next));

	// the next now has correct time and day, but may
	// not actually be in the future yet
	// continue adding 1 week until it is actually in the future:
	while (theNext <= dt) {
		theNext += Span::Weeks(1);
	}

	return theNext;

}

DateTime Weekly::previous(const DateTime & dt) const {

	DateTime thePrev(applyTo(dt, Previous));

	// the next now has correct time and day, but may
	// not actually be in the future yet
	// continue adding 1 week until it is actually in the future:
	while (thePrev >= dt) {
		thePrev -= Span::Weeks(1);
	}

	return thePrev;

}
DateTime Weekly::applyTo(const DateTime & dt, Direction dir) const {
	Chronos::TimeElements els(dt.asElements());

	/*
	if (strict_time || dir == Next)
	{
		els.Hour = hour;
		els.Minute = min;
		els.Second = sec;
	} else {

		els.Hour = 23;
		els.Minute = 59;
		els.Second = 59;
	}
	*/

	if (strict_time)
	{
		els.Hour = hour;
		els.Minute = minute;
		els.Second = sec;
	}

	DateTime mod(els);

	// the DT now has correct time, but is probably
	// not on the correct weekday...

	// Here, *monday* is day 1, sunday day 7

	WeekDay curWday = mod.weekday();
	// current, e.g.
	// monday to...
	//			friday: 1 to 5  go back 4 (5- 1)
	//			sunday: 1 to 7  go back 1 (1+7 - 7)
	//
	// friday to...
	//			tuesday: 5 to 2 go back 3 (5-2)
	// 			saturday: 5 to 6 go back 6 (5+7 - 6)
	uint8_t numDaysToGoBack =
			(curWday >= wday) ? curWday - wday : (curWday + 7) - wday;

	if (!numDaysToGoBack) {
		// this is today, need to make sure we return
		// something that's in the past, by convention
		if (strict_time) {
			// user-specified hh:mm:ss, so we care
			// about this
			if (mod > dt)
			{
				// mod is currently set to a
				// later date than the reference, owing to the hour/min/secs
				// need to go back a week
				numDaysToGoBack = 7;
			}

		} else {
			numDaysToGoBack = 7; // force into past
		}
	}

	// go back to the correct day of the week
	mod -= Span::Days(numDaysToGoBack);

	return mod;

}

} /* namespace Mark */
} /* namespace Chronos */
