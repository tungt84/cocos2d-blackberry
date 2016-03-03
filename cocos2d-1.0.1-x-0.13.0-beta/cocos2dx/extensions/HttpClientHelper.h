/*
 * HttpClientHelper.h
 *
 *  Created on: Mar 2, 2016
 *      Author: trantung
 */

#ifndef HTTPCLIENTHELPER_H_
#define HTTPCLIENTHELPER_H_
#include "cocos2d.h"
#include "network/HttpClient.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "network/HttpCookie.h"

#include <iostream>
#include <sstream>
#include <string>

using std::cout;
using std::endl;

NS_CC_BEGIN;

#ifndef COCOS2D_VERSION_1X
#define  COCOS2D_VERSION_1X 0x00010000
#endif

#ifndef COCOS2D_VERSION_2X
#define  COCOS2D_VERSION_2X 0x00020000
#endif

#ifndef COCOS2D_VERSION_3X
#define  COCOS2D_VERSION_3X 0x00030000
#endif

#ifndef USING_COCOS2D_VERSION
#if COCOS2D_VERSION >= COCOS2D_VERSION_3X
#define USING_COCOS2D_VERSION COCOS2D_VERSION_3X
#elif COCOS2D_VERSION >= COCOS2D_VERSION_2X
#define USING_COCOS2D_VERSION COCOS2D_VERSION_2X
#elif COCOS2D_VERSION >= COCOS2D_VERSION_1X
#define USING_COCOS2D_VERSION COCOS2D_VERSION_1X
#endif
#endif

#if USING_COCOS2D_VERSION == COCOS2D_VERSION_1X
#define HttpRequest_GET cocos2d::network::HttpRequest::GET
#define HttpRequest_ResponseCallbackSelector(SELECTOR,TARGET) TARGET,httpresponse_selector(SELECTOR)
#else
#define HttpRequest_GET cocos2d::network::HttpRequest::Type::GET
#define HttpRequest_ResponseCallbackSelector(SELECTOR,TARGER) CC_CALLBACK_2(SELECTOR,TARGET)
#endif



std::string urlencode(const std::string &s);


NS_CC_END;



#endif /* HTTPCLIENTHELPER_H_ */
