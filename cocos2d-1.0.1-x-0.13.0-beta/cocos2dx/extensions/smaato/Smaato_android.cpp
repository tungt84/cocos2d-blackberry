/*
 * Smaato.cpp
 *
 *  Created on: Mar 2, 2016
 *      Author: trantung
 */

#include <extensions/smaato/Smaato.h>
#ifdef __ANDROID__
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
NS_CC_BEGIN
void Smaato::openUrl(const char* target){
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t,
        "org/cocos2dx/lib/Cocos2dxHelper",
        "openURL",
        "(Ljava/lang/String;)V")) {
    jstring stringArg = t.env->NewStringUTF(target);
    t.env->CallStaticIntMethod(t.classID, t.methodID, stringArg);
    t.env->DeleteLocalRef(stringArg);
    t.env->DeleteLocalRef(t.classID);
}
}
NS_CC_END
#endif
