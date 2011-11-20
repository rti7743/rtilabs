// RDateConv.cpp: RDateConv クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////
/* ====================================================================
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2000-2002 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Apache" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
/*
	ここにかかれているソースコードは apache2 よりコピペした部分です。
	この部分に関しては apache2 のライセンス規約に従って再配布してください。

	rti.
*/



#include "RDateConv.h"
#include "RAlphabetHash.h"
#define BAD_DATE	-1
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
/**
 * tm2time:			tm 型を time_t 型に変換します。
 *
 * @param when		変換されるヤツ(tm_sec,tm_min,tm_hour,tm_mday,tm_mon,tm_yearのみ参照)
 * @return time_t	UNIX時間
 */
time_t RDateConv::tm2time(struct tm* when)
{
	//その月の残り日数
	static int LeftDay[] = 
	{
		(0),														//1
		(31),														//2
		(31 + 28),													//3
		(31 + 28 + 31),												//4
		(31 + 28 + 31 + 30),										//5
		(31 + 28 + 31 + 30 + 31),									//6
		(31 + 28 + 31 + 30 + 31 + 30),								//7
		(31 + 28 + 31 + 30 + 31 + 30 + 31),							//8
		(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31),					//9
		(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30),				//10
		(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31),			//11
		(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30),		//12
	};

	int year = when->tm_year;
    /* shift new year to 1st March in order to make leap year calc easy */
    if (when->tm_mon < 2)
	year--;

	//うるう年を踏まえて、日数を計算します。
	time_t  day  = year *365 +  (4 - year / 100 + (year / 100 + 3) / 4);
	//月を加えます。
	day += LeftDay[when->tm_mon] + when->tm_mday-1 ;
//    day -= 25508;		/* 1 jan 1970 is 25508 days since 1 mar 1900 */
	//time_t 型に変更します。
	return day * (24  * 60 * 60) + when->tm_hour * (60 * 60) + when->tm_min * (60) + when->tm_sec + (60 * 60 * 9) ;
}


/*
 * Compare a string to a mask
 * Mask characters (arbitrary maximum is 256 characters, just in case):
 *   @ - uppercase letter
 *   $ - lowercase letter
 *   & - hex digit
 *   # - digit
 *   ~ - digit or space
 *   * - swallow remaining characters 
 *  <x> - exact match for any other character
 */
int RDateConv::ap_checkmask(const char *data, const char *mask)
{
    int i;
    char d;

    for (i = 0; i < 256; i++) {
	d = data[i];
	switch (mask[i]) {
	case '\0':
	    return (d == '\0');

	case '*':
	    return 1;

	case '@':
	    if (!isupper(d))
		return 0;
	    break;
	case '$':
	    if (!islower(d))
		return 0;
	    break;
	case '#':
	    if (!isdigit(d))
		return 0;
	    break;
	case '&':
	    if (!isxdigit(d))
		return 0;
	    break;
	case '~':
	    if ((d != ' ') && !isdigit(d))
		return 0;
	    break;
	default:
	    if (mask[i] != d)
		return 0;
	    break;
	}
    }
    return 0;			/* We only get here if mask is corrupted (exceeds 256) */
}

/*
 * tm2sec converts a GMT tm structure into the number of seconds since
 * 1st January 1970 UT.  Note that we ignore tm_wday, tm_yday, and tm_dst.
 * 
 * The return value is always a valid time_t value -- (time_t)0 is returned
 * if the input date is outside that capable of being represented by time(),
 * i.e., before Thu, 01 Jan 1970 00:00:00 for all systems and 
 * beyond 2038 for 32bit systems.
 *
 * This routine is intended to be very fast, much faster than mktime().
 */
time_t RDateConv::ap_tm2sec(const struct tm * t)
{
    int year;
    time_t days;
    static const int dayoffset[12] =
    {306, 337, 0, 31, 61, 92, 122, 153, 184, 214, 245, 275};

    year = t->tm_year;

    if (year < 70 || ((sizeof(time_t) <= 4) && (year >= 138)))
	return BAD_DATE;

    /* shift new year to 1st March in order to make leap year calc easy */

    if (t->tm_mon < 2)
	year--;

    /* Find number of days since 1st March 1900 (in the Gregorian calendar). */

    days = year * 365 + year / 4 - year / 100 + (year / 100 + 3) / 4;
    days += dayoffset[t->tm_mon] + t->tm_mday - 1;
    days -= 25508;		/* 1 jan 1970 is 25508 days since 1 mar 1900 */

    days = ((days * 24 + t->tm_hour) * 60 + t->tm_min) * 60 + t->tm_sec;

    if (days < 0)
	return BAD_DATE;	/* must have overflowed */
    else
	return days;		/* must be a valid time */
}

/*
 * Parses an HTTP date in one of three standard forms:
 *
 *     Sun, 06 Nov 1994 08:49:37 GMT  ; RFC 822, updated by RFC 1123
 *     Sunday, 06-Nov-94 08:49:37 GMT ; RFC 850, obsoleted by RFC 1036
 *     Sun Nov  6 08:49:37 1994       ; ANSI C's asctime() format
 *
 * and returns the time_t number of seconds since 1 Jan 1970 GMT, or
 * 0 if this would be out of range or if the date is invalid.
 *
 * The restricted HTTP syntax is
 * 
 *     HTTP-date    = rfc1123-date | rfc850-date | asctime-date
 *
 *     rfc1123-date = wkday "," SP date1 SP time SP "GMT"
 *     rfc850-date  = weekday "," SP date2 SP time SP "GMT"
 *     asctime-date = wkday SP date3 SP time SP 4DIGIT
 *
 *     date1        = 2DIGIT SP month SP 4DIGIT
 *                    ; day month year (e.g., 02 Jun 1982)
 *     date2        = 2DIGIT "-" month "-" 2DIGIT
 *                    ; day-month-year (e.g., 02-Jun-82)
 *     date3        = month SP ( 2DIGIT | ( SP 1DIGIT ))
 *                    ; month day (e.g., Jun  2)
 *
 *     time         = 2DIGIT ":" 2DIGIT ":" 2DIGIT
 *                    ; 00:00:00 - 23:59:59
 *
 *     wkday        = "Mon" | "Tue" | "Wed"
 *                  | "Thu" | "Fri" | "Sat" | "Sun"
 *
 *     weekday      = "Monday" | "Tuesday" | "Wednesday"
 *                  | "Thursday" | "Friday" | "Saturday" | "Sunday"
 *
 *     month        = "Jan" | "Feb" | "Mar" | "Apr"
 *                  | "May" | "Jun" | "Jul" | "Aug"
 *                  | "Sep" | "Oct" | "Nov" | "Dec"
 *
 * However, for the sake of robustness (and Netscapeness), we ignore the
 * weekday and anything after the time field (including the timezone).
 *
 * This routine is intended to be very fast; 10x faster than using sscanf.
 *
 * Originally from Andrew Daviel <andrew@vancouver-webpages.com>, 29 Jul 96
 * but many changes since then.
 *
 */

//Http時間を Unix時間に変換.
time_t RDateConv::HttpTimeToUnixTime(const char* date)
{
    struct tm ds;
    int mint, mon;
    const char *monstr, *timstr;
    static const int months[12] =
    {
	('J' << 16) | ('a' << 8) | 'n', ('F' << 16) | ('e' << 8) | 'b',
	('M' << 16) | ('a' << 8) | 'r', ('A' << 16) | ('p' << 8) | 'r',
	('M' << 16) | ('a' << 8) | 'y', ('J' << 16) | ('u' << 8) | 'n',
	('J' << 16) | ('u' << 8) | 'l', ('A' << 16) | ('u' << 8) | 'g',
	('S' << 16) | ('e' << 8) | 'p', ('O' << 16) | ('c' << 8) | 't',
	('N' << 16) | ('o' << 8) | 'v', ('D' << 16) | ('e' << 8) | 'c'};

    if (!date)
	return BAD_DATE;

    while (*date && isspace(*date))	/* Find first non-whitespace char */
	++date;

    if (*date == '\0')
	return BAD_DATE;

    if ((date = strchr(date, ' ')) == NULL)	/* Find space after weekday */
	return BAD_DATE;

    ++date;			/* Now pointing to first char after space, which should be */
    /* start of the actual date information for all 3 formats. */

    if (ap_checkmask(date, "## @$$ #### ##:##:## *")) {	/* RFC 1123 format */
	ds.tm_year = ((date[7] - '0') * 10 + (date[8] - '0') - 19) * 100;
	if (ds.tm_year < 0)
	    return BAD_DATE;

	ds.tm_year += ((date[9] - '0') * 10) + (date[10] - '0');

	ds.tm_mday = ((date[0] - '0') * 10) + (date[1] - '0');

	monstr = date + 3;
	timstr = date + 12;
    }
    else if (ap_checkmask(date, "##-@$$-## ##:##:## *")) {		/* RFC 850 format  */
	ds.tm_year = ((date[7] - '0') * 10) + (date[8] - '0');
	if (ds.tm_year < 70)
	    ds.tm_year += 100;

	ds.tm_mday = ((date[0] - '0') * 10) + (date[1] - '0');

	monstr = date + 3;
	timstr = date + 10;
    }
    else if (ap_checkmask(date, "@$$ ~# ##:##:## ####*")) {	/* asctime format  */
	ds.tm_year = ((date[16] - '0') * 10 + (date[17] - '0') - 19) * 100;
	if (ds.tm_year < 0)
	    return BAD_DATE;

	ds.tm_year += ((date[18] - '0') * 10) + (date[19] - '0');

	if (date[4] == ' ')
	    ds.tm_mday = 0;
	else
	    ds.tm_mday = (date[4] - '0') * 10;

	ds.tm_mday += (date[5] - '0');

	monstr = date;
	timstr = date + 7;
    }
    else
	return BAD_DATE;

    if (ds.tm_mday <= 0 || ds.tm_mday > 31)
	return BAD_DATE;

    ds.tm_hour = ((timstr[0] - '0') * 10) + (timstr[1] - '0');
    ds.tm_min = ((timstr[3] - '0') * 10) + (timstr[4] - '0');
    ds.tm_sec = ((timstr[6] - '0') * 10) + (timstr[7] - '0');

    if ((ds.tm_hour > 23) || (ds.tm_min > 59) || (ds.tm_sec > 61))
	return BAD_DATE;

    mint = (monstr[0] << 16) | (monstr[1] << 8) | monstr[2];
    for (mon = 0; mon < 12; mon++)
	if (mint == months[mon])
	    break;
    if (mon == 12)
	return BAD_DATE;

    if ((ds.tm_mday == 31) && (mon == 3 || mon == 5 || mon == 8 || mon == 10))
	return BAD_DATE;

    /* February gets special check for leapyear */

    if ((mon == 1) &&
	((ds.tm_mday > 29)
	 || ((ds.tm_mday == 29)
	     && ((ds.tm_year & 3)
		 || (((ds.tm_year % 100) == 0)
		     && (((ds.tm_year % 400) != 100)))))))
	return BAD_DATE;

    ds.tm_mon = mon;

    return ap_tm2sec(&ds);
}
///////////////////////////////////////////////apache 2 コピペ 終了 ////////////////////////////////////////////////////

/**
 * UnixTimeToHttpTime:		Unix時間を Http 時間に変換.RFC 1123 (RFC 822) に従う
 *
 * @param inTime			Unix時間
 * @return string			RFC 1123 (RFC 822)により変換した時間.
 */
string RDateConv::UnixTimeToHttpTime(time_t inTime)
{
	static const char*	day[] = {	"Sun" , "Mon" , "Tue" , "Wed" , "Thu" , "Fri" , "Sat" };
	static const char*	month[] = {	"Jan" , "Feb" , "Mar" , "Apr" , "May" , "Jun" , "Jul" , "Aug" , "Sep" , "Oct" , "Nov" , "Dec" };

	//じゃぱにーず.
	inTime -= (60 * 60 * 9);

	//Wed, 04 Dec 2002 15:35:06 GMT

	char buf [30 + 1];
	struct tm *today = localtime( &inTime );
	_snprintf(buf , 30 , "%s, %02d %s %04d %02d:%02d:%02d GMT" , day[today->tm_wday] , today->tm_mday , month[today->tm_mon] , 1900 + today->tm_year , today->tm_hour , today->tm_min , today->tm_sec);

	return buf;
}

/**
 * UnixTime2chTime:			Unix時間を 2002/11/14 00:55 形式に文字列として変換します
 *
 * @param inTime			Unix時間
 * @return string			2002/11/14 00:55 形式に文字列として変換します
 */
string RDateConv::UnixTime2chTime(time_t inTime)
{
	char buf [30 + 1];
	struct tm *today = localtime( &inTime );
	_snprintf(buf , 30 , "%04d/%02d/%02d %02d:%02d" , 
			1900 + today->tm_year , today->tm_mon + 1 , today->tm_mday , today->tm_hour , today->tm_min);

	return buf;
}

void RDateConv::test()
{
	{
		string testdate = "Wed, 04 Dec 2002 15:35:06 GMT";
		time_t unix = HttpTimeToUnixTime(testdate.c_str());
		string time = UnixTimeToHttpTime(unix);
		ASSERT( time == testdate );
	}
	{
		string testdate = "Thu, 04 Dec 1980 15:35:06 GMT";
		time_t unix = HttpTimeToUnixTime(testdate.c_str());
		string time = UnixTimeToHttpTime(unix);
		ASSERT( time == testdate );
	}
	{
		time_t unix = time(NULL);
		string time = UnixTimeToHttpTime(unix);
		string time2 = UnixTimeToHttpTime(HttpTimeToUnixTime( time.c_str() ));
		ASSERT( time == time2 );
	}
	puts("RDateConv::test() OK");
}

