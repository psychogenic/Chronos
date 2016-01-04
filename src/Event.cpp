/*
 * Event.cpp
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

#include "chronosinc/Event.h"
#include "chronosinc/DateTime.h"
#include "chronosinc/platform/platform.h"
namespace Chronos {


namespace Mark {



Event::Event() {

}
Event::~Event()
{
}




void Event::listNext(uint8_t number, DateTime into[], const DateTime & dt) const
{
	DateTime curDt = dt;
	for (uint8_t i=0; i<number; i++)
	{
		into[i] = this->next(curDt);
		curDt = into[i] + 1;
	}
}


void Event::listPrevious(uint8_t number, DateTime into[], const DateTime & dt) const
{

	DateTime curDt = dt;
	for (uint8_t i=0; i<number; i++)
	{
		into[i] = this->previous(curDt);
		curDt = into[i] - 1;
	}
}

} /* namespace Event */
} /* namespace Chronos */
