/* $Id: //devel/FileSetDateTime/FileSetDateTime.cpp#1 $ */


/* Note:  see GNU software license in the source code below! */


/* FileSetDateTime was written because I couldn't find a decent 'touch'
 * utility on http://hobbes.nmsu.edu/ when I needed one...  It is very
 * simple -- no wildcards accepted, and the date and the time must be
 * set at the same time.
 *
 * Usage:
 *
 *      FileSetDateTime <date> <time> <filename>
 *
 * where all parameters are required, and must be specified in the following
 * format:
 *
 *      date == yyyy-mm-dd
 *
 *      time == hh:mm:ss    (in 24-hour format)
 *
 *      filename must be double-quote enclosed if it contains spaces
 *
 * example:
 *
 *      filesetdatetime 2001-12-29 18:45:32 myfile.txt
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INCL_DOSFILEMGR // DosQueryPathInfo, DosSetPathInfo
#include <os2.h>


int main(int argC, char *argV[])
{
    if(argC != 4)
    {
        printf( "\n"
                "FileSetDateTime for OS/2\n"
                "Version: $Id: //devel/FileSetDateTime/FileSetDateTime.cpp#1 $\n"
                "E-mail: charette@writeme.com\n"
                "\n"
                "-----------------------------------------------------------------\n"
                "Copyright (C) 2001 Stephane Charette\n"
                "\n"
                "This program is free software; you can redistribute it and/or\n"
                "modify it under the terms of the GNU General Public License\n"
                "as published by the Free Software Foundation; either version 2\n"
                "of the License, or (at your option) any later version.\n"
                "\n"
                "This program is distributed in the hope that it will be useful,\n"
                "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
                "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
                "GNU General Public License for more details.\n"
                "\n"
                "You should have received a copy of the GNU General Public License\n"
                "along with this program.\n"
                "-----------------------------------------------------------------\n"
                "\n"
                "Must specify 3 parameters:  <date> <time> <filename>\a\n");
        return 16;
    }

    if(strlen(argV[1]) != 10)
    {
        printf("\nDate must be in YYYY-MM-DD format!\a\n");
        return 1;
    }

    if(strlen(argV[2]) != 8)
    {
        printf("\nTime must be in HH:MM:SS format!\a\n");
        return 2;
    }

    /* get the current file information */
    FILESTATUS3 fileStatus = {0};
    APIRET rc = DosQueryPathInfo(argV[3], FIL_STANDARD, &fileStatus, sizeof(fileStatus));
    if(rc != 0)
    {
        printf( "\nERROR: DosQueryPathInfo() returned rc==%d!\n"
                "(Try \"help sys%04d\" for more information.)\a\n", rc, rc);
        return rc;
    }

    /* overwrite the date/time with the parameters supplied */

    char buffer[5] = {0};
    int i;

    /* determine the DAY */
    strncpy(buffer, &argV[1][8], 2);
    buffer[2] = '\0';
    i = atoi(buffer);

    fileStatus.fdateCreation    .day        = i;
    fileStatus.fdateLastAccess  .day        = i;
    fileStatus.fdateLastWrite   .day        = i;

    /* determine the MONTH */
    strncpy(buffer, &argV[1][5], 2);
    buffer[2] = '\0';
    i = atoi(buffer);

    fileStatus.fdateCreation    .month      = i;
    fileStatus.fdateLastAccess  .month      = i;
    fileStatus.fdateLastWrite   .month      = i;

    /* determine the YEAR */
    strncpy(buffer, &argV[1][0], 4);
    buffer[4] = '\0';
    i = atoi(buffer) - 1980;

    fileStatus.fdateCreation    .year       = i;
    fileStatus.fdateLastAccess  .year       = i;
    fileStatus.fdateLastWrite   .year       = i;

    /* determine the SECONDS */
    strncpy(buffer, &argV[2][6], 2);
    buffer[2] = '\0';
    i = atoi(buffer) / 2;

    fileStatus.ftimeCreation    .twosecs    = i;
    fileStatus.ftimeLastAccess  .twosecs    = i;
    fileStatus.ftimeLastWrite   .twosecs    = i;

    /* determine the MINUTES */
    strncpy(buffer, &argV[2][3], 2);
    buffer[2] = '\0';
    i = atoi(buffer);

    fileStatus.ftimeCreation    .minutes    = i;
    fileStatus.ftimeLastAccess  .minutes    = i;
    fileStatus.ftimeLastWrite   .minutes    = i;

    /* determine the HOURS */
    strncpy(buffer, &argV[2][0], 2);
    buffer[2] = '\0';
    i = atoi(buffer);

    fileStatus.ftimeCreation    .hours      = i;
    fileStatus.ftimeLastAccess  .hours      = i;
    fileStatus.ftimeLastWrite   .hours      = i;

    /* save the new file information */
    rc = DosSetPathInfo(argV[3], FIL_STANDARD, &fileStatus, sizeof(fileStatus), 0);
    if(rc != 0)
    {
        printf( "\nERROR: DosQueryPathInfo() returned rc==%d!\n"
                "(Try \"help sys%04d\" for more information.)\a\n", rc, rc);
        return rc;
    }

    return 0;
}
