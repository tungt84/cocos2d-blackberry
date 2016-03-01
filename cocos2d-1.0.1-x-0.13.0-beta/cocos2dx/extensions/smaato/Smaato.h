/*
 * Smaato.h
 *
 *  Created on: Mar 2, 2016
 *      Author: trantung
 */

#ifndef SMAATO_H_
#define SMAATO_H_
#include "cocos2d.h"
#include "extensions/HttpClientHelper.h"
#include <string>
NS_CC_BEGIN;
using namespace cocos2d::network;



enum SmaatoAdspaceSize {
    MMA_Small,//120x20
    MMA_Medium,//168x28
    MMA_Large,//216x36
    MMA_XLarge,//300x50
    MMA_XXLarge//320x50
};
#define SMA_URL "http://soma.smaato.net/oapi/reqAd.jsp"
class Smaato:public CCObject
{
public:
    Smaato();
    virtual ~Smaato();
    void requestAds();
    void getAdsCallback(HttpClient* client, HttpResponse* response);
protected:

};
NS_CC_END;
#endif /* SMAATO_H_ */
