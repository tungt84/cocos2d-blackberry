/*
 * CCGuide.cpp
 *
 *  Created on: Mar 15, 2016
 *      Author: tungt
 */

#include "extensions/Guide/CCGuide.h"

NS_CC_EXT_BEGIN

CCGuideScreen::CCGuideScreen():m_page(1),bg(NULL),scrollView(NULL)
{

}
bool CCGuideScreen::init() {
    if(!CCLayer::init())
    {
        return false;
    };
    return true;
    m_page = 1;
    ccSize size = ccGetWinSize();

    bg = ccSpriteWithFile("Help_BG.png");
    bg->setScaleX(2.0f);
    bg->setAnchorPoint(CCPointZero);
    bg->setPosition(CCPointZero);
    this->addChild(bg);

    // CCScrollView
    scrollView = ccScrollView::create();
    ccLayer *layer = ccLayerMake();
    char helpstr[30] = {0};
    for (int i=1;i<=6;i++)
    {
        sprintf(helpstr,"Help_%02d_chs.png",i);
        ccSprite *sprite = ccSpriteWithFile(helpstr);
        sprite->setScaleX(2.0);
        sprite->setScaleY(1.0);
        sprite->setPosition(ccp(size.width*(i-0.5),size.height/2+25));
        layer->addChild(sprite);
    }

    layer->setAnchorPoint(CCPointZero);
    layer->setPosition(CCPointZero);

    scrollView->setPosition(CCPointZero);
    scrollView->setContentOffset(CCPointZero);
    layer->setContentSize(CCSizeMake(480*6, 640));
    scrollView->setContentSize(layer->getContentSize());
    scrollView->setViewSize(layer->getContentSize());
    scrollView->setContainer(layer);
    scrollView->setTouchEnabled(false);
    scrollView->setDelegate(this);

    scrollView->setDirection(kCCScrollViewDirectionHorizontal);

    this->addChild(scrollView);

    ccSpriteFrameCache *cache = ccSharedSpriteFrameCache();
    ccTexture2D *text01 = ccSharedTextureCache()->addImage("Help_Point01.png");
    ccTexture2D *text02 = ccSharedTextureCache()->addImage("Help_Point02.png");

    cache->addSpriteFrame(ccFrameWithTexture(text01,CCRectMake(0,0,32,32)),"Help_Point01.png");
    cache->addSpriteFrame(ccFrameWithTexture(text02,CCRectMake(0,0,32,32)),"Help_Point02.png");
    for (int i=1;i<=6;i++)
    {
        ccSprite *point = ccSpriteWithSpriteFrameName("Help_Point01.png");
        point->setTag(100+i);
        point->setPosition(ccp(160+40*i,60));
        this->addChild(point);
    }
    ccSprite *point = (ccSprite *)this->getChildByTag(101);
    point->setDisplayFrame(cache->spriteFrameByName("Help_Point02.png"));
    return  true;
}
bool CCGuideScreen::onSharedTouchBegan(ccTouch *pTouch, ccEvent *pEvent) {
    CCLOG("begin");
    touchPoint = ccConvertToGL(ccLocationInView(pTouch));
    return true;
}
void CCGuideScreen::onSharedTouchMoved(ccTouch *pTouch, ccEvent *pEvent) {

}
void CCGuideScreen::onSharedTouchEnded(ccTouch *pTouch, ccEvent *pEvent)
{
    CCLOG("end");
    ccPoint endPoint = ccConvertToGL(ccLocationInView(pTouch));
    adjustScrollView(endPoint.x-touchPoint.x);
}
void CCGuideScreen::onSharedTouchCancelled(ccTouch *pTouch, ccEvent *pEvent) {
    ccPoint endPoint = ccConvertToGL(ccLocationInView(pTouch));
    adjustScrollView(endPoint.x-touchPoint.x);
}

void CCGuideScreen::scrollViewDidScroll(ccScrollView* view) {

}
void CCGuideScreen::scrollViewDidZoom(ccScrollView* view) {

}
void CCGuideScreen::adjustScrollView(float offset) {
    ccSize size = ccGetWinSize();
    ccSpriteFrameCache *cache = ccSharedSpriteFrameCache();
    ccSprite *point = (ccSprite *)this->getChildByTag(100+m_page);
    point->setDisplayFrame(cache->spriteFrameByName("Help_Point01.png"));
    if (offset<0)
    {
        m_page ++;

    } else
    {
        m_page --;
    }

    if (m_page <1)
    {
        m_page = 1;
    } else if(m_page > 6)
    {
        m_page =6;
    }

    point = (ccSprite *)this->getChildByTag(100+m_page);
    point->setDisplayFrame(cache->spriteFrameByName("Help_Point02.png"));

    ccPoint adjustPos = ccp(-size.width*(m_page-1),0);
    scrollView->setContentOffset(adjustPos, true);
}
CCGuideScreen::~CCGuideScreen()
{
    // TODO Auto-generated destructor stub
}

NS_CC_EXT_END
