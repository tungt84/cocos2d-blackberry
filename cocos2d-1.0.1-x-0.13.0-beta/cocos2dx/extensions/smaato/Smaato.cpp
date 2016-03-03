/*
 * Smaato.cpp
 *
 *  Created on: Mar 2, 2016
 *      Author: trantung
 */

#include <extensions/smaato/Smaato.h>
#include <vector>
#include <stdio.h>
#include <string>
#include <string.h>
#include <extensions/giflib/gif_lib.h>
#include <extensions/MemBuffer.h>
#include <jpeglib.h>
NS_CC_BEGIN
    typedef unsigned char UINT8;
    void DumpScreen2RGBA(ColorMapObject* ColorMap, UINT8* grb_buffer, GifRowType *ScreenBuffer,
            int ScreenWidth, int ScreenHeight)
    {
        int i, j;
        GifRowType GifRow;
        static GifColorType *ColorMapEntry;
        unsigned char *BufferP;

        for (i = 0; i < ScreenHeight; i++) {
            GifRow = ScreenBuffer[i];
            BufferP = (unsigned char *) grb_buffer + i * (ScreenWidth * 4);
            for (j = 0; j < ScreenWidth; j++) {
                ColorMapEntry = &ColorMap->Colors[GifRow[j]];
                *BufferP++ = ColorMapEntry->Blue;
                *BufferP++ = ColorMapEntry->Green;
                *BufferP++ = ColorMapEntry->Red;
                *BufferP++ = 0xff;
            }
        }
    }
    bool saveRGBBufferToJPG(const char * pszFilePath, unsigned char* pData, int imageWidth,
            int imageHeight, bool bHasAlpha)
    {
        bool bRet = false;
        do {

            CC_BREAK_IF(NULL == pszFilePath);

            struct jpeg_compress_struct cinfo;
            struct jpeg_error_mgr jerr;
            FILE * outfile; /* target file */
            JSAMPROW row_pointer[1]; /* pointer to JSAMPLE row[s] */
            int row_stride; /* physical row width in image buffer */

            cinfo.err = jpeg_std_error(&jerr);
            /* Now we can initialize the JPEG compression object. */
            jpeg_create_compress(&cinfo);

            CC_BREAK_IF((outfile = fopen(pszFilePath, "wb")) == NULL);

            jpeg_stdio_dest(&cinfo, outfile);

            cinfo.image_width = imageWidth; /* image width and height, in pixels */
            cinfo.image_height = imageHeight;
            cinfo.input_components = 3; /* # of color components per pixel */
            cinfo.in_color_space = JCS_RGB; /* colorspace of input image */

            jpeg_set_defaults(&cinfo);

            jpeg_start_compress(&cinfo, TRUE);

            row_stride = imageWidth * 3; /* JSAMPLEs per row in image_buffer */

            if (bHasAlpha) {
                unsigned char *pTempData = new unsigned char[imageWidth * imageHeight * 3];
                if (NULL == pTempData) {
                    jpeg_finish_compress(&cinfo);
                    jpeg_destroy_compress(&cinfo);
                    fclose(outfile);
                    break;
                }

                for (int i = 0; i < imageHeight; ++i) {
                    for (int j = 0; j < imageWidth; ++j)

                    {
                        pTempData[(i * imageWidth + j) * 3] = pData[(i * imageWidth + j) * 4];
                        pTempData[(i * imageWidth + j) * 3 + 1] =
                                pData[(i * imageWidth + j) * 4 + 1];
                        pTempData[(i * imageWidth + j) * 3 + 2] =
                                pData[(i * imageWidth + j) * 4 + 2];
                    }
                }

                while (cinfo.next_scanline < cinfo.image_height) {
                    row_pointer[0] = &pTempData[cinfo.next_scanline * row_stride];
                    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
                }

                CC_SAFE_DELETE_ARRAY(pTempData);
            } else {
                while (cinfo.next_scanline < cinfo.image_height) {
                    row_pointer[0] = &pData[cinfo.next_scanline * row_stride];
                    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
                }
            }

            jpeg_finish_compress(&cinfo);
            fclose(outfile);
            jpeg_destroy_compress(&cinfo);

            bRet = true;
        } while (0);
        return bRet;
    }
    bool convertGifFileToJpeg(std::string gifFilePath, std::string jpegFileSavePath)
    {
        bool isSuccess = FALSE;

        GifFileType* GifFile = NULL;
        if ((GifFile = DGifOpenFileName(gifFilePath.c_str())) == NULL) {
            //PrintGifError();
            return FALSE;
        }

        GifRowType * ScreenBuffer = NULL; //屏幕缓存
        if ((ScreenBuffer = (GifRowType *) malloc(GifFile->SHeight * sizeof(GifRowType *))) == NULL) {
            CCLOG("Failed to allocate memory required, aborted.");
            return FALSE;
        }

        int Size = GifFile->SWidth * sizeof(GifPixelType);/* Size in bytes one row.*/
        if ((ScreenBuffer[0] = (GifRowType) malloc(Size)) == NULL) /* First row. */
        {
            CCLOG("Failed to allocate memory required, aborted.");
            return FALSE;
        }

        int i = 0;
        for (i = 0; i < GifFile->SWidth; i++) /* Set its color to BackGround. */
        {
            ScreenBuffer[0][i] = GifFile->SBackGroundColor;
        }
        for (i = 1; i < GifFile->SHeight; i++) {
            /* Allocate the other rows, and set their color to background too: */
            if ((ScreenBuffer[i] = (GifRowType) malloc(Size)) == NULL) {
                CCLOG("Failed to allocate memory required, aborted.");
                return FALSE;
            }

            memcpy(ScreenBuffer[i], ScreenBuffer[0], Size);
        }

        GifWord Row = 0;
        GifWord Col = 0;
        GifWord Width = 0;
        GifWord Height = 0;
        int ImageNum = 0;
        int Count = 0;
        int j = 0;
        static int InterlacedOffset[] = { 0, 4, 2, 1 }, InterlacedJumps[] = { 8, 8, 4, 2 };
        SavedImage temp_save;
        memset(&temp_save, 0, sizeof(temp_save));

        bailin::MemBuffer buffer_rgb; //显示到屏幕上的图形

        GifRecordType RecordType = UNDEFINED_RECORD_TYPE;
        do {
            if (DGifGetRecordType(GifFile, &RecordType) == GIF_ERROR) {
                //PrintGifError();
                return FALSE;
            }

            switch (RecordType) {
                case IMAGE_DESC_RECORD_TYPE: {
                    if (DGifGetImageDesc(GifFile) == GIF_ERROR) {
                        //PrintGifError();
                        return false;
                    }
                    Row = GifFile->Image.Top; /* Image Position relative to Screen. */
                    Col = GifFile->Image.Left;
                    Width = GifFile->Image.Width;
                    Height = GifFile->Image.Height;
                    CCLOG("\n%s: Image %d at (%d, %d) [%dx%d]:     ",
                            "convert gif to jpeg", ++ImageNum, Col, Row, Width, Height);
                    if (GifFile->Image.Left + GifFile->Image.Width > GifFile->SWidth
                            || GifFile->Image.Top + GifFile->Image.Height > GifFile->SHeight) {
                        CCLOG("Image %d is not confined to screen dimension, aborted.\n",ImageNum);
                        return false;
                    }

                    if (GifFile->Image.Interlace) {
                        /* Need to perform 4 passes on the images: */
                        for (Count = i = 0; i < 4; i++)
                            for (j = Row + InterlacedOffset[i]; j < Row + Height; j +=
                                    InterlacedJumps[i]) {
                                CCLOG("\b\b\b\b%-4d", Count++);
                                if (DGifGetLine(GifFile, &ScreenBuffer[j][Col], Width) == GIF_ERROR) {
                                    //PrintGifError();
                                    return false;
                                }
                            }
                    } else {
                        for (i = 0; i < Height; i++) {
                            CCLOG("\b\b\b\b%-4d", i);
                            if (DGifGetLine(GifFile, &ScreenBuffer[Row++][Col], Width) == GIF_ERROR) {
                                //PrintGifError();
                                return false;
                            }
                        }
                    }

                    /* Get the color map */
                    ColorMapObject* ColorMap = (
                            GifFile->Image.ColorMap ? GifFile->Image.ColorMap : GifFile->SColorMap);
                    if (ColorMap == NULL) {
                        CCLOG("Gif Image does not have a colormap\n");
                        return false;
                    }

                    if (buffer_rgb.GetBufferLen() == 0) { //创建内存块
                        buffer_rgb.ReAllocBuffer(GifFile->SWidth * GifFile->SHeight * 4);
                        DumpScreen2RGBA(ColorMap, (UINT8*) (buffer_rgb.GetBuffer()), ScreenBuffer,
                                GifFile->SWidth, GifFile->SHeight);
                        CCLOG("IMAGE_DESC_RECORD_TYPE find");

                        saveRGBBufferToJPG(jpegFileSavePath.c_str(),
                                (UINT8*) (buffer_rgb.GetBuffer()), GifFile->SWidth,
                                GifFile->SHeight, true);

                        break;
                    }
                }
                    break;
                case EXTENSION_RECORD_TYPE: {
                    int gifExtCode = 0;
                    GifByteType ExtDataTemp[2];
                    GifByteType* ExtData = ExtDataTemp;
                    int status = DGifGetExtension(GifFile, &gifExtCode, &ExtData);

                    while (ExtData != NULL && status == GIF_OK) {

                        /* Create an extension block with our data */
                        if ((status = AddExtensionBlock(&temp_save, ExtData[0],
                                (unsigned char*) &(ExtData[1]))) == GIF_OK)
                            status = DGifGetExtensionNext(GifFile, &ExtData);
                        temp_save.Function = 0;
                    }

                    CCLOG("EXTENSION_RECORD_TYPE find");
                }
                    break;
                case TERMINATE_RECORD_TYPE: {
                    CCLOG("TERMINATE_RECORD_TYPE find");
                }
                    break;
                default: /* Should be traps by DGifGetRecordType. */
                    break;
            }
        } while (RecordType != TERMINATE_RECORD_TYPE);

        DGifCloseFile(GifFile);

        if (buffer_rgb.GetBufferLen() > 0) {
            return true;
        }
        return false;
    }

    Smaato::Smaato()
    {
        apiver = 501;
        adspace = 0;
        pub = 0;
        device = new char[120];
        strcpy(device,
                "Mozilla/5.0 (BB10; Kbd) AppleWebKit/537.10+ (KHTML, like Gecko) Version/10.1.0.4633 Mobile Safari/537.10+");
        format = SF_img;
    }
    void Smaato::requestAds()
    {
        cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
        cocos2d::network::HttpClient::getInstance()->enableCookies(NULL);
        //build request
        char buffer[33];
        std::string url(SMA_URL);

        //sprintf(buffer, "%d", apiver);
        //url.append("?apiver=").append(buffer);

        sprintf(buffer, "%d", adspace);
        url.append("?adspace=").append(buffer);

        sprintf(buffer, "%d", pub);
        url.append("&pub=").append(buffer);

        url.append("&device=").append(urlencode(std::string(device)));

        switch (format) {
            case SF_all:
                url.append("&format=").append("all");
                break;
            case SF_img:
                url.append("&format=").append("img");
                break;
            case SF_native:
                url.append("&format=").append("native");
                break;
            case SF_richmedia:
                url.append("&format=").append("richmedia");
                break;
            case SF_txt:
                url.append("&format=").append("txt");
                break;
            case SF_vast:
                url.append("&format=").append("vast");
                break;
        }

        url.append("&response=XML");

        request->setUrl(url.c_str());
        fprintf(stderr, "\nurl: %s", url.c_str());
        request->setRequestType(HttpRequest_GET);
        SmaatoAdsRequestCallback* callback = new SmaatoAdsRequestCallback(this);
        request->setResponseCallback(
                HttpRequest_ResponseCallbackSelector(SmaatoAdsRequestCallback::getAdsCallback,
                        callback));
        request->setTag("Request Ads");
        cocos2d::network::HttpClient::getInstance()->send(request);
        request->release();
    }
    void Smaato::getAdsCallback(HttpClient* client, HttpResponse* response)
    {
        // dump data
        std::vector<char> *buffer = response->getResponseData();
        std::string data(buffer->begin(), buffer->end());
        fprintf(stderr, "Http Test, dump data: ");
        pugi::xml_document doc;

        pugi::xml_parse_result result = doc.load(data.c_str());
        if (result.status == pugi::status_ok) {
            pugi::xml_node response = doc.child("response");

            fprintf(stderr, "\nsessionid: %s", response.child_value("sessionid"));
            fprintf(stderr, "\nstatus: %s", response.child_value("status"));

            pugi::xml_node ads = response.child("ads");
            pugi::xml_object_range<pugi::xml_named_node_iterator> adRange = ads.children("ad");
            for (pugi::xml_named_node_iterator it = adRange.begin(); it != adRange.end(); ++it) {
                pugi::xml_node ad = *it;
                const char* link = ad.child_value("link");
                fprintf(stderr, "\nlink: %s", ad.child_value("link"));
                dowloadImage(link);
                pugi::xml_node action = ad.child("action");
                fprintf(stderr, "\ntarget: %s", action.attribute("target").value());
                pugi::xml_node beacons = ad.child("beacons");
                pugi::xml_object_range<pugi::xml_named_node_iterator> beaconRange =
                        beacons.children("beacon");
                for (pugi::xml_named_node_iterator it2 = beaconRange.begin();
                        it2 != beaconRange.end(); ++it2) {
                    fprintf(stderr, "\nbeacon: %s", it2->child_value());
                }
            }
        }
        fprintf(stderr, "%s", data.c_str());
        fprintf(stderr, "\n");
    }
    void Smaato::dowloadImage(const char* url)
    {
        cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
        request->setUrl(url);
        request->setRequestType(HttpRequest_GET);
        SmaatoDownloadImage* callback = new SmaatoDownloadImage(this);
        request->setResponseCallback(
                HttpRequest_ResponseCallbackSelector(SmaatoDownloadImage::downloadImage, callback));
        request->setTag("Download Image");
        cocos2d::network::HttpClient::getInstance()->send(request);
        request->release();
    }
    void Smaato::finishDownloadImage(CCSprite* sprite)
    {
        parent->addChild(sprite);
    }
    Smaato::~Smaato()
    {
    }
    SmaatoDownloadImage::SmaatoDownloadImage(Smaato* smaato)
    {
        _smaato = smaato;
    }
    void SmaatoDownloadImage::downloadImage(HttpClient* client, HttpResponse* response)
    {
        CCLOG("AppDelegate::onHttpRequestCompleted - onHttpRequestCompleted BEGIN");
        if (!response) {
            CCLOG("onHttpRequestCompleted - No Response");
            return;
        }

        CCLOG("onHttpRequestCompleted - Response code: %lu", response->getResponseCode());

        if (!response->isSucceed()) {
            CCLOG("onHttpRequestCompleted - Error buffer: %s", response->getErrorBuffer());
            return;
        }

        CCLOG("onHttpRequestCompleted - Response code: %s", response->getResponseDataString());

        std::vector<char> *buffer = response->getResponseData();
        const char* file_char = std::string(buffer->begin(), buffer->end()).c_str();
        CCImage * image = new CCImage();
        if (image->initWithImageData((void*) file_char, buffer->size(), CCImage::kFmtPng)) {

            CCTexture2D * texture = new CCTexture2D();
            texture->initWithImage(image);
            CCSprite * sprite = CCSprite::spriteWithTexture(texture);
            //CCPoint p(visibleSize.width / 2, visibleSize.height / 2);
            //sprite->setPosition(p);
            //addChild(sprite);
            _smaato->finishDownloadImage(sprite);
            CCLOG("onHttpRequestCompleted height %f", sprite->getContentSize().height);
        }

    }
    SmaatoDownloadImage::~SmaatoDownloadImage()
    {

    }
    SmaatoAdsRequestCallback::SmaatoAdsRequestCallback(Smaato* smaato)
    {
        _smaato = smaato;
    }
    void SmaatoAdsRequestCallback::getAdsCallback(HttpClient* client, HttpResponse* response)
    {
        _smaato->getAdsCallback(client, response);
    }
    SmaatoAdsRequestCallback::~SmaatoAdsRequestCallback()
    {

    }

NS_CC_END

