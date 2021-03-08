// Include the bgfx shader library here
// Various parts of this uber shader are adopted from bgfx, and are therefore
// under the following license, henceforth refered to "bgfx licnese".
/*
 * Copyright 2011-2018 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */
#include <bgfx_shader.shh>

// Samplers
uniform vec4 u_colour;

void  main() {
	gl_FragColor = u_colour;
	}

