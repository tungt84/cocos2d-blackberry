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
    ;
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

        sprintf(buffer, "%d", apiver);
        url.append("?apiver=").append(buffer);

        sprintf(buffer, "%d", adspace);
        url.append("&adspace=").append(buffer);

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
        request->setRequestType(HttpRequest_GET);
        request->setResponseCallback(
                HttpRequest_ResponseCallbackSelector(Smaato::getAdsCallback, this));
        request->setTag("GET test1");
        cocos2d::network::HttpClient::getInstance()->send(request);
        request->release();
    }
    void Smaato::getAdsCallback(HttpClient* client, HttpResponse* response)
    {
        // dump data
        std::vector<char> *buffer = response->getResponseData();
        printf("Http Test, dump data: ");
        for (unsigned int i = 0; i < buffer->size(); i++) {
            printf("%c", (*buffer)[i]);
        }
        printf("\n");
    }
    Smaato::~Smaato()
    {
        // TODO Auto-generated destructor stub
    }
NS_CC_END
;

