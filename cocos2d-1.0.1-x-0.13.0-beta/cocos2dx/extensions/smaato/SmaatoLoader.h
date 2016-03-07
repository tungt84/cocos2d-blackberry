/*
 * SmaatoLoader.h
 *
 *  Created on: Mar 7, 2016
 *      Author: tungt
 */
#include <cocos2d.h>
#include <extensions/smaato/Smaato.h>
#include <list>
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
#ifndef SMAATOLOADER_H_
#define SMAATOLOADER_H_
NS_CC_BEGIN
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
        void getAdsCallback(HttpClient* client, HttpResponse* response);
        void downloadImage(HttpClient* client, HttpResponse* response, char* target,
                vector<char*>* beacons);

        void stopAds();
        void requestAdsView(Smaato* smaato);
        void removeAdsView();

    protected:
        void requestAdsInternal();
        void dowloadImage(const char* url, char* target, std::vector<char*>* beacons);
        void finishDownloadImage(CCSprite* sprite, char* target, std::vector<char*>* beancons);
        void downloadBeacon(char* beacon);
        void downloadBeacons(std::vector<char*>* beacons);

    protected:
        static SmaatoLoader* pInstance;
        Smaato* _smaatoInstace;
        float duration;
        bool requestedAds;
        AdsStatus adsStatus;
        pthread_mutex_t adsStatusMutex;
        int apiver;
        int adspace;
        int pub;
        char* device;
        SmaatoFormat format;
        SmaatoDimension dimension;

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
