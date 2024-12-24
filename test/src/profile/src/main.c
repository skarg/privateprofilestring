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
#include "profile.h"

/**
* Unit Test for the WritePrivateProfileString function
*
* @param pTest - test tracking pointer
* @param app_name - applicaiton name
* @param key_name - name of key to write a value
* @param string_name - string to write
* @param file_name - name of INI file
*/
static void TestWritePrivateProfileString(
  const char *app_name,
  const char *key_name,
  const char *string_name,
  const char *file_name)
{
  char default_name[MAX_LINE_LEN] = {"123456789"};
  char return_name[MAX_LINE_LEN] = {""};
  char expected_name[MAX_LINE_LEN] = {""};
  size_t count = 0; /* return value for Get */

  // only flush the cache
  if (!app_name && !key_name && !string_name)
    strcpy(expected_name,return_name);
  // undefined behavior
  else if (!app_name || !file_name)
    strcpy(expected_name,return_name);
  // these delete the key or entire section
  else if (!key_name || !string_name)
    strcpy(expected_name,default_name);
  // normal behavior
  else if (string_name)
    strcpy(expected_name,string_name);
  // do the deed
  WritePrivateProfileString(app_name,key_name,string_name, file_name);
  // check the results
  count = GetPrivateProfileString(app_name, key_name, default_name,
    return_name,sizeof(return_name),file_name);
  assert( count == strlen(expected_name));
  assert( strcmp(return_name,expected_name) == 0);

  return;
}

/**
* Unit Test for the GetPrivateProfileString function
*
* @param pTest - test tracking pointer
* @param app_name - applicaiton name
* @param key_name - name of key to read a value
* @param string_name - string to store value of the key
* @param file_name - name of INI file
*/
static void TestGetPrivateProfileString(
  const char *app_name,
  const char *key_name,
  const char *string_name,
  const char *file_name)
{
  char default_name[MAX_LINE_LEN] = {"123456789"};
  char return_name[MAX_LINE_LEN] = {""};
  char expected_name[MAX_LINE_LEN] = {""};
  size_t count = 0; /* return value for Get */

  // undefined behavior
  if (string_name)
    strcpy(expected_name,string_name);
  else
    strcpy(expected_name,default_name);
  count = GetPrivateProfileString(app_name, key_name, default_name,
    return_name,sizeof(return_name),file_name);
  assert( count == strlen(expected_name));
  assert( strcmp(return_name,expected_name) == 0);

  return;
}

/**
* Unit Tests for the Get/Write PrivateProfileString functions
*
* @param pTest - test tracking pointer
*/
static void test_PrivateProfileString(void)
{
  char app_name[MAX_LINE_LEN] = {"Joshua"};
  char key_name[MAX_LINE_LEN] = {"Anna"};
  char file_name[MAX_LINE_LEN] = {"test1.ini"};
  char default_name[MAX_LINE_LEN] = {"123"};
  char return_name[MAX_LINE_LEN] = {"Mary"};
  char short_name[16] = {"Christopher"};
  char string_name[MAX_LINE_LEN] = {""};
  char expected_name[MAX_LINE_LEN] = {""};
  size_t count = 0; /* return value for Get */
  size_t len = 0; /* temp */

  /* start clean */
  remove(file_name);

  // when no file exists, the default name should be returned
  count = GetPrivateProfileString(app_name, key_name, default_name,
    return_name,sizeof(return_name),file_name);
  assert( count == strlen(default_name));
  assert( strcmp(return_name,default_name) == 0);

  /* write new section and new key to new file */
  TestWritePrivateProfileString(
    "test1","key1","70",file_name);
  /* re-write string to existing section and key */
  TestWritePrivateProfileString(
    "test1","key1","0x72",file_name);
  /* same section, new key and string */
  TestWritePrivateProfileString(
    "test1","key 2","123 456",file_name);
  /* same section, another new key and string */
  strcpy(app_name,"test1");
  strcpy(key_name,"key 3");
  strcpy(string_name,"  \"Joe\'s Garage on Main Street\"  ");
  strcpy(expected_name,"Joe\'s Garage on Main Street");
  WritePrivateProfileString(app_name,key_name,string_name, file_name);
  count = GetPrivateProfileString(app_name, key_name, default_name,
    return_name,sizeof(return_name),file_name);
  assert( count == strlen(expected_name));
  assert( strcmp(return_name,expected_name) == 0);

  TestWritePrivateProfileString(
    "relay","relay 1",
    "\"Relay 104\",NORMAL,YES,PHASE A,50",file_name);
  TestWritePrivateProfileString(
    "switch","switch 1",
    "\"Switch 101\",LATCH,YES,0,0,GROUP 101",file_name);

  /* get the app/section list */
  len = 20;
  memmove(app_name,"test1""\0""relay""\0""switch""\0""\0",len);
  count = GetPrivateProfileString(NULL, NULL, default_name,
    return_name,sizeof(return_name),file_name);
  assert( count == (len - 2));/* less terminating null(s) */
  assert( memcmp(return_name,app_name,len) == 0);

  /* get the key list for a particular section */
  len = 18;
  strcpy(app_name,"test1");
  memmove(key_name,"key1""\0""key 2""\0""key 3""\0""\0",len);
  count = GetPrivateProfileString(app_name, NULL, default_name,
    return_name,sizeof(return_name),file_name);
  assert( count == (len - 2));/* less terminating null(s) */
  assert( memcmp(return_name,key_name,len) == 0);

  assert(sizeof(short_name) == 16);

  /* get the app/section list - limited return string size */
  len = sizeof(short_name);
  memmove(app_name,"test1""\0""relay""\0""sw""\0""\0",len);
  count = GetPrivateProfileString(NULL, NULL, default_name,
    short_name,sizeof(short_name),file_name);
  assert( count == (len - 2));/* less terminating null(s) */
  assert( memcmp(short_name,app_name,len) == 0);

  /* get the key list for a particular section - limited memory */
  len = sizeof(short_name);
  strcpy(app_name,"test1");
  memmove(key_name,"key1""\0""key 2""\0""key""\0""\0",len);
  count = GetPrivateProfileString(app_name, NULL, default_name,
    short_name,sizeof(short_name),file_name);
  assert( count == (len - 2));/* less terminating null(s) */
  assert( memcmp(short_name,key_name,len) == 0);

  /* get the string for a particular key - limited memory */
  strcpy(app_name,"test1");
  strcpy(key_name,"key 3");
  strcpy(expected_name,"Joe\'s Garage on");
  count = GetPrivateProfileString(app_name, key_name, default_name,
    short_name,sizeof(short_name),file_name);
  assert( count == strlen(expected_name));
  assert( strcmp(short_name,expected_name) == 0);

  /* get the default name */
  strcpy(app_name,"test1");
  strcpy(key_name,"key 4"); /* non-existing key */
  count = GetPrivateProfileString(app_name, key_name, default_name,
    return_name,sizeof(return_name),file_name);
  assert( count == strlen(default_name));
  assert( strcmp(return_name,default_name) == 0);

  /* get the default name */
  strcpy(app_name,"AM750 WSB"); /* non-existing section */
  strcpy(key_name,"Clark Howard");
  count = GetPrivateProfileString(app_name, key_name, default_name,
    return_name,sizeof(return_name),file_name);
  assert( count == strlen(default_name));
  assert( strcmp(return_name,default_name) == 0);

  return;
}

/**
* Unit Tests for the Write PrivateProfileString functions
*
* @param pTest - test tracking pointer
*/
static void test_PrivateProfileStringWrite(void)
{
  char file_name[MAX_LINE_LEN] = {"test2.ini"};


  /* start clean */
  remove(file_name);

  /* write new section and new key to new file */
  TestWritePrivateProfileString(
    "MySection1","MyKey1","MyKey1Value",file_name);
  TestWritePrivateProfileString(
    "MySection1","MyKey2","MyKey2Value",file_name);

  /* write new section and new key to new file */
  TestWritePrivateProfileString(
    "MySection2","MyKey3","MyKey3Value",file_name);
  TestWritePrivateProfileString(
    "MySection2","MyKey4","MyKey4Value",file_name);

  /* If I delete a section which does not exist,
     it should do nothing. */
  TestWritePrivateProfileString(
    "Non-Exist Section",NULL,NULL,file_name);
  TestGetPrivateProfileString(
    "MySection1","MyKey1","MyKey1Value",file_name);
  TestGetPrivateProfileString(
    "MySection1","MyKey2","MyKey2Value",file_name);
  TestGetPrivateProfileString(
    "MySection2","MyKey3","MyKey3Value",file_name);
  TestGetPrivateProfileString(
    "MySection2","MyKey4","MyKey4Value",file_name);

  /* If I Do :
     WritePrivateProfileString("MySection1", "MyKey1", NULL, file);
     It removes the entry "MyKey1" */
  TestWritePrivateProfileString(
    "MySection1","MyKey1",NULL,file_name);
  TestGetPrivateProfileString(
    "MySection1","MyKey1",NULL,file_name);
  TestGetPrivateProfileString(
    "MySection1","MyKey2","MyKey2Value",file_name);
  TestGetPrivateProfileString(
    "MySection2","MyKey3","MyKey3Value",file_name);
  TestGetPrivateProfileString(
    "MySection2","MyKey4","MyKey4Value",file_name);

  /* If I Do :
     WritePrivateProfileString("MySection", NULL, NULL, file);
     It removes the entire section (with entries) */
  TestWritePrivateProfileString(
    "MySection2",NULL,NULL,file_name);
  TestGetPrivateProfileString(
    "MySection1","MyKey1",NULL,file_name);
  TestGetPrivateProfileString(
    "MySection1","MyKey2","MyKey2Value",file_name);
  TestGetPrivateProfileString(
    "MySection2","MyKey3",NULL,file_name);
  TestGetPrivateProfileString(
    "MySection2","MyKey4",NULL,file_name);

  return;
}

/**
* Unit Tests for the Erase PrivateProfileString functionality
*
* @param pTest - test tracking pointer
*/
static void test_PrivateProfileStringErase(void)
{
  char file_name[MAX_LINE_LEN] = {"test3.ini"};

  /* start clean */
  remove(file_name);

  /* write new section and new key to new file */
  TestWritePrivateProfileString(
    "MySection1","MyKey1","MyKey1Value",file_name);
  TestWritePrivateProfileString(
    "MySection1","MyKey2","MyKey2Value",file_name);

  /* write new section and new key to new file */
  TestWritePrivateProfileString(
    "MySection2","MyKey3","MyKey3Value",file_name);
  TestWritePrivateProfileString(
    "MySection2","MyKey4","MyKey4Value",file_name);

  /* write new section and new key to new file */
  TestWritePrivateProfileString(
    "MySection3","MyKey5","MyKey5Value",file_name);
  TestWritePrivateProfileString(
    "MySection3","MyKey6","MyKey6Value",file_name);

  /* If I Do :
     WritePrivateProfileString("MySection", NULL, NULL, file);
     It removes the entire section (with entries) */
  TestWritePrivateProfileString(
    "MySection2",NULL,NULL,file_name);
  TestGetPrivateProfileString(
    "MySection1","MyKey1","MyKey1Value",file_name);
  TestGetPrivateProfileString(
    "MySection1","MyKey2","MyKey2Value",file_name);
  TestGetPrivateProfileString(
    "MySection2","MyKey3",NULL,file_name);
  TestGetPrivateProfileString(
    "MySection2","MyKey4",NULL,file_name);
  TestGetPrivateProfileString(
    "MySection3","MyKey5","MyKey5Value",file_name);
  TestGetPrivateProfileString(
    "MySection3","MyKey6","MyKey6Value",file_name);

}

/**
* Main program entry for Unit Test
*
* @return  returns 0 on success, and non-zero on fail.
*/
int main(void)
{
  test_PrivateProfileString();
  test_PrivateProfileStringWrite();
  test_PrivateProfileStringErase();

  return 0;
}
