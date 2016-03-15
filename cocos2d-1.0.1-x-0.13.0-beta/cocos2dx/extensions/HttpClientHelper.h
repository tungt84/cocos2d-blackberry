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
using  cocos2d::network::HttpRequest;
#include "Cocos2dHelper.h"
NS_CC_BEGIN



#if USING_COCOS2D_VERSION == COCOS2D_VERSION_1X
#define HttpRequest_GET HttpRequest::GET
#define HttpRequest_ResponseCallbackSelector(SELECTOR,TARGET) TARGET,httpresponse_selector(SELECTOR)
#else
#define HttpRequest_GET HttpRequest::Type::GET
#define HttpRequest_ResponseCallbackSelector(SELECTOR,TARGET) CC_CALLBACK_2(SELECTOR,TARGET)
#endif



std::string urlencode(const std::string &s);


NS_CC_END



#endif /* HTTPCLIENTHELPER_H_ */
