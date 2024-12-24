/**
 * @file
 * @author Steve Karg
 * @date 1997-2004
 * @brief  Reads or Writes a string to an INI file.
 *
 * @section LICENSE
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#if defined(__BORLANDC__)
  #include <alloc.h>
  #include <mem.h>
#elif defined(_MSC_VER)
  #define strcmpi _stricmp
#else
  #define strcmpi strcasecmp
#endif

#include "profile.h"
#include "rmspace.h"
#include "stptok.h"

//#define TEST
#ifdef TEST
  #include <assert.h>
  #include "ctest.h"
#endif

/* constants for copy file */
#define BLOCKSIZE 512
typedef char DATA;
/**
 * Binary copy of one file to another
 *
 * @param pDest - destination file handle
 * @param pSource - source file handle
 */
static void filecopy(
  FILE *pDest,
  FILE *pSource)
{
  DATA block[BLOCKSIZE] = {0}; /* holds data for file copy */
  int num_read = 0;  /* number of bytes read when copying file */

  if (pDest && pSource)
  {
    while ((num_read = fread(block,sizeof(DATA),
      BLOCKSIZE,pSource)) == BLOCKSIZE)
    {
      fwrite(block,sizeof(DATA),num_read,pDest);
    }
    /* write remaining stuff */
    fwrite(block,sizeof(DATA),num_read,pDest);
  }
}

/**
 * Writes a string to an INI file.
 * If all three parameters are NULL, the function
 * flushes the cache. The function always returns FALSE
 * after flushing the cache, regardless of whether the
 * flush succeeds or fails.
 * Win32 replacement function
 *
 * @param pAppName (IN) Points to a null-terminated string
 *  containing the name of the section to which the
 *  string will be copied. If the section does not exist,
 *  it is created. The name of the section is case-independent;
 *  the string can be any combination of uppercase and
 *  lowercase letters.
 * @param pKeyName (IN) Points to the null-terminated string
 *  containing the name of the key to be associated with
 *  a string. If the key does not exist in the specified
 *  section, it is created. If this parameter is NULL, the
 *  entire section, including all entries within the section,
 *  is deleted.
 * @param pString (IN) Points to a null-terminated string to
 *  be written to the file. If this parameter is NULL, the
 *  key pointed to by the pKeyName parameter is deleted.
 *  Windows 95: This platform does not support the use
 *  of the TAB (\t) character as part of this parameter.
 * @param pFileName (IN) Points to a null-terminated string
 *  that names the initialization file.
 *
 * @return If the function successfully copies the string to
 *  the initialization file, the return value is nonzero.
 *  If the function fails, or if it flushes the cached
 *  version of the most recently accessed initialization
 *  file, the return value is zero. To get extended
 *  error information, call GetLastError.
 **/
BOOL WritePrivateProfileString(
    const char *pAppName,
    const char *pKeyName,
    const char *pString,
    const char *pFileName)
{
  FILE *pFile; /* stream handle */
  FILE *pTempFile; /* stream handle */
  BOOL status = FALSE; /* return value */
  char line[MAX_LINE_LEN] = {""}; /* line in file */
  char copy_line[MAX_LINE_LEN] = {""}; /* copy of line in file */
  char token[MAX_LINE_LEN] = {""}; /* for tokenizing */
  BOOL found = FALSE; /* true if key is found */
  BOOL section = FALSE; /* true if section is found */

  /* flush cache - since we have no cache, just return */
  if (!pAppName && !pKeyName && !pString)
    return (status);

  /* undefined behavior */
  if (!pAppName || !pFileName)
    return (status);

  pFile = fopen(pFileName,"r+");
  if (!pFile)
  {
    if (pString && pKeyName)
    {
      /* new file */
      pFile = fopen(pFileName,"w");
      if (pFile)
      {
        fprintf(pFile,"[%s]\n",pAppName);
        fprintf(pFile,"%s=%s\n",pKeyName,pString);
        fclose(pFile);
        status = TRUE;
      }
    }
  }
  else
  {
    /* open a temp file to use as a buffer */
    pTempFile = tmpfile();
    /* process! */
    if (pTempFile)
    {
      while (fgets(line,sizeof(line),pFile) != NULL)
      {
        /* remove leading and trailing white space */
        rmtrail(line);
        rmlead(line);
        /* comment */
        if ((line[0] == ';') ||
            (found))
        {
          /* write line to new file */
          fprintf(pTempFile,"%s\n",line);
        }
        /* inside my section */
        else if (section)
        {
          /* comments */
          if (line[0] == ';')
          {
            /* write line to new file */
            fprintf(pTempFile,"%s\n",line);
          }
          /* new section */
          else if (line[0] == '[')
          {
            /* write out data since we didn't find it */
            if (pKeyName && pString)
              fprintf(pTempFile,"%s=%s\n",pKeyName,pString);
            found = TRUE;
            status = TRUE;
            /* write out this line, too! */
            fprintf(pTempFile,"%s\n",line);
          }
          /* delete section by not writing line to temp file */
          else if (!pKeyName)
          {
            /* do nothing */
          }
          /* finish processing the file */
          else if (found)
          {
            /* write line to new file */
            fprintf(pTempFile,"%s\n",line);
          }
          /* search */
          else
          {
            strncpy(copy_line,line,sizeof(copy_line)-1);
            (void)stptok(copy_line,token,sizeof(token),"=");
            rmtrail(token);
            if (strcmpi(token,pKeyName) == 0)
            {
              status = TRUE;
              found = TRUE;
              /* overwrite the line that was read in or remove it by
                 not writing it out if pString is NULL */
              if (pString)
                fprintf(pTempFile,"%s=%s\n",pKeyName,pString);
            }
            else
            {
              /* write the un-matched key line out to the temp file */
              fprintf(pTempFile,"%s\n",line);
            }
          }
        }
        /* look for section */
        else if (line[0] == '[')
        {
          strncpy(copy_line,line,sizeof(copy_line)-1);
          if (rmbrackets(copy_line))
          {
            // it's my section!
            if (strcmpi(pAppName,copy_line) == 0)
            {
              section = TRUE;
              // delete the section name if the KEY is NULL.
              if (pKeyName)
                fprintf(pTempFile,"%s\n",line);
            }
            else
              fprintf(pTempFile,"%s\n",line);
          }
        }
        else
        {
          /* write line to new file */
          fprintf(pTempFile,"%s\n",line);
        }
      }
      /* append to the end of the temp file */
      if (!section && pKeyName)
      {
        fprintf(pTempFile,"[%s]\n",pAppName);
      }
      if (!found && pKeyName && pString)
      {
        fprintf(pTempFile,"%s=%s\n",pKeyName,pString);
        status = TRUE;
      }
      /* finished! */
      fclose(pFile);
      /* copy the temp file data over the existing file */
      pFile = fopen(pFileName,"wb");
      if (pFile)
      {
        rewind(pTempFile);
        filecopy(pFile,pTempFile);
        fclose(pFile);
      }
      fclose(pTempFile);
    }
    /* unable to open temp file */
    else
    {
      fclose(pFile);
    }
  }
  return (status);
}

/**
 * Processes the INI file.
 * Win32 replacement function
 *
 * @param pAppName (IN) Points to a null-terminated string
 *  that specifies the section containing the key name.
 *  If this parameter is NULL, the GetPrivateProfileString
 *  function copies all section names in the file to the
 *  supplied buffer.
 * @param pKeyName (IN) Pointer to the null-terminated string
 *  containing the key name whose associated string is
 *  to be retrieved. If this parameter is NULL, all key
 *  names in the section specified by the lpAppName
 *  parameter are copied to the buffer specified by
 *  the pReturnedString parameter.
 * @param pDefault (IN) Pointer to a null-terminated default
 *  string. If the pKeyName key cannot be found in the
 *  initialization file, GetPrivateProfileString copies
 *  the default string to the pReturnedString buffer.
 *  This parameter cannot be NULL. Avoid specifying a
 *  default string with trailing blank characters.
 *  The function inserts a null character in the
 *  pReturnedString buffer to strip any trailing blanks.
 * @param pReturnedString (OUT) Pointer to the buffer that
 *  receives the retrieved string.
 * @param nSize (IN) Specifies the size, in characters, of the
 *  buffer pointed to by the pReturnedString parameter.
 * @param pFileName (IN) Pointer to a null-terminated string
 *  that names the initialization file. If this parameter
 *  does not contain a full path to the file, Windows
 *  searches for the file in the Windows directory.
 *
 * @return If the function succeeds, the return value is the
 *   number of characters copied to the buffer, not
 *   including the terminating null character.
 *   If neither pAppName nor pKeyName is NULL and the
 *   supplied destination buffer is too small to hold the
 *   requested string, the string is truncated and followed
 *   by a null character, and the return value is equal
 *   to nSize minus one.
 *   If either pAppName or pKeyName is NULL and the
 *   supplied destination buffer is too small to hold
 *   all the strings, the last string is truncated and
 *   followed by two null characters. In this case,
 *   the return value is equal to nSize minus two.
 *
 * @section DESCRIPTION
 *
 * The GetPrivateProfileString function searches the
 * specified initialization file for a key that matches
 * the name specified by the pKeyName parameter under
 * the section heading specified by the pAppName parameter.
 * If it finds the key, the function copies the
 * corresponding string to the buffer. If the key does
 * not exist, the function copies the default character
 * string specified by the pDefault parameter. A section
 * in the initialization file must have the following form:
 * {@code
 *  [section]
 *  key=string
 *  .
 *  .
 *  .
 * }
 * If pAppName is NULL, GetPrivateProfileString copies
 * all section names in the specified file to the supplied
 * buffer.
 * If pKeyName is NULL, the function copies all
 * key names in the specified section to the supplied
 * buffer. An application can use this method to enumerate
 * all of the sections and keys in a file. In either case,
 * each string is followed by a null character and the
 * final string is followed by a second null character.
 * If the supplied destination buffer is too small to hold
 * all the strings, the last string is truncated and
 * followed by two null characters.
 * If the string associated with pKeyName is enclosed
 * in single or double quotation marks, the marks are
 * discarded when the GetPrivateProfileString function
 * retrieves the string.
 * The GetPrivateProfileString function is not case-sensitive;
 * the strings can be a combination of uppercase and
 * lowercase letters.
 ******************************************************************/
size_t GetPrivateProfileString(
    const char *pAppName,
    const char *pKeyName,
    const char *pDefault,
    char *pReturnedString,
    size_t nSize,
    const char *pFileName)
{
  size_t count = 0; /* number of characters placed into return string */
  size_t len = 0; /* length of string */
  FILE *pFile = NULL; /* stream handle */
  BOOL use_default = FALSE; /* TRUE if we need to copy default string */
  char line[MAX_LINE_LEN] = {""}; /* line in file */
  char token[MAX_LINE_LEN] = {""}; /* for tokenizing */
  char *pLine = NULL; /* points to line in file */
  BOOL section = FALSE; /* true if section is found */
  BOOL found_in_section = FALSE; /* true if key is found in section */

  if (!pReturnedString || !pFileName)
    return (count);

  /* initialize the return string */
  pReturnedString[0] = '\0';

  pFile = fopen(pFileName,"r");
  if (pFile)
  {
    /* load all section names to ReturnString */
    if (!pAppName)
    {
      while (fgets(line,sizeof(line),pFile) != NULL)
      {
        /* remove leading and trailing white space */
        rmtrail(line);
        rmlead(line);
        if (line[0] == '[')
        {
          if (rmbrackets(line))
          {
            len = strlen(line);
            if ((len + count + 2) < nSize)
            {
              strcpy(pReturnedString,line);
              len++; /* add null */
              pReturnedString += len;
              count += len;
            }
            /* copy as much as we can, then truncate */
            else
            {
              len = nSize - 2 - count;
              strncpy(pReturnedString,line,len);
              len++; /* add null */
              pReturnedString += len;
              count += len;
              break;
            }
          }
        }
      }
    }
    /* find section name */
    else
    {
      while (fgets(line,sizeof(line),pFile) != NULL)
      {
        /* remove leading and trailing white space */
        rmtrail(line);
        rmlead(line);
        /* comment */
        if (line[0] == ';')
        {
          /* do nothing */
        }
        /* inside my section */
        else if (section)
        {
          /* comments */
          if (line[0] == ';')
          {
            /* do nothing */
          }
          /* new section */
          else if (line[0] == '[')
          {
            /* we must not have found our key name */
            if (pKeyName)
              use_default = TRUE;
            /* stop reading the file */
            break;
          }
          /* search */
          else if (pKeyName)
          {
            (void)stptok(line,token,sizeof(token),"=");
            rmtrail(token);
            /* found? */
            if (strcmpi(token,pKeyName) == 0)
            {
              pLine = strchr(line,'=');
              if (pLine)
              {
                found_in_section = TRUE;
                pLine++;
                /* cleanup return string */
                rmtrail(pLine);
                rmlead(pLine);
                (void)rmquotes(pLine);
                /* count what's left */
                len = strlen(pLine);
                if (len < nSize)
                {
                  strncpy(pReturnedString,pLine, len);
                }
                /* copy as much as we can, then truncate */
                else
                {
                  len = nSize - 1; /* less the null */
                  strncpy(pReturnedString,pLine,len);
                }
                count = len;
              }
              /* stop reading the file */
              break;
            }
          }
          /* load return string with key names */
          else
          {
            found_in_section = TRUE;
            (void)stptok(line,token,sizeof(token),"=");
            rmtrail(token);
            len = strlen(token);
            if ((len + count + 2) < nSize)
            {
              strcpy(pReturnedString,token);
              len++; /* add null */
              pReturnedString += len;
              count += len;
            }
            /* copy as much as we can, then truncate */
            else
            {
              len = nSize - 2 - count;
              strncpy(pReturnedString,token,len);
              len++; /* add null */
              pReturnedString += len;
              count += len;
              break;
            }
          }
        }
        /* look for section */
        else if (line[0] == '[')
        {
          if (rmbrackets(line))
          {
            if (strcmpi(pAppName,line) == 0)
              section = TRUE;
          }
        }
      }
      if (!section)
        use_default = TRUE;
    }
    if (!pKeyName || !pAppName)
    {
      /* count doesn't include last 2 nulls */
      if (count)
        count--;
      /* this pointer should be pointing to the start of next string */
      pReturnedString[0] = '\0';
    }
    fclose(pFile);
  }
  // if the file does not exist, return the default
  else
    use_default = TRUE;

  // key not found in section - return default
  if (section && !found_in_section)
    use_default = TRUE;

  if (use_default && pDefault)
  {
    (void)strncpy(pReturnedString,pDefault,nSize);
    pReturnedString[nSize-1] = '\0';
    /* cleanup return string */
    rmtrail(pReturnedString);
    rmlead(pReturnedString);
    (void)rmquotes(pReturnedString);
    /* count what's left */
    count = strlen(pReturnedString);
  }

  return (count);
}
