/**
 * @file
 * @author Ray Gardner, Bob Stout, and Steve Karg
 * @brief  Parses a string using delimiters and returns a token.
 *
 * @section LICENSE
 *
 * Public domain by Ray Gardner
 *
 * @section DESCRIPTION
 *
 * This file contains the stptok function, which parses a string
 * using delimiters and returns a token.  Similar to strtok, but
 * thread safe.
 *
 */
#include <stddef.h>
#include "stptok.h"

/**
 * Parses a string using delimiters and returns a token
 *
 * Pass this function a string to parse,
 * a buffer to receive the "token" that gets scanned,
 * the length of the buffer, and a string of "break"
 * characters that stop the scan.
 * It will copy the string into the buffer up to
 * any of the break characters, or until the buffer
 * is full, and will always leave the buffer
 * null-terminated.  It will return a pointer to the
 * first non-breaking character after the one that
 * stopped the scan.
 *
 * @param   s - string to parse
 * @param   tok - buffer that receives the "token" that gets scanned
 * @param   toklen - size of the buffer
 * @param   brk - string of break characters that will stop the scan
 *
 * @return  It will return a pointer to the first non-breaking character
 * after the one that stopped the scan or NULL on error or end of string.
 */
char *stptok(const char *s,
    char *tok,  /* buffer that receives the "token" that gets scanned */

    size_t toklen,      /* length of the buffer */

    const char *brk)
{       /* string of break characters that will stop the scan */
    char *lim;  /* limit of token */
    const char *b;      /* current break character */


    /* check for invalid pointers */
    if (!s || !tok || !brk) {
        return NULL;
    }

    /* check for empty string */
    if (!*s) {
        return NULL;
    }

    lim = tok + toklen - 1;
    while (*s && tok < lim) {
        for (b = brk; *b; b++) {
            if (*s == *b) {
                *tok = 0;
                for (++s, b = brk; *s && *b; ++b) {
                    if (*s == *b) {
                        ++s;
                        b = brk;
                    }
                }
                if (!*s) {
                    return NULL;
                }
                return (char *) s;
            }
        }
        *tok++ = *s++;
    }
    *tok = 0;
    if (!*s) {
        return NULL;
    }
    return (char *) s;
}

#ifdef TEST
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include "ctest.h"
/**
* Unit Test for the stptok function, testing the tokens
*
* @param       pTest - test tracking pointer
*/
void testTokens(Test * pTest)
{
    char *pCmd = "I Love You\r\n";
    char token[80] = "";


    pCmd = stptok(pCmd, token, sizeof(token), " \r\n");

    ct_test(pTest, strcmp(token, "I") == 0);
    ct_test(pTest, strcmp(pCmd, "Love You\r\n") == 0);

    pCmd = stptok(pCmd, token, sizeof(token), " \r\n");

    ct_test(pTest, strcmp(token, "Love") == 0);
    ct_test(pTest, strcmp(pCmd, "You\r\n") == 0);

    pCmd = stptok(pCmd, token, sizeof(token), " \r\n");

    ct_test(pTest, strcmp(token, "You") == 0);
    ct_test(pTest, pCmd == NULL);

    return;
}

#ifdef TEST_STPTOK
/**
* Main program entry for Unit Test
*
* @return  returns 0 on success, and non-zero on fail.
*/
int main(void)
{
    Test *pTest;
    bool rc;

    pTest = ct_create("stptok test", NULL);

    /* individual tests */
    rc = ct_addTestFunction(pTest, testTokens);
    assert(rc);

    ct_setStream(pTest, stdout);
    ct_run(pTest);
    (void) ct_report(pTest);

    ct_destroy(pTest);

    return 0;
}
#endif

#endif
