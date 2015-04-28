/**
 * @file
 * @author Steve Karg
 * @date 1997-2004
 */
#ifndef PROFILE_H
#define PROFILE_H

#if !defined(_INC_WINDOWS)
  #include <stdio.h> // for size_t

  typedef unsigned char BOOL;

  #ifndef FALSE
    #define FALSE 0
  #endif
  #ifndef TRUE
    #define TRUE 1
  #endif

  #ifndef MAX_LINE_LEN
  #define MAX_LINE_LEN 255
  #endif

  #ifdef __cplusplus
  extern "C" {
  #endif /* __cplusplus */

  BOOL WritePrivateProfileString(
    const char *pAppName,	// pointer to section name
    const char *pKeyName,	// pointer to key name
    const char *pString,	// pointer to string to add
    const char *pFileName); 	// pointer to initialization filename

  size_t GetPrivateProfileString(
    const char *pAppName,	// points to section name
    const char *pKeyName,	// points to key name
    const char *pDefault,	// points to default string
    char *pReturnedString,	// points to destination buffer
    size_t nSize,	// size of destination buffer
    const char *pFileName); 	// points to initialization filename

  #ifdef __cplusplus
  }
  #endif /* __cplusplus */


#endif // !Windows

#endif
