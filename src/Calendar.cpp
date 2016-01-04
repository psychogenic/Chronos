/*
 * Calendar.cpp
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

#include "chronosinc/schedule/Calendar.h"
#include "chronosinc/platform/platform.h"
#include "chronosinc/Sort.h"
namespace Chronos {


Calendar::Calendar(uint8_t maxEvents) : num_events(0), max_events(maxEvents), num_recurring(0){


}

void Calendar::clear() {

	if (! num_events)
		return;

	CHRONOS_DEBUG_OUTLN("Clearing all calendar events.");
	Chronos::Event emptyEvent;
	for (uint8_t i=0; i< num_events; i++)
	{
		Chronos::Event * evt = this->eventSlot(i);
		if (evt)
		{
			*evt = emptyEvent;
		}

	}
	num_events = 0;
	num_recurring = 0;

}
bool Calendar::remove(EventID evId)
{
	bool foundIt = false;
	uint8_t pos;

	if (evId <= EVENTID_NOTSET)
		return false;

	for (pos=0; pos<num_events; pos++)
	{
		Chronos::Event * evt = this->eventSlot(pos);
		if (evt && evt->id() == evId)
		{
			evt->reset();
			foundIt = true;
			break;
		}
	}

	if (! foundIt)
		return false;

	if (pos < (num_events - 1)) {
		// need to shift others back
		for (uint8_t i = pos; i < (num_events - 1); i++) {
			Chronos::Event * emptySlot = this->eventSlot(i);
			Chronos::Event * slotToMove = this->eventSlot(i + 1);

			if (emptySlot && slotToMove) {
				*emptySlot = *slotToMove;
				slotToMove->reset();
			}
		}

	}
	// no matter what, we now have one less...
	num_events--;


	return foundIt;
}
bool Calendar::add(const Chronos::Event & event)
{
	if (num_events >= max_events)
		return false;

	Chronos::Event * evt = this->eventSlot(num_events);

	if (NULL == evt)
		return false;

	num_events++;
	if (event.isRecurring())
	{
		num_recurring++;

	}

	CHRONOS_DEBUG_OUT("Assigning event ");
	CHRONOS_DEBUG_OUTHEX(&event);
	CHRONOS_DEBUG_OUT(" to slot ");
	CHRONOS_DEBUG_OUTINT(num_events);
	CHRONOS_DEBUG_OUTLN("!");

	*evt = event;

	return true;


}


#ifdef PLATFORM_SUPPORTS_RVAL_MOVE
bool Calendar::add(Chronos::Event&& event)
{

	if (num_events >= max_events)
		return false;

	Chronos::Event * evt = this->eventSlot(num_events);

	if (NULL == evt)
		return false;

	num_events++;
	if (event.isRecurring())
	{
		num_recurring++;

	}
	CHRONOS_DEBUG_OUT("Moving event ");
	CHRONOS_DEBUG_OUTHEX(&event);
	CHRONOS_DEBUG_OUT(" to slot ");
	CHRONOS_DEBUG_OUTINT(num_events);
	CHRONOS_DEBUG_OUTLN("!");


	*evt = event;

	return true;
}
#endif

uint8_t Calendar::listNext(uint8_t number, Event::Occurrence into[], const DateTime & dt)
{
	uint8_t ev=0;
	uint8_t addedIdx = 0;

	{
		Chronos::DateTime farFuture(Chronos::DateTime::endOfTime());
		Event::Occurrence dummy(EVENTID_NOTSET, farFuture, farFuture);
		for (uint8_t i=0; i<number;i++)
			into[i]=dummy;
	}

	while (ev < num_events)
	{

		Chronos::Event * evt = this->eventSlot(ev++);

		if (NULL == evt)
			continue;

		if (! evt->hasNext(dt))
		{
			continue;
		}

		uint8_t maxNumToAdd = evt->isRecurring() ? number : 1;
		bool didInsert = false;

		DateTime recStartDt(dt);
		for (uint8_t addNum=0; addNum<maxNumToAdd; addNum++ )
		{

			Event::Occurrence occ(evt->nextOccurrence(recStartDt));
			for (int16_t i=number; i>=0; i--)
			{

				if (occ.start < into[i].start)
				{
					// ok, this event bumps one from our return array,
					// we place it at the end of the array, as we know
					// that guy'll be bumped out
					into[number-1] = occ;


					// now we have a mostly-sorted array where all elements
					// are <= to the ones to their right, except (possibly)
					// for our new guy, e.g.
					// [1,1,2,3,4,4 ... 20, NEWGUY]

					// what we need is to move him left until we can't anymore
					Chronos::Sort::leftSortLastElement(into, number);

					// Chronos::Sort::bubble(into, number);
					didInsert = true;
					break;
				}
			}

			if (! didInsert)
			{
				// all future occurrences won't fit either
				break;
			}

			recStartDt = occ.start + Chronos::Span::Seconds(1);

		}

	}

	while (addedIdx < number)
	{
		if (into[addedIdx].id == EVENTID_NOTSET)
		{
			break;
		}


		addedIdx++;
	}
	return addedIdx;


}
bool Calendar::nextDateTimeOfInterest(const DateTime & fromDT, DateTime & returnDT)
{

	if (! num_events)
		return false;

	Event::Occurrence * eventOccs = new Event::Occurrence[max_events];
	if (! eventOccs)
	{
		CHRONOS_DEBUG_OUTLN("nextDateTimeOfInterest: Couldn't allocate space for ongoing list?");
		return false;
	}

	returnDT = DateTime::endOfTime(); // arbitrary far future date
	uint8_t numOngoing = listOngoing(max_events, eventOccs, fromDT);


	bool foundSomething = false;
	for (uint8_t i=0; i<numOngoing; i++)
	{
		// these are on-going, so happening right now... we keep the end datetimes
		if (eventOccs[i].finish < returnDT)
		{
			// is closer, we have a potential winner
			returnDT = eventOccs[i].finish;
			foundSomething = true;
		}
	}


	uint8_t numNext = listNext(max_events, eventOccs, fromDT);


	for (uint8_t i=0; i<numNext; i++)
	{
		// there are coming up, we keep the start datetimes which are in the future
		if (eventOccs[i].start < returnDT)
		{
			// is closer, we have a potential winner
			returnDT = eventOccs[i].start;
			foundSomething = true;
		}
	}

	delete [] eventOccs;
	return foundSomething;

#if 0

	Event::Occurrence * ongoingEvents = new Event::Occurrence[max_events];
	if (! ongoingEvents)
	{
		CHRONOS_DEBUG_OUTLN("nextDateTimeOfInterest: Couldn't allocate space for ongoing list?");
		return false;
	}

	Event::Occurrence * nextEvents = new Event::Occurrence[max_events];
	if (! nextEvents)
	{
		delete [] ongoingEvents;
		CHRONOS_DEBUG_OUTLN("nextDateTimeOfInterest: Couldn't allocate space for next list?");
		return false;


	}


	uint8_t numOngoing = listOngoing(max_events, ongoingEvents, fromDT);
	uint8_t numNext = listNext(max_events, nextEvents, fromDT);

	if (! (numNext || numOngoing))
	{
		// nothing going on, nothing upcoming...
		delete [] ongoingEvents;
		delete [] nextEvents;
		return false;
	}

	DateTime * dtOfInterest = new DateTime[numOngoing + numNext];
	if (! dtOfInterest)
	{
		CHRONOS_DEBUG_OUTLN("nextDateTimeOfInterest: Couldn't allocate space for DT list?");
		delete [] ongoingEvents;
		delete [] nextEvents;
		return false;

	}

	uint8_t dtCount = 0;
	for (uint8_t i=0; i<numOngoing; i++)
	{
		// these are on-going, so happening right now... we keep the end datetimes
		dtOfInterest[dtCount++] = ongoingEvents[i].finish;
	}

	for (uint8_t i=0; i<numNext; i++)
	{
		// there are coming up, we keep the start datetimes which are in the future
		dtOfInterest[dtCount++] = nextEvents[i].start;
	}

	// now we sort the list
	Chronos::Sort::bubble(dtOfInterest, dtCount);

	returnDT = dtOfInterest[0]; // first in list is smallest datetime

	// clear our lists
	delete [] ongoingEvents;
	delete [] nextEvents;
	delete [] dtOfInterest;

	return true;
#endif


}
uint8_t Calendar::listOngoing(uint8_t number, Event::Occurrence into[], const DateTime & dt)
{

	uint8_t ev=0;
	uint8_t addedIdx = 0;
	while (ev<num_events && addedIdx < number)
	{


		Chronos::Event * evt = this->eventSlot(ev++);

		if (NULL == evt)
			continue;

		Event::Occurrence occ = evt->closestOccurrence(dt);
		if (occ.isOngoing)
		{
			into[addedIdx++] = occ;
		}

	}

	Chronos::Sort::bubble(into, addedIdx);
	return addedIdx;
}

uint8_t Calendar::listForDay(uint8_t maxNumber, Event::Occurrence intoArray[], const DateTime & dt)
{
	DateTime startOfDay = dt.startOfDay();
	DateTime justBeforeDayStart = startOfDay - Chronos::Span::Seconds(1);
	// Event::Occurrence * allVals = new Event::Occurrence[maxNumber];
	/*if (! allVals)
	{
		CHRONOS_DEBUG_OUTLN("listForDay: Couldn't allocate space for list?");
		return 0;
	}
	*/


	uint8_t numNext = listNext(maxNumber, intoArray, justBeforeDayStart);
	uint8_t numFound = 0;
	for (uint8_t i=0; i< numNext; i++)
	{

		if (intoArray[i].start.sameDateAs(startOfDay))
		{
			numFound++;
		}
	}

	return numFound;

}

} /* namespace Chronos */
