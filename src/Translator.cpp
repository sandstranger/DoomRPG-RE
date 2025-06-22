#include "Translator.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "DoomRPG.h"
#ifdef __cplusplus
}
#endif

#ifdef ANDROID
#include <jni.h>
#include <string>
#include <unordered_map>

#ifdef __cplusplus
extern "C" {
#endif
extern DoomRPG_t* doomRpg;
static JavaVM *g_JavaVM = nullptr;
static jclass g_TranslationManagerClass = nullptr;
static jmethodID g_IsTranslatedMethodID = nullptr;
static jmethodID g_GetTranslationMethodID = nullptr;
static jmethodID g_TranslateMethodID = nullptr;

static std::unordered_map<std::string, std::string> translationCache;

jint JNI_OnLoad(JavaVM *vm, void *) {
    g_JavaVM = vm;
    JNIEnv *env = nullptr;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    jclass localClass = env->FindClass("com/mobilerpgpack/phone/translator/TranslationManager");
    if (!localClass) return JNI_ERR;

    g_TranslationManagerClass = (jclass) env->NewGlobalRef(localClass);
    env->DeleteLocalRef(localClass);

    g_IsTranslatedMethodID = env->GetStaticMethodID(
            g_TranslationManagerClass,
            "isTranslated",
            "(Ljava/lang/String;)Z"
    );
    g_GetTranslationMethodID = env->GetStaticMethodID(
            g_TranslationManagerClass,
            "getTranslation",
            "(Ljava/lang/String;)Ljava/lang/String;"
    );
    g_TranslateMethodID = env->GetStaticMethodID(
            g_TranslationManagerClass,
            "translate",
            "(Ljava/lang/String;)Ljava/lang/String;"
    );

    if (!g_IsTranslatedMethodID || !g_GetTranslationMethodID || !g_TranslateMethodID)
        return JNI_ERR;

    return JNI_VERSION_1_6;
}

const char *GetTranslation(const char *input) {
    if (!doomRpg->enableMachineTextTranslation || !g_JavaVM || !g_TranslationManagerClass || !g_IsTranslatedMethodID ||
        !g_GetTranslationMethodID || !g_TranslateMethodID) {
        return input; // fallback
    }

    JNIEnv *env = nullptr;
    bool didAttach = false;

    if (g_JavaVM->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        if (g_JavaVM->AttachCurrentThread(&env, nullptr) != JNI_OK) {
            return input;
        }
        didAttach = true;
    }

    auto it = translationCache.find(input);
    if (it != translationCache.end()) {
        if (didAttach) g_JavaVM->DetachCurrentThread();
        return it->second.c_str();
    }

    jstring jInput = env->NewStringUTF(input);
    jboolean isTrans = env->CallStaticBooleanMethod(
            g_TranslationManagerClass,
            g_IsTranslatedMethodID,
            jInput
    );

    if (!isTrans) {
        env->CallStaticObjectMethod(
                g_TranslationManagerClass,
                g_TranslateMethodID,
                jInput
        );

        env->DeleteLocalRef(jInput);
        if (didAttach) g_JavaVM->DetachCurrentThread();
        return input;
    }

    jstring jOutput = (jstring) env->CallStaticObjectMethod(
            g_TranslationManagerClass,
            g_GetTranslationMethodID,
            jInput
    );
    env->DeleteLocalRef(jInput);

    if (jOutput == nullptr) {
        if (didAttach) g_JavaVM->DetachCurrentThread();
        return input;
    }

    const char *utfChars = env->GetStringUTFChars(jOutput, nullptr);
    if (!utfChars) {
        env->DeleteLocalRef(jOutput);
        if (didAttach) g_JavaVM->DetachCurrentThread();
        return input;
    }

    std::string translationCopy(utfChars);
    env->ReleaseStringUTFChars(jOutput, utfChars);
    env->DeleteLocalRef(jOutput);

    {
        translationCache[input] = std::move(translationCopy);
        if (didAttach) g_JavaVM->DetachCurrentThread();
        return translationCache[input].c_str();
    }
}
#ifdef __cplusplus
}
#endif
#else
#ifdef __cplusplus
extern "C" {
#endif
const char *GetTranslation(const char *input){
    return input;
}
#ifdef __cplusplus
}
#endif
#endif
