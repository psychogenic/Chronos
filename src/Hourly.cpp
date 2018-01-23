#include "chronosinc/marks/Hourly.h"

/*
 * Hourly.cpp
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
#include "chronosinc/marks/Hourly.h"
namespace Chronos {
namespace Mark {

Hourly::Hourly(Minutes m, Seconds s) :
	Event(),
	minute(m), sec(s)
{

}

Event * Hourly::clone()  const
{
	return new Hourly(minute, sec);

}
DateTime Hourly::applyTo(const DateTime & dt) const
{


	Chronos::TimeElements els(dt.asElements());

	els.Minute = minute;
	els.Second = sec;

	return DateTime(els);


}
DateTime Hourly::next(const DateTime & dt) const {

	DateTime theNext(applyTo(dt));

	if (theNext > dt)
		return theNext;

	theNext += Span::Hours(1);

	return theNext;

}

DateTime Hourly::previous(const DateTime & dt)  const {


	DateTime thePrev(applyTo(dt));

	if (thePrev < dt)
		return thePrev;

	thePrev -= Span::Hours(1);
	return thePrev;
}

} /* namespace Event */
} /* namespace Chronos */
