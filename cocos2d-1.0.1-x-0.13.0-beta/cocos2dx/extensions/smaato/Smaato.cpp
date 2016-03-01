/*
 * Smaato.cpp
 *
 *  Created on: Mar 2, 2016
 *      Author: trantung
 */

#include <extensions/smaato/Smaato.h>
#include <vector>
NS_CC_BEGIN;
Smaato::Smaato()
{

}
void Smaato::requestAds(){
    cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
    request->setUrl("http://www.httpbin.org/get");
    request->setRequestType(HttpRequest_GET);
    request->setResponseCallback(HttpRequest_ResponseCallbackSelector(Smaato::getAdsCallback, this) );
    request->setTag("GET test1");
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();
}
void Smaato::getAdsCallback(HttpClient* client, HttpResponse* response){
    // dump data
    std::vector<char> *buffer = response->getResponseData();
    printf("Http Test, dump data: ");
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        printf("%c", (*buffer)[i]);
    }
    printf("\n");
}
Smaato::~Smaato()
{
    // TODO Auto-generated destructor stub
}
NS_CC_END;

