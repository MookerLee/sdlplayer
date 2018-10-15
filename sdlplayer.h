#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "SDL.h"

#ifdef __cplusplus
}
#endif


class CSDLPlayer
{
public:
    CSDLPlayer();
    virtual ~CSDLPlayer();

    int     Init(const void * pWnd, const Uint32 dwPixFmt = SDL_PIXELFORMAT_IYUV);
    void    Uninit();

    int     Display(const Uint32 dwPixFmt, const unsigned char * pData, const int iWidth, const int iHeight);

protected:
    SDL_Window *        _pSdlWnd;
    SDL_Renderer*       _pSdlRenderer;
    SDL_Texture *       _pSdlTexture;
    SDL_Rect            _WndRect;
    Uint32              _dwPixFmt;
    int                 _iPicWidth;
    int                 _iPicHeight;
    bool                _bInit;
};

