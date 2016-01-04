/*
 * Event.cpp
 *
 *  http://flyingcarsandstuff.com/projects/chronos
 *  Created on: Dec 19, 2015
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


#include "chronosinc/schedule/ScheduledEvent.h"
#include "chronosinc/marks/marks.h"
#include "chronosinc/platform/platform.h"

namespace Chronos {


// int Event::schedev_counter = 0;
Event::Event() : event_id(EVENTID_NOTSET),
is_recurring(false),
t_event(NULL),
duration(0)
{

}

Event::Event(EventID evId, const Chronos::Mark::Event & timeEvent, const Chronos::Span::Delta & evtDuration):
		event_id(evId),
		is_recurring(true),
		t_event(NULL),
		duration(evtDuration)
{

	t_event = timeEvent.clone();
}



Event::Event(EventID evId, const DateTime& start, const DateTime& end) :
		event_id(evId),
		is_recurring(false),
		t_event(NULL),
		duration(0),
		dt_start(start),
		dt_end(end)
{

}

Event::Event(EventID evId, const DateTime& start,
		const Chronos::Span::Delta& evtDuration) :
		event_id(evId),
		is_recurring(false),
		t_event(NULL),
		duration(evtDuration),
		dt_start(start),
		dt_end(start + evtDuration)
{

}

#ifdef PLATFORM_SUPPORTS_RVAL_MOVE
Event::Event(EventID evId, const Chronos::Mark::Event & timeEvent, Chronos::Span::Delta && evtDuration) :
				event_id(evId),
				is_recurring(true),
				duration(std::move(evtDuration))
{
		t_event = timeEvent.clone();
}
Event::Event(Chronos::EventID evId, DateTime && start, DateTime && end) :
	event_id(evId),
	is_recurring(false),
	t_event(NULL),
	duration(0),
	dt_start(std::move(start)),
	dt_end(std::move(end))
{
}
Event::Event(EventID evId, DateTime && start, Chronos::Span::Delta && evtDuration) :
	event_id(evId),
	is_recurring(false),
	t_event(NULL),
	duration(std::move(evtDuration)),
	dt_start(std::move(start)),
	dt_end(std::move(start + duration))
{


}


Event::Event(Event&& other) :
		event_id(other.event_id),
		is_recurring(other.is_recurring),
		t_event(other.t_event),
		duration(std::move(other.duration)),
		dt_start(std::move(other.dt_start)),
		dt_end(std::move(other.dt_end))
{
	// we've taken ownership of the rvalue's event pointer
	other.t_event = NULL;  // prevent it from being released in rvalue's d'tor


}
Event & Event::operator=(Event&& other)
{
	event_id = other.event_id;
	is_recurring = other.is_recurring;
	duration = std::move(other.duration);
	dt_start = std::move(other.dt_start);
	dt_end = std::move(other.dt_end);

	if (t_event)
	{
		delete t_event;
	}
	if (other.t_event)
	{
		// take ownership of the rvalue's event pointer
		t_event = other.t_event;
		other.t_event = NULL; // prevent it from being released in d'tor

	} else {
		t_event = NULL;
	}

	return *this;
}

#endif


Event::Event(const Event & other) :
		event_id(other.event_id),
		is_recurring(other.is_recurring),
		t_event(NULL),
		duration(other.duration),
		dt_start(other.dt_start),
		dt_end(other.dt_end)
{
	if (NULL != other.t_event)
	{
		t_event = other.t_event->clone();
	}
}


Event & Event::operator=(const Event & other)
{
	event_id = other.event_id;
	is_recurring = other.is_recurring;
	duration = other.duration;
	dt_start = other.dt_start;
	dt_end = other.dt_end;

	/* ???
	if (NULL != t_event)
	{

	}
	*/

	if (t_event)
	{
		delete t_event;
	}

	if (other.t_event)
	{
		t_event = other.t_event->clone();

	} else {
		t_event = NULL;
	}

	return *this;
}
Event::~Event()
{
	// schedev_counter--;
	reset();

}

void Event::reset()
{

	event_id = EVENTID_NOTSET;
	if (NULL != t_event)
	{
		delete t_event;
		t_event = NULL;
	}


}

bool Event::hasNext(const DateTime & fromDateTime) {

	if (! is_recurring)
	{
		// a one time event

		if (fromDateTime >= dt_start)
		{
			// too late.
			return false;
		}

		return true;

	}

	// it is a recurring event... it must therefore have a next
	return true;

}
Event::Occurrence Event::nextOccurrence(const DateTime & fromDateTime) {

	if (! is_recurring)
	{
			// a one time event

		if (fromDateTime >= dt_start)
		{
			// too late.
			return Event::Occurrence();
		}

		// it starts in the future... yay
		return Event::Occurrence(event_id, dt_start, dt_end, false);

	}

	if (! t_event)
		return Event::Occurrence();

	// it is a recurring event...
	DateTime nextStart(t_event->next(fromDateTime));
	DateTime nextEnd(nextStart + duration);


	bool ongoing = false;
	if (nextStart <= fromDateTime && nextEnd > fromDateTime)
		ongoing = true;

	return Event::Occurrence(event_id, nextStart, nextEnd, ongoing);


}
Event::Occurrence Event::closestOccurrence(const DateTime & fromDateTime)
{

	if (! is_recurring)
	{
		// a one time event, the closest is the only occurrence
		return Event::Occurrence(event_id, dt_start, dt_end, (dt_start <= fromDateTime && dt_end > fromDateTime));

	}

	if (! t_event)
		return Event::Occurrence();

	DateTime prevStart(t_event->previous(fromDateTime));
	DateTime prevEnd(prevStart + duration);

	// maybe we're *in* prev occurrence
	if (prevEnd > fromDateTime)
	{
		// yep
		return  Event::Occurrence(event_id, prevStart, prevEnd, true);
	}

	DateTime justAfterPrevEnd(prevEnd + Chronos::Span::Seconds(1));


	// nope... see the next one
	DateTime nextStart(t_event->next(justAfterPrevEnd));
	DateTime nextEnd(nextStart + duration);

	return Event::Occurrence(event_id, nextStart, nextEnd, (nextStart <= fromDateTime));


}


} /* namespace Chronos */

