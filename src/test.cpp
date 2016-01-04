/*
 * test.cpp
 *
 *  http://flyingcarsandstuff.com/projects/chronos
 *  Created on: Jan 2, 2016
 *      Author: Pat Deegan
 *      Part of the Chronos library project
 *      Copyright (C) 2016 Pat Deegan, http://psychogenic.com
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
#include "chronosinc/test.h"

#ifdef DATETIME_TEST_ENABLE
#include "DateTime.h"


#define CALENDAR_MAX_NUM_EVENTS   8

/* *********** Global Calendar object *********** */
// create a global calendar, "MyCalendar", that can hold up to CALENDAR_MAX_NUM_EVENTS scheduled events
// (repeating or not). To do this, specify the (max) size for calendars:
DefineCalendarType(Calendar, CALENDAR_MAX_NUM_EVENTS);

// and *then* declare a global to hold everything:
Calendar MyCalendar;

// NOTE: these are only convenience #defines... if you need multiple calendars
// of different sizes, you can just use the templating directly, e.g.
//    Chronos::CalendarStaticArray<10> a10EventMaxCalendar;
// etc.

// OCCURRENCES_LIST_SIZE: array size for calendar query results
// At a size of 30, we're right at the limit of what an ATMega328
// (basic Arduino) can handle... any bigger and we smash the stack
// and get out-of-memory weirdness/resets/etc.
//
#define OCCURRENCES_LIST_SIZE   30


//
void setupCal() {

  // setting for monday, dec 21st 2015 @ 17:30:00
  // same as setTime(17, 30, 0, 21, 12, 2015);
  Chronos::DateTime::setTime(2015, 12, 21, 17, 30, 0);


  /* Add some Chronos::Event objects to the calendar to make it useful */

  // Chronos::Event can be initialized a few ways, namely:
  //    - Bounding datetimes:           (EVENT_ID, Chronos::DateTime start, Chronos::DateTime end)
  //    - Start time and duration:      (EVENT_ID, Chronos::DateTime start, Chronos::Span::X duration)
  //    - Repeating event and duration: (EVENT_ID, Chronos::Mark::X repeatingEvent, Chronos::Span::X duration)
  // Some examples:
  // two meetings, one shot events, one setup using a duration and the other using bounding datetimes
  // meeting one, about a half hour long:
  //MyCalendar.clear();

  MyCalendar.clear();
  MyCalendar.add(Chronos::Event(1, Chronos::DateTime(2015, 12, 21, 17, 00),
                             Chronos::Span::Minutes(33))
                );



  // don't forget the daily workout, a daily recurring event every morning at 9:
  MyCalendar.add(
    Chronos::Event(2, Chronos::Mark::Daily(9, 00, 00),
                Chronos::Span::Minutes(45)));



  // that longer meeting, what a drag:
  MyCalendar.add(
    Chronos::Event(3, Chronos::DateTime(2015, 12, 21, 18, 00),
                Chronos::DateTime(2015, 12, 22, 1, 00)));


  // weekly class, repeating every monday for one hour
  MyCalendar.add(
    Chronos::Event(4,
                Chronos::Mark::Weekly(Chronos::Weekday::Monday, 10, 30, 00),
                Chronos::Span::Hours(1)));

  // new year's eve party!
  MyCalendar.add(
    Chronos::Event(5, Chronos::DateTime(2015, 12, 31, 21, 00),
                Chronos::Span::Days(2)));


  // anniversary dinner... could set this up as a yearly event
  // but since the actual time for dinner is likely to change,
  // we just do this year's:
  MyCalendar.add(
    Chronos::Event(6, Chronos::DateTime(2015, 12, 28, 19, 15),
                Chronos::Span::Minutes(90)));

  // and *never* miss a monthly meeting of the council of the elders
  MyCalendar.add(
    Chronos::Event(7, Chronos::Mark::Monthly(2, 19, 0, 0),
                Chronos::Span::Hours(2)));


}

uint32_t calendarTest() {

uint32_t numRet = 0;
  Chronos::DateTime nowTime(Chronos::DateTime::now());


  /*
   * The Calendar will provide information as Chronos::Event::Occurrence objects.
   * These objects have the following attributes:
   *
   *  - id: the EventID of the related Chronos::Event
   *  - start: DateTime object of event start
   *  - finish: DateTime object of event end
   *  - isOngoing: boolean indicating whether this event is happening at the given datetime
   *
   */

  // we create an array of Event::Occurrence objects, to hold replies from the calendar
  Chronos::Event::Occurrence occurrenceList[OCCURRENCES_LIST_SIZE];

  // listOngoing: get events that are happening at specified datetime.  Called with
  // listOngoing(MAX_NUMBER_OF_EVENTS, INTO_THIS_ARRAY, AT_DATETIME)
  // It will return the number of events set in the INTO_THIS_ARRAY array.
  numRet += MyCalendar.listOngoing(OCCURRENCES_LIST_SIZE,occurrenceList, nowTime);


  numRet += MyCalendar.listNext(OCCURRENCES_LIST_SIZE, occurrenceList,
                                        nowTime);


  // listForDay, similar to listOngoing and listNext, but the returned
  // data is restricted to a events starting on a particular day
  Chronos::DateTime nextMonday = nowTime.next(Chronos::Weekday::Monday);
  numRet += MyCalendar.listForDay(OCCURRENCES_LIST_SIZE, occurrenceList, nextMonday);

  return numRet;

}


uint32_t runTest(uint16_t numTimes)
{
	uint32_t numFound = 0;


	for (uint16_t i=0; i<numTimes; i++)
	{

		setupCal();
		numFound += calendarTest();
	}

	return numFound;
}

#endif



