/*
 * LoadingScreen.h
 *
 *  Created on: Mar 17, 2016
 *      Author: tungt
 */

#ifndef LOADINGSCREEN_H_
#define LOADINGSCREEN_H_
#include "extensions/Cocos2dHelper.h"

NS_CC_EXT_BEGIN

#if USING_COCOS2D_VERSION == COCOS2D_VERSION_1X
#define CCLoadingScreenMake() CCLoadingScreen::node()
#endif

#if USING_COCOS2D_VERSION == COCOS2D_VERSION_3X
#define CCLoadingScreenMake() CCLoadingScreen::create()
#endif

class CCLoadingScreen: public ccLayer
{
public:
    void setupUI();
    CCLoadingScreen();
    virtual ~CCLoadingScreen();
    void setBackgroundFile(const char* backgroudFile);
    void setProgressbarBorderFile(const char* progressbarBorderFile);
    void setProgressbarFile(const char* progressbarFile);
    void setProgressPercentage(float progressPercentage);
    void setProgressDescription(const char* progressDescription);
    void setProgressPadding(float progressPadding);
    virtual void update(float dt);
    virtual void onEnter();
    virtual void onExit();
protected:
    char* m_backgroundFile;
    char* m_progressbarBorderFile;
    char* m_progressbarFile;
    char* m_progressDescription;
    float m_progressPercentage;
    float m_progressPadding;
    float m_progressDescriptionPadding;
    bool m_useDefaultDescription;
    char* m_progressDescriptionFont;
    float m_progressDescriptionFontSize;
    CCLabelTTF* m_pLabel ;
    CCProgressTimer* m_progressTimer;
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_1X
public:
    LAYER_NODE_FUNC(CCLoadingScreen);
#endif
#if USING_COCOS2D_VERSION == COCOS2D_VERSION_3X
public:
    CREATE_FUNC(CCLoadingScreen);
#endif
};

NS_CC_EXT_END

#endif /* LOADINGSCREEN_H_ */
