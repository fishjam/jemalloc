#include "jni_verify.h"
#include <string.h>

const char* GetFileName(const char* fullPath) 
{
	const char* lastSlash = strrchr(fullPath, '/');
	const char* lastBackslash = strrchr(fullPath, '\\');

	const char* lastSeparator = (lastSlash > lastBackslash) ? lastSlash : lastBackslash;

	if (lastSeparator == NULL) {
		return fullPath;
	}

	return lastSeparator + 1;
}

char* getJniErrMsg(jint nResult, char* buf, int len)
{
    switch (nResult)
    {
        // jni.h
        HANDLE_CASE_TO_STRING(buf, len, JNI_ERR);
        HANDLE_CASE_TO_STRING(buf, len, JNI_EDETACHED);
        HANDLE_CASE_TO_STRING(buf, len, JNI_EVERSION);
        HANDLE_CASE_TO_STRING(buf, len, JNI_ENOMEM);
        HANDLE_CASE_TO_STRING(buf, len, JNI_EEXIST);
        HANDLE_CASE_TO_STRING(buf, len, JNI_EINVAL);

        //jvmti.h
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_INVALID_THREAD, "invalid_thread");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_INVALID_THREAD_GROUP, "invalid_thread_group");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_INVALID_PRIORITY, "invalid_priority");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_THREAD_NOT_SUSPENDED, "thread_not_suspended");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_THREAD_SUSPENDED, "thread_suspended");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_THREAD_NOT_ALIVE, "thread_not_alive");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_INVALID_OBJECT, "invalid object");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_INVALID_CLASS, "invalid class");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_CLASS_NOT_PREPARED, "class not prepared");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_INVALID_METHODID, "invalid method Id");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_INVALID_LOCATION, "invalid location");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_INVALID_FIELDID, "invalid field Id");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_INVALID_MODULE, "invalid module");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_NO_MORE_FRAMES, "no more frames");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_OPAQUE_FRAME, "opaque frame");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_TYPE_MISMATCH, "type mismatch");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_INVALID_SLOT, "invalid slot");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_DUPLICATE, "duplicate");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_NOT_FOUND, "not found");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_INVALID_MONITOR, "invalid monitor");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_NOT_MONITOR_OWNER, "not monitor owner");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_INTERRUPT, "interrupt");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_INVALID_CLASS_FORMAT, "invalid class format");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_CIRCULAR_CLASS_DEFINITION, "circular class definition");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_FAILS_VERIFICATION, "fails verification");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_UNSUPPORTED_REDEFINITION_METHOD_ADDED, "unsuppported ref method added");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_UNSUPPORTED_REDEFINITION_SCHEMA_CHANGED, "unsupported ref schema changed");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_INVALID_TYPESTATE, "invalid type state");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_UNSUPPORTED_REDEFINITION_HIERARCHY_CHANGED, "unsupported ref hierarchy changed");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_UNSUPPORTED_REDEFINITION_METHOD_DELETED, "unsupported ref method deleted");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_UNSUPPORTED_VERSION, "unsupported version");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_NAMES_DONT_MATCH, "names dont match");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_UNSUPPORTED_REDEFINITION_CLASS_MODIFIERS_CHANGED, "unsupported ref class modifiers changed");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_UNSUPPORTED_REDEFINITION_METHOD_MODIFIERS_CHANGED, "unsupported ref method modifiers changed");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_UNSUPPORTED_REDEFINITION_CLASS_ATTRIBUTE_CHANGED, "unsupported_redefinition_class_attribute_changed");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_UNMODIFIABLE_CLASS, "unmodifiable_class");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_UNMODIFIABLE_MODULE, "unmodifiable_module");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_NOT_AVAILABLE, "not_available");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_MUST_POSSESS_CAPABILITY, "must_possess_capability");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_NULL_POINTER, "null_pointer");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_ABSENT_INFORMATION, "absent_information");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_INVALID_EVENT_TYPE, "invalid_event_type");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_ILLEGAL_ARGUMENT, "illegal_argument");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_NATIVE_METHOD, "native_method");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_CLASS_LOADER_UNSUPPORTED, "class_loader_unsupported");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_OUT_OF_MEMORY, "out_of_memory");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_ACCESS_DENIED, "access_denied");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_WRONG_PHASE, "wrong_phase");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_INTERNAL, "internal");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_UNATTACHED_THREAD, "unattached_thread");
        HANDLE_CASE_TO_STRING_EX(buf, len, JVMTI_ERROR_INVALID_ENVIRONMENT, "invalid_environment");

    default:
        snprintf(buf, len, "unknown error:%d", nResult);
        break;
    }
    return buf;
}