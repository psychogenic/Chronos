/*
* TestPerf.ino  -- this is a stripped-down version of the Calendar example, used by library devs
* as a standard measuring stick for performance across platforms.
*
* To use:
*         - install Arduino Time library, if not built-in https://www.pjrc.com/teensy/td_libs_Time.html
*         - optionally, edit the configuration #defines below
*         - compile and install on Arduino-compatible device
*         - open serial monitor/terminal and check it out
*
*
* Synopsis:
*   This program demonstrates some of the functions of time marks, which can be used to locate
*   specific points on the timeline ("next Monday) or to define and query
*   repeating events ("Mondays at 08h30")).
*
*   For more info check out the documentation at http://flyingcarsandstuff.com/projects/chronos/
*   or the examples for:
*          - DateTime object manipulations in the DateTime.ino example
*          - Point event (basically repeating marks on a timeline, like "every Sunday at 07h00")
*            handling in the PointEvents.ino example.
*          - Calendar handling, for defining and querying schedule of Chronos::Events (1D occurrences,
*			with start times and duration).
*
*
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

#include <Time.h>
#include <Chronos.h>


// the serial device specifics
#define SERIAL_BAUDRATE    9600
#define SER_DEVICE     Serial



#define NUM_RUNS_PER_TEST 50


#define CALENDAR_MAX_NUM_EVENTS   8

/* *********** Global Calendar object *********** */
// create a global calendar, "MyCalendar", that can hold up to CALENDAR_MAX_NUM_EVENTS scheduled events
// (repeating or not). To do this, there's a little shortcut that allows you to define a new class
// name and to specify the (max) size for calendars.  Call it with
// DefineCalendarType(A_CLASS_NAME, MAX_NUM_EVENTS_TO_HOLD)
DefineCalendarType(Calendar, CALENDAR_MAX_NUM_EVENTS);

// and *then* declare a global to hold everything:
Calendar MyCalendar;

// NOTE: these are only convenience #defines... if you need multiple calendars
// of different sizes, you can just use the templating directly, e.g.
//    Chronos::CalendarStaticArray<10> a10EventMaxCalendar;
// etc.

// OCCURRENCES_LIST_SIZE: array size for calendar query results
#define OCCURRENCES_LIST_SIZE   30






#define PRINT(...)    SER_DEVICE.print(__VA_ARGS__)
#define PRINTLN(...)  SER_DEVICE.println(__VA_ARGS__)
#define LINE()    PRINTLN(' ')
#define LINES(n)  outLines(n)


// dumb function to give us some space on screen
void outLines(uint8_t n)
{
  for (uint8_t _bl = 0; _bl < n; _bl++) {
    LINE();
  }
}

//
void setupCal() {

  // setting for monday, dec 21st 2015 @ 17:30:00
  // same as setTime(17, 30, 0, 21, 12, 2015);
  Chronos::DateTime::setTime(2015, 12, 21, 17, 30, 0);


  // setupCal is called repeatedly, so we clear() the calendar 
  // at every run
  MyCalendar.clear();
  
  /* Add some Chronos::Event objects to the calendar to make it useful */

  // Chronos::Event can be initialized a few ways, namely:
  //    - Bounding datetimes:           (EVENT_ID, Chronos::DateTime start, Chronos::DateTime end)
  //    - Start time and duration:      (EVENT_ID, Chronos::DateTime start, Chronos::Span::X duration)
  //    - Repeating event and duration: (EVENT_ID, Chronos::Mark::X repeatingEvent, Chronos::Span::X duration)
  // Some examples:
  // two meetings, one shot events, one setup using a duration and the other using bounding datetimes
  // meeting one, about a half hour long:

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
  numRet += MyCalendar.listOngoing(OCCURRENCES_LIST_SIZE, occurrenceList, nowTime);


  numRet += MyCalendar.listNext(OCCURRENCES_LIST_SIZE, occurrenceList,
                                nowTime);


  // listForDay, similar to listOngoing and listNext, but the returned
  // data is restricted to a events starting on a particular day
  Chronos::DateTime nextMonday = nowTime.next(Chronos::Weekday::Monday);
  numRet += MyCalendar.listForDay(OCCURRENCES_LIST_SIZE, occurrenceList, nextMonday);

  PRINT('.');
  return numRet;

}


uint32_t runTest(uint16_t numTimes)
{
  uint32_t numFound = 0;

  for (uint16_t i = 0; i < numTimes; i++)
  {

    setupCal();
    numFound += calendarTest();
  }

  return numFound;
}

void setup() {

  SER_DEVICE.begin(SERIAL_BAUDRATE);

}

void loop() {
  LINES(3);
  PRINT(F("Starting test run... looping setup+cal test "));
  PRINT(NUM_RUNS_PER_TEST);
  PRINTLN(F(" times. Go!"));
  uint32_t tStart = millis();
  uint32_t numFound = runTest(NUM_RUNS_PER_TEST);
  uint32_t tEnd = millis();
  LINE();

  PRINT(F("Retrieved a total of "));
  PRINT(numFound);
  PRINTLN(F(" event occurrences."));
  LINE();

  PRINT(F("Run time: "));
  PRINT((tEnd - tStart));
  PRINTLN(F(" ms."));
  delay(1500);
}