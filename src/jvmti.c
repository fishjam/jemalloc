#include <jvmti.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "jni_verify.h"


//MALLOC_CONF="prof:true,prof_leak:false,lg_prof_interval:28,lg_prof_sample:20,prof_prefix:${HOME}/logs/jemalloc/jeprof.out" LD_PRELOAD="/home/irteam/code/jemalloc/lib/libjemalloc.so"  java -agentpath:/home/irteam/code/jemalloc/lib/libjemalloc.so -jar /home/irteam/code/fjsdk/Java/MemoryLeakTest/target/memleak-0.0.1-SNAPSHOT.jar

FILE* fpLog = NULL;

void JNICALL VMDeath(jvmtiEnv *jvmti_env, JNIEnv* jni_env)
{
	if (fpLog != NULL)
	{
		fflush(fpLog);
		fclose(fpLog);
		fpLog = NULL;
	}
}

void JNICALL VMObjectAlloc(
	jvmtiEnv *jvmti_env,
	JNIEnv* jni_env,
	jthread thread,
	jobject object,
	jclass object_klass,
	jlong size) {
	char *class_name = NULL;
	jint nResult;

	if (size >= 10 * 1024)
	{
		// 获取对象的类名
		JNI_VERIFY((*jvmti_env)->GetClassSignature(jvmti_env, object_klass, &class_name, NULL));
		if (nResult == JVMTI_ERROR_NONE && class_name != NULL) {
			fprintf(fpLog, "Object allocated: class=%s, size=%ld\n", class_name, size);
			(*jvmti_env)->Deallocate(jvmti_env, (unsigned char*)class_name); // 释放内存

			jint frame_count = 0;
			jvmtiFrameInfo frames[10] = { 0 };
			JNI_VERIFY((*jvmti_env)->GetStackTrace(jvmti_env, thread, 0, 10, frames, &frame_count));
			for (jint i = 0; i < frame_count; i++)
			{
				jmethodID method = frames[i].method;
				char *method_name = NULL;
				char *class_name = NULL;
				char *file_name = NULL;
				jint entry_count = 0;
				jint line_number = 0;
				jvmtiLineNumberEntry *lineNumberEntry = NULL;

				JNI_VERIFY((*jvmti_env)->GetMethodName(jvmti_env, method, &method_name, NULL, NULL));

				jclass declaring_class;
				JNI_VERIFY((*jvmti_env)->GetMethodDeclaringClass(jvmti_env, method, &declaring_class));
				JNI_VERIFY((*jvmti_env)->GetClassSignature(jvmti_env, declaring_class, &class_name, NULL));

				JNI_VERIFY((*jvmti_env)->GetLineNumberTable(jvmti_env, method, &entry_count, &lineNumberEntry));
				if (JNI_OK == nResult) {
					line_number = lineNumberEntry[0].line_number;
				}
				//jvmti_env->GetLineNumber(method, frames[i].location, &line_number);

				JNI_VERIFY((*jvmti_env)->GetSourceFileName(jvmti_env, declaring_class, &file_name));

				fprintf(fpLog, "Frame %d(%p): %s.%s (File: %s, Line: %d)\n", i, method, class_name, method_name, file_name, line_number);

				JNI_VERIFY((*jvmti_env)->Deallocate(jvmti_env, (unsigned char *)method_name));
				JNI_VERIFY((*jvmti_env)->Deallocate(jvmti_env, (unsigned char *)class_name));
				JNI_VERIFY((*jvmti_env)->Deallocate(jvmti_env, (unsigned char *)file_name));
			}
		}
		else {
			printf("ERROR: Failed to get class name\n");
		}
	}

}

jint JNICALL _LocalAgentLoadOrAttach(jboolean isAttach, JavaVM *vm, char *options, void *reserved) {
	jint nResult = JNI_ERR;

	jvmtiEnv *jvmti = NULL;
	nResult = (*vm)->GetEnv(vm, (void **)&jvmti, JVMTI_VERSION_1_0);
	if (nResult != JNI_OK || jvmti == NULL) {
		return JNI_ERR;
	}
	if (isAttach) {

	}

	fpLog = fopen("/tmp/jemallo.txt", "w");
	if (!fpLog) {
		fprintf(stderr, "Couldn't open /tmp/jemallo.txt: errno(%d)", errno);
		exit(0);
	}

	jvmtiCapabilities capabilities;
	memset(&capabilities, 0, sizeof(capabilities));
	
	capabilities.can_generate_vm_object_alloc_events = 1; // 启用 VMObjectAlloc 事件能力
	capabilities.can_get_source_file_name = 1;
	capabilities.can_get_line_numbers = 1;

	// 添加 capabilities
	JNI_VERIFY((*jvmti)->AddCapabilities(jvmti, &capabilities));


	jvmtiEventCallbacks callbacks;
	memset(&callbacks, 0, sizeof(callbacks));
	callbacks.VMObjectAlloc = &VMObjectAlloc;
	callbacks.VMDeath = &VMDeath;

	JNI_VERIFY((*jvmti)->SetEventCallbacks(jvmti, &callbacks, sizeof(callbacks)));
	if (nResult != JVMTI_ERROR_NONE) {
		printf("ERROR: Unable to set JVMTI callbacks\n");
		return JNI_ERR;
	}

	JNI_VERIFY((*jvmti)->SetEventNotificationMode(jvmti, JVMTI_ENABLE, JVMTI_EVENT_VM_OBJECT_ALLOC, NULL));
	if (nResult != JVMTI_ERROR_NONE) {
		printf("ERROR: Unable to enable JVMTI event, nResult=%d\n", nResult);
		return JNI_ERR;
	}

	printf("JVMTI Agent loaded successfully.\n");

	return JNI_OK;
}


JNIEXPORT jint JNICALL Agent_OnAttach(JavaVM* vm, char* options, void* reserved)
{
    printf("!!!Enter Agent_OnAttach, vm=%p, options=%s, reserved=%p\n", vm, options, reserved);
	return _LocalAgentLoadOrAttach(JNI_FALSE, vm, options, reserved);
}

JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM* vm, char* options, void* reserved)
{
	// reserved 目前好像始终都是 NULL
	JNI_TRACE("!!!Enter Agent_OnLoad, vm=%p, options=%s, reserved=%p\n", vm, options, reserved);

	return _LocalAgentLoadOrAttach(JNI_TRUE, vm, options, reserved);
}

// JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
// 	printf("!!!Enter JNI_OnLoad, vm=%p\n", vm);
// 	jint nResult = JNI_ERR;
// 
// 	jvmtiEnv *jvmti = NULL;
// 	nResult = (*vm)->GetEnv(vm, (void **)&jvmti, JVMTI_VERSION_1_0);
// 	if (nResult != JNI_OK || jvmti == NULL) {
// 		return JNI_ERR;
// 	}
// 
// 	return JNI_OK;
// }

