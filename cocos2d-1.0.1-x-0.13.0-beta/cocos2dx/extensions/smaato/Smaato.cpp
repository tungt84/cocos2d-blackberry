/*
 * Smaato.cpp
 *
 *  Created on: Mar 2, 2016
 *      Author: trantung
 */

#include <extensions/smaato/Smaato.h>
#include <vector>
#include <stdio.h>
#include <string>
#include <string.h>
#include <extensions/Gif/InstantGif.h>
#include <CCScheduler.h>

NS_CC_BEGIN

    Smaato::Smaato()
    {
        apiver = 501;
        adspace = 0;
        pub = 0;
        device = new char[120];
        strcpy(device,
                "Mozilla/5.0 (BB10; Kbd) AppleWebKit/537.10+ (KHTML, like Gecko) Version/10.1.0.4633 Mobile Safari/537.10+");
        format = SF_img;
        requestedAds = false;
        adsStatus = ADS_NaN;
        duration = 0;
        adsStatusMutex = PTHREAD_MUTEX_INITIALIZER;
        target = NULL;
        sprite = NULL;
        _pTarget = NULL;
        _pSelector = NULL;
    }
    void Smaato::setTargetLinkCallback(Ref* pTarget, SEL_TargetLink pSelector)
    {
        _pTarget = pTarget;
        _pSelector = pSelector;

        if (_pTarget) {
            _pTarget->retain();
        }

    }
    bool Smaato::init()
    {
        /////////////////////////////
        // 1. super init first
        if (!CCLayer::init()) {
            return false;
        }
        CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,0,true);
        hideAds();
        this->schedule(schedule_selector(Smaato::update));
        return true;
    }
    void Smaato::update(float dt)
    {
        if (requestedAds && adsStatus == ADS_init) { //first time
            requestAdsInternal();
        } else {
            duration += dt;
            if (duration > REFRESH_TIME) { //need to refresh
                duration = 0;
                if (requestedAds) {
                    if (adsStatus == ADS_Ready) {
                        requestAdsInternal();
                    }
                }
            }
        }
    }
    void Smaato::hideAds(){
        setIsVisible(false);
        setIsTouchEnabled(false);
    }
    void Smaato::showAds(){
        setIsVisible(true);
        setIsTouchEnabled(true);
    }
    void Smaato::stopAds()
    {
        //this->unscheduleAllSelectors();
        requestedAds = false;
        adsStatus = ADS_Ready;
        hideAds();
    }
    void Smaato::requestAdsInternal()
    {
        bool run = false;
        pthread_mutex_lock(&adsStatusMutex);
        if (adsStatus == ADS_init || adsStatus == ADS_Ready) {
            adsStatus = ADS_Requesting;
            run = true;
        }
        pthread_mutex_unlock(&adsStatusMutex);
        if (run) {
            cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
            cocos2d::network::HttpClient::getInstance()->enableCookies(NULL);
            //build request
            char buffer[33];
            std::string url(SMA_URL);

            sprintf(buffer, "%d", adspace);
            url.append("?adspace=").append(buffer);

            sprintf(buffer, "%d", pub);
            url.append("&pub=").append(buffer);

            url.append("&device=").append(urlencode(std::string(device)));

            switch (format) {
                case SF_all:
                    url.append("&format=").append("all");
                    break;
                case SF_img:
                    url.append("&format=").append("img");
                    break;
                case SF_native:
                    url.append("&format=").append("native");
                    break;
                case SF_richmedia:
                    url.append("&format=").append("richmedia");
                    break;
                case SF_txt:
                    url.append("&format=").append("txt");
                    break;
                case SF_vast:
                    url.append("&format=").append("vast");
                    break;
            }

            url.append("&response=XML");

            request->setUrl(url.c_str());
            fprintf(stderr, "\nurl: %s", url.c_str());
            request->setRequestType(HttpRequest_GET);
            SmaatoAdsRequestCallback* callback = new SmaatoAdsRequestCallback(this);
            request->setResponseCallback(
                    HttpRequest_ResponseCallbackSelector(SmaatoAdsRequestCallback::getAdsCallback,
                            callback));
            request->setTag("Request Ads");
            cocos2d::network::HttpClient::getInstance()->send(request);
            request->release();
        }
    }
    void Smaato::requestAds()
    {
        requestedAds = true;
        if(adsStatus == ADS_NaN){
            adsStatus == ADS_init;
        }
    }
    void Smaato::getAdsCallback(HttpClient* client, HttpResponse* response)
    {
        if (!response) {
            adsStatus = ADS_Ready;
            CCLOG("onHttpRequestCompleted - No Response");
            return;
        }

        CCLOG("onHttpRequestCompleted - Response code: %lu", response->getResponseCode());

        if (!response->isSucceed()) {
            adsStatus = ADS_Ready;
            CCLOG("onHttpRequestCompleted - Error buffer: %s", response->getErrorBuffer());
            return;
        }
        // dump data
        std::vector<char> *buffer = response->getResponseData();
        char* file_char = new char[buffer->size()];
        for (size_t i = 0; i < buffer->size(); i++) {
            file_char[i] = buffer->at(i);
        }
        pugi::xml_document doc;

        pugi::xml_parse_result result = doc.load(file_char);
        if (result.status == pugi::status_ok) {
            pugi::xml_node response = doc.child("response");

            fprintf(stderr, "\nsessionid: %s", response.child_value("sessionid"));
            fprintf(stderr, "\nstatus: %s", response.child_value("status"));

            pugi::xml_node ads = response.child("ads");
            pugi::xml_object_range<pugi::xml_named_node_iterator> adRange = ads.children("ad");
            for (pugi::xml_named_node_iterator it = adRange.begin(); it != adRange.end(); ++it) {
                pugi::xml_node ad = *it;
                const char* link = ad.child_value("link");
                fprintf(stderr, "\nlink: %s", ad.child_value("link"));
                pugi::xml_node action = ad.child("action");
                const char* target = action.attribute("target").value();
                fprintf(stderr, "\ntarget: %s", target);
                char* t = new char[strlen(target) + 1];
                strcpy(t, target);
                pugi::xml_node beacons = ad.child("beacons");
                pugi::xml_object_range<pugi::xml_named_node_iterator> beaconRange =
                        beacons.children("beacon");
                std::vector<char*> *beaconsVector = new std::vector<char*>();
                for (pugi::xml_named_node_iterator it2 = beaconRange.begin();
                        it2 != beaconRange.end(); ++it2) {
                    const char * beacon = it2->child_value();
                    fprintf(stderr, "\nbeacon: %s", beacon);
                    char* b = new char[strlen(beacon) + 1];
                    strcpy(b, beacon);
                    beaconsVector->push_back(b);
                }
                dowloadImage(link, t, beaconsVector);
            }
            CC_SAFE_DELETE_ARRAY(file_char);
        } else {
            adsStatus = ADS_Ready;
            CC_SAFE_DELETE_ARRAY(file_char);
        }
    }
    void Smaato::dowloadImage(const char* url, char* target, std::vector<char*>* beacons)
    {
        cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
        request->setUrl(url);
        request->setRequestType(HttpRequest_GET);
        SmaatoDownloadImage* callback = new SmaatoDownloadImage(this, target, beacons);
        request->setResponseCallback(
                HttpRequest_ResponseCallbackSelector(SmaatoDownloadImage::downloadImage, callback));
        request->setTag("Download Image");
        cocos2d::network::HttpClient::getInstance()->send(request);
        request->release();
    }
    void Smaato::downloadBeacon(char* beacon)
    {
        cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
        request->setUrl(beacon);
        request->setRequestType(HttpRequest_GET);
        SmaatoDownloadBeancon* callback = new SmaatoDownloadBeancon();
        request->setResponseCallback(
                HttpRequest_ResponseCallbackSelector(SmaatoDownloadBeancon::downloadBeacon,
                        callback));
        request->setTag("Download Beacon");
        cocos2d::network::HttpClient::getInstance()->send(request);
        request->release();
        CC_SAFE_DELETE_ARRAY(beacon);
    }
    bool Smaato::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
    {
        if (requestedAds) {
            if (sprite != NULL && target != NULL) {
                CCPoint location = convertTouchToNodeSpace(pTouch);
                if (CCRect::CCRectContainsPoint(sprite->boundingBox(), location)) {
                    if(_pSelector&&_pTarget){
                        if(target){
                            (_pTarget->*_pSelector)(target);
                            CC_SAFE_DELETE_ARRAY(target);
                            hideAds();
                        }
                    }

                    return true;
                }
            }
        }
        return false;
    }
    void Smaato::downloadBeacons(std::vector<char*>* beacons)
    {
        while (beacons->size() > 0) {
            char* beacon = beacons->at(0);
            downloadBeacon(beacon);
            beacons->erase(beacons->begin());
        }
        CC_SAFE_DELETE(beacons);
    }
    void Smaato::finishDownloadImage(CCSprite* sprite, char* target, std::vector<char*>* beancons)
    {
        setTarget(target);
        adsStatus = ADS_Ready;
        if (requestedAds) {
            showAds();
        }
        //sprite->setAnchorPoint(ccp(0.5, 1));
        //sprite->setAnchorPoint(ccp(0,0));

        this->removeAllChildrenWithCleanup(true);
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        sprite->setPosition(
                ccp(size.width / 2, size.height - (sprite->getContentSize().height / 2)));
        this->addChild(sprite);
        this->sprite = sprite;
        downloadBeacons(beancons);

    }
    void Smaato::downloadImage(HttpClient* client, HttpResponse* response, char* target,
            vector<char*>* beacons)
    {
        CCLOG("AppDelegate::onHttpRequestCompleted - onHttpRequestCompleted BEGIN");
        if (!response) {
            adsStatus = ADS_Ready;
            CCLOG("onHttpRequestCompleted - No Response");
            return;
        }

        CCLOG("onHttpRequestCompleted - Response code: %lu", response->getResponseCode());

        if (!response->isSucceed()) {
            adsStatus = ADS_Ready;
            CCLOG("onHttpRequestCompleted - Error buffer: %s", response->getErrorBuffer());
            return;
        }

        CCLOG("onHttpRequestCompleted - Response code: %s", response->getResponseDataString());

        std::vector<char> *buffer = response->getResponseData();
        char* file_char = new char[buffer->size()];
        for (size_t i = 0; i < buffer->size(); i++) {
            file_char[i] = buffer->at(i);
        }
        if (file_char[0] == 'G' && file_char[1] == 'I' && file_char[2] == 'F') { //gif file
            const char* fullFilename = CCFileUtils::getWriteablePath().append("banner.gif").c_str();
            FILE *fp = fopen(fullFilename, "wb");

            if (!fp) {
                CCLOG("can not create file %s", fullFilename);
                return;
            }

            fwrite(file_char, 1, buffer->size(), fp);

            fclose(fp);
            CC_SAFE_DELETE_ARRAY(file_char);
            GifBase *gif = InstantGif::create(fullFilename); //InstantGif ：While playing, while parsing
            finishDownloadImage(gif, target, beacons);
        } else if (file_char[1] == 'P' && file_char[2] == 'N' && file_char[3] == 'G') {
            CCImage *image = new CCImage();
            if (image->initWithImageData((void*) file_char, buffer->size(), CCImage::kFmtPng)) {
                CCTexture2D* texture = new CCTexture2D();
                texture->initWithImage(image);
                CCSprite * sprite = CCSprite::spriteWithTexture(texture);
                finishDownloadImage(sprite, target, beacons);
            }
            CC_SAFE_DELETE_ARRAY(file_char);
            CC_SAFE_DELETE(image);
        } else {
            CCImage *image = new CCImage();
            if (image->initWithImageData((void*) file_char, buffer->size(), CCImage::kFmtJpg)) {
                CCTexture2D* texture = new CCTexture2D();
                texture->initWithImage(image);
                CCSprite * sprite = CCSprite::spriteWithTexture(texture);
                finishDownloadImage(sprite, target, beacons);
            }
            CC_SAFE_DELETE_ARRAY(file_char);
            CC_SAFE_DELETE(image);
        }
    }
    Smaato::~Smaato()
    {
        CC_SAFE_DELETE_ARRAY(this->target);
        if (_pTarget) {
            _pTarget->release();
        }
    }
    void SmaatoDownloadBeancon::downloadBeacon(HttpClient* client, HttpResponse* response)
    {

    }
    SmaatoDownloadBeancon::SmaatoDownloadBeancon()
    {

    }
    SmaatoDownloadBeancon::~SmaatoDownloadBeancon()
    {

    }
    SmaatoDownloadImage::SmaatoDownloadImage(Smaato* smaato, char* target,
            std::vector<char*>* beacons)
    {
        _smaato = smaato;
        this->target = target;
        this->beacons = beacons;
    }
    void SmaatoDownloadImage::downloadImage(HttpClient* client, HttpResponse* response)
    {
        _smaato->downloadImage(client, response, target, beacons);
    }
    SmaatoDownloadImage::~SmaatoDownloadImage()
    {

    }
    SmaatoAdsRequestCallback::SmaatoAdsRequestCallback(Smaato* smaato)
    {
        _smaato = smaato;
    }
    void SmaatoAdsRequestCallback::getAdsCallback(HttpClient* client, HttpResponse* response)
    {
        _smaato->getAdsCallback(client, response);
    }
    SmaatoAdsRequestCallback::~SmaatoAdsRequestCallback()
    {

    }

NS_CC_END

