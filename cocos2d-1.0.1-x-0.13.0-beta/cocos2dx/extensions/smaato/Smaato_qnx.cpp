/*
 * Smaato.cpp
 *
 *  Created on: Mar 2, 2016
 *      Author: trantung
 */

#include <extensions/smaato/Smaato.h>
#ifdef __QNX__
#include <bps/navigator_invoke.h>
NS_CC_BEGIN
void Smaato::openUrl(const char* target){
    navigator_invoke(target,0);
}
NS_CC_END
#endif
