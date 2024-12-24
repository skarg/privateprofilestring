/**
 * @file
 * @brief Test file for a basic non-volatile data module
 * @author Steve Karg <skarg@users.sourceforge.net>
 * @date December 2024
 * @copyright SPDX-License-Identifier: MIT
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <string.h>
#include "stptok.h"

/**
* Unit Test for the stptok function, testing the tokens
*
* @param       pTest - test tracking pointer
*/
static void testTokens(void)
{
    const char *pCmd = "I Love You\r\n";
    char token[80] = "";


    pCmd = stptok(pCmd, token, sizeof(token), " \r\n");

    assert(strcmp(token, "I") == 0);
    assert(strcmp(pCmd, "Love You\r\n") == 0);

    pCmd = stptok(pCmd, token, sizeof(token), " \r\n");

    assert(strcmp(token, "Love") == 0);
    assert(strcmp(pCmd, "You\r\n") == 0);

    pCmd = stptok(pCmd, token, sizeof(token), " \r\n");

    assert(strcmp(token, "You") == 0);
    assert(pCmd == NULL);

    return;
}

/**
* Main program entry for Unit Test
*
* @return  returns 0 on success, and non-zero on fail.
*/
int main(void)
{
  testTokens();

  return 0;
}
