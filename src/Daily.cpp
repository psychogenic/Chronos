#include "chronosinc/marks/Daily.h"

/*
 * Daily.cpp
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

#include "chronosinc/Delta.h"
#include "chronosinc/marks/Daily.h"
namespace Chronos {
namespace Mark {

Daily::Daily(Hours h, Minutes m, Seconds s) :
	Event(),
	hour(h), minute(m), sec(s)
{

}

Event * Daily::clone()  const
{
	return new Daily(hour, minute, sec);

}
DateTime Daily::applyTo(const DateTime & dt) const
{


	Chronos::TimeElements els(dt.asElements());

	els.Hour = hour;
	els.Minute = minute;
	els.Second = sec;

	return DateTime(els);


}
DateTime Daily::next(const DateTime & dt) const {

	DateTime theNext(applyTo(dt));

	if (theNext > dt)
		return theNext;

	theNext += Span::Days(1);

	return theNext;

}

DateTime Daily::previous(const DateTime & dt)  const {


	DateTime thePrev(applyTo(dt));

	if (thePrev < dt)
		return thePrev;

	thePrev -= Span::Days(1);
	return thePrev;
}

} /* namespace Event */
} /* namespace Chronos */
