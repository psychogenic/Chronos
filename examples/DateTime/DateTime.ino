/*
 * DateTime.ino  -- example usage for DateTime and Span objects.
 *
 * To use:
 *         - install Arduino Time library, if not built-in  https://www.pjrc.com/teensy/td_libs_Time.html
 *         - optionally, edit the configuration #defines below
 *         - compile and install on Arduino-compatible device
 *         - open serial monitor/terminal and check it out
 *
 *
 * Synopsis:
 *   This program demonstrates some of the functions of the basic DateTime object.  In addition
 *   to the obvious, it shows how to interact with time spans and find specific time marks relative
 *   to a given DateTime.
 *
 *   For more info check out the documentation at http://flyingcarsandstuff.com/projects/chronos/
 *   or the examples for:
 *          - Point event (basically repeating marks on a timeline, like "every Sunday at 07h00")
 *            handling in the PointEvents.ino example.
 *          - Calendar handling, for defining and querying schedule of Chronos::Events (1D occurrences,
 *      with start times and duration).
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

void setup() {
  SERIAL_DEVICE.begin(SERIAL_BAUDRATE);

  PRINTLN(F("Starting up DateTime test"));

  // setting for monday, dec 21st 2015 @ 17:30:00
  // same as setTime(17, 30, 0, 21, 12, 2015);
  // call setTime once with
  // setTime(YYYY, MM, DD, HH, mm, SS)
  // or setup the source directly and don't call this at all...
  Chronos::DateTime::setTime(2015, 12, 21, 17, 30, 0);

  PRINTLN(" *** No real time source set, so pretending it is currently :");

  // Chronos::DateTime objects have a printTo() function that takes
  // any Print-complatible object and dumps info through it.  This is mostly
  // just for debugging, and you can use the various attribute accessor
  // methods to pretty-print however you like, in real life.
  Chronos::DateTime::now().printTo(SERIAL_DEVICE);

  LINES(3);

  delay(2000);

}

void basicDateTimeManips() {

  LINE();
  PRINTLN("**** Basic DateTime Manips Test ****");
  /*
   * Chronos::DateTime construction.
   *
   * You can create DateTime objects using:
   *    - specific date-time info: DateTime(YYYY, MM, DD, HH, mm, SS)
   *    - specific epoch (unix timestamp seconds): DateTime(EEEEEEEEE);
   *    - another DateTime object: DateTime(someotherDateTime)
   *    - by assignment: DateTime a = someotherDateTime
   *    - nothing at all: DateTime() -- sets to DateTime(Chronos::DateTime::now())
   *
   */
  Chronos::DateTime nowTime(Chronos::DateTime::now());
  PRINT(F("Right 'now' it's: "));
  nowTime.printTo(SERIAL_DEVICE); // dump to serial
  LINES(2);

  // you can add or subtract "deltas" to/from DateTimes to get other dateTimes
  Chronos::DateTime inTwoWeeks = nowTime + Chronos::Span::Weeks(2);
  PRINT("In two weeks exactly it will be ");
  inTwoWeeks.printTo(SERIAL_DEVICE);

  // and subtracting datetimes between each other gives the absolute difference
  Chronos::Span::Absolute diff = inTwoWeeks - nowTime;
  PRINT(F(" which is "));
  PRINT(diff.days());
  PRINTLN(F(" days away (of course)"));

  // DateTimes and Deltas can be compared using all the standard operators
  if ((inTwoWeeks > nowTime) && (diff == Chronos::Span::Days(14))) {
    PRINTLN(F("All is well, the future date > the now date..."));
  } else {
    // should never happen
    PRINTLN("Something is going horribly wrong????");
  }


  Chronos::DateTime evenLater = inTwoWeeks
                                + (Chronos::Span::Days(2) - Chronos::Span::Hours(1));

  // some overly complicate mathemagic with spans, just to show it...
  if (nowTime + (evenLater - inTwoWeeks) == (nowTime + Chronos::Span::Hours(47))) {
    (nowTime + (evenLater - inTwoWeeks)).printTo(SERIAL_DEVICE);
    PRINTLN(F(" is indeed in 47 hours"));
  } else {
    // should never happen
    PRINTLN(F("Something is going horribly wrong????"));
  }

  LINES(2);
}

void boundsAndNextPrev() {

  LINE();
  PRINTLN(F("**** Bounds and prev/next Test ****"));
  Chronos::DateTime curDateTime = Chronos::DateTime::now();

  PRINT(F("Right 'now' it's: "));
  curDateTime.printTo(SERIAL_DEVICE); // dump to serial
  LINES(2);

  // you can get the next/previous day, month rather easily
  Chronos::DateTime nextSaturday = curDateTime.next(Chronos::Weekday::Saturday);

  PRINT(F("Next saturday is in "));
  (nextSaturday - curDateTime).printTo(SERIAL_DEVICE);
  LINE();
  Chronos::DateTime previousJune = curDateTime.previous(Chronos::Named::Month::June);
  PRINT(F("And the preceding June started on "));
  previousJune.printTo(SERIAL_DEVICE);
  LINE();

  // or get fancy and use Chronos::Marks (see the PointEvents example for more on that)
  PRINT(F("The next time we see december 3rd is "));
  PRINT((curDateTime.next(Chronos::Mark::Yearly(12, 3)) - curDateTime).days());
  PRINT(F(" days away, whereas the last time we were dec. 3rd was "));
  PRINT((curDateTime - curDateTime.previous(Chronos::Mark::Yearly(12, 3))).days());
  PRINTLN(F(" days ago."));


  // a Chronos::DateTime::Bounds is a structure containing the
  // (inclusive) datetime boundaries for a period. you can query
  // dayBounds
  // monthBounds
  // yearBounds
  //
  // The important thing to remember is that the boundaries contain the first
  // and last DateTimes that are *part* of the period (with a granularity of 1 second)
  // so both
  //    bounds.start
  //    bounds.finish
  // are 'within' the period.  Thus if you subtract one from the other, you'll be off
  // by one second of what you might expect.  e.g. say you have the boundary for a
  // certain day, then
  //    (bounds.finish - bounds.start) will = 23 hours, 59 minutes and 59 seconds.
  // therefore, the total length delta may be accessed using the length() method which
  // in this case would give 24 hours (and 0 minutes, 0 seconds).

  Chronos::DateTime::Bounds todayBounds = Chronos::DateTime::dayBounds(curDateTime);

  if (todayBounds.start <= curDateTime && curDateTime <= todayBounds.finish)
  {
    PRINT(F("Yes indeed, "));
    todayBounds.start.printTo(SERIAL_DEVICE);
    PRINT(F(" <= "));
    curDateTime.printTo(SERIAL_DEVICE);
    PRINT(F(" <= "));
    todayBounds.finish.printTo(SERIAL_DEVICE);
    LINE();
    PRINT(F("And day-bounds length is "));
    todayBounds.spanRounded().printTo(SERIAL_DEVICE);
    LINE();
  } else {

    // should never happen
    PRINTLN(F("Something is going horribly wrong????"));
  }



  PRINTLN("More bounds... Feb FYI...");
  // check years 2015-2022
  for (int i = 2015; i < 2022; i++) {
    Chronos::DateTime::Bounds febBounds = Chronos::DateTime::monthBounds(i, 2);

    PRINT(F("Feb "));
    PRINT(i);
    PRINT(F(" has "));
    // could do febBounds.spanRounded() == Chronos::Span::Days(29))
    // but we'll do:
    if (febBounds.spanRounded().days() == 29) {
      PRINT(F("   29!  "));
    } else {
      PRINT(F("only 28 "));
    }

    PRINTLN(F(" days"));
  }

}

void countDownToBladeRunner() {

  LINE();
  PRINTLN(F("**** 'BladeRunner' Test ****"));

  // now time
  Chronos::DateTime curDateTime = Chronos::DateTime::now();

  // Blade runner: 2019-11-01 18:05:00
  Chronos::DateTime targetDateTime(2019, 11, 1, 18, 05, 00);

  // is that still in the glorious future?
  if (curDateTime > targetDateTime) {
    PRINTLN(F("Time sure flies... replicants are already here!"));
    return;
  }

  // how far away is that?
  Chronos::Span::Absolute timeDiff = targetDateTime - curDateTime;

  PRINT(F("There are "));
  PRINT(timeDiff.days());
  PRINT(F(" days, "));

  PRINT(timeDiff.hours());
  PRINT(F(" hours, "));

  PRINT(timeDiff.minutes());
  PRINT(F(" minutes and  "));

  PRINT(timeDiff.seconds());
  PRINT(F(" seconds between 'now' ("));

  curDateTime.printTo(SERIAL_DEVICE);

  PRINTLN(F(") and when Deckard chases after rogue Nexus-6 models on earth, on "));

  targetDateTime.printTo(SERIAL_DEVICE);
  PRINTLN(F("... moving forward, 10 days at a time: "));

  while (curDateTime < targetDateTime) {
    // increment using a convoluted 1 week + 3 days, just to
    // show the operators in action
    curDateTime += Chronos::Span::Weeks(1) + Chronos::Span::Days(3);

    PRINT('.');

  }

  PRINTLN('!');

  PRINT(F("Ooof, we have reached "));

  PRINT(curDateTime.year());
  PRINT('-');
  PRINT(curDateTime.month());
  PRINT('-');
  PRINT(curDateTime.day());
  PRINT(' ');
  PRINT(curDateTime.hour());
  PRINT(':');
  PRINT(curDateTime.minute());
  PRINT(':');
  PRINT(curDateTime.second());
  PRINTLN('!');

  LINES(2);

}

void loop() {

  basicDateTimeManips();
  delay(DELAY_BETWEEN_TESTS_MS);
  boundsAndNextPrev();
  delay(DELAY_BETWEEN_TESTS_MS);

  countDownToBladeRunner();
  delay(DELAY_BETWEEN_TESTS_MS);

}