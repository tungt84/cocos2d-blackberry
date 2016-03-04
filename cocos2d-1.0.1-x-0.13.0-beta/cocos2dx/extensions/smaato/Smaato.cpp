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
        adsStatus = ADS_init;
        duration = 0;
        scheduled = false;
    }
    bool Smaato::init()
    {
        /////////////////////////////
        // 1. super init first
        if (!CCLayer::init()) {
            return false;
        }
        setIsVisible(false);
        setIsTouchEnabled(false);

        return true;
    }
    void Smaato::update(float dt)
    {
        duration += dt;
        if (duration > REFRESH_TIME) {
            duration = 0;
            if (requestedAds) {
                if (adsStatus == ADS_Ready || adsStatus == ADS_init) {
                    requestAds();
                }
            }
        }
    }
    void Smaato::stopAds()
    {
        requestedAds = false;
        this->unscheduleAllSelectors();
        setIsVisible(false);
        setIsTouchEnabled(false);
    }
    void Smaato::requestAds()
    {
        requestedAds = true;
        adsStatus = ADS_Requesting;
        if (!scheduled) {
            this->schedule(schedule_selector(Smaato::update));
            scheduled = true;
        }
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
    void Smaato::getAdsCallback(HttpClient* client, HttpResponse* response)
    {
        // dump data
        std::vector<char> *buffer = response->getResponseData();
        std::string data(buffer->begin(), buffer->end());
        fprintf(stderr, "Http Test, dump data: ");
        pugi::xml_document doc;

        pugi::xml_parse_result result = doc.load(data.c_str());
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
                dowloadImage(link);
                pugi::xml_node action = ad.child("action");
                fprintf(stderr, "\ntarget: %s", action.attribute("target").value());
                pugi::xml_node beacons = ad.child("beacons");
                pugi::xml_object_range<pugi::xml_named_node_iterator> beaconRange =
                        beacons.children("beacon");
                for (pugi::xml_named_node_iterator it2 = beaconRange.begin();
                        it2 != beaconRange.end(); ++it2) {
                    fprintf(stderr, "\nbeacon: %s", it2->child_value());
                }
            }
        }
        fprintf(stderr, "%s", data.c_str());
        fprintf(stderr, "\n");
    }
    void Smaato::dowloadImage(const char* url)
    {
        cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
        request->setUrl(url);
        request->setRequestType(HttpRequest_GET);
        SmaatoDownloadImage* callback = new SmaatoDownloadImage(this);
        request->setResponseCallback(
                HttpRequest_ResponseCallbackSelector(SmaatoDownloadImage::downloadImage, callback));
        request->setTag("Download Image");
        cocos2d::network::HttpClient::getInstance()->send(request);
        request->release();
    }
    void Smaato::finishDownloadImage(CCSprite* sprite)
    {

        adsStatus = ADS_Ready;

        setIsVisible(true);
        setIsTouchEnabled(true);
        sprite->setAnchorPoint(ccp(0.5, 1));
        this->addChild(sprite);
    }
    Smaato::~Smaato()
    {
    }
    SmaatoDownloadImage::SmaatoDownloadImage(Smaato* smaato)
    {
        _smaato = smaato;
    }
    void SmaatoDownloadImage::downloadImage(HttpClient* client, HttpResponse* response)
    {
        CCLOG("AppDelegate::onHttpRequestCompleted - onHttpRequestCompleted BEGIN");
        if (!response) {
            CCLOG("onHttpRequestCompleted - No Response");
            return;
        }

        CCLOG("onHttpRequestCompleted - Response code: %lu", response->getResponseCode());

        if (!response->isSucceed()) {
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
            GifBase *gif = InstantGif::create(fullFilename); //InstantGif ：While playing, while parsing
            _smaato->finishDownloadImage(gif);
        } else if (file_char[1] == 'P' && file_char[2] == 'N' && file_char[3] == 'G') {
            CCImage *image = new CCImage();
            if (image->initWithImageData((void*) file_char, buffer->size(), CCImage::kFmtPng)) {
                CCTexture2D* texture = new CCTexture2D();
                texture->initWithImage(image);
                CCSprite * sprite = CCSprite::spriteWithTexture(texture);
                _smaato->finishDownloadImage(sprite);
            }
            CC_SAFE_DELETE(image);
        } else {
            CCImage *image = new CCImage();
            if (image->initWithImageData((void*) file_char, buffer->size(), CCImage::kFmtJpg)) {
                CCTexture2D* texture = new CCTexture2D();
                texture->initWithImage(image);
                CCSprite * sprite = CCSprite::spriteWithTexture(texture);
                _smaato->finishDownloadImage(sprite);
            }
            CC_SAFE_DELETE(image);
        }
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

