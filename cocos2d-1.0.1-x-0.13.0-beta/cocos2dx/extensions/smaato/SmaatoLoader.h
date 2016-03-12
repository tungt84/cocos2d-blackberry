/*
 * SmaatoLoader.h
 *
 *  Created on: Mar 7, 2016
 *      Author: tungt
 */
#include "cocos2d.h"
#include "extensions/smaato/Smaato.h"
#include <list>
#include "extensions/HttpClientHelper.h"
#include <string>
#include "extensions/pugixml/pugixml.hpp"

#include "platform/CCImage.h"
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_1X
#include "CCTexture2D.h"
#include "CCSprite.h"
#endif
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_3X
#include "renderer/CCTexture2D.h"
#include "2d/CCSprite.h"
#endif
#include <pthread.h>
using namespace cocos2d::network;
#define SMA_URL "http://soma.smaato.net/oapi/reqAd.jsp"
#define REFRESH_TIME 60
#ifndef SMAATOLOADER_H_
#define SMAATOLOADER_H_

#define ADS_TAG 10201
#define ADS_OZDER 10


#if USING_COCOS2D_VERSION == COCOS2D_VERSION_1X
#define ccSmaatoCreate() cocos2d::Smaato::node()
#endif

#if USING_COCOS2D_VERSION == COCOS2D_VERSION_3X
#define ccSmaatoCreate() cocos2d::Smaato::create()
#endif
NS_CC_BEGIN
	enum SmaatoAdspaceSize
   {
       MMA_Small, //120x20
       MMA_Medium, //168x28
       MMA_Large, //216x36
       MMA_XLarge, //300x50
       MMA_XXLarge //320x50
   };
   enum SmaatoDimension
   {
       D_mma, // Any MMA size
       D_medrect, //(300 x 250)
       D_sky, //(120 x 600)
       D_leader, //(728 x 90)
       D_full_320x480,
       D_full_768x1024,
       D_small
   };
   enum SmaatoFormat
   {
       SF_all, SF_img, SF_txt, SF_richmedia, SF_vast, SF_native
   };
    enum AdsStatus
    {
        ADS_NaN, ADS_init, ADS_Requesting, ADS_Ready
    };
    class SmaatoLoader: public CCObject
    {
    public:
        SmaatoLoader();
        virtual ~SmaatoLoader();
        static SmaatoLoader* sharedSmaatoLoader();
        void update(float dt);

        bool init();
        void setAdspace(int adspace);
        void setPub(int pub);
        void setAutoCheckDimesion(bool autoCheckDimesion);
        void setDimension(SmaatoDimension dimension);
        void getAdsCallback(HttpClient* client, HttpResponse* response);
        void downloadImage(HttpClient* client, HttpResponse* response, char* target,
                std::vector<char*>* beacons);

        void stopAds();
        void requestAdsView(Smaato* smaato);
        Smaato*  removeAdsView();
        void setAdsAttachedStatus(bool adsAttachedStatus);
        void setAdsZoder(int adsZoder);
        void setAdsTag(int adsTag);
    protected:
        void requestAdsInternal();
        void dowloadImage(const char* url, char* target, std::vector<char*>* beacons);
        void finishDownloadImage(CCSprite* sprite, char* target, std::vector<char*>* beancons);
        void downloadBeacon(char* beacon);
        void downloadBeacons(std::vector<char*>* beacons);

    protected:
        bool autoCheckDimesion;
        int adsTag;
        int adsAttachedStatus;
        int adsZoder;
        static SmaatoLoader* pInstance;
        Smaato* _smaatoInstace;
        float duration;
        bool requestedAds;
        AdsStatus adsStatus;
        pthread_mutex_t adsStatusMutex;
        pthread_mutex_t adsAttachedStatusMutex;
        int apiver;
        int adspace;
        int pub;
        char* device;
        SmaatoFormat format;
        SmaatoDimension dimension;
        SmaatoDimension* supportedDimension;
        int numberOfSupportDimension;

    };
    class SmaatoAdsRequestCallback: public Ref
    {
    public:
        void getAdsCallback(HttpClient* client, HttpResponse* response);
        SmaatoAdsRequestCallback(SmaatoLoader* smaatoLoader);
        virtual ~SmaatoAdsRequestCallback();
    private:
        SmaatoLoader* _smaato;

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
        SmaatoDownloadImage(SmaatoLoader* smaato, char* target, std::vector<char*>* beacons);
        virtual ~SmaatoDownloadImage();
    private:
        SmaatoLoader* _smaato;
        char* target;
        std::vector<char*>* beacons;
    };
NS_CC_END

#endif /* SMAATOLOADER_H_ */
