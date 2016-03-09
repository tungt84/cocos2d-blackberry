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
        borderColor1 = ccWHITE;
        borderColor2 = ccORANGE;
    }

    bool Smaato::init()
    {
        /////////////////////////////
        // 1. super init first
        if (!CCLayer::init()) {
            return false;
        }
        setIsTouchEnabled(true);
        CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, TOUCH_DISPATCHER_PRIORITY,
                true);
        setIsVisible(false);
        border = new CCSprite();
        if (border && border->init()) {
            border->autorelease();
        } else {
            CC_SAFE_DELETE(border);
            border = NULL;
        }
        blackBorder = new CCSprite();
        if (blackBorder && blackBorder->init()) {
            blackBorder->autorelease();
        } else {
            CC_SAFE_DELETE(blackBorder);
            blackBorder = NULL;
        }
        if (border)
            this->addChild(border, 0);
        if (blackBorder)
            this->addChild(blackBorder, 1);
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
    void Smaato::setCloseSprite(CCSprite* closeSprite)
    {
        if (this->closeSprite) {
            CCSprite * tmp = this->closeSprite;
            this->removeChild(tmp, true);
        }
        this->closeSprite = closeSprite;
        if (closeSprite) {
            this->addChild(closeSprite, 3);
        }
    }
    void Smaato::setBorderColor1(ccColor3B borderColor1)
    {
        this->borderColor1 = borderColor1;
    }
    void Smaato::setBorderColor2(ccColor3B borderColor2)
    {
        this->borderColor2 = borderColor2;
    }
    void Smaato::updateUI(char* target, CCSprite* imageSprite)
    {
        CC_SAFE_DELETE_ARRAY(this->target);
        this->target = target;
        if (this->sprite) {
            CCSprite* tmp = this->sprite;
            this->sprite = NULL;
            this->removeChild(tmp, true);

        }
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        imageSprite->setPosition(ccp(size.width / 2, size.height / 2));
        this->addChild(imageSprite, 2);
        if (closeSprite) {
            closeSprite->setPosition(
                    ccp(size.width / 2 + imageSprite->getContentSize().width / 2,
                            size.height / 2 + imageSprite->getContentSize().height / 2));
        }
        if (show) {
            setIsVisible(true);
        }
        this->sprite = imageSprite;
        if (border) {
            border->setTextureRectInPixels(
                    CCRectMake(0, 0, imageSprite->getContentSize().width + 20,
                            imageSprite->getContentSize().height + 20), false,
                    CCSizeMake(imageSprite->getContentSize().width + 20,
                            imageSprite->getContentSize().height + 20));
            border->setPosition(ccp(size.width / 2, size.height / 2));
            border->setColor(borderColor2);

        }
        if (blackBorder) {
            blackBorder->setTextureRectInPixels(
                    CCRectMake(0, 0, imageSprite->getContentSize().width + 10,
                            imageSprite->getContentSize().height + 10), false,
                    CCSizeMake(imageSprite->getContentSize().width + 10,
                            imageSprite->getContentSize().height + 10));
            blackBorder->setPosition(ccp(size.width / 2, size.height / 2));
            blackBorder->setColor(borderColor1);

        }

    }

    bool Smaato::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
    {
        if (sprite != NULL && target != NULL) {
            CCPoint location = convertTouchToNodeSpace(pTouch);
            if (closeSprite != NULL
                    && CCRect::CCRectContainsPoint(closeSprite->boundingBox(), location)) {
                hideAds();
                return true;
            } else if (CCRect::CCRectContainsPoint(sprite->boundingBox(), location)) {

                if (target) {
                    openUrl(target);
                    CC_SAFE_DELETE_ARRAY(target);
                    setIsVisible(false);
                    return true;
                }
            }else if (CCRect::CCRectContainsPoint(border->boundingBox(), location)) {
                return true;
            }
        }
        return false;
    }

    Smaato::~Smaato()
    {
        CC_SAFE_DELETE_ARRAY(this->target);
    }

NS_CC_END

