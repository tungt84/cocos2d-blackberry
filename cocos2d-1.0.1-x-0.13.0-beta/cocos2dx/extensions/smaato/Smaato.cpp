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
        this->closeSprite = NULL;
        target = NULL;
        sprite = NULL;
        show = false;
    }

    bool Smaato::init()
    {
        /////////////////////////////
        // 1. super init first
        if (!CCLayer::init()) {
            return false;
        }
        setIsTouchEnabled(true);
        CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
        setIsVisible(false);
        return true;
    }

    void Smaato::hideAds()
    {
        setIsVisible(false);
        show = false;
    }
    void Smaato::showAds()
    {
        if (sprite != NULL && target != NULL) {
            setIsVisible(true);
        }
        show = true;

    }
    void Smaato::setImageSprite(CCSprite* sprite)
    {

        if (this->sprite) {
            CCSprite* tmp = this->sprite;
            this->sprite =NULL;
            this->removeChild(tmp, true);

        }
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        sprite->setPosition(ccp(size.width / 2, size.height / 2));
        this->addChild(sprite, 1);
        if (closeSprite) {
            closeSprite->setPosition(
                    ccp(size.width / 2 + sprite->getContentSize().width / 2,
                            size.height / 2 + sprite->getContentSize().height / 2));
        }
        if (show) {
            setIsVisible(true);
        }
        this->sprite = sprite;
    }

    bool Smaato::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
    {
        if (sprite != NULL && target != NULL) {
            CCPoint location = convertTouchToNodeSpace(pTouch);
            if (closeSprite != NULL
                    && CCRect::CCRectContainsPoint(closeSprite->boundingBox(), location)) {
                hideAds();
            } else if (CCRect::CCRectContainsPoint(sprite->boundingBox(), location)) {

                if (target) {
                    openUrl(target);
                    CC_SAFE_DELETE_ARRAY(target);
                    setIsVisible(false);
                }
            }
        }
        return false;
    }

    Smaato::~Smaato()
    {
        CC_SAFE_DELETE_ARRAY(this->target);
    }

NS_CC_END

