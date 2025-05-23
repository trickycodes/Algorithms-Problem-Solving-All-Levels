﻿#include <iostream>
#include "../mylibs/headers/date_utils.h"

// stDate is defined in date_utils.h file

using namespace std;
using date_utils::stDate;

stDate getDateFromDayOrderInYear(short dateOrderInYear, short year)
{
	stDate date;
	short remainingDays = dateOrderInYear;
	short monthDays = 0;

	date.year = year;
	date.month = 1;

	while (true)
	{
		monthDays = date_utils::numberOfDaysInAMonth(year, date.month);

		if (remainingDays > monthDays)
		{
			// اذا كان الايام المتبقية اكبر من ايام الشهر في اللوب فاطرحه وزود عداد الشهر واحد
			remainingDays -= monthDays;
			date.month++;
		}
		else
		{
			date.day = remainingDays; // assigns the left over days to day variable.
			break;
		}
			
	}

	return date;
}

int main()
{
	short day, month, year;

	day = date_utils::readDay();
	month = date_utils::readMonth();
	year = date_utils::readYear();

	short totalDays = date_utils::numberOfDaysFromBeginningOfTheYear(year, month, day);

	cout << "Number of days from the beginning of the year is: " << totalDays << endl;

	getDateFromDayOrderInYear(year, totalDays);

	return 0;
}