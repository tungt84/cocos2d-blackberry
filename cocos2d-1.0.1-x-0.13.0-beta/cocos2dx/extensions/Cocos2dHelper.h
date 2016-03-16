/*
 * Cocos2dHelper.h
 *
 *  Created on: Mar 10, 2016
 *      Author: tungt
 */

#ifndef COCOS2DHELPER_H_
#define COCOS2DHELPER_H_
#include "cocos2d.h"
#ifndef COCOS2D_VERSION_1X
#define  COCOS2D_VERSION_1X 0x00010000
#endif

#ifndef COCOS2D_VERSION_2X
#define  COCOS2D_VERSION_2X 0x00020000
#endif

#ifndef COCOS2D_VERSION_3X
#define  COCOS2D_VERSION_3X 0x00030000
#endif

#ifndef USING_COCOS2D_VERSION
#if COCOS2D_VERSION >= COCOS2D_VERSION_3X
#define USING_COCOS2D_VERSION COCOS2D_VERSION_3X
#elif COCOS2D_VERSION >= COCOS2D_VERSION_2X
#define USING_COCOS2D_VERSION COCOS2D_VERSION_2X
#elif COCOS2D_VERSION >= COCOS2D_VERSION_1X
#define USING_COCOS2D_VERSION COCOS2D_VERSION_1X
#endif
#endif


#if USING_COCOS2D_VERSION == COCOS2D_VERSION_1X
#include "extensions/GUI/CCScrollView.h"
NS_CC_BEGIN
typedef  CCSize ccSize;
typedef  CCSprite ccSprite;
typedef  CCRect ccRect;
typedef  CCLayer ccLayer;
typedef  CCTouch ccTouch;
typedef  CCEvent ccEvent;
typedef  CCPoint ccPoint;
typedef  CCTexture2D ccTexture2D;
typedef  CCSpriteFrameCache ccSpriteFrameCache;
#define ccLocationInView(target) target->locationInView()
#define ccLayerMake() CCLayer::node()
#define ccSpriteWithFile(file,...) CCSprite::spriteWithFile(file,##__VA_ARGS__)
#define ccSpriteWithSpriteFrameName(pszSpriteFrameName) CCSprite::spriteWithSpriteFrameName(pszSpriteFrameName)
#define ccFrameWithTexture(pobTexture,rect,...) CCSpriteFrame::frameWithTexture(pobTexture,rect,##__VA_ARGS__)
#define ccSharedTextureCache() CCTextureCache::sharedTextureCache()
#define ccSharedSpriteFrameCache() CCSpriteFrameCache::sharedSpriteFrameCache()
#define ccSetVisible(target,visible) target->setIsVisible(visible)
#define ccGetWinSize() CCDirector::sharedDirector()->getWinSize()
#define ccConvertToGL(p) CCDirector::sharedDirector()->convertToGL(p)
#define ccSetPosition(target,x,y) target->setPosition(ccp(x,y))
#define ccSetTextureRect(target,rect,rotated,untrimmedSize) target->setTextureRectInPixels(rect,rotated,untrimmedSize)
#define ccRectMake(x, y, width, height) CCRectMake(x, y, width, height)
#define ccSizeMake(width, height) CCSizeMake(width, height)
#define ccNodeAddTouchOneByOneListener(node,target,ccTouchBegan,ccTouchMoved,ccTouchEnded,ccTouchCancelled,priority,swallowTouch,_listener)  CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(target, priority, swallowTouch);
#define ccNodeRemoveTouchOneByOneListener(node,target,recursive)  CCTouchDispatcher::sharedDispatcher()->removeDelegate(target)
#define ccOnSharedTouchBegan virtual bool onSharedTouchBegan(ccTouch *pTouch, ccEvent *pEvent);\
		virtual bool ccTouchBegan(ccTouch *pTouch, ccEvent *pEvent){return onSharedTouchBegan(pTouch,pEvent);}
#define ccOnSharedTouchMoved virtual void onSharedTouchMoved(ccTouch *pTouch, ccEvent *pEvent);\
		virtual void ccTouchMoved(ccTouch *pTouch, ccEvent *pEvent) { onSharedTouchMoved(pTouch,pEvent);}
#define ccOnSharedTouchEnded virtual void onSharedTouchEnded(ccTouch *pTouch, ccEvent *pEvent);\
		virtual void ccTouchEnded(ccTouch *pTouch, ccEvent *pEvent){onSharedTouchEnded(pTouch,pEvent);}
#define ccOnSharedTouchCancelled virtual void onSharedTouchCancelled(ccTouch *pTouch, ccEvent *pEvent);\
		virtual void  ccTouchCancelled(ccTouch *pTouch, ccEvent *pEvent){ onSharedTouchCancelled(pTouch,pEvent); }
NS_CC_END
NS_CC_EXT_BEGIN
typedef  CCScrollViewDelegate ccScrollViewDelegate;
typedef  CCScrollView ccScrollView;
NS_CC_EXT_END
#endif

#if USING_COCOS2D_VERSION == COCOS2D_VERSION_3X
#include "extensions/GUI/CCScrollView/CCScrollView.h"

NS_CC_BEGIN
typedef Size ccSize;
typedef Sprite ccSprite;
typedef Layer ccLayer;
typedef Rect ccRect;
typedef Touch ccTouch;
typedef Event ccEvent;
typedef Vec2 ccPoint;
typedef  Texture2D ccTexture2D;
typedef  SpriteFrameCache ccSpriteFrameCache;
#define ccLocationInView(target) target->getLocationInView()
#define ccLayerMake() Layer::create()
#define ccSpriteWithFile(file,...) Sprite::create(file,##__VA_ARGS__)
#define ccSpriteWithSpriteFrameName(pszSpriteFrameName) Sprite::create(pszSpriteFrameName)
#define ccFrameWithTexture(pobTexture,rect,...) SpriteFrame::createWithTexture(pobTexture,rect,##__VA_ARGS__)
#define ccSharedTextureCache() Director::getInstance()->getTextureCache()
#define ccSharedSpriteFrameCache() SpriteFrameCache::getInstance()
#define ccSetVisible(target,visible) target->setVisible(visible)
#define ccGetWinSize() Director::getInstance()->getWinSize()
#define ccConvertToGL(p) Director::getInstance()->convertToGL(p)
#define ccSetPosition(target,x,y) target->setPosition(x,y)
#define ccSetTextureRect(target,rect,rotated,untrimmedSize) target->setTextureRect(rect,rotated,untrimmedSize)
#define ccRectMake(x, y, width, height) Rect(x, y, width, height)
#define ccSizeMake(width, height) ccSize(width, height)
#define ccOnSharedTouchBegan virtual bool onSharedTouchBegan(ccTouch *pTouch, ccEvent *pEvent)
#define ccOnSharedTouchMoved virtual void onSharedTouchMoved(ccTouch *pTouch, ccEvent *pEvent)
#define ccOnSharedTouchEnded virtual void onSharedTouchEnded(ccTouch *pTouch, ccEvent *pEvent)
#define ccOnSharedTouchCancelled virtual void onSharedTouchCancelled(ccTouch *pTouch, ccEvent *pEvent)
#define ccNodeAddTouchOneByOneListener(node,target,ccTouchBegan,ccTouchMoved,ccTouchEnded,ccTouchCancelled,priority,swallowTouch,_listener) EventListenerTouchOneByOne* _listener = EventListenerTouchOneByOne::create();\
		_listener->onTouchBegan =CC_CALLBACK_2(ccTouchBegan,target);\
		_listener->onTouchMoved =CC_CALLBACK_2(ccTouchMoved,target);\
		_listener->onTouchEnded =CC_CALLBACK_2(ccTouchEnded,target);\
		_listener->onTouchCancelled = CC_CALLBACK_2(ccTouchCancelled, target);\
		_listener->setSwallowTouches(swallowTouch);\
		node->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener,node);
#define ccNodeRemoveTouchOneByOneListener(node,target,recursive) node->getEventDispatcher()->removeEventListenersForTarget(target,recursive)

NS_CC_END

NS_CC_EXT_BEGIN
typedef ScrollViewDelegate ccScrollViewDelegate;
typedef ScrollView ccScrollView;
const ScrollView::Direction kCCScrollViewDirectionHorizontal = ScrollView::Direction::HORIZONTAL;
const ScrollView::Direction kCCScrollViewDirectionVertical = ScrollView::Direction::VERTICAL;
const ScrollView::Direction kCCScrollViewDirectionBoth = ScrollView::Direction::BOTH;
const ScrollView::Direction kCCScrollViewDirectionNone = ScrollView::Direction::NONE;
NS_CC_EXT_END
#endif

#endif /* COCOS2DHELPER_H_ */
