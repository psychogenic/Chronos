/*
* PointEvents.ino  -- example usage for sets of "marks" on the time-line, single point events, like
* "Monday @ 7am".
*
* To use:
*         - install Arduino Time library, if not built-in  https://www.pjrc.com/teensy/td_libs_Time.html
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

#define DELAY_BETWEEN_TESTS_MS    10000

#define SERIAL_BAUDRATE    9600
#define SERIAL_DEVICE     Serial


// a few defines to abstract away the Serial-specific stuff
#define PRINT(...)    SERIAL_DEVICE.print(__VA_ARGS__)
#define PRINTLN(...)  SERIAL_DEVICE.println(__VA_ARGS__)
#define LINE()    PRINTLN(' ')
#define LINES(n)  for (uint8_t _bl=0; _bl<n; _bl++) { PRINTLN(' '); }


void setup()
{
  SERIAL_DEVICE.begin(SERIAL_BAUDRATE);

  PRINTLN(F("Starting up PointsEvents test"));
  // setting for monday, dec 21st 2015 @ 17:30:00
  // same as setTime(17, 30, 0, 21, 12, 2015);
  Chronos::DateTime::setTime(2015, 12, 21, 17, 30, 0);


  LINE();

  PRINTLN(F(" *** No real time source set, so pretending it is currently :"));

  // printTo is a convenience method useful for debugging
  // in real life, you'd use accessors and format it however you like.
  Chronos::DateTime::now().printTo(SERIAL_DEVICE);

  LINES(3);

}


void timeboundEvents()
{
  Chronos::DateTime curDateTime = Chronos::DateTime::now();

  LINE();
  PRINTLN(F("**** Time-bound events Test ****"));

  PRINT(F("Time 'now' is: "));
  curDateTime.printTo(SERIAL_DEVICE);
  LINES(2);



  /* ****** single point event ****** */
  // A single point in time: sunset of last summer's solstice was:
  Chronos::DateTime lastSummerSolstice = curDateTime.previous(Chronos::Mark::Yearly(6, 21, 21, 30, 20));

  PRINT(F("Last summer solstice was "));

  PRINT( (curDateTime - lastSummerSolstice).days() );
  PRINT(F(" days ago... that was "));
  lastSummerSolstice.printTo(SERIAL_DEVICE);
  LINE();


  /* ****** Weekly point event ****** */
  // a recurring weekly event: "Mondays" (specified here without a precise time)
  Chronos::Mark::Weekly caseOfTheMondays(Chronos::Weekday::Monday);


  Chronos::DateTime nextMonday = curDateTime.next(caseOfTheMondays);
  Chronos::DateTime lastMonday = curDateTime.previous(caseOfTheMondays);



  PRINT(F("Today is: "));
  curDateTime.printTo(SERIAL_DEVICE);
  LINES(2);

  PRINT(F("Last Monday ("));
  lastMonday.printTo(SERIAL_DEVICE);
  PRINT(F(")) was "));
  (curDateTime - lastMonday).printTo(SERIAL_DEVICE);
  PRINTLN(F(" ago."));
  LINE();


  Chronos::Span::Absolute absDiff = nextMonday - curDateTime;

  PRINT(F("Next monday ("));
  nextMonday.printTo(SERIAL_DEVICE);
  PRINT(F(")) is in "));
  PRINT(absDiff.days());
  PRINT(F(" days, "));
  PRINT(absDiff.hours());
  PRINT(F(" hours and "));
  PRINT(absDiff.minutes());
  PRINTLN(F(" minutes."));
  LINES(2);






  /* ****** Daily point event ****** */
  // specify a recurring daily alarm clock, at 9:30:10 each morning.
  Chronos::Mark::Daily alarmClock(9, 30, 10);
  Chronos::Span::Absolute alarmIn = curDateTime.next(alarmClock) - curDateTime;


  LINE();

  PRINT(F("My blasted alarm clock will ring on "));
  LINE();
  curDateTime.next(alarmClock).printTo(SERIAL_DEVICE);
  PRINTLN(F(" which is in "));
  PRINT(alarmIn.hours());
  PRINT(F(" hours, "));
  PRINT(alarmIn.minutes());
  PRINT(F(" mins and "));
  PRINT(alarmIn.seconds());
  PRINTLN(F(" precious seconds."));


  LINES(3);


}


void repeatingEventsTest()
{
  // an array to hold datetimes for up to 15 classes
  Chronos::DateTime yogaClassList[15];
  Chronos::DateTime curDateTime = Chronos::DateTime::now();

  LINE();
  PRINTLN(F("**** Repeating Events Test ****"));

  PRINT(F("Time 'now' is: "));
  curDateTime.printTo(SERIAL_DEVICE);
  LINE();



  // my yoga classes start on Mondays @ 10:30 :
  Chronos::Mark::Weekly yogaClassWeekly(Chronos::Weekday::Monday, 10, 30, 00);


  // we can use any datetime to get a list of previous and next Chronos::Marks.


  // get the last 3, which I unfortunately didn't attend...

  // listPrevious will fill the array from closest to furthest date
  // meaning yogaClassWeekly[n] is closer to us in time than yogaClassWeekly[n+1]
  // so yogaClassWeekly[n] > yogaClassWeekly[n+1]

  // Call it with:
  //   listPrevious(MAX_NUMBER, SOME_TIME_EVENT, ARRAY_FOR_RESULTS)
  // It returns the number actually inserted into array, but since this
  // is a repeating event (we'll never run out of mondays), we can be sure
  // we'll get our MAX_NUMBER of results set.
  curDateTime.listPrevious(3, yogaClassWeekly, yogaClassList);


  LINES(2);
  PRINTLN(F("Ah, Yoga..."));
  PRINTLN(F("I missed my last three classes: "));
  for (uint8_t i = 0; i < 3; i++)
  {
    PRINT('\t');
    // print
    yogaClassList[i].printTo(SERIAL_DEVICE);
    LINE();
  }
  LINE();


  // now we get the future classes, with listNext
  // again, the method will return events that are "closer" in time
  // with lower indexes... since we are moving forward, now
  // yogaClassWeekly[n] < yogaClassWeekly[n+1]
  PRINTLN(F("But I've promised myself I'd go to the next 15: "));
  curDateTime.listNext(15, yogaClassWeekly, yogaClassList);
  for (int i = 0; i < 15; i++)
  {
    PRINT('\t');
    yogaClassList[i].printTo(SERIAL_DEVICE);
    LINE();
  }

  LINES(2);

}

void loop()
{

  timeboundEvents();
  delay(DELAY_BETWEEN_TESTS_MS);

  repeatingEventsTest();
  delay(DELAY_BETWEEN_TESTS_MS);

}