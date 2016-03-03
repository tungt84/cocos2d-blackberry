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
#include <extensions/pugixml/pugixml.hpp>

#include <platform/CCImage.h>
#include <CCTexture2D.h>
#include <CCSprite.h>

using namespace cocos2d::network;
#define SMA_URL "http://soma.smaato.net/oapi/reqAd.jsp"
NS_CC_BEGIN

    enum SmaatoAdspaceSize
    {
        MMA_Small, //120x20
        MMA_Medium, //168x28
        MMA_Large, //216x36
        MMA_XLarge, //300x50
        MMA_XXLarge //320x50
    };
    enum SmaatoFormat
    {
        SF_all, SF_img, SF_txt, SF_richmedia, SF_vast, SF_native
    };

    class Smaato
    {
    public:
        Smaato();
        virtual ~Smaato();
        void requestAds();
        void getAdsCallback(HttpClient* client, HttpResponse* response);
        void finishDownloadImage(CCSprite* sprite);
        CCNode* parent;
    protected:
        void dowloadImage(const char* url);
        int apiver;
        int adspace;
        int pub;
        char* device;
        SmaatoFormat format;

    };
    class SmaatoDownloadImage: public Ref
    {
    public:
        void downloadImage(HttpClient* client, HttpResponse* response);
        SmaatoDownloadImage(Smaato* smaato);
        virtual ~SmaatoDownloadImage();
    private:
           Smaato* _smaato;
    };
    class SmaatoAdsRequestCallback: public Ref
    {
    public:
        void getAdsCallback(HttpClient* client, HttpResponse* response);
        SmaatoAdsRequestCallback(Smaato* smaato);
        virtual ~SmaatoAdsRequestCallback();
    private:
        Smaato* _smaato;

    };
NS_CC_END
;
#endif /* SMAATO_H_ */
