/*
Copyright (C) 2002 Rice1964

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifndef _DLLINTERFACE_H_
#define _DLLINTERFACE_H_

#ifdef M64P_STATIC_PLUGINS
#define M64P_CORE_PROTOTYPES 1
#endif

#if defined(__GNUC__)
#define ATTR_FMT(fmtpos, attrpos) __attribute__ ((format (printf, fmtpos, attrpos)))
#else
#define ATTR_FMT(fmtpos, attrpos)
#endif

#define M64P_PLUGIN_PROTOTYPES 1
#include "m64p_config.h"
#include "m64p_plugin.h"
#include "m64p_vidext.h"

#if (!M64P_STATIC_PLUGINS)

#include "osal_preproc.h"
#include "typedefs.h"

#else
#include "stdbool.h"
#define uchar  unsigned char
#define uint16 unsigned short
#define uint32 unsigned int
#define uint64 unsigned long long

typedef unsigned int BOOL;
#endif

#ifdef __cplusplus
extern "C" {
#endif

EXPORT m64p_error CALL PluginStartupVideo(m64p_dynlib_handle, void *, void (*)(void *, int, const char *));
  
EXPORT m64p_error CALL PluginGetVersionVideo(m64p_plugin_type *PluginType, int *PluginVersion, int *APIVersion, const char **PluginNamePtr, int *Capabilities);

#ifdef __cplusplus
}
#endif


typedef struct {
    float   fViWidth, fViHeight;
    unsigned short        uViWidth, uViHeight;
    unsigned short        uDisplayWidth, uDisplayHeight;
    
    BOOL    bDisplayFullscreen;
    BOOL    bVerticalSync;

    float   fMultX, fMultY;
    int     vpLeftW, vpTopW, vpRightW, vpBottomW, vpWidthW, vpHeightW;

    int     statusBarHeight, statusBarHeightToUse, toolbarHeight, toolbarHeightToUse;
    BOOL    screenSaverStatus;

    struct {
        uint32      left;
        uint32      top;
        uint32      right;
        uint32      bottom;
        uint32      width;
        uint32      height;
        bool        needToClip;
    } clipping;

    int     timer;
    float   fps;    // frame per second
    float   dps;    // dlist per second
    uint32  lastSecFrameCount;
    uint32  lastSecDlistCount;
}WindowSettingStruct;

typedef enum 
{
    PRIM_TRI1,
    PRIM_TRI2,
    PRIM_TRI3,
    PRIM_DMA_TRI,
    PRIM_LINE3D,
    PRIM_TEXTRECT,
    PRIM_TEXTRECTFLIP,
    PRIM_FILLRECT,
} PrimitiveType;

typedef enum 
{
    RSP_SCISSOR,
    RDP_SCISSOR,
    UNKNOWN_SCISSOR,
} CurScissorType;

typedef struct {
    bool    bGameIsRunning;
    uint32  dwTvSystem;
    float   fRatio;

    BOOL    frameReadByCPU;
    BOOL    frameWriteByCPU;

    uint32  SPCycleCount;       // Count how many CPU cycles SP used in this DLIST
    uint32  DPCycleCount;       // Count how many CPU cycles DP used in this DLIST

    uint32  dwNumTrisRendered;
    uint32  dwNumDListsCulled;
    uint32  dwNumTrisClipped;
    uint32  dwNumVertices;
    uint32  dwBiggestVertexIndex;

    uint32  gDlistCount;
    uint32  gFrameCount;
    uint32  gUcodeCount;
    uint32  gRDPTime;
    BOOL    ToToggleFullScreen;
    bool    ToResize;
    uint32  gNewResizeWidth, gNewResizeHeight;
    bool    bDisableFPS;

    bool    bUseModifiedUcodeMap;
    bool    ucodeHasBeenSet;
    bool    bUcodeIsKnown;

    uint32  curRenderBuffer;
    uint32  curDisplayBuffer;
    uint32  curVIOriginReg;
    CurScissorType  curScissor;

    PrimitiveType primitiveType;

    uint32  lastPurgeTimeTime;      // Time textures were last purged

    bool    UseLargerTile[2];       // This is a speed up for large tile loading,
    uint32  LargerTileRealLeft[2];  // works only for TexRect, LoadTile, large width, large pitch

    bool    bVIOriginIsUpdated;
    bool    bCIBufferIsRendered;
    int     leftRendered,topRendered,rightRendered,bottomRendered;

    bool    isMMXSupported;
    bool    isSSESupported;

    bool    isMMXEnabled;
    bool    isSSEEnabled;

    bool    toShowCFB;

    bool    bAllowLoadFromTMEM;

    // Frame buffer simulation related status variables
    bool    bN64FrameBufferIsUsed;      // Frame buffer is used in the frame
    bool    bN64IsDrawingTextureBuffer; // The current N64 game is rendering into render_texture, to create self-rendering texture
    bool    bHandleN64RenderTexture;    // Do we need to handle of the N64 render_texture stuff?
    bool    bDirectWriteIntoRDRAM;      // When drawing into render_texture, this value =
                                        // = true   don't render, but write real N64 graphic value into RDRAM
                                        // = false  rendering into render_texture of DX or OGL, the render_texture
                                        //          will be copied into RDRAM at the end
    bool    bFrameBufferIsDrawn;        // flag to mark if the frame buffer is ever drawn
    bool    bFrameBufferDrawnByTriangles;   // flag to tell if the buffer is even drawn by Triangle cmds

    bool    bScreenIsDrawn;

} PluginStatus;

#define MI_INTR_DP          0x00000020  
#define MI_INTR_SP          0x00000001  

extern PluginStatus status;
extern GFX_INFO g_GraphicsInfo;
extern WindowSettingStruct windowSetting;

extern unsigned int   g_dwRamSize;
extern unsigned int  *g_pRDRAMu32;
extern signed char   *g_pRDRAMs8;
extern unsigned char *g_pRDRAMu8;

#if (!M64P_STATIC_PLUGINS)
/* declarations of pointers to Core config functions */
extern ptr_ConfigListSections     ConfigListSections;
extern ptr_ConfigOpenSection      ConfigOpenSection;
extern ptr_ConfigListParameters   ConfigListParameters;
extern ptr_ConfigSetParameter     ConfigSetParameter;
extern ptr_ConfigSetParameterHelp ConfigSetParameterHelp;
extern ptr_ConfigGetParameter     ConfigGetParameter;
extern ptr_ConfigGetParameterHelp ConfigGetParameterHelp;
extern ptr_ConfigSetDefaultInt    ConfigSetDefaultInt;
extern ptr_ConfigSetDefaultFloat  ConfigSetDefaultFloat;
extern ptr_ConfigSetDefaultBool   ConfigSetDefaultBool;
extern ptr_ConfigSetDefaultString ConfigSetDefaultString;
extern ptr_ConfigGetParamInt      ConfigGetParamInt;
extern ptr_ConfigGetParamFloat    ConfigGetParamFloat;
extern ptr_ConfigGetParamBool     ConfigGetParamBool;
extern ptr_ConfigGetParamString   ConfigGetParamString;

extern ptr_ConfigGetSharedDataFilepath ConfigGetSharedDataFilepath;
extern ptr_ConfigGetUserConfigPath     ConfigGetUserConfigPath;
extern ptr_ConfigGetUserDataPath       ConfigGetUserDataPath;
extern ptr_ConfigGetUserCachePath      ConfigGetUserCachePath;

/* declarations of pointers to Core video extension functions */
extern ptr_VidExt_Init                  CoreVideo_Init;
extern ptr_VidExt_Quit                  CoreVideo_Quit;
extern ptr_VidExt_ListFullscreenModes   CoreVideo_ListFullscreenModes;
extern ptr_VidExt_SetVideoMode          CoreVideo_SetVideoMode;
extern ptr_VidExt_SetCaption            CoreVideo_SetCaption;
extern ptr_VidExt_ToggleFullScreen      CoreVideo_ToggleFullScreen;
extern ptr_VidExt_ResizeWindow          CoreVideo_ResizeWindow;
extern ptr_VidExt_GL_GetProcAddress     CoreVideo_GL_GetProcAddress;
extern ptr_VidExt_GL_SetAttribute       CoreVideo_GL_SetAttribute;
extern ptr_VidExt_GL_GetAttribute       CoreVideo_GL_GetAttribute;
extern ptr_VidExt_GL_SwapBuffers        CoreVideo_GL_SwapBuffers;

#else

#define CoreVideo_Init VidExt_Init
#define CoreVideo_Quit VidExt_Init
#define CoreVideo_ListFullscreenModes VidExt_ListFullscreenModes
#define CoreVideo_SetVideoMode VidExt_SetVideoMode
#define CoreVideo_SetCaption VidExt_SetCaption
#define CoreVideo_ToggleFullScreen VidExt_ToggleFullScreen
#define CoreVideo_ResizeWindow VidExt_ResizeWindow
#define CoreVideo_GL_GetProcAddress VidExt_GL_GetProcAddress
#define CoreVideo_GL_SetAttribute VidExt_GL_SetAttribute
#define CoreVideo_GL_GetAttribute VidExt_GL_GetAttribute
#define CoreVideo_GL_SwapBuffers VidExt_GL_SwapBuffers

#endif


/* global functions provided by Video.cpp */
extern char generalText[];
extern void (*renderCallback)(int);
void DebugMessage(int level, const char *message, ...) ATTR_FMT(2,3);

void SetVIScales();

#if M64P_STATIC_PLUGINS

#ifdef __cplusplus
extern "C" {
#endif

EXPORT int CALL RomOpenVideo(void);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

EXPORT void CALL RomClosedVideo(void);

#ifdef __cplusplus
}
#endif
#endif // M64P_STATIC_PLUGINS

extern void _VIDEO_DisplayTemporaryMessage2(const char *msg, ...) ATTR_FMT(1,2);
extern void _VIDEO_DisplayTemporaryMessage(const char *msg);
extern void XBOX_Debugger_Log(const char *Message, ...) ATTR_FMT(1,2);

#endif

