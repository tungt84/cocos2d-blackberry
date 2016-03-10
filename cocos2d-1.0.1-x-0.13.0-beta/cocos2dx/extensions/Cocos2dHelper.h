/*
 * Cocos2dHelper.h
 *
 *  Created on: Mar 10, 2016
 *      Author: tungt
 */

#ifndef COCOS2DHELPER_H_
#define COCOS2DHELPER_H_

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
#define ccSize CCSize
#define ccSprite CCSprite
#define ccRect CCRect
#define ccSetVisible(target,visible) target->setIsVisible(visible)
#define ccGetWinSize() CCDirector::sharedDirector()->getWinSize()
#define ccSetPosition(target,x,y) target->setPosition(ccp(x,y))
#define ccSetTextureRect(target,rect,rotated,untrimmedSize) target->setTextureRectInPixels(rect,rotated,untrimmedSize)
#define ccRectMake(x, y, width, height) CCRectMake(x, y, width, height)
#define ccSizeMake(width, height) CCSizeMake(width, height)
#endif

#if USING_COCOS2D_VERSION == COCOS2D_VERSION_3X
#define ccSize Size
#define ccSprite Sprite
#define ccColor3B Color3B
#define ccRect Rect
#define ccSetVisible(target,visible) target->setVisible(visible)
#define ccGetWinSize() Director::getInstance()->getWinSize()
#define ccSetPosition(target,x,y) target->setPosition(x,y)
#define ccSetTextureRect(target,rect,rotated,untrimmedSize) target->setTextureRect(rect,rotated,untrimmedSize)
#define ccRectMake(x, y, width, height) Rect(x, y, width, height)
#define ccSizeMake(width, height) ccSize(width, height)
#endif

#endif /* COCOS2DHELPER_H_ */
