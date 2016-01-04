/*
 * Calendar.ino  -- scheduling and event manipulation example.
 *
 * To use: 
 *         - install Arduino Time library, if not built-in https://www.pjrc.com/teensy/td_libs_Time.html
 *         - optionally, edit the configuration #defines below
 *         - compile and install on Arduino-compatible device
 *         - open serial monitor/terminal and check it out
 * 
 * 
 * Synopsis:
 *   This program demonstrates some of the functions of the higher level APIs in 
 *   the Chronos library, related to scheduling one-time and repeat events and 
 *   querying the calendar to know what's going on and when.
 *
 *   For more info check out the documentation at http://flyingcarsandstuff.com/projects/chronos/
 *   or the examples for:
 *          - DateTime object manipulations in the DateTime.ino example
 *          - Point event (basically repeating marks on a timeline, like "every Sunday at 07h00") 
 *            handling in the PointEvents.ino example.
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
#define SERIAL_DEVICE     Serial


// delay between screen dumps, leave enough ms to read the screen...
#define DELAY_BETWEEN_TESTS_MS    10000

// CALENDAR_MAX_NUM_EVENTS:
// The max number of events the calendar will accept.
// The larger you make this, the more memory it'll eat up.
// The calendar takes a little over 5 bytes + (41*NUM_EVENTS), 
// whether they are one-shot or recurring events.
// 46 bytes for 1 event
// 87 for 2 events ...
// 415 bytes for 10 events, etc.

#define CALENDAR_MAX_NUM_EVENTS   8

/* *********** Global Calendar object *********** */
// create a global calendar, "MyCalendar", that can hold up to CALENDAR_MAX_NUM_EVENTS scheduled events
// (repeating or not). To do this, there's a little shortcut that allows you to define a new class
// name and to specify the (max) size for calendars.  Call it with
// DefineCalendarType(A_CLASS_NAME, MAX_NUM_EVENTS_TO_HOLD)
DefineCalendarType(Calendar, CALENDAR_MAX_NUM_EVENTS);

// and *then* declare a global using that new type ("Calendar", here) to hold everything:
Calendar MyCalendar;

// NOTE: this is just a convenience #define... if you need multiple calendars
// of different sizes, or want to sidestep the syntactic sugar, 
// you can just use the templating directly, e.g. declare a cal with:
//    Chronos::CalendarStaticArray<10> a10EventMaxCalendar;
// etc.


// OCCURRENCES_LIST_SIZE: array size for calendar query results
// At a size of 30, with the RAM strings and other stuff we're 
// using here, we're right at the limit of what an ATMega328
// (basic Arduino) can handle... any bigger and we smash the stack
// and get out-of-memory weirdness/resets/etc.
//
#define OCCURRENCES_LIST_SIZE   30

// some human-legible names for our events
// NOTE: include an entry in this array for each EventID used, below,
// otherwise... I see crashes in your future.
const char * EventNames[] = {
  "N/A", // just a placeholder, for indexing easily
  "Project Meeting   ",
  "Daily Workout     ",
  "Looong Meeting    ",
  "Yoga Class--uummm ",
  "New Year's eve!!!!",
  "** Anniv dinner **",
  "Council of Elders ",
  NULL
};


#define PRINT(...)    SERIAL_DEVICE.print(__VA_ARGS__)
#define PRINTLN(...)  SERIAL_DEVICE.println(__VA_ARGS__)
#define WAITFORINPUT() while (!SERIAL_DEVICE.available()) { delay(50);}
#define LINE()    PRINTLN(' ')
#define LINES(n)  outLines(n)
#define DUMPSIZE(nm, s) PRINT(F("RAM for ")); PRINT(nm) ; PRINT(F(": ")); PRINT(sizeof(s)); PRINTLN(F(" bytes."))


// dumb function to give us some space on screen
void outLines(uint8_t n)
{
  for (uint8_t _bl = 0; _bl < n; _bl++) {
    PRINTLN(' ');
  }
}

//
void setup() {
  SERIAL_DEVICE.begin(SERIAL_BAUDRATE);

  // setting for monday, dec 21st 2015 @ 17:30:00
  // same as Time lib's setTime(17, 30, 0, 21, 12, 2015);
  Chronos::DateTime::setTime(2015, 12, 21, 17, 30, 0);

  LINES(2);
  PRINTLN(F(" *** No real time source set, so pretending it is currently :"));
  
  // printTo is a convenience method useful for debugging
  // in real life, you'd use accessors and format it however you like.
  Chronos::DateTime::now().printTo(SERIAL_DEVICE);

  LINES(2);

  DUMPSIZE(F("Calendar"), MyCalendar);
  LINES(2);
  
  
  /* Add some Chronos::Event objects to the calendar to make it useful */

  // Chronos::Event can be initialized a few ways, namely:
  //    - Bounding datetimes:           (EVENT_ID, Chronos::DateTime start, Chronos::DateTime end)
  //    - Start time and duration:      (EVENT_ID, Chronos::DateTime start, Chronos::Span::X duration)
  //    - Repeating event and duration: (EVENT_ID, Chronos::Mark::X repeatingEvent, Chronos::Span::X duration)
  // Some examples:
  // two meetings, one shot events, one setup using a duration and the other using bounding datetimes
  // meeting one, about a half hour long.
  
  // NOTE: MyCalendar.add() returns a true value if the add succeeded, which you might 
  // want to check in real life, if you're dynamically adding/removing events...
  
  // Also note that, though you can keep these various Event, Span etc. objects 
  // around if you like, everything's designed to let you just pass temporary
  // "anonymous" objects like this.
  
  // so here's the meeting, an event created with a DateTime and a Span
  MyCalendar.add(Chronos::Event(1, Chronos::DateTime(2015, 12, 21, 17, 00),
                             Chronos::Span::Minutes(33))
                );



  // don't forget the daily workout, a daily recurring event every morning at 9:
  // another Event constructed with (DateTime, Span) 
  MyCalendar.add(
    Chronos::Event(2, Chronos::Mark::Daily(9, 00, 00),
                Chronos::Span::Minutes(45)));



  // that longer meeting, what a drag...
  // This one is constructed using bounding DateTimes for 
  // the start and finish
  MyCalendar.add(
    Chronos::Event(3, Chronos::DateTime(2015, 12, 21, 18, 00),
                Chronos::DateTime(2015, 12, 22, 1, 00)));


  // weekly class, repeating every monday for one hour
  // an Event created using a repeating Mark, and a Span
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

  // WARNING: if you choose to add more events, make sure to adjust the EventNames array, above, so
  // we don't try to print out-of-bounds (no checking in this simple test).
  
  
  PRINT(F("Calendar now set up with "));
  PRINT(MyCalendar.numEvents());
  PRINT(F(" events ("));
  PRINT(MyCalendar.numRecurring());
  PRINTLN(F(" of which are repeat events)."));
  LINES(2);
  

  PRINTLN(F("Now we'll display the current and upcoming events, then delete them one by one and refresh..."));
  LINE();
  PRINTLN(F("Send a <newline> or enter any text to begin..."));
  LINES(2);

  WAITFORINPUT();
}

void calendarTest() {

  LINES(3);
  PRINTLN(F("**** Calendar Test ****"));

  Chronos::DateTime nowTime(Chronos::DateTime::now());
  PRINT("Right \"now\" it's: ");
  
  nowTime.printTo(SERIAL_DEVICE);
  LINES(2);

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
  int numOngoing = MyCalendar.listOngoing(OCCURRENCES_LIST_SIZE,
                                          occurrenceList, nowTime);
  if (numOngoing) {

    // At least one event is happening at "nowTime"...
    
    LINE();
    PRINTLN("**** Some things are going on this very minute! ****");

    for (int i = 0; i < numOngoing; i++) {

      PRINT(F("**** Event: "));
      PRINT((int )occurrenceList[i].id);
      PRINT('\t');
      PRINT(EventNames[occurrenceList[i].id]);
      PRINT(F("\tends in: "));
      (nowTime - occurrenceList[i].finish).printTo(SERIAL_DEVICE);
      LINES(2);

    }

  } else {
    PRINTLN(F("Looks like we're free for the moment..."));
  }

  LINES(2);

  // listNext works like listOngoing, described above, but will instead fill the
  // array with up-coming events (things that are still in the future)

  PRINTLN(F("Coming up:"));

  int numUpcoming = MyCalendar.listNext(OCCURRENCES_LIST_SIZE, occurrenceList,
                                        nowTime);
  if (numUpcoming) {
    for (int i = 0; i < numUpcoming; i++) {

      Chronos::Span::Absolute startsIn = (occurrenceList[i].start - nowTime);
      if (startsIn < Chronos::Span::Days(3)) {

        // coming soon, print a detailed listing...
        PRINT(F("Event "));
        PRINT((int )occurrenceList[i].id);
        PRINT('\t');
        PRINT(EventNames[occurrenceList[i].id]);
        PRINT('\t');
        occurrenceList[i].start.printTo(SERIAL_DEVICE);
        PRINT(F(" - "));
        occurrenceList[i].finish.printTo(SERIAL_DEVICE);
        LINE();

        PRINT(F("Starts in "));
        startsIn.printTo(SERIAL_DEVICE);

        LINES(2);
      } else {
        // further into the future, just print summary
        PRINT(F("Event "));
        PRINT((int )occurrenceList[i].id);
        PRINT('\t');
        PRINT(EventNames[occurrenceList[i].id]);
        PRINT('\t');
        occurrenceList[i].start.printTo(SERIAL_DEVICE);
        LINE();
      }

    }
  } else {
  // nothing returned by listNext()
    PRINTLN(F("Calendar is wiiiide open... Anyone got plans?"));
  }

  LINES(3);

  // listForDay, similar to listOngoing and listNext, but the returned
  // data is restricted to a events starting on the particular day of the query
  
  // let's check the following monday:
  Chronos::DateTime nextMonday = nowTime.next(Chronos::Weekday::Monday);
  
  int numMon = MyCalendar.listForDay(OCCURRENCES_LIST_SIZE, occurrenceList, nextMonday);
  if (numMon) {
    LINES(2);
    PRINTLN(F("On the plate for next monday:"));
    for (int i = 0; i < numMon; i++) {
      PRINT(F("Event "));
      PRINT((int )occurrenceList[i].id);
      PRINT(F(" ("));
      PRINT(EventNames[occurrenceList[i].id]);
      PRINT(F(")\t"));
      occurrenceList[i].start.printTo(SERIAL_DEVICE);
      LINE();

    }

    LINES(2);
  }

}

// start our delete id counter 
// negative, so we see the full list a couple of times
// before deleting items:
int8_t anEventId = -1;


void loop() {

  // call the test function
  calendarTest();
  
  // wait a bit
  delay(DELAY_BETWEEN_TESTS_MS);
  
  // try and delete an event, to show 
  // how the schedule is modified
  if (MyCalendar.remove(anEventId++))
  {
    LINES(2);
    PRINT(F("**** DELETED  event: "));
    PRINTLN(EventNames[anEventId - 1]);
  }
}