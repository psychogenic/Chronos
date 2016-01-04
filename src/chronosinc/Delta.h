/*
 * Delta.h
 *
 * Time span "deltas" -- a floating time length.
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

#ifndef CHRONOS_INTINCLUDES_DELTA_H_

#define CHRONOS_INTINCLUDES_DELTA_H_
#include "../chronosinc/timeExtInc.h"
#include "../chronosinc/timeTypes.h"
#include "../chronosinc/platform/platform.h"

namespace Chronos {
namespace Span {

/*
 * class Delta
 *
 * Base class for all time spans -- in essence, just a number of seconds but with some
 * added functionality for breaking it down into human-readable elements (days, hours, etc)
 * and for arithmetic operations and size comparisons.
 */
class Delta {
public:

	typedef struct elementsStruct {
		uint32_t 	days;
		int 		hours;
		int 		minutes;
		int 		seconds;
		bool		is_init;
		elementsStruct(uint32_t dy, int h, int m, int s) :
			days(dy), hours(h), minutes(m), seconds(s), is_init(true)
		{

		}

		elementsStruct() : days(0), hours(0), minutes(0), seconds(0),is_init(false)
		{

		}

	} Elements;

	/*
	 * Delta constructor
	 * @param secs: number of seconds in this delta.
	 */
	Delta(Chronos::EpochTime secs);

#ifdef PLATFORM_SUPPORTS_RVAL_MOVE
	// make the fact we want rvalue move defaults explicit,
	// in case a virtual dtor, copy ctor or op= are defined later
	Delta(Delta&&) = default;
	Delta& operator=(Delta&&) = default;

	Delta(const Delta&) = default;
	Delta& operator=(const Delta &) = default;
#endif

	/*
	 * Attributes.  Deltas can provide their total seconds count as
	 *
	 *  totalSeconds()
	 *
	 * or may be broken down into:
	 *
	 *  days()
	 *  hours()
	 *  minutes()
	 *  seconds()
	 *
	 * For simplicity, there are no setters for these.  If you want a delta equivalent to
	 * 3 days, 6 hours, 5 minutes and 2 seconds you just create one:
	 *
	 * (Span::Days(3) + Span::Hours(6) + Span::Minutes(5) + Span::Seconds(2))
	 */

	inline Chronos::EpochTime totalSeconds() const { return total_seconds;}

	inline uint32_t 		days() const { initElements(); return delta_elements.days; }
	inline Chronos::Hours 	 hours() const { initElements(); return delta_elements.hours; }
	inline Chronos::Minutes  minutes() const { initElements(); return delta_elements.minutes; }
	inline Chronos::Seconds  seconds() const { initElements(); return delta_elements.seconds; }





	/*
	 * Arithmetic operators.  You can add and subtract Deltas in the normal ways,
	 * with results mostly as expected.
	 *
	 * @note: "mostly" here is because Delta's are always *positive*.  This works as
	 * expected for any additions, but if you invert things and subtract a larger from
	 * a smaller delta, you'll still get a positive difference:
	 * (Chronos::Span::Minutes(1) - Chronos::Span::Hours(1)) == Chronos::Span::Minutes(59)
	 */
	Delta & operator+=(Chronos::EpochTime secs);
	Delta & operator+=(const Delta & delta);

	Delta & operator-=(Chronos::EpochTime secs);
	Delta & operator-=(const Delta & delta);


	Delta operator+(Chronos::EpochTime secs) const ;
	Delta operator+(const Delta & delta) const;

	Delta operator-(Chronos::EpochTime secs) const ;
	Delta operator-(const Delta & delta) const;

	/* Size comparators.  Everything is supported ==, !=, <, >, <=, >=.
	 *
	 *
	 */
	inline bool operator<(const Delta & other) const {return total_seconds < other.total_seconds;}
	bool operator==(const Delta & other) const {return total_seconds == other.total_seconds; }

	inline bool operator<=(const Delta & other) const { return ( ( (*this) < other) || (*this) == other ); }
	inline bool operator>(const Delta & other)  const { return ! ( (*this) <= other) ;}
	inline bool operator>=(const Delta & other) const { return ! ( (*this) < other) ;}
	inline bool operator!=(const Delta & other) const { return ! ( (*this) == other); }



	/*
	 * printTo(p)
	 *
	 * Similar to the member method for DateTime,
	 * printTo is mainly a convenience function for debugging, on Arduino
	 * or any platform that has some object with a compatible "Print" interface.
	 *
	 * @param p: the "printer" (e.g. Serial with Arduino)
	 */
	void printTo(Print & p) const ;
	//const Elements & elements() const ;
private:
	// private, I say!
	void initElements() const;

	Chronos::EpochTime total_seconds;
	mutable Elements delta_elements;

};


// Chronos::Span::Absolute is just a typedef to Delta
// that makes things "clear"
typedef Chronos::Span::Delta Absolute;


/*
 * Span utility classes.  The following make constructing arbitrary Deltas easy:
 *
 *  Seconds(s)
 *  Minutes(m)
 *  Hours(h)
 *  Days(d)
 *  Weeks(w)
 *
 * where the parameter is an integer number of whatever your creating... so
 *
 * Weeks(1) == Days(7) == Hours(7*24) etc.
 *
 */
class Seconds : public Chronos::Span::Delta
{
public:
	Seconds(Chronos::EpochTime secs);

};

class Minutes : public Chronos::Span::Delta
{
public:
	Minutes(uint32_t mins);

};


class Hours : public Chronos::Span::Delta
{
public:
	Hours(uint32_t num);

};

class Days : public Chronos::Span::Delta
{
public:
	Days(uint32_t num);

};


class Weeks : public Chronos::Span::Delta
{
public:
	Weeks(uint32_t num);

};



}
} /* namespace Chronos */

#endif /* CHRONOS_INTINCLUDES_DELTA_H_ */
