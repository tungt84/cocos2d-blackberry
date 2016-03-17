/*
 * CCGuide.cpp
 *
 *  Created on: Mar 15, 2016
 *      Author: tungt
 */

#include "extensions/Guide/CCGuide.h"

NS_CC_EXT_BEGIN

CCGuideScreen::CCGuideScreen():
m_page(1),
bg(NULL),
scrollView(NULL),
m_backgroundFile(NULL),
m_showPoint(false),
m_activedPointFile(NULL),
m_disablePointFile(NULL),
m_paddingPoint(10)
{

}
void CCGuideScreen::setPaddingPoint(float paddingPoint) {
	m_paddingPoint = paddingPoint;
}
void CCGuideScreen::setActivedPointFile(const char* activedPointFile) {
	CC_SAFE_DELETE_ARRAY(m_activedPointFile);
	if(strlen(activedPointFile)>0) {
		m_activedPointFile = new char[strlen(activedPointFile)+1];
		strcpy(m_activedPointFile,activedPointFile);
	}
}
void CCGuideScreen::setDisablePointFile(const char* disablePointFile) {
	CC_SAFE_DELETE_ARRAY(m_disablePointFile);
	if(strlen(disablePointFile)>0) {
		m_disablePointFile = new char[strlen(disablePointFile)+1];
		strcpy(m_disablePointFile,disablePointFile);
	}
}
void CCGuideScreen::setShowPoint(bool showPoint) {
	m_showPoint = showPoint;
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

		float widthPosition = 0;
		float maxWith;
		float widthViewSize = 0;
		for (std::vector<char*>::iterator it = helpFilesVector.begin();it!=helpFilesVector.end();it++)
		{

			ccSprite *sprite = ccSpriteWithFile(*it);
			if(sprite) {
				ccSize sSize = sprite->getContentSize();
				maxWith = sSize.width>size.width?sSize.width:size.width;
				helpSpriteVector.push_back(sprite);
				sprite->setPosition(ccp(widthPosition+maxWith/2,size.height/2));
				widthPosition+= maxWith;
				widthViewSize+= maxWith;
				layer->addChild(sprite);
			}
		}

		layer->setAnchorPoint(CCPointZero);
		layer->setPosition(CCPointZero);
		scrollView->setPosition(CCPointZero);
		scrollView->setContentOffset(CCPointZero);
		layer->setContentSize(CCSizeMake(widthViewSize, size.height));
		scrollView->setContentSize(layer->getContentSize());
		scrollView->setViewSize(layer->getContentSize());
		scrollView->setContainer(layer);
		scrollView->setTouchEnabled(false);
		scrollView->setDelegate(this);

		scrollView->setDirection(kCCScrollViewDirectionHorizontal);

		this->addChild(scrollView);
		if(m_showPoint && m_disablePointFile &&m_activedPointFile&&helpSpriteVector.size()>0) {
			ccSpriteFrameCache *cache = ccSharedSpriteFrameCache();
			ccTexture2D *text01 = ccSharedTextureCache()->addImage(m_disablePointFile);
			ccTexture2D *text02 = ccSharedTextureCache()->addImage(m_activedPointFile);
			ccSize pointSize = text01->getContentSize();
			cache->addSpriteFrame(ccFrameWithTexture(text01,CCRectMake(0,0,pointSize.width,pointSize.height)),m_disablePointFile);
			cache->addSpriteFrame(ccFrameWithTexture(text02,CCRectMake(0,0,pointSize.width,pointSize.height)),m_activedPointFile);
			bool even = helpSpriteVector.size()%2==0;

			int mid = helpSpriteVector.size()/2;
			for (int i=0;i<helpSpriteVector.size();i++)
			{
				ccSprite *point = ccSpriteWithSpriteFrameName(m_disablePointFile);
				point->setTag(100+i+1);
				float h =pointSize.height+m_paddingPoint;
				float w;
				if(i<mid) {
					if(even) {
						w = size.width/2 - ((mid-1-i)*(m_paddingPoint+pointSize.width) + pointSize.width/2+m_paddingPoint/2);
						point->setPosition(ccp(w,h));
					} else {
						w =size.width/2 - (mid-i)*(m_paddingPoint+pointSize.width);
						point->setPosition(ccp(w,h));
					}

				} else if(i==mid) {
					if(even) {
						w = size.width/2 + ((i-mid)*(m_paddingPoint+pointSize.width) + pointSize.width/2+m_paddingPoint/2);
						point->setPosition(ccp(w,h));
					} else {
						w = size.width/2;
						point->setPosition(ccp(w,h));
					}
				} else {
					if(even) {
						w = size.width/2 + ((i-mid)*(m_paddingPoint+pointSize.width) + pointSize.width/2+m_paddingPoint/2);
						point->setPosition(ccp(w,h));
					} else {
						w = size.width/2 + ((i-mid)*(m_paddingPoint+pointSize.width));
						point->setPosition(ccp(w,h));
					}
				}
				this->addChild(point);
			}

			ccSprite *point = (ccSprite *)this->getChildByTag(101);
			point->setDisplayFrame(cache->spriteFrameByName(m_activedPointFile));
		}
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
	if(helpSpriteVector.size()==0) {
		return;
	}
	ccSize size = ccGetWinSize();
	ccSpriteFrameCache *cache = ccSharedSpriteFrameCache();
	ccSprite *point;
	if(m_showPoint&&m_disablePointFile) {
		point = (ccSprite *)this->getChildByTag(100+m_page);
		point->setDisplayFrame(cache->spriteFrameByName(m_disablePointFile));
	}
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
	} else if(m_page > helpSpriteVector.size())
	{
		m_page =helpSpriteVector.size();
	}
	if(m_showPoint&&m_activedPointFile) {
		point = (ccSprite *)this->getChildByTag(100+m_page);
		point->setDisplayFrame(cache->spriteFrameByName(m_activedPointFile));
	}

	ccPoint adjustPos = ccp(-size.width*(m_page-1),0);
	scrollView->setContentOffset(adjustPos, true);
}
CCGuideScreen::~CCGuideScreen()
{
	CC_SAFE_DELETE(m_backgroundFile);
	CC_SAFE_DELETE_ARRAY(m_activedPointFile);
	CC_SAFE_DELETE_ARRAY(m_disablePointFile);
	while(helpFilesVector.size()>0) {
		char* tmp = helpFilesVector.at(0);
		helpFilesVector.erase(helpFilesVector.begin());
		CC_SAFE_DELETE_ARRAY(tmp);
	}
}

NS_CC_EXT_END
