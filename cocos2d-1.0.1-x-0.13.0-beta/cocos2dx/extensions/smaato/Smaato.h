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
#include <pthread.h>
using namespace cocos2d::network;
#define SMA_URL "http://soma.smaato.net/oapi/reqAd.jsp"
#define REFRESH_TIME 60
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
    enum AdsStatus{
        ADS_init,
        ADS_Requesting,
        ADS_Ready
    };
    /**
     *
     * Smaato* smaato = Smaato::node();
     * scene->addChild(smaato,2);
     * scene->addChild(layer,1);//add other layer under smaato layer
     * smaato->requestAds();
     */
    class Smaato :public CCLayer
    {

    public:
        Smaato();
        virtual ~Smaato();
        void requestAds();
        void stopAds();
        void getAdsCallback(HttpClient* client, HttpResponse* response);
        void finishDownloadImage(CCSprite* sprite);
        void update(float dt);

        virtual bool init();
        LAYER_NODE_FUNC(Smaato);
        void setAdspace(int adspace){
            this->adspace =  adspace;
        }
        void setPub(int pub){
            this->pub =  pub;
        }
    protected:
        void dowloadImage(const char* url);
        int apiver;
        int adspace;
        int pub;
        char* device;
        SmaatoFormat format;
        bool requestedAds;
        AdsStatus adsStatus;
        float duration;
        bool scheduled;

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
