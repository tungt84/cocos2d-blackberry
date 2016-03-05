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
typedef void (cocos2d::Ref::*SEL_TargetLink)(const char* target);
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
    enum AdsStatus
    {
        ADS_NaN,ADS_init, ADS_Requesting, ADS_Ready
    };
    /**
     *
     * Smaato* smaato = Smaato::node();
     * scene->addChild(smaato,2);
     * scene->addChild(layer,1);//add other layer under smaato layer
     * smaato->requestAds();
     */
    class Smaato: public CCLayer
    {

    public:
        Smaato();
        virtual ~Smaato();
        void requestAds();
        void hideAds();
        void showAds();
        void stopAds();
        void getAdsCallback(HttpClient* client, HttpResponse* response);
        void downloadImage(HttpClient* client, HttpResponse* response, char* target,
                vector<char*>* beacons);
        void finishDownloadImage(CCSprite* sprite, char* target, std::vector<char*>* beancons);
        void downloadBeacons(std::vector<char*>* beacons);
        void downloadBeacon(char* beacon);
        void update(float dt);
        void setTargetLinkCallback(Ref* pTarget, SEL_TargetLink pSelector);

        virtual bool init();
        virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
        LAYER_NODE_FUNC(Smaato)

        void setAdspace(int adspace)
        {
            this->adspace = adspace;
        }
        void setPub(int pub)
        {
            this->pub = pub;
        }
        void setTarget(char* target){
            CC_SAFE_DELETE_ARRAY(this->target);
            this->target =  target;
        }
    protected:
        void requestAdsInternal();
        void dowloadImage(const char* url, char* target, std::vector<char*>* beacons);
        int apiver;
        int adspace;
        int pub;
        char* device;
        SmaatoFormat format;
        bool requestedAds;
        AdsStatus adsStatus;
        pthread_mutex_t adsStatusMutex;
        float duration;
        char* target;
        CCSprite* sprite;
        Ref*                        _pTarget;        /// callback target of pSelector function
        SEL_TargetLink            _pSelector;      /// callback function, e.g. MyLayer::onTargetLink(const char* target){navigator_invoke(target, 0);}

    };
    class SmaatoDownloadBeancon: public Ref
    {
    public:
        void downloadBeacon(HttpClient* client, HttpResponse* response);
        SmaatoDownloadBeancon();
        virtual ~SmaatoDownloadBeancon();
    };
    class SmaatoDownloadImage: public Ref
    {
    public:
        void downloadImage(HttpClient* client, HttpResponse* response);
        SmaatoDownloadImage(Smaato* smaato, char* target, std::vector<char*>* beacons);
        virtual ~SmaatoDownloadImage();
    private:
        Smaato* _smaato;
        char* target;
        std::vector<char*>* beacons;
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
