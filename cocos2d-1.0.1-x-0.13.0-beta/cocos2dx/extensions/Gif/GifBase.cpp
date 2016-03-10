#include "GifBase.h"
using namespace cocos2d;
#include "../Cocos2dHelper.h"
CCTexture2D* GifBase::createTexture(Bitmap* bm, int index, bool getCache)
{
	std::string textureName = getGifFrameName(index);
	CCTexture2D* texture = NULL;

	if(getCache)
	{
		texture = CCTextureCache::sharedTextureCache()->textureForKey(textureName.c_str());
		if(texture) return texture;
	}

	if(bm == NULL
		|| ! bm->isValid()
		|| index == UNINITIALIZED_UINT)
	{
		return NULL;
	}

	CCImage* img = new CCImage();
	do
	{
		bool res = true;
		const uint32_t* RgbaData = bm->getRGBA();
#if USING_COCOS2D_VERSION ==COCOS2D_VERSION_1X
		res = img->initWithImageData((void*)RgbaData,bm->getPixelLenth() ,CCImage::kFmtRawData, bm->m_width, bm->m_hight, 8);
#endif
#if USING_COCOS2D_VERSION ==COCOS2D_VERSION_3X
		res = img->initWithRawData((const unsigned char *)RgbaData,bm->getPixelLenth() , bm->m_width, bm->m_hight, 8);
#endif
		if(!res) break;

		CCTextureCache::sharedTextureCache()->removeTextureForKey(textureName.c_str());

        //Adding texture to CCTextureCache  to ensure that on the Android platform, when cut into the foreground from the background, the VolatileTexture can reload our texture
		texture = CCTextureCache::sharedTextureCache()->addUIImage(img, textureName.c_str());
	} while (0);

	//CC_SAFE_RELEASE(img);
	CC_SAFE_DELETE(img);
	return texture;
}
