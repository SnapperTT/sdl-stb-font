// bgfxh_embedded_shader.hh
//

#ifndef LZZ_bgfxh_embedded_shader_hh
#define LZZ_bgfxh_embedded_shader_hh
// This is just a bunch of macros so that bgfxh can internally embed the c-header shaders
#ifndef BGFXH_EMBEDDED_SHADER

#include <bgfx/embedded_shader.h>
//#define BGFXH_EMBEDDED_SHADER_DX9BC BGFX_EMBEDDED_SHADER_DX9BC
#define BGFXH_EMBEDDED_SHADER_DXBC BGFX_EMBEDDED_SHADER_DXBC
#define BGFXH_EMBEDDED_SHADER_PSSL BGFX_EMBEDDED_SHADER_PSSL
#define BGFXH_EMBEDDED_SHADER_GLSL BGFX_EMBEDDED_SHADER_GLSL
#define BGFXH_EMBEDDED_SHADER_NVN BGFX_EMBEDDED_SHADER_NVN
#define BGFXH_EMBEDDED_SHADER_ESSL BGFX_EMBEDDED_SHADER_ESSL
#define BGFXH_EMBEDDED_SHADER_SPIRV BGFX_EMBEDDED_SHADER_SPIRV
#define BGFXH_EMBEDDED_SHADER_METAL BGFX_EMBEDDED_SHADER_METAL

#if 0 //BGFX_PLATFORM_SUPPORTS_DX9BC
#	undef  BGFXH_EMBEDDED_SHADER_DX9BC
#	define BGFXH_EMBEDDED_SHADER_DX9BC(_renderer, _name) 		{ _renderer, BX_CONCATENATE(_name, _dx9 ), BX_CONCATENATE(_name, _dx9_size) },
#endif // BGFX_PLATFORM_SUPPORTS_DX9BC

#if BGFX_PLATFORM_SUPPORTS_DXBC
#	undef  BGFXH_EMBEDDED_SHADER_DXBC
#	define BGFXH_EMBEDDED_SHADER_DXBC(_renderer, _name) 		{ _renderer, BX_CONCATENATE(_name, _dx11), BX_CONCATENATE(_name, _dx11_size) },
#endif // BGFX_PLATFORM_SUPPORTS_DXBC

#if BGFX_PLATFORM_SUPPORTS_PSSL
#	undef  BGFXH_EMBEDDED_SHADER_PSSL
#	define BGFXH_EMBEDDED_SHADER_PSSL(_renderer, _name) 		{ _renderer, BX_CONCATENATE(_name, _pssl), BX_CONCATENATE(_name, _pssl_size) },
#endif // BGFX_PLATFORM_SUPPORTS_PSSL

#if BGFX_PLATFORM_SUPPORTS_ESSL
#	undef  BGFXH_EMBEDDED_SHADER_ESSL
	#ifdef BGFXH_GOOGLE_NACL
	#	define BGFXH_EMBEDDED_SHADER_ESSL(_renderer, _name) 			{ _renderer, BX_CONCATENATE(_name, _esslnacl), BX_CONCATENATE(_name, _esslnacl_size) },
	#else
	#	define BGFXH_EMBEDDED_SHADER_ESSL(_renderer, _name) 			{ _renderer, BX_CONCATENATE(_name, _esslandroid), BX_CONCATENATE(_name, _esslandroid_size) },
	#endif // BGFXH_GOOGLE
#endif // BGFX_PLATFORM_SUPPORTS_ESSL

#if BGFX_PLATFORM_SUPPORTS_GLSL
#	undef  BGFXH_EMBEDDED_SHADER_GLSL
#	define BGFXH_EMBEDDED_SHADER_GLSL(_renderer, _name) 		{ _renderer, BX_CONCATENATE(_name, _glsl), BX_CONCATENATE(_name, _glsl_size)  },
#endif // BGFX_PLATFORM_SUPPORTS_GLSL

#if BGFX_PLATFORM_SUPPORTS_SPIRV
#	undef  BGFXH_EMBEDDED_SHADER_SPIRV
#	define BGFXH_EMBEDDED_SHADER_SPIRV(_renderer, _name) 		{ _renderer, BX_CONCATENATE(_name, _spirv), BX_CONCATENATE(_name, _spirv_size) },
#endif // BGFX_PLATFORM_SUPPORTS_SPIRV

#if BGFX_PLATFORM_SUPPORTS_METAL
#	undef  BGFXH_EMBEDDED_SHADER_METAL
#	define BGFXH_EMBEDDED_SHADER_METAL(_renderer, _name) 		{ _renderer, BX_CONCATENATE(_name, _metal), BX_CONCATENATE(_name, _metal_size) },
#endif // BGFX_PLATFORM_SUPPORTS_METAL

#define BGFXH_EMBEDDED_SHADER(_name)                                                               			{                                                                                      				#_name,                                                                            				{                                                                                  					BGFXH_EMBEDDED_SHADER_DXBC (bgfx::RendererType::Direct3D11, _name)             					BGFXH_EMBEDDED_SHADER_DXBC (bgfx::RendererType::Direct3D12, _name)             					BGFXH_EMBEDDED_SHADER_PSSL (bgfx::RendererType::Gnm,        _name)             					BGFXH_EMBEDDED_SHADER_METAL(bgfx::RendererType::Metal,      _name)             					BGFXH_EMBEDDED_SHADER_NVN  (bgfx::RendererType::Nvn,        _name)             					BGFXH_EMBEDDED_SHADER_ESSL (bgfx::RendererType::OpenGLES,   _name)             					BGFXH_EMBEDDED_SHADER_GLSL (bgfx::RendererType::OpenGL,     _name)             					BGFXH_EMBEDDED_SHADER_SPIRV(bgfx::RendererType::Vulkan,     _name)             					{ bgfx::RendererType::Noop,  (const uint8_t*)"VSH\x5\x0\x0\x0\x0\x0\x0", 10 }, 					{ bgfx::RendererType::Count, NULL, 0 }                                         				}                                                                                  			}

#endif //#ifndef BGFXH_EMBEDDED_SHADER
#define LZZ_INLINE inline
#undef LZZ_INLINE
#endif

////////////////////////////////////////////////////////////////////////

#ifdef BGFXH_IMPL
#ifndef BGFXH_DOUBLE_GUARD_bgfxh_embedded_shader
#define BGFXH_DOUBLE_GUARD_bgfxh_embedded_shader
// bgfxh_embedded_shader.cpp
//

// No impl
#define LZZ_INLINE inline
#undef LZZ_INLINE
#endif //BGFXH_DOUBLE_GUARD_bgfxh_embedded_shader
#endif //BGFXH_IMPL
