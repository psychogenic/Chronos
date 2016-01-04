#include "chronosinc/DateTime.h"

/*
 * DateTime.cpp
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

#include "chronosinc/DateTime.h"

#include "chronosinc/marks/marks.h"

#define DATETIME_TIMELEMENTS_LAZY_INIT




#define DATETIME_TIMELEMENTS_UNINIT() 	_elements.Year = 0; _elements.Month = 0

#ifdef DATETIME_TIMELEMENTS_LAZY_INIT
#define DATETIME_INTERNAL_EPOCH_MODIFIED()	DATETIME_TIMELEMENTS_UNINIT()
#else
#define DATETIME_INTERNAL_EPOCH_MODIFIED()	DATETIME_CONVERT_EPOCH_INTO_TIMELEMENTS(epoch, elements)
#endif

namespace Chronos {
void DateTime::setTime(Year year, Month month, Day day, Hours hours, Minutes minutes, Seconds secs)
{
	DATETIME_SET_CURRENT_TIME(year, month, day, hours, minutes, seconds)
}


DateTime DateTime::now() {

	return DateTime(DATETIME_GET_CURRENT_EPOCH());
}


DateTime::Bounds DateTime::dayBounds(Year year, Month month, Day day) {
	return dayBounds(DateTime(year, month, day));
}

DateTime::Bounds DateTime::dayBounds(const DateTime& fromDt) {
	DateTime::Bounds retBounds(fromDt, fromDt);
	retBounds.start.setToStartOfDay();
	retBounds.finish.setToEndOfDay();
	return retBounds;

}

DateTime::Bounds DateTime::weekBounds(Year year, Month month, Day day) {
	return weekBounds(DateTime(year, month, day));
}

DateTime::Bounds DateTime::weekBounds(const DateTime& fromDt) {

	DateTime start(fromDt.startOfDay());

	if (start.weekday() != Chronos::Weekday::Sunday)
	{
		// not already at start of week...
		start = start.previous(Chronos::Weekday::Sunday);

	}
	return DateTime::Bounds(start, start.next(Chronos::Weekday::Saturday).endOfDay());
}




DateTime::Bounds DateTime::monthBounds(Year year, Month month) {
	DateTime dt(year, month, 1);
	return monthBounds(dt);
}

DateTime::Bounds DateTime::monthBounds(const DateTime& fromDt) {
	DateTime startDt(fromDt.startOfDay());
	startDt.setDay(1);
	DateTime endDt(startDt + Chronos::Span::Days(32));
	endDt.setToEndOfDay();
	Chronos::Span::Days oneDay(1);

	while (endDt.month() != startDt.month())
	{

		endDt -= oneDay;
	}


	return Bounds(startDt, endDt);

}

DateTime::Bounds DateTime::yearBounds(Year year) {
	return yearBounds(DateTime(year, 1, 1));
}

DateTime::Bounds DateTime::yearBounds(const DateTime& fromDt) {

	DateTime startDt(fromDt.startOfDay());
	startDt.setMonth(1);
	startDt.setDay(1);


	DateTime endDt(startDt + Chronos::Span::Days(367));

	Chronos::Span::Days oneDay(1);
	while (endDt.year() != startDt.year())
	{
		endDt -= oneDay;
	}
	return Bounds(startDt, endDt);

}


DateTime DateTime::previous(Chronos::Weekday::Day aDay) const
{

	return previous(aDay, *this);
}
DateTime DateTime::previous(Chronos::Weekday::Day aDay, const DateTime & fromDt)
{

	Chronos::Mark::Weekly targetDayEvent(aDay, 0,0,0);
	return targetDayEvent.previous(fromDt);

}
DateTime DateTime::previous(Chronos::Named::Month::Month month) const
{
	return previous(month, *this);

}
DateTime DateTime::previous(Chronos::Named::Month::Month month, const DateTime & fromDt)
{
	DateTime possibleOccurrences[13];
		Chronos::Mark::Monthly targetMonthlyEvent(1, 0,0,0);
		targetMonthlyEvent.listPrevious(13, possibleOccurrences, fromDt);

		for (uint8_t i=0; i<13; i++)
		{
			if (possibleOccurrences[i].month() == (uint8_t)month)
			{
				return possibleOccurrences[i];
			}
		}

		// shouldn't happen
		return DateTime(0);

}

DateTime DateTime::next(Chronos::Named::Month::Month month, const DateTime & fromDt) {
	DateTime possibleOccurrences[13];
	Chronos::Mark::Monthly targetMonthlyEvent(1, 0,0,0);
	targetMonthlyEvent.listNext(13, possibleOccurrences, fromDt);

	for (uint8_t i=0; i<13; i++)
	{
		if (possibleOccurrences[i].month() == (uint8_t)month)
		{
			return possibleOccurrences[i];
		}
	}

	// shouldn't happen
	return DateTime(0);


}

DateTime DateTime::next(Chronos::Weekday::Day aDay, const DateTime & fromDt)
{
	Chronos::Mark::Weekly targetDayEvent(aDay, 0,0,0);
	return targetDayEvent.next(fromDt);
}
DateTime DateTime::next(Chronos::Weekday::Day aDay) const
{
	return next(aDay, *this);
}


DateTime DateTime::next(Chronos::Named::Month::Month month) const
{
	return next(month, *this);

}




DateTime::DateTime(Year y, Month mo, Day d, Hours h, Minutes min, Seconds s)
{
	_elements.Year = CalendarYrToTm(y);
	_elements.Month = (mo <= 12) ? mo : 12;
	_elements.Day = (d <= 31) ? d : 31;
	_elements.Hour = (h <= 23) ? h : 23;
	_elements.Minute = (min <= 59) ? min : 59;
	_elements.Second = (s <= 59) ? s : 59;

	epoch = DATETIME_CONVERT_TIMELEMENTS_TO_EPOCH(_elements);


}

DateTime::DateTime(Chronos::EpochTime atEpochSecs) : epoch(atEpochSecs) {

	DATETIME_INTERNAL_EPOCH_MODIFIED();
}


DateTime::DateTime() : epoch(DATETIME_GET_CURRENT_EPOCH()) {

	DATETIME_INTERNAL_EPOCH_MODIFIED();

}
DateTime::DateTime(const Chronos::TimeElements& atTime) : _elements(atTime) {
	epoch = DATETIME_CONVERT_TIMELEMENTS_TO_EPOCH(_elements);
}

bool DateTime::isWithin(const DateTime::Bounds & bounds) const
{
	return ( (*this) >= bounds.start && (*this) <= bounds.finish);

}

bool DateTime::isWeekend() const
{

	return (getElements().Wday == 1 || getElements().Wday == 7);
}


bool DateTime::sameTimeAs(const DateTime & dt) const
{

	return (getElements().Second == dt.getElements().Second && getElements().Minute == dt.getElements().Minute && getElements().Hour == dt.getElements().Hour);


}
bool DateTime::sameDateAs(const DateTime & dt) const
{

	return (getElements().Day == dt.getElements().Day && getElements().Month == dt.getElements().Month && getElements().Year == dt.getElements().Year);

}

DateTime DateTime::operator-(Chronos::EpochTime numSeconds)  const{

	if (numSeconds >= epoch)
		return DateTime(0);

	return DateTime(epoch - numSeconds);

}

DateTime& DateTime::operator-=(Chronos::EpochTime numSeconds) {

	if (numSeconds >= epoch) {
		epoch = 0;
	} else {
		epoch -= numSeconds;
	}
	DATETIME_INTERNAL_EPOCH_MODIFIED();

	return *this;
}

DateTime DateTime::operator +(Chronos::EpochTime numSeconds)  const {
	return DateTime(epoch + numSeconds);
}

DateTime& DateTime::operator +=(Chronos::EpochTime numSeconds) {

	epoch += numSeconds;

	DATETIME_INTERNAL_EPOCH_MODIFIED();

	return *this;
}


Span::Absolute DateTime::operator-(const DateTime & otherDt) const {

	Chronos::EpochTime deltaSecs = 0;

	// note: shouldn't matter as uint diff should
	// alway be absolute delta in either direction,
	// but this way doesn't rely on any magic.
	if (epoch > otherDt.epoch)
	{
		deltaSecs = epoch - otherDt.epoch;
	} else {
		deltaSecs = otherDt.epoch - epoch;
	}

	return Span::Delta(deltaSecs);


}



DateTime DateTime::operator+(const Span::Delta & delta) const
{
	return (*this) + delta.totalSeconds();

}
DateTime & DateTime::operator+=(const Span::Delta & delta) {
	return ((*this) += delta.totalSeconds());

}
DateTime DateTime::operator-(const Span::Delta & delta)  const {
	return (*this) - delta.totalSeconds();

}
DateTime & DateTime::operator-=(const Span::Delta & delta) {
	return ((*this) -= delta.totalSeconds());

}

void DateTime::reInitEpoch()
{

	epoch = DATETIME_CONVERT_TIMELEMENTS_TO_EPOCH(getElements());

}
void DateTime::setSecond(Seconds s) {

	s = (s <= 59) ? s : 59;


	getElements().Second = s;
	reInitEpoch();
}

void DateTime::setMinute(Minutes m) {

	m = (m <= 59) ? m : 59;


	getElements().Minute = m;
	reInitEpoch();
}

void DateTime::setHour(Hours h) {

	h = (h <= 23) ? h : 23;

	getElements().Hour = h;
	reInitEpoch();
}

void DateTime::setDay(Day d) {

	d = (d <= 31) ? d : 31;

	getElements().Day = d;
	reInitEpoch();
}

void DateTime::setMonth(Month m) {

	m = (m <= 12) ? m : 12;

	getElements().Month = m;
	reInitEpoch();
}

void DateTime::setYear(Year y) {


	getElements().Year = CalendarYrToTm(y);
	reInitEpoch();
}

void DateTime::printTo(Print & p, bool includeTime) const
{

	p.print(monthShortStr(month()));
	p.print(" ");
	p.print((int)getElements().Day);
	p.print(", ");
	p.print(year());

	if (includeTime)
	{
		p.print(" @ ");
		p.print((int)getElements().Hour);
		p.print(':');
		if (getElements().Minute < 10)
		{
			p.print(0L);
		}
		p.print((int)getElements().Minute);

		p.print(':');

		if (getElements().Second < 10)
		{
			p.print(0L);
		}
		p.print((int)getElements().Second);

	}


}

void DateTime::setToStartOfDay() {

	getElements().Second = 0;
	getElements().Minute = 0;
	getElements().Hour = 0;
	reInitEpoch();

}
void DateTime::setToEndOfDay() {

	getElements().Second = 59;
	getElements().Minute = 59;
	getElements().Hour = 23;

	reInitEpoch();

}

DateTime DateTime::startOfDay() const
{

	Chronos::TimeElements sod(getElements());
	sod.Second = 0;
	sod.Minute = 0;
	sod.Hour = 0;

	return DateTime(sod);


}
DateTime DateTime::endOfDay() const {

	Chronos::TimeElements sod(getElements());
	sod.Second = 59;
	sod.Minute = 59;
	sod.Hour = 23;

	return DateTime(sod);
}

DateTime DateTime::next(const Mark::Event & ev) const
{
	return ev.next(*this);
}
DateTime DateTime::previous(const Mark::Event & ev) const
{
	return ev.previous(*this);
}


void DateTime::listNext(uint8_t num, const Mark::Event & ev, DateTime  into[]) const
{
	ev.listNext(num, into, *this);
}
void DateTime::listPrevious(uint8_t num, const Mark::Event & ev, DateTime  into[]) const
{

	ev.listPrevious(num, into, *this);
}


} /* namespace Chronos */

