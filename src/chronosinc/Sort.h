/*
 * Sort.h
 * A few generic sorting algorithms, used internally.
 *
 *  http://flyingcarsandstuff.com/projects/chronos
 *  Created on: Dec 21, 2015
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

#ifndef CHRONOS_INTINCLUDES_SORT_H_

#define CHRONOS_INTINCLUDES_SORT_H_
#include "../chronosinc/timeExtInc.h"

namespace Chronos {

namespace Sort {

template<class T>
void bubble(T a[], uint8_t n)
{
    uint8_t i, j;
    if (n < 2)
    	return;

    for(i=0;i<n-1;i++)
    {
        for(j=i+1;j<n;j++)
        {
            if(a[i]>a[j])
            {
                T element(a[i]);
                a[i] = a[j];
                a[j] = element;
            }
        }
    }
}

template<class T>
void leftSortLastElement(T presorted[], uint8_t n)
{
    uint8_t i, j;
    if (n < 2)
    	return;

    for (i=n-1; i>0; i--)
    {
    	j = i - 1;
    	if (presorted[j] > presorted[i])
    	{
    		// our target element is smaller than the
			// one to its left... movit
    		T element(presorted[i]);
    		presorted[i] = presorted[j];
    		presorted[j] = element;
    	} else {
    		// the element we've been moving is now
    		// > than the element to it's left... we are done
    		return;
    	}
    }

    return;
}


} /* namespace Sort */
} /* namespace Chronos */



#endif /* CHRONOS_INTINCLUDES_SORT_H_ */
