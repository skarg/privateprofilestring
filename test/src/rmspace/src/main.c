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
#include "rmspace.h"

/**
* Unit Test for the C string leading space removal function
*
* @param pTest - test tracking pointer
*/
static void test_rmlead(void)
{
  char token[80];
  char result[80];
  size_t len;

  strcpy(token,"  spacing is crucial  ");
  strcpy(result,"spacing is crucial  ");
    len = rmlead(token);
    assert(strlen(token) == len);
    assert(strlen(token) == strlen(result));
  assert(strcmp(result,token) == 0);

  strcpy(token,"spacing is crucial  ");
  strcpy(result,token);
    len = rmlead(token);
  assert(strlen(token) == len);
    assert(strlen(token) == strlen(result));
  assert(strcmp(result,token) == 0);

  strcpy(token,"s          spacing is crucial   ");
  strcpy(result,token);
    len = rmlead(token);
  assert(strlen(token) == len);
    assert(strlen(token) == strlen(result));
  assert(strcmp(result,token) == 0);

  return;
}

/**
* Unit Test for the C string trailing space removal function
*
* @param pTest - test tracking pointer
*/
static void test_rmtrail(void)
{
  char token[80];
  char result[80];
  size_t len;

  strcpy(token,"  spacing is crucial ");
  strcpy(result,"  spacing is crucial");
    len = rmtrail(token);
    assert(strlen(token) == strlen(result));
    assert(strlen(token) == len);
  assert(strcmp(result,token) == 0);

  strcpy(token,"  spacing is crucial  ");
  strcpy(result,"  spacing is crucial");
    len = rmtrail(token);
    assert(strlen(token) == strlen(result));
    assert(strlen(token) == len);
  assert(strcmp(result,token) == 0);

  strcpy(token,"  spacing is crucial");
  strcpy(result,token);
    len = rmtrail(token);
    assert(strlen(token) == strlen(result));
  assert(strlen(token) == len);
  assert(strcmp(result,token) == 0);

  strcpy(token,"   spacing is crucial   s");
  strcpy(result,token);
    len = rmtrail(token);
    assert(strlen(token) == strlen(result));
  assert(strlen(token) == len);
  assert(strcmp(result,token) == 0);

  return;
}

/**
* Unit Test for the C string quotation mark removal function
*
* @param pTest - test tracking pointer
*/
static void test_rmquotes(void)
{
  char token[80];
  char result[80];
  size_t len;
  int status;

  strcpy(token,"\"spacing is crucial\"");
  strcpy(result,"spacing is crucial");
  len = strlen(token);
  status = rmquotes(token);
  assert(status != 0);
  assert(strlen(token) == (len-2));
  assert(strcmp(result,token) == 0);

  strcpy(token,"\'spacing is crucial\'");
  strcpy(result,"spacing is crucial");
  len = strlen(token);
  status = rmquotes(token);
  assert(status != 0);
  assert(strlen(token) == (len-2));
  assert(strcmp(result,token) == 0);

  strcpy(token,"spacing is crucial");
  strcpy(result,token);
  len = strlen(token);
  status = rmquotes(token);
  assert(status == 0);
  assert(strlen(token) == len);
  assert(strcmp(result,token) == 0);

  strcpy(token,"\'spacing is crucial");
  strcpy(result,token);
  len = strlen(token);
  status = rmquotes(token);
  assert(status == 0);
  assert(strlen(token) == len);
  assert(strcmp(result,token) == 0);

  strcpy(token,"\"spacing is crucial");
  strcpy(result,token);
  len = strlen(token);
  status = rmquotes(token);
  assert(status == 0);
  assert(strlen(token) == len);
  assert(strcmp(result,token) == 0);

  strcpy(token,"spacing is crucial\'");
  strcpy(result,token);
  len = strlen(token);
  status = rmquotes(token);
  assert(status == 0);
  assert(strlen(token) == len);
  assert(strcmp(result,token) == 0);

  strcpy(token,"spacing is crucial\"");
  strcpy(result,token);
  len = strlen(token);
  status = rmquotes(token);
  assert(status == 0);
  assert(strlen(token) == len);
  assert(strcmp(result,token) == 0);

  return;
}

/**
* Unit Test for the C string bracket removal function
*
* @param pTest - test tracking pointer
*/
static void test_rmbrackets(void)
{
  char token[80];
  char result[80];
  size_t len;
  int status;

  strcpy(token,"[spacing is crucial]");
  strcpy(result,"spacing is crucial");
  len = strlen(token);
  status = rmbrackets(token);
  assert(status != 0);
  assert(strlen(token) == (len-2));
  assert(strcmp(result,token) == 0);

  strcpy(token,"spacing is crucial");
  strcpy(result,token);
  len = strlen(token);
  status = rmbrackets(token);
  assert(status == 0);
  assert(strlen(token) == len);
  assert(strcmp(result,token) == 0);

  strcpy(token,"[spacing is crucial");
  strcpy(result,token);
  len = strlen(token);
  status = rmbrackets(token);
  assert(status == 0);
  assert(strlen(token) == len);
  assert(strcmp(result,token) == 0);

  strcpy(token,"spacing is crucial]");
  strcpy(result,token);
  len = strlen(token);
  status = rmbrackets(token);
  assert(status == 0);
  assert(strlen(token) == len);
  assert(strcmp(result,token) == 0);

  return;
}

/**
* Main program entry for Unit Test
*
* @return  returns 0 on success, and non-zero on fail.
*/
int main(void)
{
  /* individual tests */
  test_rmlead();
  test_rmtrail();
  test_rmquotes();
  test_rmbrackets();

  return 0;
}
