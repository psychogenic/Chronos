# Chronos
Easily manipulate date-times to perform scheduling, setup calendars and find points in time that are of interest, on pretty much any system (*embedded* or otherwise).

*Chronos* is comprised of three major components: DateTimes and Spans (OO date-time objects that can be manipulated in many ways), Marks (sets of calendar point events like "every Sunday at 15h00") and Calendar Events (actual events with fixed or repeating datetimes of specific duration, and the scheduling functions needed to find/manipulate events of interest).

## API and Examples
Full description, tutorials and APIs are available on the project site:

  http://flyingcarsandstuff.com/projects/chronos/

and the examples directory contains samples that should run as-is on any Arduino or compatible device.

To whet your appetite, a few simple samples:
> // a datetime
>
> Chronos::DateTime inAWhile = Chronos::DateTime::now() + Chronos::Span::Hours(2);
>
> // a little later
>
> inAWhile += Chronos::Span::Minutes(20);
>
> 
> // how long until the following tuesday?
>
> (inAWhile.next(Chronos::Weekday::Tuesday) - inAWhile).days()
>
>
> // add an event to a calendar, my yoga class every monday morning:
>
> MyCalendar.add(
>
>    Chronos::Event(MY_YOGA_CLASS_EVENT_ID,
>
>                Chronos::Mark::Weekly(Chronos::Weekday::Monday, 8, 30),
>
>                Chronos::Span::Hours(1)));
>
>
> // find the next 5 classes, from now
>
> Chronos::Event::Occurrences myClasses[5];
>
> MyCalendar.listNext(5, myClasses, Chronos::DateTime::now());
>
>
> // etc.

## Design
Chronos is cross-platform but specifically designed for use on **embedded systems**, like *Arduino*.  It's priorities are:

* Offering intuitive manipulation of date-times and calendar events
* Minimizing memory usage; and finally
* Performance

So RAM footprint trumps optimization, here.  Still, with my old laptop I can setup a calendar 65k times and locate over 2,000,000 event occurrences in under 15 seconds. A lot less on a ATMega328, of course, but still very usable.

More importantly, you can have a 10 (recurring) event calendar in only 415 bytes. Pretty decent.

## License

Chronos is 
**Copyright (C) 2015-2018 Pat Deegan**
and released under the *LGPL*.

      Chronos is free software: you can redistribute it and/or modify
      it under the terms of the GNU Lesser Public License as published by
      the Free Software Foundation, either version 3 of the License, or
      (at your option) any later version.
  
      Chronos is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU Lesser Public License for more details.
  

