/*
 * Monthly.cpp
 *
 *  http://flyingcarsandstuff.com/projects/chronos
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


#include "chronosinc/marks/Monthly.h"
namespace Chronos {
namespace Mark {


Monthly::Monthly(Day d) :
		Event(),
		strict_time(false),
		day(d),
		hour(0),
		minute(0),
		sec(0)
{
	if (day > 31)
	{
		day = 31;
	}
}

Monthly::Monthly(Day d, Hours h, Minutes m, Seconds s) :
				Event(),
				strict_time(true),
				day(d),
				hour(h),
				minute(m),
				sec(s)
{
	if (day > 31)
	{
		day = 31;
	}

}

DateTime Monthly::next(const DateTime& dt) const {
	return applyTo(dt, Next);

}

DateTime Monthly::previous(const DateTime& dt) const {
	return applyTo(dt, Previous);
}

Event* Monthly::clone() const {
	Monthly * theClone = new Monthly(day, hour, minute, sec);
	theClone->strict_time = strict_time;
	return theClone;
}

DateTime Monthly::applyTo(const DateTime& dt,
		Direction dir) const
{
	Chronos::TimeElements els(dt.asElements());


	if (strict_time) {
		els.Hour = hour;
		els.Minute = minute;
		els.Second = sec;
	}

	if (day == els.Day)
	{
		// we're on this month's target... need to switch months

		DateTime mod(els);
		if (dir == Next)
		{
			if (mod <= dt)
			{
				if (els.Month < 12)
				{
					els.Month++;
				} else {
					els.Month = 1;
					els.Year++;
				}
			}
		} else {
			// backward
			if (mod >= dt)
			{
				if (els.Month > 1)
				{
					els.Month --;
				} else {
					els.Month = 12;
					els.Year--;
				}
			}
		}
	} else if (day > els.Day)
	{
		// target is in the future of current month
		if (dir == Next)
		{
			// and that's what we're looking for... easypeasy
			els.Day = day;
		} else {
			// but we're heading backwards
			if (els.Month <= 1)
			{
				// agh, need to change years
				els.Year -= 1;
				els.Month = 12;
			} else {
				// ok for moving back in same year
				els.Month -= 1;
			}
		}
	} else {
		// target is in the past of current month
		if (dir == Previous)
		{

			// and we're looking for the past... easypeasy
			els.Day = day;
		} else {
			// but we're heading forwardh

			if (els.Month >= 12)
			{
				// agh, need to move to next year
				els.Year += 1;
				els.Month = 1;
			} else {
				// ok for moving forward in same year
				els.Month += 1;
			}
		}
	}

	// set our target day
	els.Day = day;


	DateTime mod(els);
	if (mod.day() != day)
	{
		// well that didn't workout...
		// say we're feb 3rd and we're looking for the 31st
		// then we just landed in march... well that sucks, so we KISS it and
		// move to the next time we hit the right date
		Chronos::Span::Days aDay(1);
		while (mod.day() != day)
		{
			if (dir == Next)
			{
				mod += aDay;
			} else {
				mod -= aDay;
			}
		}

	}

	return mod;
}


} /* namespace Mark */
} /* namespace Chronos */
