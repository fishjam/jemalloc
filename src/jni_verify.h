#pragma once

#include <jni.h>
#include <jvmti.h>

#define _QUOTE(x) #x
#define QUOTE(x) _QUOTE(x)
#define __FILE__LINE__ __FILE__ "(" QUOTE(__LINE__) ") : "
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))

# define JNI_TRACE(fmt, ...) printf("%s(%d): " fmt, GetFileName(__FILE__), __LINE__, ##__VA_ARGS__)

// #define MY_LOG(level, format, ...) printf("[%s] %s:%d: " format, level, __FILE__, __LINE__, ##__VA_ARGS__)


#define REPORT_ERROR_INFO(c, e, x) \
      {\
        char errMsg[256]={0}; \
		fprintf(stderr, "!!!Error at:%s , call %s , Reason=%d(%s)\n", \
			GetFileName(__FILE__LINE__), #x, e, getJniErrMsg(e, errMsg, _countof(errMsg))); \
      }


# define JNI_VERIFY(x)\
        nResult = (x);\
        if(JNI_OK != nResult)\
        {\
            REPORT_ERROR_INFO(c, nResult, x); \
        }

#ifndef HANDLE_CASE_TO_STRING_EX
# define HANDLE_CASE_TO_STRING_EX(buf,len,c, v)\
            case (c):\
                snprintf(buf, len, "%s", v); \
            break;
#endif

# define HANDLE_CASE_TO_STRING(buf,len,c)    HANDLE_CASE_TO_STRING_EX(buf, len, c, #c)


const char* GetFileName(const char* fullPath);
char* getJniErrMsg(jint nResult, char* buf, int len);
