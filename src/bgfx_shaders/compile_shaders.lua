-- compile_shaders.lua
-- To use: Invoke from the command line
-- lua compile_shaders.lua (fs/vs)_<INPUT_FILE>.sc
-- Example: "lua compile_shaders.lua path/to/texturedPassthrough" will compile path/to/vs_texturedPassthrough.sc and path/to/fs_texturedPassthrough.sc with varying_texturedPassthrough.def.sc to path/to/../glsl/vs_texturedPassthrough.bin, path/to/../dx9/vs_texturedPassthrough.bin, etc. This will also create a c header file that contains the files as char arrays
-- Options: -o/-output <FILE> - output filename. Must not be a path
-- 			-d/-debug - only compile GLSL
--			-f - only do the frag
--			-v - only do the vert
--			-vdef - specify a varying.def Eg: -vdef foo will make this look for varying_foo.def
-- 			-dx11 - only compile HLSL for dx11. (Warning: HLSL will not compile on non-windows machines)
--          -c  - only build a c header file containing all the binary shaders that exist.
--			-p	- ammends a path to the input file - use this if invoking this script from some weird directory
--			-D VALUE - #defines VALUE in the shaders that are being compiled. Can use -D VAL1 -D VAL2 etc.. for multiple defines
--		The following are used to override defaults defined in this file
--			-C_DO (ON/OFF) - compile c header shaders?
--			-DX9_DO (ON/OFF) - compile DX9 shaders?
--			-DX11_DO (ON/OFF) - compile DX11/DX12 shaders?
--			-NACL_DO (ON/OFF) - compile NACL shaders?
--			-ANDROID_DO (ON/OFF) - compile ANDRIOD shaders?
--			-GLSL_DO (ON/OFF) - compile GLSL shaders?
--			-METAL_DO (ON/OFF) - compile METAL shaders?
--			-ORBIS_DO (ON/OFF) - compile ORBIS shaders?
--			-SPRIV_DO (ON/OFF) - compile SPRIV shaders?
-- 



INPUT_FILE = false;
OUTPUT_FILE = false;
VDEF_FILE = false;
DEFS = {}
GLSL_ONLY = false;
HLSL_ONLY = false;
C_ONLY = false;

local arg={...}

local doFrag = true;
local doVert = true;

function isOnOrOff(a)
	if (a == "ON" or a == "on") then return true; end
	return false;
end

for i=1,#arg do
	if (arg[i] == "-debug" or arg[i] == "-d") then
		GLSL_ONLY = true;
	elseif (arg[i] == "-dx11") then
		HLSL_ONLY = true;
	elseif (arg[i] == "-c") then
		C_ONLY = true;
	elseif (arg[i] == "-f") then
		doFrag = true;
		doVert = false;
	elseif (arg[i] == "-v") then
		doFrag = false;
		doVert = true;
	elseif (arg[i] == "-p") then
		INPUT_PREFIX = arg[i+1];
		i=i+1;
	elseif (arg[i] == "-vdef") then
		VDEF_FILE = arg[i+1];
		i=i+1;
	elseif (arg[i] == "-D") then
		DEFS[#DEFS+1] = arg[i+1];
		i=i+1;
	elseif (not INPUT_FILE) then
		INPUT_FILE = arg[i];
		OUTPUT_FILE = INPUT_FILE;
	elseif (arg[i] == "-output" or arg[i] == "-o") then
		OUTPUT_FILE = arg[i+1];
		i=i+1;
	elseif (arg[i] == "-C_DO") then
		GLOBAL_C_DO = true;
	elseif (arg[i] == "-DX9_DO") then
		GLOBAL_DX9_DO = true;
	elseif (arg[i] == "-DX11_DO") then
		GLOBAL_DX11_DO = true;
	elseif (arg[i] == "-NACL_DO") then
		GLOBAL_NACL_DO = true;
	elseif (arg[i] == "-ANDROID_DO") then
		GLOBAL_ANDROID_DO = true;
	elseif (arg[i] == "-GLSL_DO") then
		GLOBAL_GLSL_DO = true;
	elseif (arg[i] == "-METAL_DO") then
		GLOBAL_METAL_DO = true;
	elseif (arg[i] == "-ORBIS_DO") then
		GLOBAL_ORBIS_DO = true;
	elseif (arg[i] == "-SPRIV_DO") then
		GLOBAL_SPRIV_DO = true;
	elseif (arg[i] == "-C_DONT") then
		GLOBAL_C_DO = false;
	elseif (arg[i] == "-DX9_DONT") then
		GLOBAL_DX9_DO = false;
	elseif (arg[i] == "-DX11_DONT") then
		GLOBAL_DX11_DO = false;
	elseif (arg[i] == "-NACL_DONT") then
		GLOBAL_NACL_DO = false;
	elseif (arg[i] == "-ANDROID_DONT") then
		GLOBAL_ANDROID_DO = false;
	elseif (arg[i] == "-GLSL_DONT") then
		GLOBAL_GLSL_DO = false;
	elseif (arg[i] == "-METAL_DONT") then
		GLOBAL_METAL_DO = false;
	elseif (arg[i] == "-ORBIS_DONT") then
		GLOBAL_ORBIS_DO = false;
	elseif (arg[i] == "-SPRIV_DONT") then
		GLOBAL_SPRIV_DO = false;
	end
end

if (not INPUT_FILE) then
	print("compile_shaders.lua: ERROR: no input files!");
	os.exit(false);
end

print ("INPUT_FILE", INPUT_FILE, INPUT_PREFIX)

if (INPUT_PREFIX) then
	INPUT_FILE = INPUT_PREFIX .. INPUT_FILE;
end

--[[
#Command to manually make a shader (OpenGL):
# Vertex:
#./shaderc -f vs_cubes.sc -o vs_cubes.bin --type v -i . --platform linux -p 120
# Fragment:
#./shaderc -f fs_cubes.sc -o fs_cubes.bin --type f -i . --platform linux -p 120
]]

-- Platform specific helper
local isWindows = (package.config:sub(1,1) == '\\');
function fixPath(path)
	if (isWindows) then
		return path:gsub("/", "\\");
	else
		return path;
	end
end
function getPath(str)
    return str:match("(.*[/\\])");
end

-- Search for shaderc
function file_exists(name)
   local f=io.open(name,"r")
   if f~=nil then io.close(f) return true else return false end
end

CC=fixPath("tools/shadercRelease")
if (isWindows) then CC = CC .. ".exe"; end
for i=1,5 do
	CC=fixPath("../"..CC)
	if (file_exists(CC)) then goto found_cc end
end

print ("Failed to find tools/shadercRelease!");
os.exit(false);
::found_cc::
--CC="wine "..CC..".exe";
print ("Found shaderc: "..CC)

CFLAGS=fixPath(" -i . -i include/ -i ../include/ ")

-- Flags for different targets
DX9_VS_FLAGS="--platform windows -p vs_4_0 -O 3"
DX9_FS_FLAGS="--platform windows -p ps_4_0 -O 3"
DX9_SHADER_PATH="dx9"
DX9_DO = true;

DX11_VS_FLAGS="--platform windows -p vs_4_0 -O 3"
DX11_FS_FLAGS="--platform windows -p ps_4_0 -O 3"
DX11_CS_FLAGS="--platform windows -p cs_5_0 -O 1"
DX11_SHADER_PATH="dx11"
DX11_DO = true;

NACL_VS_FLAGS="--platform nacl"
NACL_FS_FLAGS="--platform nacl"
NACL_SHADER_PATH="esslnacl"
NACL_DO = false;

ANDROID_VS_FLAGS="--platform android"
ANDROID_FS_FLAGS="--platform android"
ANDROID_CS_FLAGS="--platform android"
ANDROID_SHADER_PATH="esslandroid"
ANDROID_DO = false;

GLSL_VS_FLAGS="--platform linux -p 120"
GLSL_FS_FLAGS="--platform linux -p 120"
GLSL_CS_FLAGS="--platform linux -p 430"
GLSL_SHADER_PATH="glsl"
GLSL_DO = true;

METAL_VS_FLAGS="--platform osx -p metal"
METAL_FS_FLAGS="--platform osx -p metal"
METAL_CS_FLAGS="--platform osx -p metal"
METAL_SHADER_PATH="metal"
METAL_DO = true;

ORBIS_VS_FLAGS="--platform orbis -p pssl"
ORBIS_FS_FLAGS="--platform orbis -p pssl"
ORBIS_CS_FLAGS="--platform orbis -p pssl"
ORBIS_SHADER_PATH="pssl"
ORBIS_DO = false;

SPRIV_VS_FLAGS="--platform linux -p spirv"
SPRIV_FS_FLAGS="--platform linux -p spirv"
SPRIV_CS_FLAGS="--platform linux -p spirv"
SPRIV_SHADER_PATH="spirv"
SPRIV_DO = true;

C_SHADER_PATH="c"
C_DO = true;

if (GLOBAL_C_DO ~= nil) then C_DO = GLOBAL_C_DO; end -- Used invoking this from other scripts to override value
if (GLOBAL_DX9_DO ~= nil) then DX9_DO = GLOBAL_DX9_DO; end
if (GLOBAL_DX11_DO ~= nil) then DX11_DO = GLOBAL_DX11_DO; end
if (GLOBAL_NACL_DO ~= nil) then NACL_DO = GLOBAL_NACL_DO; end
if (GLOBAL_ANDROID_DO ~= nil) then ANDROID_DO = GLOBAL_ANDROID_DO; end
if (GLOBAL_GLSL_DO ~= nil) then GLSL_DO = GLOBAL_GLSL_DO; end
if (GLOBAL_METAL_DO ~= nil) then METAL_DO = GLOBAL_METAL_DO; end
if (GLOBAL_ORBIS_DO ~= nil) then ORBIS_DO = GLOBAL_ORBIS_DO; end
if (GLOBAL_SPRIV_DO ~= nil) then SPRIV_DO = GLOBAL_SPRIV_DO; end

function getBinOutput (path, shaderOutputPath, opPrefix, outputFile,suffix)
	suffix = suffix or ".bin";
	return path.."../"..shaderOutputPath.."/"..opPrefix..outputFile..suffix;
end

function buildCommand (path, inputFile, vdefFile, shaderOutputPath, outputFile, isFragment, sFlags, defs)
	local vtype = "v";
	local opPrefix = "vs_";
	
	if (isFragment) then vtype = "f"; opPrefix = "fs_"; end
	local r = CC .. " -f "..path..opPrefix..inputFile..".sc --varyingdef "..path.."varying_"..vdefFile..".def.sc -o "..getBinOutput(path, shaderOutputPath, opPrefix, outputFile).." --type "..vtype.." " .. CFLAGS .. " " .. sFlags;

	r = r .. " --define " .. inputFile:upper();
	if (defs and #defs > 0) then
		for i=1,#defs do
			if (i <= #defs) then
				if (isWindows) then
					r = r .. ";";
				else
					r = r .. "\\;"; --Escape character for semicolon
				end
			end
			r = r .. defs[i];
		end
	end

	return fixPath(r);
end


function buildC (path, inputFile, outputFile, isFragment)
	local vtype = "v";
	local opPrefix = "vs_";
	if (isFragment) then vtype = "f"; opPrefix = "fs_"; end
	
	local wo = getBinOutput(path, C_SHADER_PATH, opPrefix, outputFile, ".bin.h");
	
	local r = ""
	r = r .. xxd(getBinOutput(path, GLSL_SHADER_PATH, opPrefix, outputFile), "_"..GLSL_SHADER_PATH, not GLSL_DO);
	r = r .. xxd(getBinOutput(path, DX9_SHADER_PATH, opPrefix, outputFile), "_"..DX9_SHADER_PATH, not DX9_DO);
	r = r .. xxd(getBinOutput(path, DX11_SHADER_PATH, opPrefix, outputFile), "_"..DX11_SHADER_PATH, not DX11_DO);
	r = r .. xxd(getBinOutput(path, METAL_SHADER_PATH, opPrefix, outputFile), "_"..METAL_SHADER_PATH, not METAL_DO);
	r = r .. xxd(getBinOutput(path, SPRIV_SHADER_PATH, opPrefix, outputFile), "_"..SPRIV_SHADER_PATH, not SPRIV_DO); 
	r = r .. xxd(getBinOutput(path, ANDROID_SHADER_PATH, opPrefix, outputFile), "_"..ANDROID_SHADER_PATH, not ANDROID_DO); 
	r = r .. xxd(getBinOutput(path, ORBIS_SHADER_PATH, opPrefix, outputFile), "_"..ORBIS_SHADER_PATH, not ORBIS_DO); 
	r = r .. xxd(getBinOutput(path, NACL_SHADER_PATH, opPrefix, outputFile), "_"..NACL_SHADER_PATH, not NACL_DO); 
	return r, wo;
end

function buildShader (inputFile, outputFile, vdefFile, defs)
	-- Builds a shader for each api
	--if (outputFile == nil) then outputFile = shaderName end;
	--VDEF_FILE
	
	local path = getPath(inputFile) or "";
	if (path:len() > 0) then
		inputFile = inputFile:sub(path:len() + 1);
	end
	local patho = getPath(outputFile) or "";
	if (patho:len() > 0) then
		outputFile = outputFile:sub(patho:len() + 1);
	end
	if (not vdefFile) then
		vdefFile = inputFile;
	end
	
	print ("PATH:", path)
	print ("INPUT:", inputFile)
	print ("Varying.def: " .. path .. "varying_"..vdefFile..".def.sc")
	print ("Compile Frag: " .. tostring(doFrag) .. ", Source: " .. path.."fs_"..inputFile..".sc")
	print ("Compile Vert: " .. tostring(doVert) .. ", Source: " .. path.."vs_"..inputFile..".sc")
	print ("")
	print ("Building shader \""..path..inputFile.."\" to \""..path..outputFile.."\"")

	local cf_glsl  = buildCommand (path, inputFile, vdefFile, GLSL_SHADER_PATH, outputFile, true, GLSL_FS_FLAGS, defs);
	local cf_dx9   = buildCommand (path, inputFile, vdefFile, DX9_SHADER_PATH, outputFile, true, DX9_FS_FLAGS, defs);
	local cf_dx11  = buildCommand (path, inputFile, vdefFile, DX11_SHADER_PATH, outputFile, true, DX11_FS_FLAGS, defs);
	local cf_metal = buildCommand (path, inputFile, vdefFile, METAL_SHADER_PATH, outputFile, true, METAL_FS_FLAGS, defs);
	local cf_vk    = buildCommand (path, inputFile, vdefFile, SPRIV_SHADER_PATH, outputFile, true, SPRIV_FS_FLAGS, defs);
	local cf_an    = buildCommand (path, inputFile, vdefFile, ANDROID_SHADER_PATH, outputFile, true, ANDROID_FS_FLAGS, defs);
	local cf_pssl  = buildCommand (path, inputFile, vdefFile, ORBIS_SHADER_PATH, outputFile, true, ORBIS_FS_FLAGS, defs);
	local cf_nacl  = buildCommand (path, inputFile, vdefFile, NACL_SHADER_PATH, outputFile, true, NACL_FS_FLAGS, defs);

	local cv_glsl  = buildCommand (path, inputFile, vdefFile, GLSL_SHADER_PATH, outputFile, false, GLSL_VS_FLAGS, defs);
	local cv_dx9   = buildCommand (path, inputFile, vdefFile, DX9_SHADER_PATH, outputFile, false, DX9_VS_FLAGS, defs);
	local cv_dx11  = buildCommand (path, inputFile, vdefFile, DX11_SHADER_PATH, outputFile, false, DX11_VS_FLAGS, defs);
	local cv_metal = buildCommand (path, inputFile, vdefFile, METAL_SHADER_PATH, outputFile, false, METAL_VS_FLAGS, defs);
	local cv_vk    = buildCommand (path, inputFile, vdefFile, SPRIV_SHADER_PATH, outputFile, false, SPRIV_VS_FLAGS, defs);
	local cv_an    = buildCommand (path, inputFile, vdefFile, ANDROID_SHADER_PATH, outputFile, false, ANDROID_VS_FLAGS, defs);
	local cv_pssl  = buildCommand (path, inputFile, vdefFile, ORBIS_SHADER_PATH, outputFile, false, ORBIS_VS_FLAGS, defs);
	local cv_nacl  = buildCommand (path, inputFile, vdefFile, NACL_SHADER_PATH, outputFile, false, NACL_VS_FLAGS, defs);

	if (not C_ONLY) then
		if (GLSL_ONLY) then
			execc (cv_glsl , doit)
			execc (cf_glsl)
		elseif (HLSL_ONLY) then
			execc (cv_dx11)
			execc (cf_dx11)
		else
			execc (cf_glsl, GLSL_DO and doFrag)
			execc (cf_dx9, DX9_DO and doFrag)
			execc (cf_dx11, DX11_DO and doFrag)
			execc (cf_metal, METAL_DO and doFrag)
			execc (cf_vk, SPRIV_DO and doFrag)
			execc (cf_an, ANDROID_DO and doFrag)
			execc (cf_pssl, ORBIS_DO and doFrag)
			execc (cf_nacl, NACL_DO and doFrag)
			
			execc (cv_glsl, GLSL_DO and doVert)
			execc (cv_dx9, DX9_DO and doVert)
			execc (cv_dx11, DX11_DO and doVert)
			execc (cv_metal, METAL_DO and doVert)
			execc (cv_vk, SPRIV_DO and doVert)
			execc (cv_an, ANDROID_DO and doVert)
			execc (cv_pssl, ORBIS_DO and doVert)
			execc (cv_nacl, NACL_DO and doVert)
		end
	end
	
	--if ((C_DO and not (GLSL_ONLY or HLSL_ONLY)) or C_ONLY) then
	if (C_DO or C_ONLY) then
		print ("Building c header file:");
		if (doVert) then
			local c,cp = buildC (path, inputFile, outputFile, false);
			print ("",cp);
			local fh = io.open (cp, "w+");
			fh:write(c);
			fh:close();
		end
		if (doFrag) then
			local c,cp = buildC (path, inputFile, outputFile, true);
			print ("",cp);
			local fh = io.open (cp, "w+");
			fh:write(c);
			fh:close();
		end
	end
end

function execc (c, doit)
	-- Calls "c" and kills this script if the command results in an error
	if (doit == false) then return; end
	print (">> "..c);
	stat = os.execute(c);
	if (not stat) then print ("Failed to compile!"); os.exit(false); end
end

function btyeToPrintable (b)
	if (b >= 32 and b <= 126 and b~=92) then
		return string.char(b);
	end
	return '.';
end

function xxd(fname, variableSuffix, dummy)
	-- Implements xxd -i
	-- Forked from https://gist.github.com/cuhsat/cfa118f4398cae9e543b25cb5e19ecb6
	-- By Christian Uhsat
	variableSuffix = variableSuffix or "";
	local path = getPath(fname) or "";
	local fnameWoPath = fname;
	if (path:len() > 0) then
		fnameWoPath = fnameWoPath:sub(path:len() + 1);
	end
	local cSafeFname = (fnameWoPath):gsub("%.", "_");
	local HEADER = "static const uint8_t %s[] = {\n";
	local HEADER2 = "static const uint8_t * %s = NULL;\n";
	local FOOTER = " };"
	local LENGTH = "unsigned int %s_size = %s;\n\n"
	local r = "";
	local buf = {};

	if (not dummy) then
		local fh = io.open(fname, 'rb');
		if (not fh) then
			print ("xxd: could not open "..fname);
			dummy = true;
		else
			buf = fh:read('*all');
			fh:close();
		end
	end

	if (dummy) then
		r = r .. (HEADER2:format(cSafeFname..variableSuffix));
		r = r .. (LENGTH:format(cSafeFname..variableSuffix, #buf));
		r = r .. "BX_UNUSED("..(cSafeFname..variableSuffix)..")\n";
		r = r .. "BX_UNUSED("..(cSafeFname..variableSuffix).."_size)\n";
		return r;
	end
	r = r .. (HEADER:format(cSafeFname..variableSuffix));
	

	for i=1,#buf do
	  local b = string.format('0x%02x', buf:byte(i));

	  -- Indent block
	  if (i % 16 == 1) then
		b = '  ' .. b;
	  end

	  -- Ignore comma for last byte
	  if (i < #buf) then
		b = b .. ', ';
	  end

	  -- Add line break to line end + comment
	  if (i % 16 == 0) or (i == #buf) then
		if (i == #buf) then
			b = b .. (FOOTER);
			local rp = "      ";
			b = b .. rp:rep(16-(i%16));
			b = b:sub(0, b:len()-1);
		end
		b = b .. " // ";
		for j=i-15,i do
			b = b .. btyeToPrintable(buf:byte(j));
		end
		b = b ..'\n'
	  end

	  r = r .. b;
	end

	r = r .. (LENGTH:format(cSafeFname..variableSuffix, #buf));
	r = r .. "BX_UNUSED("..(cSafeFname..variableSuffix)..")\n";
	r = r .. "BX_UNUSED("..(cSafeFname..variableSuffix).."_size)\n";
	return r;
end

-- Actually build the shaders
buildShader (INPUT_FILE, OUTPUT_FILE, VDEF_FILE, DEFS);
