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
    enum SmaatoDimension
    {
        D_mma, // Any MMA size
        D_medrect, //(300 x 250)
        D_sky, //(120 x 600)
        D_leader, //(728 x 90)
        D_full_320x480,
        D_full_768x1024
    };
    enum SmaatoFormat
    {
        SF_all, SF_img, SF_txt, SF_richmedia, SF_vast, SF_native
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
        void openUrl(const char* target);

        virtual bool init();
        virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

        LAYER_NODE_FUNC(Smaato)

        void setCloseSprite(CCSprite* closeSprite)
        {
            if (this->closeSprite) {
                this->removeChild(this->closeSprite, true);
            }
            this->closeSprite = closeSprite;
            if (closeSprite) {
                this->addChild(closeSprite, 2);
            }
        }
        void setTarget(char* target)
        {
            CC_SAFE_DELETE_ARRAY(this->target);
            this->target = target;
        }
        void setImageSprite(CCSprite* sprite);
    protected:

        char* target;
        CCSprite* sprite;
        CCSprite* closeSprite;
        bool show;
        //_pSelector;      /// callback function, e.g. MyLaRef*                        _pTarget;        /// callback target of pSelector function
        //SEL_TargetLink           yer::onTargetLink(const char* target){navigator_invoke(target, 0);}

    };


NS_CC_END
;
#endif /* SMAATO_H_ */
