/*
 * LoadingScreen.cpp
 *
 *  Created on: Mar 17, 2016
 *      Author: tungt
 */

#include "extensions/LoadingScreen/CCLoadingScreen.h"

NS_CC_EXT_BEGIN

CCLoadingScreen::CCLoadingScreen():
m_backgroundFile(NULL),
m_progressbarBorderFile(NULL),
m_progressbarFile(NULL),
m_progressDescription(NULL),
m_progressPercentage(0.0),
m_progressTimer(NULL),
m_progressPadding(10.0),
m_progressDescriptionPadding(10),
m_useDefaultDescription(true),
m_progressDescriptionFont("Arial"),
m_progressDescriptionFontSize(24)
{

}
void CCLoadingScreen::setBackgroundFile(const char* backgroundFile) {
    CC_SAFE_DELETE_ARRAY(m_backgroundFile);
    if(backgroundFile && strlen(backgroundFile)>0) {
        m_backgroundFile = new char[strlen(backgroundFile)+1];
        strcpy(m_backgroundFile,backgroundFile);
    }
}
void CCLoadingScreen::setProgressbarBorderFile(const char* progressbarBorderFile) {
    CC_SAFE_DELETE_ARRAY(m_progressbarBorderFile);
    if(progressbarBorderFile && strlen(progressbarBorderFile)>0) {
        m_progressbarBorderFile = new char[strlen(progressbarBorderFile)+1];
        strcpy(m_progressbarBorderFile,progressbarBorderFile);
    }
}
void CCLoadingScreen::setProgressbarFile(const char* progressbarFile) {
    CC_SAFE_DELETE_ARRAY(m_progressbarFile);
    if(progressbarFile && strlen(progressbarFile)>0) {
        m_progressbarFile = new char[strlen(progressbarFile)+1];
        strcpy(m_progressbarFile,progressbarFile);
    }
}
void CCLoadingScreen::setProgressPercentage(float progressPercentage) {
    m_progressPercentage = progressPercentage>100.0?100.0:progressPercentage;
    if(m_useDefaultDescription) {
        char buffer[50]={0};
        sprintf(buffer,"Loading %.2f%%",m_progressPercentage);
        setProgressDescription(buffer);
    }
}
void CCLoadingScreen::setProgressDescription(const char* progressDescription) {
    CC_SAFE_DELETE_ARRAY(m_progressDescription);
    if(progressDescription && strlen(progressDescription)>0) {
        m_progressDescription = new char[strlen(progressDescription)+1];
        strcpy(m_progressDescription,progressDescription);
    }
}
void CCLoadingScreen::setProgressPadding(float progressPadding) {
    m_progressPadding = progressPadding;
}
void CCLoadingScreen::onEnter() {
    CCLayer::onEnter();
    CCScheduler* scheduler = ccSharedScheduler();
    scheduler->scheduleSelector(schedule_selector(CCLoadingScreen::update),this,0,false);

}
void CCLoadingScreen::onExit() {
    CCLayer::onExit();
    CCScheduler* scheduler = ccSharedScheduler();
    ccUnscheduleAllSelectorsForTarget(scheduler,this);

}
#define CCLoadingScreen_DEBUG
void CCLoadingScreen::update(float dt) {
#ifdef 	CCLoadingScreen_DEBUG
    setProgressPercentage(m_progressPercentage + 0.1);
#endif
    if(m_progressTimer) {
        m_progressTimer->setPercentage(m_progressPercentage);
    }
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_1X
    if(m_progressDescription&&m_pLabel&&strcmp((const char*)m_progressDescription,m_pLabel->getString())!=0) {
        m_pLabel->setString(m_progressDescription);
    }
#endif
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_3X
    if(m_progressDescription&&m_pLabel&&strcmp((const char*)m_progressDescription,m_pLabel->getString().c_str())!=0) {
        m_pLabel->setString(m_progressDescription);
    }
#endif
}
void CCLoadingScreen::setupUI() {
    ccSize windowSize = ccGetWinSize();
    if(m_backgroundFile) {
        ccSprite* bg = ccSpriteWithFile(m_backgroundFile);
        if(bg) {
            bg->setPosition(ccp(windowSize.width/2,windowSize.height/2));
            this->addChild(bg);
        }
    }
    ccSprite* barBorder = NULL;
    m_pLabel = ccLabelWithString("Loading 0.0%", m_progressDescriptionFont,  m_progressDescriptionFontSize);
    if(m_progressbarFile) {
        m_progressTimer = ccProgressWithFile(m_progressbarFile);
        m_progressTimer->setType(kCCProgressTimerTypeHorizontalBarLR);
        m_progressTimer->setPercentage(m_progressPercentage);
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_3X
        m_progressTimer->setBarChangeRate(ccp(1,0));
        m_progressTimer->setMidpoint(ccp(0,1));

#endif
        if(m_progressbarBorderFile) {
            barBorder = ccSpriteWithFile(m_progressbarBorderFile);
            barBorder->setPosition(ccp(windowSize.width/2,barBorder->getContentSize().height/2 +m_progressPadding));
            this->addChild(barBorder);
            m_progressTimer->setAnchorPoint(ccp(0,0));
            barBorder->addChild(m_progressTimer,50);
            m_pLabel->setPosition(ccp(windowSize.width/2,barBorder->getContentSize().height + m_pLabel->getContentSize().height/2 + m_progressDescriptionPadding + m_progressPadding));
            this->addChild(m_pLabel);
        } else {
            m_progressTimer->setPosition(ccp(windowSize.width/2,m_progressTimer->getContentSize().height/2 + m_progressPadding));
            this->addChild(m_progressTimer);
            m_pLabel->setPosition(ccp(windowSize.width/2, m_progressTimer->getContentSize().height + m_pLabel->getContentSize().height/2 + m_progressDescriptionPadding+ m_progressPadding));
            this->addChild(m_pLabel);
        }
    } else {
        m_pLabel->setPosition(ccp(windowSize.width/2,m_pLabel->getContentSize().height/2 + m_progressDescriptionPadding));
        this->addChild(m_pLabel);
    }
}
CCLoadingScreen::~CCLoadingScreen()
{
    CC_SAFE_DELETE_ARRAY(m_backgroundFile);
    CC_SAFE_DELETE_ARRAY(m_progressbarBorderFile);
    CC_SAFE_DELETE_ARRAY(m_progressbarFile);
    CC_SAFE_DELETE_ARRAY(m_progressDescription);
}

NS_CC_EXT_END
