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
#include "extensions/GUI/CCScrollView/CCScrollView.h"


#include <vector>
#include <string>

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
    ccOnSharedTouchBegan;
    ccOnSharedTouchMoved;
    ccOnSharedTouchEnded;
    ccOnSharedTouchCancelled;
    virtual void scrollViewDidScroll(ccScrollView* view) ;
    virtual void scrollViewDidZoom(ccScrollView* view) ;
    virtual void onEnter();
    virtual void onExit();
    virtual void setupUI();
    void setBackgroundFile(const char* backgroundFile);
    void addHelpFile(const char* helpFile);
    void setShowPoint(bool showPoint);
    void setActivedPointFile(const char* activedPointFile);
    void setDisablePointFile(const char* disablePointFile);
    void setPaddingPoint(float paddingPoint);
protected:
    void adjustScrollView(float offset);
protected:
    int m_page;
    char* m_backgroundFile;
    char* m_activedPointFile;
    char* m_disablePointFile;
    bool m_showPoint;
    float m_paddingPoint;
    std::vector<char*> helpFilesVector;
    std::vector<ccSprite*> helpSpriteVector;
    ccSprite *bg;
    ccPoint touchPoint;
    ccScrollView *  scrollView;

#if USING_COCOS2D_VERSION == COCOS2D_VERSION_1X
public:
    LAYER_NODE_FUNC(CCGuideScreen);
#endif
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_3X
public:
    CREATE_FUNC(CCGuideScreen);
#endif
};

NS_CC_EXT_END

#endif /* CCGUIDE_H_ */
