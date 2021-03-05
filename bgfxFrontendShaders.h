// bgfxh_embedded_shader.hh
//

#ifndef LZZ_bgfxh_embedded_shader_hh
#define LZZ_bgfxh_embedded_shader_hh
// This is just a bunch of macros so that bgfxh can internally embed the c-header shaders

#include <bgfx/embedded_shader.h>
#define BGFXH_EMBEDDED_SHADER_DX9BC BGFX_EMBEDDED_SHADER_DX9BC
#define BGFXH_EMBEDDED_SHADER_DXBC BGFX_EMBEDDED_SHADER_DXBC
#define BGFXH_EMBEDDED_SHADER_PSSL BGFX_EMBEDDED_SHADER_PSSL
#define BGFXH_EMBEDDED_SHADER_GLSL BGFX_EMBEDDED_SHADER_GLSL
#define BGFXH_EMBEDDED_SHADER_NVN BGFX_EMBEDDED_SHADER_NVN
#define BGFXH_EMBEDDED_SHADER_ESSL BGFX_EMBEDDED_SHADER_ESSL
#define BGFXH_EMBEDDED_SHADER_SPIRV BGFX_EMBEDDED_SHADER_SPIRV
#define BGFXH_EMBEDDED_SHADER_METAL BGFX_EMBEDDED_SHADER_METAL

#if BGFX_PLATFORM_SUPPORTS_DX9BC
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

#define BGFXH_EMBEDDED_SHADER(_name)                                                               			{                                                                                      				#_name,                                                                            				{                                                                                  					BGFXH_EMBEDDED_SHADER_DX9BC(bgfx::RendererType::Direct3D9,  _name)             					BGFXH_EMBEDDED_SHADER_DXBC (bgfx::RendererType::Direct3D11, _name)             					BGFXH_EMBEDDED_SHADER_DXBC (bgfx::RendererType::Direct3D12, _name)             					BGFXH_EMBEDDED_SHADER_PSSL (bgfx::RendererType::Gnm,        _name)             					BGFXH_EMBEDDED_SHADER_METAL(bgfx::RendererType::Metal,      _name)             					BGFXH_EMBEDDED_SHADER_NVN  (bgfx::RendererType::Nvn,        _name)             					BGFXH_EMBEDDED_SHADER_ESSL (bgfx::RendererType::OpenGLES,   _name)             					BGFXH_EMBEDDED_SHADER_GLSL (bgfx::RendererType::OpenGL,     _name)             					BGFXH_EMBEDDED_SHADER_SPIRV(bgfx::RendererType::Vulkan,     _name)             					{ bgfx::RendererType::Noop,  (const uint8_t*)"VSH\x5\x0\x0\x0\x0\x0\x0", 10 }, 					{ bgfx::RendererType::Count, NULL, 0 }                                         				}                                                                                  			}

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
static const uint8_t fs_textured_passthrough_bin_glsl[] = {
  0x46, 0x53, 0x48, 0x09, 0x6f, 0x1e, 0x3e, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x09, 0x73,  // FSH.o.><.......s
  0x5f, 0x74, 0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,  // _texture........
  0x08, 0x75, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x75, 0x72, 0x02, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00,  // .u_colour.......
  0x00, 0xa2, 0x00, 0x00, 0x00, 0x76, 0x61, 0x72, 0x79, 0x69, 0x6e, 0x67, 0x20, 0x76, 0x65, 0x63,  // .....varying vec
  0x32, 0x20, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x3b, 0x0a, 0x75,  // 2 v_texcoord0;.u
  0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x72, 0x32, 0x44,  // niform sampler2D
  0x20, 0x73, 0x5f, 0x74, 0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 0x3b, 0x0a, 0x75, 0x6e, 0x69, 0x66,  //  s_texture;.unif
  0x6f, 0x72, 0x6d, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x75, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x75,  // orm vec4 u_colou
  0x72, 0x3b, 0x0a, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x6d, 0x61, 0x69, 0x6e, 0x20, 0x28, 0x29, 0x0a,  // r;.void main ().
  0x7b, 0x0a, 0x20, 0x20, 0x67, 0x6c, 0x5f, 0x46, 0x72, 0x61, 0x67, 0x43, 0x6f, 0x6c, 0x6f, 0x72,  // {.  gl_FragColor
  0x20, 0x3d, 0x20, 0x28, 0x74, 0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 0x32, 0x44, 0x20, 0x28, 0x73,  //  = (texture2D (s
  0x5f, 0x74, 0x65, 0x78, 0x74, 0x75, 0x72, 0x65, 0x2c, 0x20, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63,  // _texture, v_texc
  0x6f, 0x6f, 0x72, 0x64, 0x30, 0x29, 0x20, 0x2a, 0x20, 0x75, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x75,  // oord0) * u_colou
  0x72, 0x29, 0x3b, 0x0a, 0x7d, 0x0a, 0x0a, 0x00 };                                                //  u_colour);.}...
unsigned int fs_textured_passthrough_bin_glsl_size = 216;

BX_UNUSED(fs_textured_passthrough_bin_glsl)
BX_UNUSED(fs_textured_passthrough_bin_glsl_size)
static const uint8_t * fs_textured_passthrough_bin_dx9 = NULL;
unsigned int fs_textured_passthrough_bin_dx9_size = 0;

BX_UNUSED(fs_textured_passthrough_bin_dx9)
BX_UNUSED(fs_textured_passthrough_bin_dx9_size)
static const uint8_t * fs_textured_passthrough_bin_dx11 = NULL;
unsigned int fs_textured_passthrough_bin_dx11_size = 0;

BX_UNUSED(fs_textured_passthrough_bin_dx11)
BX_UNUSED(fs_textured_passthrough_bin_dx11_size)
static const uint8_t * fs_textured_passthrough_bin_metal = NULL;
unsigned int fs_textured_passthrough_bin_metal_size = 0;

BX_UNUSED(fs_textured_passthrough_bin_metal)
BX_UNUSED(fs_textured_passthrough_bin_metal_size)
static const uint8_t * fs_textured_passthrough_bin_spirv = NULL;
unsigned int fs_textured_passthrough_bin_spirv_size = 0;

BX_UNUSED(fs_textured_passthrough_bin_spirv)
BX_UNUSED(fs_textured_passthrough_bin_spirv_size)
static const uint8_t * fs_textured_passthrough_bin_esslandroid = NULL;
unsigned int fs_textured_passthrough_bin_esslandroid_size = 0;

BX_UNUSED(fs_textured_passthrough_bin_esslandroid)
BX_UNUSED(fs_textured_passthrough_bin_esslandroid_size)
static const uint8_t * fs_textured_passthrough_bin_pssl = NULL;
unsigned int fs_textured_passthrough_bin_pssl_size = 0;

BX_UNUSED(fs_textured_passthrough_bin_pssl)
BX_UNUSED(fs_textured_passthrough_bin_pssl_size)
static const uint8_t * fs_textured_passthrough_bin_esslnacl = NULL;
unsigned int fs_textured_passthrough_bin_esslnacl_size = 0;

BX_UNUSED(fs_textured_passthrough_bin_esslnacl)
BX_UNUSED(fs_textured_passthrough_bin_esslnacl_size)
static const uint8_t vs_textured_passthrough_bin_glsl[] = {
  0x56, 0x53, 0x48, 0x09, 0x00, 0x00, 0x00, 0x00, 0x6f, 0x1e, 0x3e, 0x3c, 0x01, 0x00, 0x0f, 0x75,  // VSH.....o.><...u
  0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x04, 0x01,  // _modelViewProj..
  0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0e, 0x01, 0x00, 0x00, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62,  // ..........attrib
  0x75, 0x74, 0x65, 0x20, 0x76, 0x65, 0x63, 0x33, 0x20, 0x61, 0x5f, 0x70, 0x6f, 0x73, 0x69, 0x74,  // ute vec3 a_posit
  0x69, 0x6f, 0x6e, 0x3b, 0x0a, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x20, 0x76,  // ion;.attribute v
  0x65, 0x63, 0x32, 0x20, 0x61, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x3b,  // ec2 a_texcoord0;
  0x0a, 0x76, 0x61, 0x72, 0x79, 0x69, 0x6e, 0x67, 0x20, 0x76, 0x65, 0x63, 0x32, 0x20, 0x76, 0x5f,  // .varying vec2 v_
  0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x3b, 0x0a, 0x75, 0x6e, 0x69, 0x66, 0x6f,  // texcoord0;.unifo
  0x72, 0x6d, 0x20, 0x6d, 0x61, 0x74, 0x34, 0x20, 0x75, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56,  // rm mat4 u_modelV
  0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x3b, 0x0a, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x6d, 0x61,  // iewProj;.void ma
  0x69, 0x6e, 0x20, 0x28, 0x29, 0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x74,  // in ().{.  vec4 t
  0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61,  // mpvar_1;.  tmpva
  0x72, 0x5f, 0x31, 0x2e, 0x77, 0x20, 0x3d, 0x20, 0x31, 0x2e, 0x30, 0x3b, 0x0a, 0x20, 0x20, 0x74,  // r_1.w = 1.0;.  t
  0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, 0x78, 0x79, 0x7a, 0x20, 0x3d, 0x20, 0x61, 0x5f,  // mpvar_1.xyz = a_
  0x70, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x3b, 0x0a, 0x20, 0x20, 0x67, 0x6c, 0x5f, 0x50,  // position;.  gl_P
  0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x3d, 0x20, 0x28, 0x75, 0x5f, 0x6d, 0x6f, 0x64,  // osition = (u_mod
  0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x20, 0x2a, 0x20, 0x74, 0x6d, 0x70,  // elViewProj * tmp
  0x76, 0x61, 0x72, 0x5f, 0x31, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63,  // var_1);.  v_texc
  0x6f, 0x6f, 0x72, 0x64, 0x30, 0x20, 0x3d, 0x20, 0x61, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f,  // oord0 = a_texcoo
  0x72, 0x64, 0x30, 0x3b, 0x0a, 0x7d, 0x0a, 0x0a, 0x00 };                                          // _texcoord0;.}...
unsigned int vs_textured_passthrough_bin_glsl_size = 313;

BX_UNUSED(vs_textured_passthrough_bin_glsl)
BX_UNUSED(vs_textured_passthrough_bin_glsl_size)
static const uint8_t * vs_textured_passthrough_bin_dx9 = NULL;
unsigned int vs_textured_passthrough_bin_dx9_size = 0;

BX_UNUSED(vs_textured_passthrough_bin_dx9)
BX_UNUSED(vs_textured_passthrough_bin_dx9_size)
static const uint8_t * vs_textured_passthrough_bin_dx11 = NULL;
unsigned int vs_textured_passthrough_bin_dx11_size = 0;

BX_UNUSED(vs_textured_passthrough_bin_dx11)
BX_UNUSED(vs_textured_passthrough_bin_dx11_size)
static const uint8_t * vs_textured_passthrough_bin_metal = NULL;
unsigned int vs_textured_passthrough_bin_metal_size = 0;

BX_UNUSED(vs_textured_passthrough_bin_metal)
BX_UNUSED(vs_textured_passthrough_bin_metal_size)
static const uint8_t * vs_textured_passthrough_bin_spirv = NULL;
unsigned int vs_textured_passthrough_bin_spirv_size = 0;

BX_UNUSED(vs_textured_passthrough_bin_spirv)
BX_UNUSED(vs_textured_passthrough_bin_spirv_size)
static const uint8_t * vs_textured_passthrough_bin_esslandroid = NULL;
unsigned int vs_textured_passthrough_bin_esslandroid_size = 0;

BX_UNUSED(vs_textured_passthrough_bin_esslandroid)
BX_UNUSED(vs_textured_passthrough_bin_esslandroid_size)
static const uint8_t * vs_textured_passthrough_bin_pssl = NULL;
unsigned int vs_textured_passthrough_bin_pssl_size = 0;

BX_UNUSED(vs_textured_passthrough_bin_pssl)
BX_UNUSED(vs_textured_passthrough_bin_pssl_size)
static const uint8_t * vs_textured_passthrough_bin_esslnacl = NULL;
unsigned int vs_textured_passthrough_bin_esslnacl_size = 0;

BX_UNUSED(vs_textured_passthrough_bin_esslnacl)
BX_UNUSED(vs_textured_passthrough_bin_esslnacl_size)
static const uint8_t fs_untextured_passthrough_bin_glsl[] = {
  0x46, 0x53, 0x48, 0x09, 0x6f, 0x1e, 0x3e, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x75,  // FSH.o.><.......u
  0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x75, 0x72, 0x02, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x44,  // _colour........D
  0x00, 0x00, 0x00, 0x75, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20,  // ...uniform vec4 
  0x75, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x75, 0x72, 0x3b, 0x0a, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x6d,  // u_colour;.void m
  0x61, 0x69, 0x6e, 0x20, 0x28, 0x29, 0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x67, 0x6c, 0x5f, 0x46, 0x72,  // ain ().{.  gl_Fr
  0x61, 0x67, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x20, 0x3d, 0x20, 0x75, 0x5f, 0x63, 0x6f, 0x6c, 0x6f,  // agColor = u_colo
  0x75, 0x72, 0x3b, 0x0a, 0x7d, 0x0a, 0x0a, 0x00 };                                                // = u_colour;.}...
unsigned int fs_untextured_passthrough_bin_glsl_size = 104;

BX_UNUSED(fs_untextured_passthrough_bin_glsl)
BX_UNUSED(fs_untextured_passthrough_bin_glsl_size)
static const uint8_t * fs_untextured_passthrough_bin_dx9 = NULL;
unsigned int fs_untextured_passthrough_bin_dx9_size = 0;

BX_UNUSED(fs_untextured_passthrough_bin_dx9)
BX_UNUSED(fs_untextured_passthrough_bin_dx9_size)
static const uint8_t * fs_untextured_passthrough_bin_dx11 = NULL;
unsigned int fs_untextured_passthrough_bin_dx11_size = 0;

BX_UNUSED(fs_untextured_passthrough_bin_dx11)
BX_UNUSED(fs_untextured_passthrough_bin_dx11_size)
static const uint8_t * fs_untextured_passthrough_bin_metal = NULL;
unsigned int fs_untextured_passthrough_bin_metal_size = 0;

BX_UNUSED(fs_untextured_passthrough_bin_metal)
BX_UNUSED(fs_untextured_passthrough_bin_metal_size)
static const uint8_t * fs_untextured_passthrough_bin_spirv = NULL;
unsigned int fs_untextured_passthrough_bin_spirv_size = 0;

BX_UNUSED(fs_untextured_passthrough_bin_spirv)
BX_UNUSED(fs_untextured_passthrough_bin_spirv_size)
static const uint8_t * fs_untextured_passthrough_bin_esslandroid = NULL;
unsigned int fs_untextured_passthrough_bin_esslandroid_size = 0;

BX_UNUSED(fs_untextured_passthrough_bin_esslandroid)
BX_UNUSED(fs_untextured_passthrough_bin_esslandroid_size)
static const uint8_t * fs_untextured_passthrough_bin_pssl = NULL;
unsigned int fs_untextured_passthrough_bin_pssl_size = 0;

BX_UNUSED(fs_untextured_passthrough_bin_pssl)
BX_UNUSED(fs_untextured_passthrough_bin_pssl_size)
static const uint8_t * fs_untextured_passthrough_bin_esslnacl = NULL;
unsigned int fs_untextured_passthrough_bin_esslnacl_size = 0;

BX_UNUSED(fs_untextured_passthrough_bin_esslnacl)
BX_UNUSED(fs_untextured_passthrough_bin_esslnacl_size)
static const uint8_t vs_untextured_passthrough_bin_glsl[] = {
  0x56, 0x53, 0x48, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0f, 0x75,  // VSH............u
  0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x04, 0x01,  // _modelViewProj..
  0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xbb, 0x00, 0x00, 0x00, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62,  // ..........attrib
  0x75, 0x74, 0x65, 0x20, 0x76, 0x65, 0x63, 0x33, 0x20, 0x61, 0x5f, 0x70, 0x6f, 0x73, 0x69, 0x74,  // ute vec3 a_posit
  0x69, 0x6f, 0x6e, 0x3b, 0x0a, 0x75, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x6d, 0x61, 0x74,  // ion;.uniform mat
  0x34, 0x20, 0x75, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f,  // 4 u_modelViewPro
  0x6a, 0x3b, 0x0a, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x6d, 0x61, 0x69, 0x6e, 0x20, 0x28, 0x29, 0x0a,  // j;.void main ().
  0x7b, 0x0a, 0x20, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f,  // {.  vec4 tmpvar_
  0x31, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, 0x77, 0x20,  // 1;.  tmpvar_1.w 
  0x3d, 0x20, 0x31, 0x2e, 0x30, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f,  // = 1.0;.  tmpvar_
  0x31, 0x2e, 0x78, 0x79, 0x7a, 0x20, 0x3d, 0x20, 0x61, 0x5f, 0x70, 0x6f, 0x73, 0x69, 0x74, 0x69,  // 1.xyz = a_positi
  0x6f, 0x6e, 0x3b, 0x0a, 0x20, 0x20, 0x67, 0x6c, 0x5f, 0x50, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f,  // on;.  gl_Positio
  0x6e, 0x20, 0x3d, 0x20, 0x28, 0x75, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77,  // n = (u_modelView
  0x50, 0x72, 0x6f, 0x6a, 0x20, 0x2a, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x29,  // Proj * tmpvar_1)
  0x3b, 0x0a, 0x7d, 0x0a, 0x0a, 0x00 };                                                            //  tmpvar_1);.}...
unsigned int vs_untextured_passthrough_bin_glsl_size = 230;

BX_UNUSED(vs_untextured_passthrough_bin_glsl)
BX_UNUSED(vs_untextured_passthrough_bin_glsl_size)
static const uint8_t * vs_untextured_passthrough_bin_dx9 = NULL;
unsigned int vs_untextured_passthrough_bin_dx9_size = 0;

BX_UNUSED(vs_untextured_passthrough_bin_dx9)
BX_UNUSED(vs_untextured_passthrough_bin_dx9_size)
static const uint8_t * vs_untextured_passthrough_bin_dx11 = NULL;
unsigned int vs_untextured_passthrough_bin_dx11_size = 0;

BX_UNUSED(vs_untextured_passthrough_bin_dx11)
BX_UNUSED(vs_untextured_passthrough_bin_dx11_size)
static const uint8_t * vs_untextured_passthrough_bin_metal = NULL;
unsigned int vs_untextured_passthrough_bin_metal_size = 0;

BX_UNUSED(vs_untextured_passthrough_bin_metal)
BX_UNUSED(vs_untextured_passthrough_bin_metal_size)
static const uint8_t * vs_untextured_passthrough_bin_spirv = NULL;
unsigned int vs_untextured_passthrough_bin_spirv_size = 0;

BX_UNUSED(vs_untextured_passthrough_bin_spirv)
BX_UNUSED(vs_untextured_passthrough_bin_spirv_size)
static const uint8_t * vs_untextured_passthrough_bin_esslandroid = NULL;
unsigned int vs_untextured_passthrough_bin_esslandroid_size = 0;

BX_UNUSED(vs_untextured_passthrough_bin_esslandroid)
BX_UNUSED(vs_untextured_passthrough_bin_esslandroid_size)
static const uint8_t * vs_untextured_passthrough_bin_pssl = NULL;
unsigned int vs_untextured_passthrough_bin_pssl_size = 0;

BX_UNUSED(vs_untextured_passthrough_bin_pssl)
BX_UNUSED(vs_untextured_passthrough_bin_pssl_size)
static const uint8_t * vs_untextured_passthrough_bin_esslnacl = NULL;
unsigned int vs_untextured_passthrough_bin_esslnacl_size = 0;

BX_UNUSED(vs_untextured_passthrough_bin_esslnacl)
BX_UNUSED(vs_untextured_passthrough_bin_esslnacl_size)
