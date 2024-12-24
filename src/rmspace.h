/**
* @file
* @author Steve Karg
* @date 1997-2002
*/
#ifndef RMSPACE_H
#define RMSPACE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    size_t rmlead(char *str);
    size_t rmtrail(char *str);
    int rmquotes(char *str);
    int rmbrackets(char *str);

#ifdef TEST
#include "ctest.h"
    void test_rmlead(Test * pTest);
    void test_rmtrail(Test * pTest);
    void test_rmquotes(Test * pTest);
    void test_rmbrackets(Test * pTest);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* RMSPACE_H */
