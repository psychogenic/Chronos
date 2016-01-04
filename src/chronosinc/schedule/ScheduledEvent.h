/*
 * Event.h
 * Scheduled events (one-dimensional objects in time, something with length, e.g. a 1 hour meeting @ 14h00),
 * created using fixed date-time bounds (one time events) or time marks
 * (repeating events, e.g. 30 minutes every Monday @ 14h00).
 *
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

#ifndef CHRONOS_INTINCLUDES_SCHEDULE_SCHEDULEDEVENT_H_

#define CHRONOS_INTINCLUDES_SCHEDULE_SCHEDULEDEVENT_H_
#include "../../chronosinc/timeExtInc.h"
#include "../../chronosinc/timeTypes.h"
#include "../../chronosinc/DateTime.h"
#include "../Delta.h"
#include "../../chronosinc/marks/marks.h"
#include "../platform/platform.h"

#define EVENTID_NOTSET		-1

namespace Chronos {


typedef int8_t EventID;


/*
 * Chronos::Event class
 *
 *
 * Events (one-dimensional objects in time, something with a duration
 * e.g. a 1 hour meeting @ 14h00), are created using fixed date-time bounds
 * (one time events) or time marks (repeating events, e.g. 30 minutes
 * every Monday @ 14h00).
 *
 * The are normally only interacted with while adding to a calendar, which
 * is then used to query for specific Event::Occurrences.
 *
 * An occurrence is an actualized event--for one shot events these are pretty
 * much the same thing as the Event itself. Repeating events are more abstract
 * entities (e.g. "every year on Feb 2nd") whereas a an occurrence will provide
 * actual DateTime objects for the beginning and end of a particular time the event
 * takes place.
 *
 */
class Event {
public:

	/*
	 * Chronos::Event::Occurrence
	 *
	 * Specifics about one time a given event takes place.  It's attributes are
	 *
	 * id: 			EventID for this event
	 * start:		DateTime of event start
	 * finish:		last DateTime that can be considered as occupied by the event
	 * isOngoing: 	boolean, true if the event is happening at the time specified in query
	 */

	class Occurrence {
	public:
		// act like a struct
		EventID id;
		DateTime start;
		DateTime finish;
		bool isOngoing;
		Occurrence(EventID evId, const DateTime & begin, const DateTime & end, bool ongoing=false) :
			id(evId), start(begin), finish(end), isOngoing(ongoing)
		{

		}
		Occurrence() : id(-1), isOngoing(false)
		{

		}
	#ifdef PLATFORM_SUPPORTS_RVAL_MOVE
		Occurrence(const Occurrence &) = default;
		Occurrence & operator=(const Occurrence &) = default;
		Occurrence(Occurrence&& other) :
			id(other.id),
			start(std::move(other.start)),
			finish(std::move(other.finish)),
			isOngoing(other.isOngoing)
		{

		}
		Occurrence & operator=(Occurrence&& other)
		{
			id = other.id;
			start = std::move(other.start);
			finish = std::move(other.finish);
			isOngoing = other.isOngoing;
			return *this;
		}

	#endif

		inline bool operator>(const Occurrence & other)
		{
			return (start > other.start);
		}
		inline bool operator<=(const Occurrence & other)
		{
			return (start <= other.start);
		}


	};


	/*
	 * Chronos::Event constructors.
	 *
	 * This is how the specifics of a given event are defined.  You can create events using:
	 *
	 * 	- bounding DateTimes (start and finish)
	 * 	- a start DateTime and a Chronos::Span::* to set the duration
	 * 	- a Chronos::Mark::* to set a repeating event that starts at the mark, and a Chronos::Span duration.
	 *
	 * All events need an EventID, which should be a positive integer but need not be unique (though
	 * it probably should be).
	 */

	/*
	 * Chronos::Event(id, start, end)
	 *
	 * @param id: EventID for this event
	 * @param start: DateTime at which event begins
	 * @param end: DateTime at which event ends
	 */
	Event(Chronos::EventID id, const DateTime & start, const DateTime & end);

	/*
	 * Chronos::Event(id, start, duration)
	 *
	 * @param id: EventID for this event
	 * @param start: DateTime at which event begins
	 * @param duration: a Chronos::Span to set how long it lasts, e.g. Chronos::Span::Minutes(30)
	 *
	 */
	Event(EventID id, const DateTime & start, const Chronos::Span::Delta & duration);

	/*
	 * Chronos::Event(id, mark, duration)
	 *
	 * @param id: EventID for this event
	 * @param mark: a time mark for the event start, e.g. Chronos::Mark::Weekly(Chronos::Weekday::Monday)
	 * @parma duration: a Chronos::Span to set how long it lasts, e.g. Chronos::Span::Hours(2)
	 */
	Event(EventID id, const Chronos::Mark::Event & timeEvent, const Chronos::Span::Delta & duration);

#ifdef PLATFORM_SUPPORTS_RVAL_MOVE
	Event(EventID id, const Chronos::Mark::Event & timeEvent, Chronos::Span::Delta && duration);
	Event(Chronos::EventID id, DateTime && start, DateTime && end);
	Event(EventID id, DateTime && start, Chronos::Span::Delta && duration);

	Event(Event&& rhs);
	Event & operator=(Event&& rhs);
#endif

	// copy and assignment operator
	Event(const Event & other);
	Event & operator=(const Event & other);


	// d'tor
	~Event();

	/*
	 * reset()
	 *
	 * Used internally -- basically the equivalent of destruction, while keeping the
	 * instance alive (used in the calendar to hold a spot for events but mark it as "unset").
	 */
	void reset();

	/*
	 * id()
	 *
	 * @return: EventID specified during construction.
	 */
	EventID id() { return event_id;}

	/*
	 * isRecurring()
	 *
	 * @return: boolean true if the event was created using a time mark (e.g. a monthly repeating event)
	 */
	bool isRecurring() const { return is_recurring; }

	/*
	 * hasNext(dt)
	 *
	 * @param dt: a DateTime
	 * @return: boolean true if there will be an occurrence of the event that starts in the future,
	 * 	        relative to dt (strictly so the start > dt, not >=).
	 */
	bool hasNext(const DateTime & fromDateTime);

	/*
	 * nextOccurrence(dt)
	 *
	 * @param dt: a DateTime
	 * @return: Event::Occurrence for the *next* time this event happens, relative to dt (occurrence.start > dt)
	 *
	 * @note: will always return something valid for repeating events. For one-time events, this only makes sense
	 * when dt > event.start.
	 */
	Event::Occurrence nextOccurrence(const DateTime & fromDateTime);

	/*
	 * closestOccurrence(dt)
	 *
	 * @param dt: a DateTime
	 * @return: Event::Occurrence closest in time to dt.
	 *
	 *
	 * @note: If the event is non-recurring (a one-time event), then that single occurrence will be
	 * the "closest" no matter how far away it is.
	 * If the event is happening @ dt, then that's the occurrence that will be returned.
	 * Otherwise, the next occurrence will be returned, as specified above in nextOccurrence().
	 */
	Event::Occurrence closestOccurrence(const DateTime & fromDateTime);


	/*
	 * Default constructor.  Needed internally but creates an Event with an invalid
	 * EventID, so probably not too useful outside the lib.
	 */
	Event();

private:
	// private, I say!
	EventID event_id;
	bool is_recurring;
	Chronos::Mark::Event * t_event;
	Chronos::Span::Delta duration;
	DateTime dt_start;
	DateTime dt_end;


};

} /* namespace Chronos */

#endif /* CHRONOS_INTINCLUDES_SCHEDULE_SCHEDULEDEVENT_H_ */
