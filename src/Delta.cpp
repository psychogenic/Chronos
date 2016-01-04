#include "chronosinc/Delta.h"

/*
 * Delta.cpp
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


namespace Chronos {

namespace Span {

Delta::Delta(Chronos::EpochTime secs) : total_seconds(secs), delta_elements(){


}


Chronos::Span::Delta Delta::operator+(Chronos::EpochTime secs) const {
	return Delta(total_seconds + secs);
}

Chronos::Span::Delta Delta::operator+(const Delta& delta) const {
	return Delta(total_seconds + delta.total_seconds);
}

Chronos::Span::Delta& Delta::operator+=(Chronos::EpochTime secs) {

	delta_elements.is_init = false;
	total_seconds += secs;
	return *this;
}

Chronos::Span::Delta& Delta::operator+=(const Delta& delta) {

	delta_elements.is_init = false;
	total_seconds += delta.total_seconds;
	return *this;
}

Chronos::Span::Delta& Delta::operator-=(Chronos::EpochTime secs) {

	delta_elements.is_init = false;
	if (secs > total_seconds)
	{
		total_seconds = secs - total_seconds ;
	} else {
		total_seconds -= secs;
	}

	return *this;

}

Chronos::Span::Delta& Delta::operator-=(const Delta& delta) {

	delta_elements.is_init = false;

	if (delta.total_seconds > total_seconds) {
		total_seconds = delta.total_seconds - total_seconds;
	} else {
		total_seconds -= delta.total_seconds;
	}
	return *this;
}

Chronos::Span::Delta Delta::operator-(Chronos::EpochTime secs) const {
	if (secs > total_seconds) {
		return Delta(secs - total_seconds);
	}

	return Delta(total_seconds - secs);
}

Chronos::Span::Delta Delta::operator-(const Delta& delta) const {

	if (delta.total_seconds > total_seconds) {
		return Delta(delta.total_seconds - total_seconds);
	}

	return Delta(total_seconds - delta.total_seconds);
}



/*
const Delta::Elements& Delta::elements() const  {
	return delta_elements;
}
*/

void Delta::initElements() const {
	if (delta_elements.is_init)
		return;
	{
		Elements calcEls;

		calcEls.is_init = true;

		uint32_t time = (uint32_t) total_seconds;
		calcEls.seconds = time % 60;
		time /= 60; // now it is minutes
		calcEls.minutes = time % 60;
		time /= 60; // now it is hours
		calcEls.hours = time % 24;
		time /= 24; // now it is days
		calcEls.days = time;

		delta_elements = calcEls;
	}

}


void Delta::printTo(Print & p) const
{
	initElements();

	if (delta_elements.days) {
		p.print(delta_elements.days);
		p.print(" days, ");
	}

	if (delta_elements.hours) {
		p.print(delta_elements.hours);
		p.print(" hours, ");
	}

	p.print(delta_elements.minutes);
	p.print(" minutes and ");
	p.print(delta_elements.seconds);
	p.print(" seconds");


}


Seconds::Seconds(Chronos::EpochTime secs) : Chronos::Span::Delta(secs) {

}

Minutes::Minutes(uint32_t num) : Chronos::Span::Delta(num * SECS_PER_MIN)
{

}

Hours::Hours(uint32_t num) : Chronos::Span::Delta(num*SECS_PER_HOUR)
{

}

Days::Days(uint32_t num) : Chronos::Span::Delta(num * SECS_PER_DAY)
{

}

Weeks::Weeks(uint32_t num) : Chronos::Span::Delta(num * SECS_PER_WEEK)
{

}
} /* namespace Delta */
} /* namespace Chronos */

