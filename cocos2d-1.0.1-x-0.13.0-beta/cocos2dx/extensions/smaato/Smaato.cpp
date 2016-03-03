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
    }
    void Smaato::requestAds()
    {
        cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
        cocos2d::network::HttpClient::getInstance()->enableCookies(NULL);
        //build request
        char buffer[33];
        std::string url(SMA_URL);

        //sprintf(buffer, "%d", apiver);
        //url.append("?apiver=").append(buffer);

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
                HttpRequest_ResponseCallbackSelector(SmaatoDownloadImage::downloadImage,
                        callback));
        request->setTag("Download Image");
        cocos2d::network::HttpClient::getInstance()->send(request);
        request->release();
    }
    void Smaato::finishDownloadImage(CCSprite* sprite)
    {
        parent->addChild(sprite);
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
        const char* file_char = std::string(buffer->begin(), buffer->end()).c_str();
        CCImage * image = new CCImage();
        if (image->initWithImageData((void*) file_char, buffer->size(), CCImage::kFmtPng)) {

            CCTexture2D * texture = new CCTexture2D();
            texture->initWithImage(image);
            CCSprite * sprite = CCSprite::spriteWithTexture(texture);
            //CCPoint p(visibleSize.width / 2, visibleSize.height / 2);
            //sprite->setPosition(p);
            //addChild(sprite);
            _smaato->finishDownloadImage(sprite);
            CCLOG("onHttpRequestCompleted height %f", sprite->getContentSize().height);
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

