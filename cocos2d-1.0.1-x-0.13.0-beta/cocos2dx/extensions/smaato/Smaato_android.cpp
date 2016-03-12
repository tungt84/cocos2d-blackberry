/*
 * Smaato.cpp
 *
 *  Created on: Mar 2, 2016
 *      Author: trantung
 */

#include <extensions/smaato/Smaato.h>
#ifdef __ANDROID__
#include "cocos2d.h"
#include "platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
NS_CC_BEGIN
void Smaato::openUrl(const char* target){
	openURLJNI(target);
}
NS_CC_END
#endif
