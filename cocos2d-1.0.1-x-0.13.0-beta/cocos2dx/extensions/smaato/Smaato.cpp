/*
 * Smaato.cpp
 *
 *  Created on: Mar 2, 2016
 *      Author: trantung
 */

#include "extensions/smaato/Smaato.h"
#include <vector>
#include <stdio.h>
#include <string>
#include <string.h>
#include "extensions/Gif/InstantGif.h"

NS_CC_BEGIN

Smaato::Smaato() {
	this->closeSprite = NULL;
	target = NULL;
	sprite = NULL;
	show = false;
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_1X
	borderColor1 = ccWHITE;
	borderColor2 = ccORANGE;
#endif
#if USING_COCOS2D_VERSION ==  COCOS2D_VERSION_3X
	borderColor1 = Color3B::WHITE;
	borderColor2 = Color3B::ORANGE;
	border = NULL;
	blackBorder = NULL;
	hasBorder = false;
	adsPosition = SMA_Pos_Top;
#endif
}

bool Smaato::init() {
	/////////////////////////////
	// 1. super init first
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_1X
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
#endif
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_3X
	if (!Layer::init()) {
		return false;
	}
	setTouchEnabled(true);
	//CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, TOUCH_DISPATCHER_PRIORITY,
	//        true);
	EventListenerTouchOneByOne* _listener =
			EventListenerTouchOneByOne::create();
	_listener->setSwallowTouches(true);
	_listener->onTouchBegan = CC_CALLBACK_2(Smaato::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
	setVisible(false);
	border = Sprite::create();
	blackBorder = Sprite::create();
#endif
	if (border) {
		this->addChild(border, 0);
		if (hasBorder) {
			ccSetVisible(border, true);
		} else {
			ccSetVisible(border, false);
		}
	}
	if (blackBorder) {
		this->addChild(blackBorder, 1);
		if (hasBorder) {
			ccSetVisible(blackBorder, true);
		} else {
			ccSetVisible(blackBorder, false);
		}
	}
	return true;
}

void Smaato::hideAds() {
	ccSetVisible(this, false);
	show = false;
}
void Smaato::showAds() {
	if (sprite != NULL && target != NULL) {
		ccSetVisible(this, true);
	}
	show = true;

}
void Smaato::setCloseSprite(ccSprite* closeSprite) {
	if (this->closeSprite) {
		ccSprite * tmp = this->closeSprite;
		this->removeChild(tmp, true);
	}
	this->closeSprite = closeSprite;
	if (closeSprite) {
		this->addChild(closeSprite, 3);
	}
}
void Smaato::setBorderColor1(ccColor3B borderColor1) {
	this->borderColor1 = borderColor1;
}
void Smaato::setBorderColor2(ccColor3B borderColor2) {
	this->borderColor2 = borderColor2;
}
void Smaato::setHasBorder(bool hasBorder) {
	this->hasBorder = hasBorder;
}
void Smaato::updateUI(char* target, ccSprite* imageSprite) {
	CC_SAFE_DELETE_ARRAY(this->target);
	this->target = target;
	if (this->sprite) {
		ccSprite* tmp = this->sprite;
		this->sprite = NULL;
		this->removeChild(tmp, true);

	}
	ccSize size = ccGetWinSize();
	if (adsPosition == SMA_Pos_Middle) {
		ccSetPosition(imageSprite, size.width / 2, size.height / 2);
	}else if(adsPosition == SMA_Pos_Top){
		ccSetPosition(imageSprite, size.width / 2, size.height - imageSprite->getContentSize().width / 2);
	}
	this->addChild(imageSprite, 2);
	if (closeSprite) {
		if (adsPosition == SMA_Pos_Middle) {
			int width =
					size.width / 2 + imageSprite->getContentSize().width / 2
							+ closeSprite->getContentSize().width / 2
							> size.width ?
							(size.width / 2
									+ imageSprite->getContentSize().width / 2
									- closeSprite->getContentSize().width / 2) :
							(size.width / 2
									+ imageSprite->getContentSize().width / 2);
			int height =
					size.height / 2 + imageSprite->getContentSize().height / 2
							+ closeSprite->getContentSize().height / 2
							> size.height ?
							(size.height / 2
									+ imageSprite->getContentSize().height / 2
									- closeSprite->getContentSize().height / 2) :
							(size.height / 2
									+ imageSprite->getContentSize().height / 2);
			ccSetPosition(closeSprite, width, height);
		}
	}
	if (show) {
		ccSetVisible(this, true);
	}
	this->sprite = imageSprite;

	if (border) {
		ccRect rect = ccRectMake(0, 0, imageSprite->getContentSize().width + 20,
				imageSprite->getContentSize().height + 20);
		ccSize untrimmedSize = ccSizeMake(
				imageSprite->getContentSize().width + 20,
				imageSprite->getContentSize().height + 20);
		if (adsPosition == SMA_Pos_Middle) {
			ccSetPosition(border, size.width / 2, size.height / 2);
		}
		ccSetTextureRect(border, rect, false, untrimmedSize);
		border->setColor(borderColor2);
		if (hasBorder) {
			ccSetVisible(border, true);
		} else {
			ccSetVisible(border, false);
		}
	}
	if (blackBorder) {
		ccRect rect = ccRectMake(0, 0, imageSprite->getContentSize().width + 10,
				imageSprite->getContentSize().height + 10);
		ccSize untrimmedSize = ccSizeMake(
				imageSprite->getContentSize().width + 10,
				imageSprite->getContentSize().height + 10);
		ccSetTextureRect(blackBorder, rect, false, untrimmedSize);
		if (adsPosition == SMA_Pos_Middle) {
			ccSetPosition(blackBorder, size.width / 2, size.height / 2);
		}
		blackBorder->setColor(borderColor1);
		if (hasBorder) {
			ccSetVisible(blackBorder, true);
		} else {
			ccSetVisible(blackBorder, false);
		}

	}

}

#if USING_COCOS2D_VERSION == COCOS2D_VERSION_1X
bool Smaato::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
	if (sprite != NULL && target != NULL) {
		CCPoint location = convertTouchToNodeSpace(pTouch);
		if (closeSprite != NULL
				&& CCRect::CCRectContainsPoint(closeSprite->boundingBox(),
						location)) {
			hideAds();
			return true;
		} else if (CCRect::CCRectContainsPoint(sprite->boundingBox(),
						location)) {

			if (target) {
				openUrl(target);
				CC_SAFE_DELETE_ARRAY(target);
				setIsVisible(false);
				return true;
			}
		} else if (CCRect::CCRectContainsPoint(border->boundingBox(),
						location)) {
			return true;
		}
	}
	return false;
}
#endif
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_3X
bool Smaato::onTouchBegan(Touch *touch, Event *event) {
	if (sprite != NULL && target != NULL) {
		Vec2 location = convertTouchToNodeSpace(touch);
		if (closeSprite != NULL
				&& closeSprite->getBoundingBox().containsPoint(location)) {
			hideAds();
			return true;
		} else if (sprite->getBoundingBox().containsPoint(location)) {

			if (target) {
				openUrl(target);
				CC_SAFE_DELETE_ARRAY(target);
				setVisible(false);
				return true;
			}
		} else if (border->getBoundingBox().containsPoint(location)) {
			return true;
		}
	}
	return false;
}
#endif
Smaato::~Smaato() {
	CC_SAFE_DELETE_ARRAY(this->target);
}

NS_CC_END

