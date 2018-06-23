char strBuffer[128];
typedef long time_t;
struct tm
{
    int tm_sec;    // seconds after the minute (0-59)
    int tm_min;    // minutes after the hour (0-59)
    int tm_hour;   // hours since midnight (0-23)
    int tm_mday;   // day of the month (1-31)
    int tm_mon;    // months since January (0-11)
    int tm_year;   // years since 1900
    int tm_wday;   // days since Sunday (0-6)
    int tm_yday;   // days since January 1 (0-365)
    int tm_isdst;  // Daylight Saving Time flag (=0 not in effect, >0 if in effect)
};
 
int GetFormatLength(char cCode)
{
    if (cCode == 'd') { return 2; }
    if (cCode == 'm') { return 2; }
    if (cCode == 'b') { return 3; }
    if (cCode == 'Y') { return 4; }
    if (cCode == 'H') { return 2; }
    if (cCode == 'M') { return 2; }
    if (cCode == 'S') { return 2; }
 
    return 0;
}
 
int GetMonthPosition(char *strMonth)
{
    if (strcmp (strMonth, "Jan") == 0) { return  0; }
    if (strcmp (strMonth, "Feb") == 0) { return  1; }
    if (strcmp (strMonth, "Mar") == 0) { return  2; }
    if (strcmp (strMonth, "Apr") == 0) { return  3; }
    if (strcmp (strMonth, "May") == 0) { return  4; }
    if (strcmp (strMonth, "Jun") == 0) { return  5; }
    if (strcmp (strMonth, "Jul") == 0) { return  6; }
    if (strcmp (strMonth, "Aug") == 0) { return  7; }
    if (strcmp (strMonth, "Sep") == 0) { return  8; }
    if (strcmp (strMonth, "Oct") == 0) { return  9; }
    if (strcmp (strMonth, "Nov") == 0) { return 10; }
    if (strcmp (strMonth, "Dec") == 0) { return 11; }
 
    return 0;
}
 
long ChangeDateTime (char *strDate,
                     char *strNewDateParam,
                     char *strFormat,
                     char charType,
                     int iChange)
{
    time_t t;
    struct tm *tmNewDate;
    int i = 0;
    int j = 0;
    char cFormatCode;
    int iLen;
    int iFormatLen;
 
    // -------------------------------------------------------------------------
 
    iLen = strlen (strFormat);
 
    // -------------------------------------------------------------------------
    // The current date/time will be used if any date/time values are missing in
    // the strDate string.
    // -------------------------------------------------------------------------
      
    time (&t);
    tmNewDate = (struct tm *) localtime (&t);
 
    // -------------------------------------------------------------------------
    // Fill the tmNewDate structure with the date given.
    // Parse the date based on the date format.  For each format code found, set
    // the corresponding tmNewDate value.
    // -------------------------------------------------------------------------
 
    while (j<iLen)
    {
        if (strFormat[j++] == '%')
        {
            // -----------------------------------------------------------------
            // Assume format codes are 2 characters in length, with the first
            // character being the % character.  The format code tells us how
            // many characters it will represent in the date.
            // -----------------------------------------------------------------
             
            cFormatCode = (char)strFormat[j++];
            iFormatLen = GetFormatLength(cFormatCode);
 
            strncpy (strBuffer, &strDate[i], iFormatLen);
            strBuffer[iFormatLen] = 0;
            i = i + iFormatLen;
 
            switch (cFormatCode)
            {
                case 'Y': tmNewDate->tm_year = atoi (strBuffer) - 1900;     break;
                case 'm': tmNewDate->tm_mon  = atoi (strBuffer) - 1;        break;
                case 'b': tmNewDate->tm_mon  = GetMonthPosition(strBuffer); break;
                case 'd': tmNewDate->tm_mday = atoi (strBuffer);            break;
                case 'H': tmNewDate->tm_hour = atoi (strBuffer);            break;
                case 'M': tmNewDate->tm_min  = atoi (strBuffer);            break;
                case 'S': tmNewDate->tm_sec  = atoi (strBuffer);            break;
 
                default:
                    lr_error_message ("Unknown date format code : %c", cFormatCode);
                    lr_save_string (strDate, strNewDateParam);
                    return -1;
            }
        }
        else
        {
            // -----------------------------------------------------------------
            // Not a format code, plain text, nothing to do.
            // -----------------------------------------------------------------
 
            i++;    // Move to next position in the date.
        }
    }
 
    // -------------------------------------------------------------------------
    // Change the requested date/time part.
    // -------------------------------------------------------------------------
 
    switch (charType)
    {
        case 'Y': tmNewDate->tm_year = tmNewDate->tm_year + iChange; break;
        case 'm': tmNewDate->tm_mon  = tmNewDate->tm_mon  + iChange; break;
        case 'b': tmNewDate->tm_mon  = tmNewDate->tm_mon  + iChange; break;
        case 'd': tmNewDate->tm_mday = tmNewDate->tm_mday + iChange; break;
        case 'H': tmNewDate->tm_hour = tmNewDate->tm_hour + iChange; break;
        case 'M': tmNewDate->tm_min  = tmNewDate->tm_min  + iChange; break;
        case 'S': tmNewDate->tm_sec  = tmNewDate->tm_sec  + iChange; break;
 
        default:
            lr_error_message ("Unknown date code for change : %c", charType);
            lr_save_string (strDate, strNewDateParam);
            return -1;
    }
 
    // -------------------------------------------------------------------------
    // Call mktime to generate the resulting date, format and save to parameter.
    // -------------------------------------------------------------------------
 
    t = mktime (tmNewDate);
    strftime (strBuffer, 128, strFormat, tmNewDate);
    lr_save_string (strBuffer, strNewDateParam);
 
    // -------------------------------------------------------------------------
 

// ACTION.C EXAMPLE
//    // -------------------------------------------------------------------------
//    // Date from server simulation
//    // -------------------------------------------------------------------------
// 
//    long epoch;
//    char *strServerDate = "17-Aug-2012 06:49:41";
//    char *strServerDate2 = "2012-06-04 10:52:32";
//    lr_output_message ("Server Date = %s", strServerDate);
//    lr_output_message ("Server Date 2 = %s", strServerDate2);
// 
//    // -------------------------------------------------------------------------
//    // Add 1 minute to the returned server date.
//    // -------------------------------------------------------------------------
// 
//    lr_output_message (" ");
// 
//    ChangeDateTime (strServerDate, "pServerNewDate", "%d-%b-%Y %H:%M:%S", 'M', 1);
//    lr_output_message ("Add 1 Minute = %s", lr_eval_string ("{pServerNewDate}"));
// 
//    // -------------------------------------------------------------------------
//    // Add 70 to each of the date/time components.  The mktime function will
//    // automatically recalculate to the correct date/time.
//    // -------------------------------------------------------------------------
// 
//    lr_output_message (" ");
// 
//    ChangeDateTime (strServerDate, "pServerNewDate", "%d-%b-%Y %H:%M:%S", 'Y', 70);
//    lr_output_message ("Add 70 Years   = %s", lr_eval_string ("{pServerNewDate}"));
// 
//    ChangeDateTime (strServerDate, "pServerNewDate", "%d-%b-%Y %H:%M:%S", 'b', 70);
//    lr_output_message ("Add 70 Months  = %s", lr_eval_string ("{pServerNewDate}"));
// 
//    ChangeDateTime (strServerDate, "pServerNewDate", "%d-%b-%Y %H:%M:%S", 'd', 70);
//    lr_output_message ("Add 70 Days    = %s", lr_eval_string ("{pServerNewDate}"));
// 
//    ChangeDateTime (strServerDate, "pServerNewDate", "%d-%b-%Y %H:%M:%S", 'H', 70);
//    lr_output_message ("Add 70 Hours   = %s", lr_eval_string ("{pServerNewDate}"));
// 
//    ChangeDateTime (strServerDate, "pServerNewDate", "%d-%b-%Y %H:%M:%S", 'M', 70);
//    lr_output_message ("Add 70 Minutes = %s", lr_eval_string ("{pServerNewDate}"));
// 
//    ChangeDateTime (strServerDate, "pServerNewDate", "%d-%b-%Y %H:%M:%S", 'S', 70);
//    lr_output_message ("Add 70 Seconds = %s", lr_eval_string ("{pServerNewDate}"));
// 
//    // -------------------------------------------------------------------------
//    // Convert the given date to its epoch value.  Epoch is the number of
//    // seconds since 01/01/1970 00:00:00.  
//    // -------------------------------------------------------------------------
// 
//    epoch = ChangeDateTime (strServerDate2, "pServerNewDate", "%Y-%m-%d %H:%M:%S", 'S',0);
//    lr_output_message ("Epoch of %s = %ld", lr_eval_string ("{pServerNewDate}"), epoch);
// 
//    // -------------------------------------------------------------------------

    
    return (long) t;
}