/*
 * CCGuide.cpp
 *
 *  Created on: Mar 15, 2016
 *      Author: tungt
 */

#include "extensions/Guide/CCGuide.h"

NS_CC_EXT_BEGIN

CCGuideScreen::CCGuideScreen():m_page(1),bg(NULL),scrollView(NULL),m_backgroundFile(NULL)
{

}
void CCGuideScreen::setBackgroundFile(const char* backgroundFile) {
    CC_SAFE_DELETE_ARRAY(m_backgroundFile);
    if(strlen(backgroundFile)>0) {
        m_backgroundFile = new char[strlen(backgroundFile)+1];
        strcpy(m_backgroundFile,backgroundFile);
    }
}
void CCGuideScreen::addHelpFile(const char* helpFile) {
    if(helpFile) {
        char* hf = new char[strlen(helpFile)+1];
        strcpy(hf,helpFile);
        helpFilesVector.push_back(hf);
    }
}
void CCGuideScreen::setupUI() {
    m_page = 1;
    ccSize size = ccGetWinSize();
    if(m_backgroundFile) {
        bg = ccSpriteWithFile(m_backgroundFile);
        bg->setAnchorPoint(CCPointZero);
        bg->setPosition(CCPointZero);
        this->addChild(bg);
    }
    if(helpFilesVector.size()>0) {
        scrollView = ccScrollView::create();
        ccLayer *layer = ccLayerMake();

        float widthPosition =0;
        for (std::vector<char*>::iterator it = helpFilesVector.begin();it!=helpFilesVector.end();it++)
        {
            ccSprite *sprite = ccSpriteWithFile(*it);
            ccSize sSize = sprite->getContentSize();
            sprite->setPosition(ccp(widthPosition+sSize.width/2,size.height/2));
            widthPosition+=sSize.width;
            layer->addChild(sprite);
        }

        layer->setAnchorPoint(CCPointZero);
        layer->setPosition(CCPointZero);

        scrollView->setPosition(CCPointZero);
        scrollView->setContentOffset(CCPointZero);
        layer->setContentSize(CCSizeMake(widthPosition, size.height));
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
    }
}
bool CCGuideScreen::init() {
    if(!CCLayer::init())
    {
        return false;
    };
    return true;
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
void CCGuideScreen::onEnter() {
    ccLayer::onEnter();
    ccNodeAddTouchOneByOneListener(this,this,
            CCGuideScreen::onSharedTouchBegan,
            CCGuideScreen::onSharedTouchMoved,
            CCGuideScreen::onSharedTouchEnded,
            CCGuideScreen::onSharedTouchCancelled,1,false,_listener);
}
void CCGuideScreen::onExit() {
    ccNodeRemoveTouchOneByOneListener(this,this,true);
    ccLayer::onExit();
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
    CC_SAFE_DELETE(m_backgroundFile);
    while(helpFilesVector.size()>0) {
        char* tmp = helpFilesVector.at(0);
        helpFilesVector.erase(helpFilesVector.begin());
        CC_SAFE_DELETE_ARRAY(tmp);
    }
}

NS_CC_EXT_END
