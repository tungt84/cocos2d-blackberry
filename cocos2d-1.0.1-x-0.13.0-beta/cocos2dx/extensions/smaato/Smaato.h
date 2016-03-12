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
#include "extensions/pugixml/pugixml.hpp"

#include "platform/CCImage.h"


#include <pthread.h>
using namespace cocos2d::network;
#define SMA_URL "http://soma.smaato.net/oapi/reqAd.jsp"
#define REFRESH_TIME 60
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_1X
#include <CCTexture2D.h>
#include <CCSprite.h>
#define TOUCH_DISPATCHER_PRIORITY kCCMenuTouchPriority-30
#endif
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_3X
#include "renderer/CCTexture2D.h"
#include "2d/CCSprite.h"
#endif

NS_CC_BEGIN
	enum SmaatoAdsPosition{
		SMA_Pos_Top,
		SMA_Pos_Middle,
		SMA_Pos_Bottom
	};
    typedef void (cocos2d::Ref::*SEL_TargetLink)(const char* target);
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
        void openUrl(const char* target);
        virtual bool init();
#if  USING_COCOS2D_VERSION ==  COCOS2D_VERSION_1X
        virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
        LAYER_NODE_FUNC(Smaato)
#endif
#if  USING_COCOS2D_VERSION ==  COCOS2D_VERSION_3X
		virtual bool onTouchBegan(Touch *touch, Event *event);
        CREATE_FUNC(Smaato);

#endif
        void setCloseSprite(ccSprite* closeSprite);
		void updateUI(char* target,ccSprite* imageSprite);
		void setBorderColor1(ccColor3B borderColor1);
		void setBorderColor2(ccColor3B borderColor2);
		void setHasBorder(bool hasBorder);
    protected:
        char* target;
        bool hasBorder;
        SmaatoAdsPosition adsPosition;
        ccSprite* sprite;
        ccSprite *border;
        ccSprite* closeSprite;
        ccSprite *blackBorder;
        ccColor3B borderColor1;
        ccColor3B borderColor2;
        bool show;
        //_pSelector;      /// callback function, e.g. MyLaRef*                        _pTarget;        /// callback target of pSelector function
        //SEL_TargetLink           yer::onTargetLink(const char* target){navigator_invoke(target, 0);}

    };


NS_CC_END
;
#endif /* SMAATO_H_ */
