/**
* @file
* @author Steve Karg
* @date 1997-2002
* @brief Generic C string functions to remove leading or trailing spaces,
*  quotes, or brackets.
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
*
* @section DESCRIPTION
*
* This is a collection of generic C string functions that quickly
* remove leading or trailing spaces, quotes, or brackets from a C string.
*
* To use this library, simply pass a C string to the appropriate function:
* {@code
* char my_string[80] = "   [\"my string\"]   ";
*
* rmlead(my_string);
* rmtrail(my_string);
* rmbrackets(my_string);
* rmquotes(my_string);
*
* printf("%s", my_string);
* }
*
* Note that the brackets and quotes removal only removes the
* quote or bracket in the first and last position of the string.
*/

/* includes */
#include <string.h>
#include <ctype.h>
#if defined(__BORLANDC__)
  #include <mem.h>
#endif
#include "rmspace.h"

/**
* Remove all leading whitespace (isspace) from a C string
*
* @param str - C string potentially containing leading spaces
*
* @return The number of characters between the beginning of the
* string and the terminating null character (without including
* the terminating null character itself).
*/
size_t rmlead(char *str)
{
  char *obuf;
    size_t len = 0;

    if (str) {
        for (obuf = str; *obuf && isspace(*obuf); ++obuf) {
        }
        len = strlen(obuf);
        if (str != obuf) {
            memmove(str, obuf, len + 1);
        }
  }

    return len;
}

/**
* Remove all trailing whitespace (isspace) from a C string
*
* @param str - C string potentially containing trailing spaces
*
* @return The number of characters between the beginning of the
* string and the terminating null character (without including
* the terminating null character itself).
*/
size_t rmtrail(char *str)
{
    size_t i = 0;

    if (str && 0 != (i = strlen(str))) {
    /* start at end, not at 0 */
    --i;
        do {
      if (!isspace(str[i]))
        break;
    } while (--i);
    str[++i] = '\0';
  }

    return i;
}

/**
* Remove single or double quotation marks enclosing a C string.
* Only removes them from the first and last character position.
*
* @param str - C string potentially containing quotes
*
* @return non-zero when successful, zero when no quotes are found
*/
int rmquotes(char *str)
{
  size_t len;
  int status = 0;

    if (str) {
    len = strlen(str);
        if (len > 1) {
            if (((str[0] == '\'') && (str[len - 1] == '\'')) ||
                ((str[0] == '\"') && (str[len - 1] == '\"'))) {
        str[len-1] = '\0';
        memmove(str,&str[1],len);
        status = 1;
      }
    }
  }
  return (status);
}

/**
* Remove brackets enclosing a C string.
* Only removes them from the first and last character position.
*
* @param str - C string potentially containing brackets
*
* @return non-zero when successful, zero when no brackets are found
*/
int rmbrackets(char *str)
{
  size_t len;
  int status = 0;

    if (str) {
    len = strlen(str);
        if (len > 1) {
            if ((str[0] == '[') && (str[len - 1] == ']')) {
        str[len-1] = '\0';
        memmove(str,&str[1],len);
        status = 1;
      }
    }
  }
  return (status);
}
