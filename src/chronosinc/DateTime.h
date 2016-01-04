/*
 * DateTime.h
 *
 * The Chronos::DateTime is a primary "low-level"
 * component of the library and most other elements
 * are built around its use.
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

#ifndef CHRONOS_INTINCLUDES_DATETIME_DT_H_
#define CHRONOS_INTINCLUDES_DATETIME_DT_H_

#include "../chronosinc/timeExtInc.h"
#include "../chronosinc/timeTypes.h"
#include "../chronosinc/Delta.h"
#include "../chronosinc/Event.h"
#include "../chronosinc/platform/platform.h"
#include "../chronosinc/platform/timesource.h"

// 07 Feb 2106 06:28:14
#define END_OF_TIMEDATES_INIT 	2106, 2, 7, 6, 28, 0
namespace Chronos {

/*
 * class Chronos::DateTime
 *
 * Represents, oddly enough, a date-time -- a combination of year/month/day and hour/minute/seconds.
 *
 * DateTimes can be queried and compared, and they are "movable" in that they may be modified to change the
 * date/time in a number of ways.
 *
 *  // creation and setup
 *  Chronos::DateTime aDT = Chronos::DateTime::now();
 *
 *
 *  aDT.setHour(11);
 *  aDT += Chronos::Span::Days(7);
 *
 *  // comparison
 *  assert(aDT > (aDT - 60));
 *
 *  Chronos::DateTime nextMonday = aDT.next(Chronos::Mark::Weekly(Chronos::Weekday::Monday));
 *
 *  See the full interface below, and the DateTime and other examples in the examples/ directory.
 *
 */
class DateTime {
public:


	/*
	 * Chronos::DateTime::setTime(YYYY, MM, DD, HH, mm, SS)
	 *
	 * Tell the _external_ time-keeper to set the current date-time, e.g. the Arduino Time library.
	 */
	static void setTime(Year year, Month month, Day day, Hours hours, Minutes minutes, Seconds secs=0);


	/*
	 * Chronos::DateTime::now()
	 *
	 * @return: a DateTime object representing the current instant, according to the external
	 * time keeper.
	 */
	static DateTime now();




	class Bounds; // forward decl
	/* Bounds
	 * all *Bounds() methods return a DateTime::Bounds which contain the farthest start and finish
	 * DateTime objects which are still within the specified boundary:
	 *
	 *  [start, finish] <-- note: _inclusive_
	 *
	 * Any datetime that is outside the bounds will therefore either be
	 *
	 * 	(outDate < bounds.start) || (outDate > bounds.finish)
	 *
	 * The caveat here is that, owing to the seconds-granularity of our base clock, the delta of the bounds:
	 *
	 * 	(bounds.finish - bounds.start) (or, equivalently, bounds.span())
	 *
	 * will be a "second short" of intuitive value, e.g. 23:59:59 for a dayBounds(),
	 * 6 days, 23h 59m and 59 seconds for a weekBounds() etc.  If you really want it to account for the
	 * lost second, you can use
	 *
	 * bounds.spanRounded() -- a Span::Delta object that will give
	 * 7 days, 0 hours, 0 mins, 0 secs in the week example above.
	 *
	 */
	static Bounds dayBounds(Year year, Month month, Day day);
	static Bounds dayBounds(const DateTime & fromDt);
	static Bounds weekBounds(Year year, Month month, Day day);
	static Bounds weekBounds(const DateTime & fromDt);
	static Bounds monthBounds(Year year, Month month);
	static Bounds monthBounds(const DateTime & fromDt);
	static Bounds yearBounds(Year year);
	static Bounds yearBounds(const DateTime & fromDt);

	/*
	 * previous/next weekday or month
	 * Returns the previous/next ... weekday or month, a DateTime object set
	 * to the start of the relevant day.
	 *
	 * May be statically called with the relevent day/month and a DateTime object.
	 */
	static DateTime previous(Chronos::Weekday::Day aDay, const DateTime & fromDt);
	static DateTime previous(Chronos::Named::Month::Month month, const DateTime & fromDt);
	static DateTime next(Chronos::Weekday::Day aDay, const DateTime & fromDt);
	static DateTime next(Chronos::Named::Month::Month month, const DateTime & fromDt);


	/*
	 * DateTime Constructors
	 */
	/*
	 * DateTime(YYYY, MM, DD, HH, mm, SS)
	 */
	DateTime(Year year, Month month, Day day, Hours hours=0, Minutes minutes=0, Seconds secs=0);

	/*
	 * DateTime(UNIX_EPOCH)
	 */
	DateTime(Chronos::EpochTime atEpochSecs);

	/*
	 * DateTime(Chronos::TimeElements)
	 *
	 * When using the Arduino Time library, Chronos::TimeElements is just an alias
	 * to the tmElements_t struct they use.
	 */
	DateTime(const Chronos::TimeElements & atTime);

#ifdef PLATFORM_SUPPORTS_RVAL_MOVE
	// make the fact we want defaults explicit,
	// in case a virtual dtor, copy ctor or op= are defined later
	DateTime(DateTime&&) = default;
	DateTime& operator=(DateTime&&) = default;

	DateTime& operator=(const DateTime& ) = default;
	DateTime(const DateTime &) = default;
#endif



	/*
	 * Accessors
	 *
	 * Getters for DateTime attributes (mostly obvious):
	 * 	second()
	 * 	minute()
	 * 	hour()
	 * 	day() (day of month)
	 * 	weekday() (day of week)
	 * 	month() (month starting at 1 for january)
	 * 	year()  (4-digit YYYY)
	 *
	 */
	inline Seconds 	second()  const { return getElements().Second;}
	inline Minutes 	minute()  const { return getElements().Minute;}
	inline Hours 	hour()    const { return getElements().Hour;}
	inline Day		day()     const { return getElements().Day;}
	inline WeekDay	weekday() const { return getElements().Wday;}
	inline Month	month()	  const { return getElements().Month;}
	inline Year		year()    const { return tmYearToCalendar(getElements().Year);}


	/*
	 * Setters for attributes above
	 * setSecond(ss)
	 * setMinute(mm)
	 * setHour(hh)
	 * setDay(dd)
	 * setMonth(mm)
	 * setYear(yyyy)
	 *
	 */
	void setSecond(Seconds s);
	void setMinute(Minutes m);
	void setHour(Hours h);
	void setDay(Day d);
	void setMonth(Month m);
	void setYear(Year y);

	/*
	 * setToStartOfDay()
	 * setToEndOfDay()
	 *
	 * Set the calling object to the earliest (00:00:00) or latest (23:59:59)
	 * time that is still within the current day, respectively.
	 */
	void setToStartOfDay();
	void setToEndOfDay();



	/*
	 * startOfDay()
	 * endOfDay()
	 *
	 * Similar to setToStartOfDay/setToEndOfDay but instead of modifying the caller,
	 * returns a fresh DateTime object set to the same date but with appropriate time.
	 */
	DateTime startOfDay() const;
	DateTime endOfDay() const;




	/*
	 * Member method for the previous/next described above, returning
	 * DateTimes relative to the calling object.
	 */
	DateTime previous(Chronos::Named::Month::Month month) const;
	DateTime previous(Chronos::Weekday::Day aDay) const;
	DateTime next(Chronos::Weekday::Day aDay) const;
	DateTime next(Chronos::Named::Month::Month month) const;


	/*
	 * next(mark)
	 *
	 * @param mark: a Chronos::Mark::Event which specifies some time mark (point)
	 * @return: DateTime object corresponding to next occurrence of mark.
	 *
	 * E.g. Chronos::DateTime::now().next(Chronos::Mark::Monthly(13, 18, 30, 0))
	 * would return a DateTime corresponding to the next time it will be the 13th
	 * of the month at 18h30.
	 */
	DateTime next(const Mark::Event & ev) const ;
	/*
	 * previous(mark)
	 * Same description as above, for next(mark) but for... previous occurrence.
	 */
	DateTime previous(const Mark::Event & ev) const;
	/*
	 * listNext/listPrevious(n, mark, returnArray[])
	 *
	 * Fetch the list of the n next/previous occurrences of mark, and stick them
	 * into returnArray.
	 *
	 * @param n: uint8_t number of DateTimes to set in returnArray
	 * @param mark: Chronos::Mark::* time mark specification
	 * @param: returnArray[]: an array of (at least) n DateTimes to return values in.
	 *
	 */
	void listNext(uint8_t num, const Mark::Event & ev, DateTime into[]) const ;
	void listPrevious(uint8_t num, const Mark::Event & ev, DateTime  into[]) const;



	/*
	 * Arithmetic operators.
	 *
	 * You can perform arithmetic operations on DateTime objects and they will behave
	 * (mostly) as you expect.
	 *
	 * DateTime anExample = DateTime::now();
	 *
	 * DateTime result = anExample + 120; // result will be advanced by 120 seconds
	 * result = anExample + Chronos::Span::Minutes(2); // same thing
	 *
	 * anExample += Chronos::Span::Hours(3); in place operations
	 *
	 *
	 * Same thing holds for subtraction but for two caveats: if you subtract the equivalent of
	 * "more seconds than have occurred since the epoch from the DT" then you'll wind up at
	 * epoch == 0 -- so in the 70s.  Probably shouldn't do that.
	 *
	 * The second thing is that if you subtract one DateTime from another, you don't get a
	 * DateTime as a result... you get a Chronos::Span -- i.e. the difference between the two,
	 * the length of time to get from one from the other (see below).
	 *
	 */
	DateTime operator-(Chronos::EpochTime numSeconds) const;
	DateTime & operator-=(Chronos::EpochTime numSeconds);
	DateTime operator-(const Span::Delta & delta) const;
	DateTime & operator-=(const Span::Delta & delta);

	DateTime operator+(Chronos::EpochTime numSeconds) const;
	DateTime & operator+=(Chronos::EpochTime numSeconds);

	DateTime operator+(const Span::Delta & delta) const;
	DateTime & operator+=(const Span::Delta & delta);


	/*
	 * DateTime - DateTime
	 *
	 * You can subtract one datetime from another but what you get (a Chronos::Span::Absolute)
	 * will alway be *positive* (equal to the absolute difference).
	 *
	 * Use comparison operators if needed to subtract the smaller from the bigger (later) datetime,
	 * if you care about "direction" of the result.
	 *
	 * @return: Span::Absolute  a Span:Delta object that has methods to query the
	 * size (in seconds) and break it down into days/hours/mins/secs etc.  See Delta.h for details there.
	 *
	 */
	Span::Absolute operator-(const DateTime & otherDt) const;



	/*
	 * isWithin(bounds)
	 * @param bounds: DateTime::Bounds object determining (inclusive) boundaries
	 * @return: true if calling object is within bounds
	 */
	bool isWithin(const Bounds & bounds) const;



	bool isWeekend() const;
	inline bool isWeekday() const { return ! (isWeekend()); }
	inline bool operator==(const DateTime & dt) const { return (epoch == dt.epoch); }
	inline bool operator>(const DateTime & dt) const { return (epoch > dt.epoch);}
	inline bool operator>=(const DateTime & dt) const {return (epoch >= dt.epoch); }

	inline bool operator!=(const DateTime & dt) const { return (epoch != dt.epoch);}
	inline bool operator<(const DateTime & dt)  const { return (epoch < dt.epoch);}
	inline bool operator<=(const DateTime & dt) const { return (epoch <= dt.epoch); }

	/*
	 * sameTimeAs/sameDateAs
	 *
	 * Similar to == comparison operator, but only applies to object's time/date (respectively).
	 */
	bool sameTimeAs(const DateTime & dt) const;
	bool sameDateAs(const DateTime & dt) const;

	inline const Chronos::TimeElements & asElements() const { return getElements();}
	inline Chronos::EpochTime asEpoch() const { return epoch;}

	/*
	 * printTo is mainly a convenience function for debugging, on Arduino or any platform
	 * that has some object with a compatible "Print" interface.
	 *
	 * @param p: the "printer" (e.g. Serial with Arduino)
	 * @param includeTime: boolean to indicate whether we include the
	 *                     time in the output (defaults to true)
	 */
	void printTo(Print & p, bool includeTime=true) const;


	DateTime();

	/*
	 * endOfTime()
	 * Mostly for internal use, but returns a DateTime that is a the limit of what we
	 * can represent with the library... normal dates should all be < Chronos::DateTime::endOfTime()
	 *
	 */
	static inline DateTime endOfTime() { return DateTime(END_OF_TIMEDATES_INIT); }
private:

	// private, I say!
	inline Chronos::TimeElements & getElements() const {initElements(); return _elements;}
	void initElements() const {if (_elements.Year || _elements.Month) {return;} DATETIME_CONVERT_EPOCH_INTO_TIMELEMENTS(epoch, _elements); }

	void reInitEpoch();
	mutable Chronos::TimeElements _elements;
	Chronos::EpochTime epoch;
};


/*
 * class Chronos::DateTime::Bounds
 *
 * A simple wrapper around a set of datetime boundaries (start and finish).
 * See the relevent methods above, in the DateTime class.
 */
class DateTime::Bounds {
public:
	DateTime start;
	DateTime finish;
	Bounds()
	{

	}
	Bounds(const DateTime & begin, const DateTime & end) : start(begin), finish(end)
	{

	}

	Chronos::Span::Delta spanRounded() {
		return (finish - start) + 1;
	}

	Chronos::Span::Delta span() {
		return (finish - start);
	}



};


} /* namespace Chronos */

#endif /* CHRONOS_INTINCLUDES_DATETIME_H_ */
