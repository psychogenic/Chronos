/*
 * Yearly.h
 *  A time mark recurring on a particular day, each year.
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

#ifndef CHRONOS_INTINCLUDES_EVENTS_YEARLY_H_
#define CHRONOS_INTINCLUDES_EVENTS_YEARLY_H_


#include "../DateTime.h"
#include "../Event.h"

namespace Chronos {
namespace Mark {

class Yearly : public Event {
public:
	Yearly(Month month, Day day);
	Yearly(Month month, Day day,
			Hours hour, Minutes mins, Seconds secs=0);



	virtual DateTime next(const DateTime & dt) const;
	virtual DateTime previous(const DateTime & dt)  const;


	virtual Event * clone() const;

private:
	DateTime applyTo(const DateTime & dt, Direction dir) const;

	bool strict_time;
	Month month;
	Day day;
	Hours hour;
	Minutes minute;
	Seconds sec;
};

} /* namespace Mark */
} /* namespace Chronos */

#endif /* CHRONOS_INTINCLUDES_EVENTS_YEARLY_H_ */
