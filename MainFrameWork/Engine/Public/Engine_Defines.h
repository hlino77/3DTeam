
#pragma once

#pragma warning (disable : 4005)
#pragma warning (disable : 4251)
#pragma warning (disable : 26812)

#ifndef WinSock_Include_h__
#define WinSock_Include_h__

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#endif

#include <wrl.h>
#include <random>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <FX11/d3dx11effect.h>
#include <DirectXTK/DDSTextureLoader.h>
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK/ScreenGrab.h>

#include <DirectXTK/PrimitiveBatch.h>
#include <DirectXTK/Effects.h>
#include <DirectXTK/VertexTypes.h>
#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>
#include <DirectXTK/SimpleMath.h>

#include <DirectXCollision.h>

#include <DirectXTex/DirectXTex.h>
#include <GFSDK_SSAO.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace SimpleMath;

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <typeinfo>
#include <array>
#include <unordered_map>
#include <iostream>

#include <process.h>

#include "fmod.h"
#include "fmod.hpp"
#include <io.h>

#include "Engine_Macro.h"
#include "Engine_Typedef.h"
#include "Engine_Struct.h"
#include "Engine_Function.h"
#include "Engine_Enum.h"
#include "TLS.h"
#include "TypeLessValue.h"

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//
//#ifndef DBG_NEW 
//
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
//#define new DBG_NEW 
//
//#endif

#endif // _DEBUG

using namespace std;
using namespace Engine;


