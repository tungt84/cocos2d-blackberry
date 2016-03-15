/*
 * CCGuide.h
 *
 *  Created on: Mar 15, 2016
 *      Author: tungt
 */

#ifndef CCGUIDE_H_
#define CCGUIDE_H_
#include "extensions/ExtensionMacros.h"
#include "extensions/Cocos2dHelper.h"
#include "cocos2d.h"
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_1X
#include "extensions/GUI/CCScrollView.h"
#endif
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_3X
#include "extensions/GUI/CCScrollView/CCScrollView.h"
#include "2d/CCLayer.h"
#endif




USING_NS_CC;
USING_NS_CC_EXT;

NS_CC_EXT_BEGIN
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_1X
#define CCGuideMake() CCGuideScreen::node()
#endif

#if USING_COCOS2D_VERSION == COCOS2D_VERSION_3X
#define CCGuideMake() CCGuideScreen::create()
#endif


class CCGuideScreen :public ccLayer, public ccScrollViewDelegate
{

public:
    CCGuideScreen();
    virtual bool init();
    virtual ~CCGuideScreen();
    virtual bool onSharedTouchBegan(ccTouch *pTouch, ccEvent *pEvent);
    virtual void onSharedTouchMoved(ccTouch *pTouch, ccEvent *pEvent);
    virtual void onSharedTouchEnded(ccTouch *pTouch, ccEvent *pEvent);
    virtual void onSharedTouchCancelled(ccTouch *pTouch, ccEvent *pEvent);
    virtual void scrollViewDidScroll(ccScrollView* view) ;
    virtual void scrollViewDidZoom(ccScrollView* view) ;
protected:
    void adjustScrollView(float offset);
protected:
    int m_page;
    ccSprite *bg;
    ccPoint touchPoint;
    ccScrollView *  scrollView;

#if USING_COCOS2D_VERSION == COCOS2D_VERSION_1X
public:
    LAYER_NODE_FUNC(CCGuideScreen);
    virtual bool ccTouchBegan(ccTouch *pTouch, ccEvent *pEvent){
        return onSharedTouchBegan(pTouch,pEvent);
    }
    virtual void ccTouchMoved(ccTouch *pTouch, ccEvent *pEvent){
        onSharedTouchMoved(pTouch,pEvent);
    }
    virtual void ccTouchEnded(ccTouch *pTouch, ccEvent *pEvent){
        onSharedTouchEnded(pTouch,pEvent);
    }
    virtual void ccTouchCancelled(ccTouch *pTouch, ccEvent *pEvent){
        onSharedTouchCancelled(pTouch,pEvent);
    }
    virtual void onEnter(){
        CCLayer::onEnter();
        CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 1, false);
    }
    virtual void onExit(){
        CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
        CCLayer::onExit();
    }
#endif
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_3X
public:
    CREATE_FUNC(CCGuideScreen);
#endif
};

NS_CC_EXT_END

#endif /* CCGUIDE_H_ */
