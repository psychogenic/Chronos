/*
 * Calendar.h
 *
 * The Calendar is the primary mode of interaction with Chronos, when
 * you're dealing with scheduling/events.
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

#ifndef CHRONOS_INTINCLUDES_SCHEDULE_CALENDAR_H_
#define CHRONOS_INTINCLUDES_SCHEDULE_CALENDAR_H_

#include "../../chronosinc/schedule/ScheduledEvent.h"
#include "../timeTypes.h"
#include "../platform/platform.h"

/*
 * DefineCalendarType(name, maxSize)
 *
 * A bit of syntactic sugar to make instantiating calendars more pretty.
 *
 *   DefineCalendarType(BigCalendar, 20); // a calendar type that holds up to 20 events
 *   DefineCalendarType(LittleCalendar, 4); // a smaller cal, for only 4 events
 *
 * Then, you can use the defined types anywhere to create calendars
 *
 * BigCalendar teamCalendar;
 * LittleCalendar myDailyExerciseRoutines;
 * etc...
 *
 * Using this is optional, you could just do
 *
 * Chronos::CalendarStaticArray<20> teamCalendar;
 *
 *
 */
#define DefineCalendarType(name, num)		typedef	Chronos::CalendarStaticArray<num> name;


namespace Chronos {

class Calendar {
public:
	Calendar(uint8_t maxEvents);
	virtual ~Calendar() {}


	/*
	 * numEvents()
	 *
	 * @return: number of events setup in calendar.
	 */
	inline uint8_t numEvents() { return num_events;}

	/*
	 * numRecurring()
	 *
	 * @return: number of events setup that are repeating events (created using a Chronos::Mark::*)
	 *
	 * numRecurring() will always be <= numEvents().
	 */
	inline uint8_t numRecurring() { return num_recurring;}


	/*
	 * remove -- remove an event from the calendar
	 * @param eventId: the EventID to search for
	 * @return success: event was found and removed
	 */
	bool remove(EventID evId);

	/*
	 * add(event) -- add an event to the calendar.
	 * @param event: the Chronos::Event to add
	 * @return success: returns true if there was enough room in the calendar for this event.
	 */
	bool add(const Chronos::Event & event);
#ifdef PLATFORM_SUPPORTS_RVAL_MOVE
	// rvalue move
	bool add(Chronos::Event&& event);
#endif

	/*
	 * clear()
	 *
	 * Empty the calendar of all events add()ed.
	 *
	 */
	void clear();


	/*
	 * listOngoing(maxNumber, intoArray, dt)
	 *
	 *  Fill a list (up to maxNumber) with events that are actually happening at a specific DateTime.
	 *
	 * @param maxNumer: maximum number return array can hold
	 * @param intoArray: Event::Occurrence array to hold return values
	 * @param dt: DateTime of interest
	 *
	 * @return: number of entries loaded into intoArray
	 *
	 *
	 * @note: At return, Occurrences [0, returnValue] will be set in intoArray, and sorted by start DateTime
	 */
	uint8_t listOngoing(uint8_t maxNumber, Event::Occurrence intoArray[], const DateTime & dt) ;

	/*
	 * listNext(maxNumber, intoArray, dt)
	 *
	 *  Fill a list (up to maxNumber) with events that will happen starting after DateTime dt. Note events
	 * returned may be any amount of time into the future.
	 *
	 *
	 * @param maxNumer: maximum number return array can hold
	 * @param intoArray: Event::Occurrence array to hold return values
	 * @param dt: DateTime of interest
	 * @return: number of entries loaded into intoArray
	 *
	 * @note: At return, Occurrences [0, returnValue] will be set in intoArray, and sorted by start DateTime
	 */
	uint8_t listNext(uint8_t maxNumber, Event::Occurrence intoArray[], const DateTime & dt);

	/*
	 * listForDay -- list all events that will begin on the day specified in dt.
	 * @param maxNumer: maximum number return array can hold
	 * @param intoArray: Event::Occurrence array to hold return values
	 * @param dt: DateTime of interest
	 * @return: number of entries loaded into intoArray
	 *
	 *
	 * @note: At return, Occurrences [0, returnValue] will be set in intoArray, and sorted by start DateTime
	 *
	 */
	uint8_t listForDay(uint8_t maxNumber, Event::Occurrence intoArray[], const DateTime & dt);



	/*
	 * nextDateTimeOfInterest(dt, returnDT)
	 *
	 * Get the next DateTime "of interest" -- meaning the closest future DateTime at which any
	 * calendar event will end or begin.
	 *
	 *
	 * @param dt: base DateTime from which the relative current/next events are calculated
	 * @param returnDT: a DateTime that will be set by the method with our result, if found.
	 *
	 * @return: boolean true if the value in returnDT actually contains a valid "next datetime of interest"
	 * when the call returns.
	 *
	 */
	bool nextDateTimeOfInterest(const DateTime & fromDT, DateTime & returnDT);


protected:
	virtual Chronos::Event * eventSlot(uint8_t i) = 0;

private:
	uint8_t num_events;
	uint8_t max_events;
	uint8_t num_recurring;

};

/*
 * CalendarStaticArray
 *
 * A Calendar implementation that maintains a static array of Chronos::Events.
 */

template<uint8_t MAXNUM>
class CalendarStaticArray : public Calendar {

public:
	CalendarStaticArray() : Calendar(MAXNUM)
	{

	}

protected:
	virtual Chronos::Event * eventSlot(uint8_t i) {

		if (i >= MAXNUM)
			return NULL;

		return &(event_list[i]);
	}
private:
	Chronos::Event event_list[MAXNUM];


};


} /* namespace Chronos */

#endif /* CHRONOS_INTINCLUDES_SCHEDULE_CALENDAR_H_ */
