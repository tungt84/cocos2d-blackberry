/*
 * CCEditBoxImpl_qnx.h
 *
 *  Created on: Mar 19, 2016
 *      Author: tungt
 */

#ifndef CCEDITBOXIMPL_QNX_H_
#define CCEDITBOXIMPL_QNX_H_

#include "CCEditBox.h"
#include "CCEditBoxImpl.h"

NS_CC_EXT_BEGIN
class CCEditBoxImpl_qnx: public CCEditBoxImpl {
public:
    CCEditBoxImpl_qnx(CCEditBox* pEditBox) : CCEditBoxImpl(pEditBox),m_bIsEditing(false) {

    }
    virtual ~CCEditBoxImpl_qnx() {}
    virtual bool initWithSize(const CCSize& size);
    virtual void setFontColor(const ccColor3B& color);
    virtual void setPlaceholderFontColor(const ccColor3B& color);
    virtual void setInputMode(EditBoxInputMode inputMode);
    virtual void setInputFlag(EditBoxInputFlag inputFlag);
    virtual void setMaxLength(int maxLength);
    virtual int getMaxLength();
    virtual void setReturnType(KeyboardReturnType returnType);
    virtual bool isEditing();

    virtual void setText(const char* pText);
    virtual const char* getText(void);
    virtual void setPlaceHolder(const char* pText);
    virtual void doAnimationWhenKeyboardMove(float duration, float distance);

    virtual void openKeyboard();
    virtual void closeKeyboard();

    virtual void setPosition(const CCPoint& pos);
    virtual void setContentSize(const CCSize& size);
    virtual void visit(void);

protected:
    bool m_bIsEditing;
    KeyboardReturnType m_iReturnType;
    int m_iMaxLength;
    EditBoxInputFlag m_iInputFlag;
    EditBoxInputMode m_iInputMode;
    ccColor3B m_fontColor;
    ccColor3B m_placeholderFontColor;


};
NS_CC_EXT_END

#endif /* CCEDITBOXIMPL_QNX_H_ */
