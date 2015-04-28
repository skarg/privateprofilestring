/**
 * @file
 * @author Ray Gardner, Bob Stout, and Steve Karg
 * @section DESCRIPTION
 *
 * This file contains the function prototypes for the stptok function.
 *
 */
#ifndef STPTOK_H
#define STPTOK_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    char *stptok(const char *s,
        char *tok,
        size_t toklen,
        const char *brk);

#ifdef TEST
#include "ctest.h"
    void testTokens(Test * pTest);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
