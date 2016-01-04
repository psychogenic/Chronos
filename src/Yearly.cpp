/*
 * Yearly.cpp
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


#include "chronosinc/marks/Yearly.h"
namespace Chronos {
namespace Mark {

Yearly::Yearly(Month mo, Day d, Hours h, Minutes mn, Seconds s) :
		Event(), strict_time(true), month(mo), day(d), hour(h), minute(mn), sec(s) {
}
Yearly::Yearly(Month mo, Day d):
				Event(), strict_time(false), month(mo), day(d), hour(0), minute(0), sec(0)
{

}

Event * Yearly::clone()  const
{
	Yearly * theClone = new Yearly(month, day, hour, minute, sec);
	theClone->strict_time = strict_time;
	return theClone;

}
DateTime Yearly::next(const DateTime& dt) const {
	DateTime theNext(applyTo(dt, Next));

	if (theNext > dt)
		return theNext;

	theNext.setYear(theNext.year() + 1);

	return theNext;
}

DateTime Yearly::previous(const DateTime& dt) const {
	DateTime thePrev(applyTo(dt, Previous));

	if (thePrev < dt)
		return thePrev;

	thePrev.setYear(thePrev.year() - 1);

	return thePrev;
}

DateTime Yearly::applyTo(const DateTime& dt, Direction dir) const {
	Chronos::TimeElements els(dt.asElements());

	els.Month = month;
	els.Day = day;
	/*
	if (strict_time || dir == Next)
	{
		els.Hour = hour;
		els.Minute = min;
		els.Second = sec;
	} else {

		// neither strict, nor Next direction search --
		// i.e. non-strict previous search
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

	return mod;
}

} /* namespace Mark */
} /* namespace Chronos */
