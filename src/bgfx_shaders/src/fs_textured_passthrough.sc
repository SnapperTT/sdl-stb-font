$input v_texcoord0

// Include the bgfx shader library here
// Various parts of this uber shader are adopted from bgfx, and are therefore
// under the following license, henceforth refered to "bgfx licnese".
/*
 * Copyright 2011-2018 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */
#include <bgfx_shader.shh>

// Samplers
SAMPLER2D(s_texture,  0);
uniform vec4 u_colour;

void  main() {
	vec4 tex = texture2D(s_texture, v_texcoord0);
	gl_FragColor = tex*u_colour;
	}

