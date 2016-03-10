/*
 * SmaatoLoader.cpp
 *
 *  Created on: Mar 7, 2016
 *      Author: tungt
 */

#include "extensions/smaato/SmaatoLoader.h"
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_1X
#include "CCScheduler.h"
#endif
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_3X
#include "base/CCScheduler.h"
#include "base/CCDirector.h"
#endif
#include <vector>
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "extensions/Gif/InstantGif.h"

NS_CC_BEGIN
SmaatoLoader* SmaatoLoader::pInstance = NULL;

SmaatoLoader* SmaatoLoader::sharedSmaatoLoader() {
	if (pInstance == NULL) {
		pInstance = new SmaatoLoader();
		pInstance->init();
	}
	return pInstance;
}
bool SmaatoLoader::init() {
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_1X
	CCScheduler* scheduler = CCScheduler::sharedScheduler();
	scheduler->scheduleSelector(schedule_selector(SmaatoLoader::update), this, 0, false);
#endif
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_3X
	Scheduler* scheduler = Director::getInstance()->getScheduler();
	scheduler->schedule(schedule_selector(SmaatoLoader::update), this, 0,
			false);
#endif
	ccSize supportSize[5] = { ccSizeMake(300, 250), ccSizeMake(120, 600),
				ccSizeMake(728, 90), ccSizeMake(320, 480), ccSizeMake(768, 1024) };
	SmaatoDimension dimensions[5] ={D_medrect, //(300 x 250)
				D_sky,//(120 x 600)
				D_leader,//(728 x 90)
				D_full_320x480,
				D_full_768x1024};
	ccSize windowSize = ccGetWinSize();
	std::vector<SmaatoDimension> tmp;
	for(int i=0;i<5;i++){
		if(windowSize.width>=supportSize[i].width && windowSize.height>=supportSize[i].height){
			tmp.push_back(dimensions[i]);
		}
	}
	if(tmp.size()>0){
		numberOfSupportDimension = tmp.size();
		supportedDimension =  new SmaatoDimension[numberOfSupportDimension];
		for(int i=0;i<numberOfSupportDimension;i++){
			supportedDimension[i] = tmp.at(i);
		}
	}
	return true;
}
void SmaatoLoader::setAdsAttachedStatus(bool adsAttachedStatus) {
	this->adsAttachedStatus = adsAttachedStatus;
}
void SmaatoLoader::setAdsZoder(int adsZoder) {
	this->adsZoder = adsZoder;
}
void SmaatoLoader::setAdsTag(int adsTag) {
	this->adsTag = adsTag;
}
void SmaatoLoader::update(float dt) {
	if (adsAttachedStatus) {
		CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
		if (scene) {
			int tag = scene->getTag();
			if (tag == adsTag && _smaatoInstace != NULL) {
				adsAttachedStatus = false;
				scene->addChild(_smaatoInstace, adsZoder);
			}
		}

	}
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
void SmaatoLoader::stopAds() {
	requestedAds = false;
	adsStatus = ADS_Ready;
	if (_smaatoInstace) {
		_smaatoInstace->hideAds();
	}
}

Smaato* SmaatoLoader::removeAdsView() {
	Smaato* tmp = _smaatoInstace;
	_smaatoInstace = NULL;
	return tmp;
}
void SmaatoLoader::requestAdsView(Smaato* smaato) {
	requestedAds = true;
	if (adsStatus == ADS_NaN) {
		adsStatus = ADS_init;
	}
	_smaatoInstace = smaato;
	if (_smaatoInstace) {
		_smaatoInstace->showAds();
	}
}
void SmaatoLoader::setAdspace(int adspace) {
	this->adspace = adspace;
}
void SmaatoLoader::setPub(int pub) {
	this->pub = pub;
}
void SmaatoLoader::requestAdsInternal() {

	srand (time(NULL));
	if(numberOfSupportDimension>0){
		this->dimension  =  supportedDimension[random()%numberOfSupportDimension];
	}
	bool run = false;
	pthread_mutex_lock(&adsStatusMutex);
	if (adsStatus == ADS_init || adsStatus == ADS_Ready) {
		adsStatus = ADS_Requesting;
		run = true;
	}
	pthread_mutex_unlock(&adsStatusMutex);
	if (run) {
		cocos2d::network::HttpRequest* request =
				new cocos2d::network::HttpRequest();
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
		url.append("&formatstrict=true");
		switch (dimension) {
		case D_mma:
			url.append("&dimension=mma");
			break;
		case D_full_320x480:
			url.append("&dimension=full_320x480");
			break;
		case D_full_768x1024:
			url.append("&dimension=full_768x1024");
			break;
		case D_leader:
			url.append("&dimension=leader");
			break;
		case D_medrect:
			url.append("&dimension=medrect");
			break;
		case D_sky:
			url.append("&dimension=sky");
			break;
		}
		if (dimension != D_mma) {
			url.append("&dimensionstrict=true");
		}
		url.append("&response=XML");

		request->setUrl(url.c_str());
		fprintf(stderr, "\nurl: %s", url.c_str());
		request->setRequestType(HttpRequest_GET);
		SmaatoAdsRequestCallback* callback = new SmaatoAdsRequestCallback(this);
		request->setResponseCallback(
				HttpRequest_ResponseCallbackSelector(
						SmaatoAdsRequestCallback::getAdsCallback, callback));
		request->setTag("Request Ads");
		cocos2d::network::HttpClient::getInstance()->send(request);
		request->release();
	}
}

SmaatoLoader::SmaatoLoader() {
	apiver = 501;
	adspace = 0;
	pub = 0;
	device = new char[120];
	strcpy(device,
			"Mozilla/5.0 (BB10; Kbd) AppleWebKit/537.10+ (KHTML, like Gecko) Version/10.1.0.4633 Mobile Safari/537.10+");
	format = SF_img;
	_smaatoInstace = NULL;
	requestedAds = false;
	adsStatus = ADS_NaN;
	duration = 0;
	adsStatusMutex = PTHREAD_MUTEX_INITIALIZER;
	dimension = D_mma;
	adsTag = ADS_TAG;
	adsAttachedStatus = false;
	adsZoder = ADS_OZDER;
	supportedDimension=NULL;
	numberOfSupportDimension=0;
}
void SmaatoLoader::getAdsCallback(HttpClient* client, HttpResponse* response) {
	if (!response) {
		adsStatus = ADS_Ready;
		CCLOG("onHttpRequestCompleted - No Response");
		return;
	}

	CCLOG("onHttpRequestCompleted - Response code: %lu",
			response->getResponseCode());

	if (!response->isSucceed()) {
		adsStatus = ADS_Ready;
		CCLOG("onHttpRequestCompleted - Error buffer: %s",
				response->getErrorBuffer());
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
		const char* status = response.child_value("status");
		fprintf(stderr, "\nstatus: %s", status);
		if (strcmp(status, "success") == 0) {
			pugi::xml_node ads = response.child("ads");
			pugi::xml_object_range<pugi::xml_named_node_iterator> adRange =
					ads.children("ad");
			for (pugi::xml_named_node_iterator it = adRange.begin();
					it != adRange.end(); ++it) {
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
	} else {
		adsStatus = ADS_Ready;
		CC_SAFE_DELETE_ARRAY(file_char);
	}
}
void SmaatoLoader::downloadImage(HttpClient* client, HttpResponse* response,
		char* target, std::vector<char*>* beacons) {
	CCLOG("AppDelegate::onHttpRequestCompleted - onHttpRequestCompleted BEGIN");
	if (!response) {
		adsStatus = ADS_Ready;
		CCLOG("onHttpRequestCompleted - No Response");
		return;
	}

	CCLOG("onHttpRequestCompleted - Response code: %lu",
			response->getResponseCode());

	if (!response->isSucceed()) {
		adsStatus = ADS_Ready;
		CCLOG("onHttpRequestCompleted - Error buffer: %s",
				response->getErrorBuffer());
		return;
	}

	CCLOG("onHttpRequestCompleted - Response code: %s",
			response->getResponseDataString());

	std::vector<char> *buffer = response->getResponseData();
	char* file_char = new char[buffer->size()];
	for (size_t i = 0; i < buffer->size(); i++) {
		file_char[i] = buffer->at(i);
	}
	if (file_char[0] == 'G' && file_char[1] == 'I'
			&& file_char[2] == 'F') { //gif file
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_1X
					const char* fullFilename = CCFileUtils::getWriteablePath().append("banner.gif").c_str();
#endif
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_3X
		const char* fullFilename =
				FileUtils::getInstance()->getWritablePath().append("banner.gif").c_str();
#endif
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
	} else if (file_char[1] == 'P' && file_char[2] == 'N'
			&& file_char[3] == 'G') {
#if USING_COCOS2D_VERSION ==  COCOS2D_VERSION_1X
		CCImage *image = new CCImage();
		if (image->initWithImageData((void*) file_char, buffer->size(), CCImage::kFmtPng)) {
			CCTexture2D* texture = new CCTexture2D();
			texture->initWithImage(image);
			CCSprite * sprite = CCSprite::spriteWithTexture(texture);
			finishDownloadImage(sprite, target, beacons);
		}
#endif
#if USING_COCOS2D_VERSION ==  COCOS2D_VERSION_3X
		Image *image = new Image();
		if (image->initWithImageData((const unsigned char *) file_char,
				buffer->size())) {
			Texture2D* texture = new Texture2D();
			texture->initWithImage(image);
			Sprite * sprite = Sprite::createWithTexture(texture);
			finishDownloadImage(sprite, target, beacons);
		}
#endif
		CC_SAFE_DELETE_ARRAY(file_char);
		CC_SAFE_DELETE(image);
	} else {
#if USING_COCOS2D_VERSION ==  COCOS2D_VERSION_1X
		CCImage *image = new CCImage();
		if (image->initWithImageData((void*) file_char, buffer->size(), CCImage::kFmtJpg)) {
			CCTexture2D* texture = new CCTexture2D();
			texture->initWithImage(image);
			CCSprite * sprite = CCSprite::spriteWithTexture(texture);
			finishDownloadImage(sprite, target, beacons);
		}
#endif
#if USING_COCOS2D_VERSION ==  COCOS2D_VERSION_3X
		Image *image = new Image();
		if (image->initWithImageData((const unsigned char *) file_char,
				buffer->size())) {
			Texture2D* texture = new Texture2D();
			texture->initWithImage(image);
			CCSprite * sprite = CCSprite::createWithTexture(texture);
			finishDownloadImage(sprite, target, beacons);
		}
#endif
		CC_SAFE_DELETE_ARRAY(file_char);
		CC_SAFE_DELETE(image);
	}
}
void SmaatoLoader::downloadBeacon(char* beacon) {
	cocos2d::network::HttpRequest* request =
			new cocos2d::network::HttpRequest();
	request->setUrl(beacon);
	request->setRequestType(HttpRequest_GET);
	SmaatoDownloadBeancon* callback = new SmaatoDownloadBeancon();
	request->setResponseCallback(
			HttpRequest_ResponseCallbackSelector(
					SmaatoDownloadBeancon::downloadBeacon, callback));
	request->setTag("Download Beacon");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
	CC_SAFE_DELETE_ARRAY(beacon);
}
void SmaatoLoader::downloadBeacons(std::vector<char*>* beacons) {
	while (beacons->size() > 0) {
		char* beacon = beacons->at(0);
		downloadBeacon(beacon);
		beacons->erase(beacons->begin());
	}
	CC_SAFE_DELETE(beacons);
}

void SmaatoLoader::finishDownloadImage(CCSprite* sprite, char* target,
		std::vector<char*>* beancons) {
	if (_smaatoInstace != NULL) {
		adsStatus = ADS_Ready;
		_smaatoInstace->updateUI(target, sprite);
		downloadBeacons(beancons);
	}

}
void SmaatoLoader::dowloadImage(const char* url, char* target,
		std::vector<char*>* beacons) {
	cocos2d::network::HttpRequest* request =
			new cocos2d::network::HttpRequest();
	request->setUrl(url);
	request->setRequestType(HttpRequest_GET);
	SmaatoDownloadImage* callback = new SmaatoDownloadImage(this, target,
			beacons);
	request->setResponseCallback(
			HttpRequest_ResponseCallbackSelector(
					SmaatoDownloadImage::downloadImage, callback));
	request->setTag("Download Image");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}
SmaatoLoader::~SmaatoLoader() {
	// TODO Auto-generated destructor stub
}

//SmaatoAdsRequestCallback class
SmaatoAdsRequestCallback::SmaatoAdsRequestCallback(SmaatoLoader* smaato) {
	_smaato = smaato;
}
void SmaatoAdsRequestCallback::getAdsCallback(HttpClient* client,
		HttpResponse* response) {
	_smaato->getAdsCallback(client, response);
}
SmaatoAdsRequestCallback::~SmaatoAdsRequestCallback() {

}

//SmaatoDownloadImage class
SmaatoDownloadImage::SmaatoDownloadImage(SmaatoLoader* smaato, char* target,
		std::vector<char*>* beacons) {
	_smaato = smaato;
	this->target = target;
	this->beacons = beacons;
}
void SmaatoDownloadImage::downloadImage(HttpClient* client,
		HttpResponse* response) {
	_smaato->downloadImage(client, response, target, beacons);
}
SmaatoDownloadImage::~SmaatoDownloadImage() {

}

void SmaatoDownloadBeancon::downloadBeacon(HttpClient* client,
		HttpResponse* response) {

}
SmaatoDownloadBeancon::SmaatoDownloadBeancon() {

}
SmaatoDownloadBeancon::~SmaatoDownloadBeancon() {

}

NS_CC_END
