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



std::string urlencode(const std::string &s)
{
    static const char lookup[]= "0123456789abcdef";
    std::stringstream e;
    for(int i=0, ix=s.length(); i<ix; i++)
    {
        const char& c = s[i];
        if ( (48 <= c && c <= 57) ||//0-9
             (65 <= c && c <= 90) ||//abc...xyz
             (97 <= c && c <= 122) || //ABC...XYZ
             (c=='-' || c=='_' || c=='.' || c=='~')
        )
        {
            e << c;
        }
        else
        {
            e << '%';
            e << lookup[ (c&0xF0)>>4 ];
            e << lookup[ (c&0x0F) ];
        }
    }
    return e.str();
}

NS_CC_END;



#endif /* HTTPCLIENTHELPER_H_ */
