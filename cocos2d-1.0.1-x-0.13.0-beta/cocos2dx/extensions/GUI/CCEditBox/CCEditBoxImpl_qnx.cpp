/*
 * CCEditBoxImpl_qnx.cpp
 *
 *  Created on: Mar 19, 2016
 *      Author: tungt
 */

#include "CCEditBox.h"
#include "CCEditBoxImpl.h"
#include "CCEditBoxImpl_qnx.h"
#include <bps/virtualkeyboard.h>

NS_CC_EXT_BEGIN

CCEditBoxImpl* __createSystemEditBox(CCEditBox* pEditBox) {
    return new CCEditBoxImpl_qnx(pEditBox);
}

void CCEditBoxImpl_qnx::setText(const char* pText){

}
const char* CCEditBoxImpl_qnx::getText(void){
    return NULL;
}
void CCEditBoxImpl_qnx::setPlaceHolder(const char* pText) {

}
bool CCEditBoxImpl_qnx::initWithSize(const CCSize& size) {
    return true;
}
void CCEditBoxImpl_qnx::setFontColor(const ccColor3B& color) {

}
void CCEditBoxImpl_qnx::setPlaceholderFontColor(const ccColor3B& color) {

}
void CCEditBoxImpl_qnx::setInputMode(EditBoxInputMode inputMode) {

}
void CCEditBoxImpl_qnx::setInputFlag(EditBoxInputFlag inputFlag) {

}
void CCEditBoxImpl_qnx::setMaxLength(int maxLength) {
    m_iMaxLength = maxLength;
}
int CCEditBoxImpl_qnx::getMaxLength() {
    return m_iMaxLength;
}
void CCEditBoxImpl_qnx::setReturnType(KeyboardReturnType returnType) {
    m_iReturnType = returnType;
}
bool CCEditBoxImpl_qnx::isEditing() {
    return m_bIsEditing;
}
void CCEditBoxImpl_qnx::openKeyboard() {
    CCEGLView::sharedOpenGLView().setIMEKeyboardState(true);
}
void CCEditBoxImpl_qnx::closeKeyboard() {
    CCEGLView::sharedOpenGLView().setIMEKeyboardState(false);
}
void CCEditBoxImpl_qnx::setPosition(const CCPoint& pos) {

}
void CCEditBoxImpl_qnx::setContentSize(const CCSize& size) {

}
void CCEditBoxImpl_qnx::visit(void) {

}
void CCEditBoxImpl_qnx::doAnimationWhenKeyboardMove(float duration, float distance){

}

NS_CC_EXT_END

