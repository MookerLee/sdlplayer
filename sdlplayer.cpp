
#include "SDLPlayer.h"


CSDLPlayer::CSDLPlayer() :
    _pSdlWnd(NULL),
    _pSdlRenderer(NULL),
    _pSdlTexture(NULL),
    _dwPixFmt(SDL_PIXELFORMAT_UNKNOWN),
    _iPicWidth(1280),
    _iPicHeight(720),
    _bInit(false)
{
    _WndRect.x = _WndRect.y = _WndRect.w = _WndRect.h = 0;
}



CSDLPlayer::~CSDLPlayer()
{
    Uninit();
}


int CSDLPlayer::Init(const void * pWnd, const Uint32 dwPixFmt)
{
    SDL_Event;
    int iRet = -1;
    do
    {
        _pSdlWnd = SDL_CreateWindowFrom(pWnd);
        if (!_pSdlWnd)
        {
            iRet = -2;
            break;
        }

        SDL_RendererFlags;
        _pSdlRenderer = SDL_CreateRenderer(_pSdlWnd, -1, 0);
        if (!_pSdlRenderer)
        {
            iRet = -3;
            break;
        }

        _pSdlTexture = SDL_CreateTexture(_pSdlRenderer, dwPixFmt, SDL_TEXTUREACCESS_STREAMING, _iPicWidth, _iPicHeight);
        if (!_pSdlTexture)
        {
            iRet = -4;
            break;
        }
        
        _dwPixFmt = dwPixFmt;
        SDL_GetWindowSize(_pSdlWnd, &_WndRect.w, &_WndRect.h);

        iRet = 0;
        _bInit = true;
    } while (false);

    if (iRet < 0)
        Uninit();

    return iRet;
}


void CSDLPlayer::Uninit()
{
    if (_pSdlTexture)
    {
        SDL_DestroyTexture(_pSdlTexture);
        _pSdlTexture = NULL;
    }
    if (_pSdlRenderer)
    {
        SDL_DestroyRenderer(_pSdlRenderer);
        _pSdlRenderer = NULL;
    }

    _bInit = false;
}


int CSDLPlayer::Display(const Uint32 dwPixFmt, const unsigned char * pData, const int iWidth, const int iHeight)
{
    int iRet = 0;

    if (!_bInit)
        return -1;

    if (dwPixFmt != _dwPixFmt || iWidth != _iPicWidth || iHeight != _iPicHeight)
    {
        SDL_DestroyTexture(_pSdlTexture);
        _pSdlTexture = SDL_CreateTexture(_pSdlRenderer, dwPixFmt, SDL_TEXTUREACCESS_STREAMING, iWidth, iHeight);
        if (!_pSdlTexture)
            return -2;

        _iPicWidth = iWidth;
        _iPicHeight = iHeight;
        _dwPixFmt = dwPixFmt;
    }

    int iWndWidth = 0, iWndHeight = 0;
    SDL_GetWindowSize(_pSdlWnd, &iWndWidth, &iWndHeight);

    if (iWndWidth != _WndRect.w || iWndHeight != _WndRect.h)
    {
        _WndRect.x = _WndRect.y = 0;
        _WndRect.w = iWndWidth;
        _WndRect.h = iWndHeight;

        SDL_DestroyTexture(_pSdlTexture);
        SDL_DestroyRenderer(_pSdlRenderer);

        _pSdlRenderer = SDL_CreateRenderer(_pSdlWnd, -1, 0);
        if (!_pSdlRenderer)
            return -3;

        _pSdlTexture = SDL_CreateTexture(_pSdlRenderer, dwPixFmt, SDL_TEXTUREACCESS_STREAMING, _iPicWidth, _iPicHeight);
        if (!_pSdlTexture)
            return -4;
    }

    iRet = SDL_UpdateTexture(_pSdlTexture, NULL, pData, _iPicWidth);
    if (0 != iRet)
        return -3;

    iRet = SDL_RenderClear(_pSdlRenderer);
    iRet = SDL_RenderCopy(_pSdlRenderer, _pSdlTexture, NULL, &_WndRect);
    SDL_RenderPresent(_pSdlRenderer);

    return 0;
}
