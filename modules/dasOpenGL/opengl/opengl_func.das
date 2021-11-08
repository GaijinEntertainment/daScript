require dasbind public

[extern(opengl,late,name="glCullFace",library="Opengl32")]
def glCullFace ( mode:uint ) : void
	pass

[extern(opengl,late,name="glFrontFace",library="Opengl32")]
def glFrontFace ( mode:uint ) : void
	pass

[extern(opengl,late,name="glHint",library="Opengl32")]
def glHint ( target:uint; mode:uint ) : void
	pass

[extern(opengl,late,name="glLineWidth",library="Opengl32")]
def glLineWidth ( width:float ) : void
	pass

[extern(opengl,late,name="glPointSize",library="Opengl32")]
def glPointSize ( size:float ) : void
	pass

[extern(opengl,late,name="glPolygonMode",library="Opengl32")]
def glPolygonMode ( face:uint; mode:uint ) : void
	pass

[extern(opengl,late,name="glScissor",library="Opengl32")]
def glScissor ( x:int; y:int; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glTexParameterf",library="Opengl32")]
def glTexParameterf ( target:uint; pname:uint; param:float ) : void
	pass

[extern(opengl,late,name="glTexParameterfv",library="Opengl32")]
def glTexParameterfv ( target:uint; pname:uint; params:float const? implicit ) : void
	pass

[extern(opengl,late,name="glTexParameteri",library="Opengl32")]
def glTexParameteri ( target:uint; pname:uint; param:int ) : void
	pass

[extern(opengl,late,name="glTexParameteriv",library="Opengl32")]
def glTexParameteriv ( target:uint; pname:uint; params:int const? implicit ) : void
	pass

[extern(opengl,late,name="glTexImage1D",library="Opengl32")]
def glTexImage1D ( target:uint; level:int; internalformat:int; width:int; border:int; format:uint; _type:uint; pixels:void const? implicit ) : void
	pass

[extern(opengl,late,name="glTexImage2D",library="Opengl32")]
def glTexImage2D ( target:uint; level:int; internalformat:int; width:int; height:int; border:int; format:uint; _type:uint; pixels:void const? implicit ) : void
	pass

[extern(opengl,late,name="glDrawBuffer",library="Opengl32")]
def glDrawBuffer ( buf:uint ) : void
	pass

[extern(opengl,late,name="glClear",library="Opengl32")]
def glClear ( mask:uint ) : void
	pass

[extern(opengl,late,name="glClearColor",library="Opengl32")]
def glClearColor ( red:float; green:float; blue:float; alpha:float ) : void
	pass

[extern(opengl,late,name="glClearStencil",library="Opengl32")]
def glClearStencil ( s:int ) : void
	pass

[extern(opengl,late,name="glClearDepth",library="Opengl32")]
def glClearDepth ( depth:double ) : void
	pass

[extern(opengl,late,name="glStencilMask",library="Opengl32")]
def glStencilMask ( mask:uint ) : void
	pass

[extern(opengl,late,name="glColorMask",library="Opengl32")]
def glColorMask ( red:bool; green:bool; blue:bool; alpha:bool ) : void
	pass

[extern(opengl,late,name="glDepthMask",library="Opengl32")]
def glDepthMask ( flag:bool ) : void
	pass

[extern(opengl,late,name="glDisable",library="Opengl32")]
def glDisable ( cap:uint ) : void
	pass

[extern(opengl,late,name="glEnable",library="Opengl32")]
def glEnable ( cap:uint ) : void
	pass

[extern(opengl,late,name="glFinish",library="Opengl32")]
def glFinish (  ) : void
	pass

[extern(opengl,late,name="glFlush",library="Opengl32")]
def glFlush (  ) : void
	pass

[extern(opengl,late,name="glBlendFunc",library="Opengl32")]
def glBlendFunc ( sfactor:uint; dfactor:uint ) : void
	pass

[extern(opengl,late,name="glLogicOp",library="Opengl32")]
def glLogicOp ( opcode:uint ) : void
	pass

[extern(opengl,late,name="glStencilFunc",library="Opengl32")]
def glStencilFunc ( func:uint; ref:int; mask:uint ) : void
	pass

[extern(opengl,late,name="glStencilOp",library="Opengl32")]
def glStencilOp ( fail:uint; zfail:uint; zpass:uint ) : void
	pass

[extern(opengl,late,name="glDepthFunc",library="Opengl32")]
def glDepthFunc ( func:uint ) : void
	pass

[extern(opengl,late,name="glPixelStoref",library="Opengl32")]
def glPixelStoref ( pname:uint; param:float ) : void
	pass

[extern(opengl,late,name="glPixelStorei",library="Opengl32")]
def glPixelStorei ( pname:uint; param:int ) : void
	pass

[extern(opengl,late,name="glReadBuffer",library="Opengl32")]
def glReadBuffer ( src:uint ) : void
	pass

[extern(opengl,late,name="glReadPixels",library="Opengl32")]
def glReadPixels ( x:int; y:int; width:int; height:int; format:uint; _type:uint; pixels:void? implicit ) : void
	pass

[extern(opengl,late,name="glGetBooleanv",library="Opengl32")]
def glGetBooleanv ( pname:uint; data:bool? implicit ) : void
	pass

[extern(opengl,late,name="glGetDoublev",library="Opengl32")]
def glGetDoublev ( pname:uint; data:double? implicit ) : void
	pass

[extern(opengl,late,name="glGetError",library="Opengl32")]
def glGetError (  ) : uint
	return [[uint]]

[extern(opengl,late,name="glGetFloatv",library="Opengl32")]
def glGetFloatv ( pname:uint; data:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetIntegerv",library="Opengl32")]
def glGetIntegerv ( pname:uint; data:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetString",library="Opengl32")]
def glGetString ( name:uint ) : uint8 const?
	return [[uint8 const?]]

[extern(opengl,late,name="glGetTexImage",library="Opengl32")]
def glGetTexImage ( target:uint; level:int; format:uint; _type:uint; pixels:void? implicit ) : void
	pass

[extern(opengl,late,name="glGetTexParameterfv",library="Opengl32")]
def glGetTexParameterfv ( target:uint; pname:uint; params:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetTexParameteriv",library="Opengl32")]
def glGetTexParameteriv ( target:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetTexLevelParameterfv",library="Opengl32")]
def glGetTexLevelParameterfv ( target:uint; level:int; pname:uint; params:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetTexLevelParameteriv",library="Opengl32")]
def glGetTexLevelParameteriv ( target:uint; level:int; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glIsEnabled",library="Opengl32")]
def glIsEnabled ( cap:uint ) : bool
	return [[bool]]

[extern(opengl,late,name="glDepthRange",library="Opengl32")]
def glDepthRange ( n:double; f:double ) : void
	pass

[extern(opengl,late,name="glViewport",library="Opengl32")]
def glViewport ( x:int; y:int; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glDrawArrays",library="Opengl32")]
def glDrawArrays ( mode:uint; first:int; count:int ) : void
	pass

[extern(opengl,late,name="glDrawElements",library="Opengl32")]
def glDrawElements ( mode:uint; count:int; _type:uint; indices:void const? implicit ) : void
	pass

[extern(opengl,late,name="glGetPointerv",library="Opengl32")]
def glGetPointerv ( pname:uint; params:void? implicit ) : void
	pass

[extern(opengl,late,name="glPolygonOffset",library="Opengl32")]
def glPolygonOffset ( factor:float; units:float ) : void
	pass

[extern(opengl,late,name="glCopyTexImage1D",library="Opengl32")]
def glCopyTexImage1D ( target:uint; level:int; internalformat:uint; x:int; y:int; width:int; border:int ) : void
	pass

[extern(opengl,late,name="glCopyTexImage2D",library="Opengl32")]
def glCopyTexImage2D ( target:uint; level:int; internalformat:uint; x:int; y:int; width:int; height:int; border:int ) : void
	pass

[extern(opengl,late,name="glCopyTexSubImage1D",library="Opengl32")]
def glCopyTexSubImage1D ( target:uint; level:int; xoffset:int; x:int; y:int; width:int ) : void
	pass

[extern(opengl,late,name="glCopyTexSubImage2D",library="Opengl32")]
def glCopyTexSubImage2D ( target:uint; level:int; xoffset:int; yoffset:int; x:int; y:int; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glTexSubImage1D",library="Opengl32")]
def glTexSubImage1D ( target:uint; level:int; xoffset:int; width:int; format:uint; _type:uint; pixels:void const? implicit ) : void
	pass

[extern(opengl,late,name="glTexSubImage2D",library="Opengl32")]
def glTexSubImage2D ( target:uint; level:int; xoffset:int; yoffset:int; width:int; height:int; format:uint; _type:uint; pixels:void const? implicit ) : void
	pass

[extern(opengl,late,name="glBindTexture",library="Opengl32")]
def glBindTexture ( target:uint; texture:uint ) : void
	pass

[extern(opengl,late,name="glDeleteTextures",library="Opengl32")]
def glDeleteTextures ( n:int; textures:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glGenTextures",library="Opengl32")]
def glGenTextures ( n:int; textures:uint? implicit ) : void
	pass

[extern(opengl,late,name="glIsTexture",library="Opengl32")]
def glIsTexture ( texture:uint ) : bool
	return [[bool]]

[extern(opengl,late,name="glDrawRangeElements",library="Opengl32")]
def glDrawRangeElements ( mode:uint; start:uint; end:uint; count:int; _type:uint; indices:void const? implicit ) : void
	pass

[extern(opengl,late,name="glTexImage3D",library="Opengl32")]
def glTexImage3D ( target:uint; level:int; internalformat:int; width:int; height:int; depth:int; border:int; format:uint; _type:uint; pixels:void const? implicit ) : void
	pass

[extern(opengl,late,name="glTexSubImage3D",library="Opengl32")]
def glTexSubImage3D ( target:uint; level:int; xoffset:int; yoffset:int; zoffset:int; width:int; height:int; depth:int; format:uint; _type:uint; pixels:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCopyTexSubImage3D",library="Opengl32")]
def glCopyTexSubImage3D ( target:uint; level:int; xoffset:int; yoffset:int; zoffset:int; x:int; y:int; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glActiveTexture",library="Opengl32")]
def glActiveTexture ( texture:uint ) : void
	pass

[extern(opengl,late,name="glSampleCoverage",library="Opengl32")]
def glSampleCoverage ( value:float; invert:bool ) : void
	pass

[extern(opengl,late,name="glCompressedTexImage3D",library="Opengl32")]
def glCompressedTexImage3D ( target:uint; level:int; internalformat:uint; width:int; height:int; depth:int; border:int; imageSize:int; data:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCompressedTexImage2D",library="Opengl32")]
def glCompressedTexImage2D ( target:uint; level:int; internalformat:uint; width:int; height:int; border:int; imageSize:int; data:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCompressedTexImage1D",library="Opengl32")]
def glCompressedTexImage1D ( target:uint; level:int; internalformat:uint; width:int; border:int; imageSize:int; data:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCompressedTexSubImage3D",library="Opengl32")]
def glCompressedTexSubImage3D ( target:uint; level:int; xoffset:int; yoffset:int; zoffset:int; width:int; height:int; depth:int; format:uint; imageSize:int; data:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCompressedTexSubImage2D",library="Opengl32")]
def glCompressedTexSubImage2D ( target:uint; level:int; xoffset:int; yoffset:int; width:int; height:int; format:uint; imageSize:int; data:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCompressedTexSubImage1D",library="Opengl32")]
def glCompressedTexSubImage1D ( target:uint; level:int; xoffset:int; width:int; format:uint; imageSize:int; data:void const? implicit ) : void
	pass

[extern(opengl,late,name="glGetCompressedTexImage",library="Opengl32")]
def glGetCompressedTexImage ( target:uint; level:int; img:void? implicit ) : void
	pass

[extern(opengl,late,name="glBlendFuncSeparate",library="Opengl32")]
def glBlendFuncSeparate ( sfactorRGB:uint; dfactorRGB:uint; sfactorAlpha:uint; dfactorAlpha:uint ) : void
	pass

[extern(opengl,late,name="glMultiDrawArrays",library="Opengl32")]
def glMultiDrawArrays ( mode:uint; first:int const? implicit; count:int const? implicit; drawcount:int ) : void
	pass

[extern(opengl,late,name="glMultiDrawElements",library="Opengl32")]
def glMultiDrawElements ( mode:uint; count:int const? implicit; _type:uint; indices:void const? implicit; drawcount:int ) : void
	pass

[extern(opengl,late,name="glPointParameterf",library="Opengl32")]
def glPointParameterf ( pname:uint; param:float ) : void
	pass

[extern(opengl,late,name="glPointParameterfv",library="Opengl32")]
def glPointParameterfv ( pname:uint; params:float const? implicit ) : void
	pass

[extern(opengl,late,name="glPointParameteri",library="Opengl32")]
def glPointParameteri ( pname:uint; param:int ) : void
	pass

[extern(opengl,late,name="glPointParameteriv",library="Opengl32")]
def glPointParameteriv ( pname:uint; params:int const? implicit ) : void
	pass

[extern(opengl,late,name="glBlendColor",library="Opengl32")]
def glBlendColor ( red:float; green:float; blue:float; alpha:float ) : void
	pass

[extern(opengl,late,name="glBlendEquation",library="Opengl32")]
def glBlendEquation ( mode:uint ) : void
	pass

[extern(opengl,late,name="glGenQueries",library="Opengl32")]
def glGenQueries ( n:int; ids:uint? implicit ) : void
	pass

[extern(opengl,late,name="glDeleteQueries",library="Opengl32")]
def glDeleteQueries ( n:int; ids:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glIsQuery",library="Opengl32")]
def glIsQuery ( id:uint ) : bool
	return [[bool]]

[extern(opengl,late,name="glBeginQuery",library="Opengl32")]
def glBeginQuery ( target:uint; id:uint ) : void
	pass

[extern(opengl,late,name="glEndQuery",library="Opengl32")]
def glEndQuery ( target:uint ) : void
	pass

[extern(opengl,late,name="glGetQueryiv",library="Opengl32")]
def glGetQueryiv ( target:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetQueryObjectiv",library="Opengl32")]
def glGetQueryObjectiv ( id:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetQueryObjectuiv",library="Opengl32")]
def glGetQueryObjectuiv ( id:uint; pname:uint; params:uint? implicit ) : void
	pass

[extern(opengl,late,name="glBindBuffer",library="Opengl32")]
def glBindBuffer ( target:uint; buffer:uint ) : void
	pass

[extern(opengl,late,name="glDeleteBuffers",library="Opengl32")]
def glDeleteBuffers ( n:int; buffers:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glGenBuffers",library="Opengl32")]
def glGenBuffers ( n:int; buffers:uint? implicit ) : void
	pass

[extern(opengl,late,name="glIsBuffer",library="Opengl32")]
def glIsBuffer ( buffer:uint ) : bool
	return [[bool]]

[extern(opengl,late,name="glBufferData",library="Opengl32")]
def glBufferData ( target:uint; size:int64; data:void const? implicit; usage:uint ) : void
	pass

[extern(opengl,late,name="glBufferSubData",library="Opengl32")]
def glBufferSubData ( target:uint; offset:int64; size:int64; data:void const? implicit ) : void
	pass

[extern(opengl,late,name="glGetBufferSubData",library="Opengl32")]
def glGetBufferSubData ( target:uint; offset:int64; size:int64; data:void? implicit ) : void
	pass

[extern(opengl,late,name="glMapBuffer",library="Opengl32")]
def glMapBuffer ( target:uint; access:uint ) : void?
	return [[void?]]

[extern(opengl,late,name="glUnmapBuffer",library="Opengl32")]
def glUnmapBuffer ( target:uint ) : bool
	return [[bool]]

[extern(opengl,late,name="glGetBufferParameteriv",library="Opengl32")]
def glGetBufferParameteriv ( target:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetBufferPointerv",library="Opengl32")]
def glGetBufferPointerv ( target:uint; pname:uint; params:void? implicit ) : void
	pass

[extern(opengl,late,name="glBlendEquationSeparate",library="Opengl32")]
def glBlendEquationSeparate ( modeRGB:uint; modeAlpha:uint ) : void
	pass

[extern(opengl,late,name="glDrawBuffers",library="Opengl32")]
def glDrawBuffers ( n:int; bufs:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glStencilOpSeparate",library="Opengl32")]
def glStencilOpSeparate ( face:uint; sfail:uint; dpfail:uint; dppass:uint ) : void
	pass

[extern(opengl,late,name="glStencilFuncSeparate",library="Opengl32")]
def glStencilFuncSeparate ( face:uint; func:uint; ref:int; mask:uint ) : void
	pass

[extern(opengl,late,name="glStencilMaskSeparate",library="Opengl32")]
def glStencilMaskSeparate ( face:uint; mask:uint ) : void
	pass

[extern(opengl,late,name="glAttachShader",library="Opengl32")]
def glAttachShader ( program:uint; shader:uint ) : void
	pass

[extern(opengl,late,name="glBindAttribLocation",library="Opengl32")]
def glBindAttribLocation ( program:uint; index:uint; name:string ) : void
	pass

[extern(opengl,late,name="glCompileShader",library="Opengl32")]
def glCompileShader ( shader:uint ) : void
	pass

[extern(opengl,late,name="glCreateProgram",library="Opengl32")]
def glCreateProgram (  ) : uint
	return [[uint]]

[extern(opengl,late,name="glCreateShader",library="Opengl32")]
def glCreateShader ( _type:uint ) : uint
	return [[uint]]

[extern(opengl,late,name="glDeleteProgram",library="Opengl32")]
def glDeleteProgram ( program:uint ) : void
	pass

[extern(opengl,late,name="glDeleteShader",library="Opengl32")]
def glDeleteShader ( shader:uint ) : void
	pass

[extern(opengl,late,name="glDetachShader",library="Opengl32")]
def glDetachShader ( program:uint; shader:uint ) : void
	pass

[extern(opengl,late,name="glDisableVertexAttribArray",library="Opengl32")]
def glDisableVertexAttribArray ( index:uint ) : void
	pass

[extern(opengl,late,name="glEnableVertexAttribArray",library="Opengl32")]
def glEnableVertexAttribArray ( index:uint ) : void
	pass

[extern(opengl,late,name="glGetActiveAttrib",library="Opengl32")]
def glGetActiveAttrib ( program:uint; index:uint; bufSize:int; length:int? implicit; size:int? implicit; _type:uint? implicit; name:string const ) : void
	pass

[extern(opengl,late,name="glGetActiveUniform",library="Opengl32")]
def glGetActiveUniform ( program:uint; index:uint; bufSize:int; length:int? implicit; size:int? implicit; _type:uint? implicit; name:string const ) : void
	pass

[extern(opengl,late,name="glGetAttachedShaders",library="Opengl32")]
def glGetAttachedShaders ( program:uint; maxCount:int; count:int? implicit; shaders:uint? implicit ) : void
	pass

[extern(opengl,late,name="glGetAttribLocation",library="Opengl32")]
def glGetAttribLocation ( program:uint; name:string ) : int
	return [[int]]

[extern(opengl,late,name="glGetProgramiv",library="Opengl32")]
def glGetProgramiv ( program:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetProgramInfoLog",library="Opengl32")]
def glGetProgramInfoLog ( program:uint; bufSize:int; length:int? implicit; infoLog:string const ) : void
	pass

[extern(opengl,late,name="glGetShaderiv",library="Opengl32")]
def glGetShaderiv ( shader:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetShaderInfoLog",library="Opengl32")]
def glGetShaderInfoLog ( shader:uint; bufSize:int; length:int? implicit; infoLog:string const ) : void
	pass

[extern(opengl,late,name="glGetShaderSource",library="Opengl32")]
def glGetShaderSource ( shader:uint; bufSize:int; length:int? implicit; source:string const ) : void
	pass

[extern(opengl,late,name="glGetUniformLocation",library="Opengl32")]
def glGetUniformLocation ( program:uint; name:string ) : int
	return [[int]]

[extern(opengl,late,name="glGetUniformfv",library="Opengl32")]
def glGetUniformfv ( program:uint; location:int; params:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetUniformiv",library="Opengl32")]
def glGetUniformiv ( program:uint; location:int; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetVertexAttribdv",library="Opengl32")]
def glGetVertexAttribdv ( index:uint; pname:uint; params:double? implicit ) : void
	pass

[extern(opengl,late,name="glGetVertexAttribfv",library="Opengl32")]
def glGetVertexAttribfv ( index:uint; pname:uint; params:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetVertexAttribiv",library="Opengl32")]
def glGetVertexAttribiv ( index:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetVertexAttribPointerv",library="Opengl32")]
def glGetVertexAttribPointerv ( index:uint; pname:uint; pointer:void? implicit ) : void
	pass

[extern(opengl,late,name="glIsProgram",library="Opengl32")]
def glIsProgram ( program:uint ) : bool
	return [[bool]]

[extern(opengl,late,name="glIsShader",library="Opengl32")]
def glIsShader ( shader:uint ) : bool
	return [[bool]]

[extern(opengl,late,name="glLinkProgram",library="Opengl32")]
def glLinkProgram ( program:uint ) : void
	pass

[extern(opengl,late,name="glShaderSource",library="Opengl32")]
def glShaderSource ( shader:uint; count:int; _string:string const? implicit; length:int const? implicit ) : void
	pass

[extern(opengl,late,name="glUseProgram",library="Opengl32")]
def glUseProgram ( program:uint ) : void
	pass

[extern(opengl,late,name="glUniform1f",library="Opengl32")]
def glUniform1f ( location:int; v0:float ) : void
	pass

[extern(opengl,late,name="glUniform2f",library="Opengl32")]
def glUniform2f ( location:int; v0:float; v1:float ) : void
	pass

[extern(opengl,late,name="glUniform3f",library="Opengl32")]
def glUniform3f ( location:int; v0:float; v1:float; v2:float ) : void
	pass

[extern(opengl,late,name="glUniform4f",library="Opengl32")]
def glUniform4f ( location:int; v0:float; v1:float; v2:float; v3:float ) : void
	pass

[extern(opengl,late,name="glUniform1i",library="Opengl32")]
def glUniform1i ( location:int; v0:int ) : void
	pass

[extern(opengl,late,name="glUniform2i",library="Opengl32")]
def glUniform2i ( location:int; v0:int; v1:int ) : void
	pass

[extern(opengl,late,name="glUniform3i",library="Opengl32")]
def glUniform3i ( location:int; v0:int; v1:int; v2:int ) : void
	pass

[extern(opengl,late,name="glUniform4i",library="Opengl32")]
def glUniform4i ( location:int; v0:int; v1:int; v2:int; v3:int ) : void
	pass

[extern(opengl,late,name="glUniform1fv",library="Opengl32")]
def glUniform1fv ( location:int; count:int; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform2fv",library="Opengl32")]
def glUniform2fv ( location:int; count:int; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform3fv",library="Opengl32")]
def glUniform3fv ( location:int; count:int; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform4fv",library="Opengl32")]
def glUniform4fv ( location:int; count:int; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform1iv",library="Opengl32")]
def glUniform1iv ( location:int; count:int; value:int const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform2iv",library="Opengl32")]
def glUniform2iv ( location:int; count:int; value:int const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform3iv",library="Opengl32")]
def glUniform3iv ( location:int; count:int; value:int const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform4iv",library="Opengl32")]
def glUniform4iv ( location:int; count:int; value:int const? implicit ) : void
	pass

[extern(opengl,late,name="glUniformMatrix2fv",library="Opengl32")]
def glUniformMatrix2fv ( location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glUniformMatrix3fv",library="Opengl32")]
def glUniformMatrix3fv ( location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glUniformMatrix4fv",library="Opengl32")]
def glUniformMatrix4fv ( location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glValidateProgram",library="Opengl32")]
def glValidateProgram ( program:uint ) : void
	pass

[extern(opengl,late,name="glVertexAttrib1d",library="Opengl32")]
def glVertexAttrib1d ( index:uint; x:double ) : void
	pass

[extern(opengl,late,name="glVertexAttrib1dv",library="Opengl32")]
def glVertexAttrib1dv ( index:uint; v:double const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttrib1f",library="Opengl32")]
def glVertexAttrib1f ( index:uint; x:float ) : void
	pass

[extern(opengl,late,name="glVertexAttrib1fv",library="Opengl32")]
def glVertexAttrib1fv ( index:uint; v:float const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttrib1s",library="Opengl32")]
def glVertexAttrib1s ( index:uint; x:int16 ) : void
	pass

[extern(opengl,late,name="glVertexAttrib1sv",library="Opengl32")]
def glVertexAttrib1sv ( index:uint; v:int16 const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttrib2d",library="Opengl32")]
def glVertexAttrib2d ( index:uint; x:double; y:double ) : void
	pass

[extern(opengl,late,name="glVertexAttrib2dv",library="Opengl32")]
def glVertexAttrib2dv ( index:uint; v:double const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttrib2f",library="Opengl32")]
def glVertexAttrib2f ( index:uint; x:float; y:float ) : void
	pass

[extern(opengl,late,name="glVertexAttrib2fv",library="Opengl32")]
def glVertexAttrib2fv ( index:uint; v:float const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttrib2s",library="Opengl32")]
def glVertexAttrib2s ( index:uint; x:int16; y:int16 ) : void
	pass

[extern(opengl,late,name="glVertexAttrib2sv",library="Opengl32")]
def glVertexAttrib2sv ( index:uint; v:int16 const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttrib3d",library="Opengl32")]
def glVertexAttrib3d ( index:uint; x:double; y:double; z:double ) : void
	pass

[extern(opengl,late,name="glVertexAttrib3dv",library="Opengl32")]
def glVertexAttrib3dv ( index:uint; v:double const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttrib3f",library="Opengl32")]
def glVertexAttrib3f ( index:uint; x:float; y:float; z:float ) : void
	pass

[extern(opengl,late,name="glVertexAttrib3fv",library="Opengl32")]
def glVertexAttrib3fv ( index:uint; v:float const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttrib3s",library="Opengl32")]
def glVertexAttrib3s ( index:uint; x:int16; y:int16; z:int16 ) : void
	pass

[extern(opengl,late,name="glVertexAttrib3sv",library="Opengl32")]
def glVertexAttrib3sv ( index:uint; v:int16 const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttrib4Nbv",library="Opengl32")]
def glVertexAttrib4Nbv ( index:uint; v:int8 const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttrib4Niv",library="Opengl32")]
def glVertexAttrib4Niv ( index:uint; v:int const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttrib4Nsv",library="Opengl32")]
def glVertexAttrib4Nsv ( index:uint; v:int16 const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttrib4Nub",library="Opengl32")]
def glVertexAttrib4Nub ( index:uint; x:uint8; y:uint8; z:uint8; w:uint8 ) : void
	pass

[extern(opengl,late,name="glVertexAttrib4Nubv",library="Opengl32")]
def glVertexAttrib4Nubv ( index:uint; v:uint8 const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttrib4Nuiv",library="Opengl32")]
def glVertexAttrib4Nuiv ( index:uint; v:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttrib4Nusv",library="Opengl32")]
def glVertexAttrib4Nusv ( index:uint; v:uint16 const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttrib4bv",library="Opengl32")]
def glVertexAttrib4bv ( index:uint; v:int8 const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttrib4d",library="Opengl32")]
def glVertexAttrib4d ( index:uint; x:double; y:double; z:double; w:double ) : void
	pass

[extern(opengl,late,name="glVertexAttrib4dv",library="Opengl32")]
def glVertexAttrib4dv ( index:uint; v:double const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttrib4f",library="Opengl32")]
def glVertexAttrib4f ( index:uint; x:float; y:float; z:float; w:float ) : void
	pass

[extern(opengl,late,name="glVertexAttrib4fv",library="Opengl32")]
def glVertexAttrib4fv ( index:uint; v:float const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttrib4iv",library="Opengl32")]
def glVertexAttrib4iv ( index:uint; v:int const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttrib4s",library="Opengl32")]
def glVertexAttrib4s ( index:uint; x:int16; y:int16; z:int16; w:int16 ) : void
	pass

[extern(opengl,late,name="glVertexAttrib4sv",library="Opengl32")]
def glVertexAttrib4sv ( index:uint; v:int16 const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttrib4ubv",library="Opengl32")]
def glVertexAttrib4ubv ( index:uint; v:uint8 const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttrib4uiv",library="Opengl32")]
def glVertexAttrib4uiv ( index:uint; v:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttrib4usv",library="Opengl32")]
def glVertexAttrib4usv ( index:uint; v:uint16 const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribPointer",library="Opengl32")]
def glVertexAttribPointer ( index:uint; size:int; _type:uint; normalized:bool; stride:int; pointer:void const? implicit ) : void
	pass

[extern(opengl,late,name="glUniformMatrix2x3fv",library="Opengl32")]
def glUniformMatrix2x3fv ( location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glUniformMatrix3x2fv",library="Opengl32")]
def glUniformMatrix3x2fv ( location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glUniformMatrix2x4fv",library="Opengl32")]
def glUniformMatrix2x4fv ( location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glUniformMatrix4x2fv",library="Opengl32")]
def glUniformMatrix4x2fv ( location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glUniformMatrix3x4fv",library="Opengl32")]
def glUniformMatrix3x4fv ( location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glUniformMatrix4x3fv",library="Opengl32")]
def glUniformMatrix4x3fv ( location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glColorMaski",library="Opengl32")]
def glColorMaski ( index:uint; r:bool; g:bool; b:bool; a:bool ) : void
	pass

[extern(opengl,late,name="glGetBooleani_v",library="Opengl32")]
def glGetBooleani_v ( target:uint; index:uint; data:bool? implicit ) : void
	pass

[extern(opengl,late,name="glGetIntegeri_v",library="Opengl32")]
def glGetIntegeri_v ( target:uint; index:uint; data:int? implicit ) : void
	pass

[extern(opengl,late,name="glEnablei",library="Opengl32")]
def glEnablei ( target:uint; index:uint ) : void
	pass

[extern(opengl,late,name="glDisablei",library="Opengl32")]
def glDisablei ( target:uint; index:uint ) : void
	pass

[extern(opengl,late,name="glIsEnabledi",library="Opengl32")]
def glIsEnabledi ( target:uint; index:uint ) : bool
	return [[bool]]

[extern(opengl,late,name="glBeginTransformFeedback",library="Opengl32")]
def glBeginTransformFeedback ( primitiveMode:uint ) : void
	pass

[extern(opengl,late,name="glEndTransformFeedback",library="Opengl32")]
def glEndTransformFeedback (  ) : void
	pass

[extern(opengl,late,name="glBindBufferRange",library="Opengl32")]
def glBindBufferRange ( target:uint; index:uint; buffer:uint; offset:int64; size:int64 ) : void
	pass

[extern(opengl,late,name="glBindBufferBase",library="Opengl32")]
def glBindBufferBase ( target:uint; index:uint; buffer:uint ) : void
	pass

[extern(opengl,late,name="glTransformFeedbackVaryings",library="Opengl32")]
def glTransformFeedbackVaryings ( program:uint; count:int; varyings:string const? implicit; bufferMode:uint ) : void
	pass

[extern(opengl,late,name="glGetTransformFeedbackVarying",library="Opengl32")]
def glGetTransformFeedbackVarying ( program:uint; index:uint; bufSize:int; length:int? implicit; size:int? implicit; _type:uint? implicit; name:string const ) : void
	pass

[extern(opengl,late,name="glClampColor",library="Opengl32")]
def glClampColor ( target:uint; clamp:uint ) : void
	pass

[extern(opengl,late,name="glBeginConditionalRender",library="Opengl32")]
def glBeginConditionalRender ( id:uint; mode:uint ) : void
	pass

[extern(opengl,late,name="glEndConditionalRender",library="Opengl32")]
def glEndConditionalRender (  ) : void
	pass

[extern(opengl,late,name="glVertexAttribIPointer",library="Opengl32")]
def glVertexAttribIPointer ( index:uint; size:int; _type:uint; stride:int; pointer:void const? implicit ) : void
	pass

[extern(opengl,late,name="glGetVertexAttribIiv",library="Opengl32")]
def glGetVertexAttribIiv ( index:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetVertexAttribIuiv",library="Opengl32")]
def glGetVertexAttribIuiv ( index:uint; pname:uint; params:uint? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribI1i",library="Opengl32")]
def glVertexAttribI1i ( index:uint; x:int ) : void
	pass

[extern(opengl,late,name="glVertexAttribI2i",library="Opengl32")]
def glVertexAttribI2i ( index:uint; x:int; y:int ) : void
	pass

[extern(opengl,late,name="glVertexAttribI3i",library="Opengl32")]
def glVertexAttribI3i ( index:uint; x:int; y:int; z:int ) : void
	pass

[extern(opengl,late,name="glVertexAttribI4i",library="Opengl32")]
def glVertexAttribI4i ( index:uint; x:int; y:int; z:int; w:int ) : void
	pass

[extern(opengl,late,name="glVertexAttribI1ui",library="Opengl32")]
def glVertexAttribI1ui ( index:uint; x:uint ) : void
	pass

[extern(opengl,late,name="glVertexAttribI2ui",library="Opengl32")]
def glVertexAttribI2ui ( index:uint; x:uint; y:uint ) : void
	pass

[extern(opengl,late,name="glVertexAttribI3ui",library="Opengl32")]
def glVertexAttribI3ui ( index:uint; x:uint; y:uint; z:uint ) : void
	pass

[extern(opengl,late,name="glVertexAttribI4ui",library="Opengl32")]
def glVertexAttribI4ui ( index:uint; x:uint; y:uint; z:uint; w:uint ) : void
	pass

[extern(opengl,late,name="glVertexAttribI1iv",library="Opengl32")]
def glVertexAttribI1iv ( index:uint; v:int const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribI2iv",library="Opengl32")]
def glVertexAttribI2iv ( index:uint; v:int const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribI3iv",library="Opengl32")]
def glVertexAttribI3iv ( index:uint; v:int const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribI4iv",library="Opengl32")]
def glVertexAttribI4iv ( index:uint; v:int const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribI1uiv",library="Opengl32")]
def glVertexAttribI1uiv ( index:uint; v:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribI2uiv",library="Opengl32")]
def glVertexAttribI2uiv ( index:uint; v:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribI3uiv",library="Opengl32")]
def glVertexAttribI3uiv ( index:uint; v:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribI4uiv",library="Opengl32")]
def glVertexAttribI4uiv ( index:uint; v:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribI4bv",library="Opengl32")]
def glVertexAttribI4bv ( index:uint; v:int8 const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribI4sv",library="Opengl32")]
def glVertexAttribI4sv ( index:uint; v:int16 const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribI4ubv",library="Opengl32")]
def glVertexAttribI4ubv ( index:uint; v:uint8 const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribI4usv",library="Opengl32")]
def glVertexAttribI4usv ( index:uint; v:uint16 const? implicit ) : void
	pass

[extern(opengl,late,name="glGetUniformuiv",library="Opengl32")]
def glGetUniformuiv ( program:uint; location:int; params:uint? implicit ) : void
	pass

[extern(opengl,late,name="glBindFragDataLocation",library="Opengl32")]
def glBindFragDataLocation ( program:uint; color:uint; name:string ) : void
	pass

[extern(opengl,late,name="glGetFragDataLocation",library="Opengl32")]
def glGetFragDataLocation ( program:uint; name:string ) : int
	return [[int]]

[extern(opengl,late,name="glUniform1ui",library="Opengl32")]
def glUniform1ui ( location:int; v0:uint ) : void
	pass

[extern(opengl,late,name="glUniform2ui",library="Opengl32")]
def glUniform2ui ( location:int; v0:uint; v1:uint ) : void
	pass

[extern(opengl,late,name="glUniform3ui",library="Opengl32")]
def glUniform3ui ( location:int; v0:uint; v1:uint; v2:uint ) : void
	pass

[extern(opengl,late,name="glUniform4ui",library="Opengl32")]
def glUniform4ui ( location:int; v0:uint; v1:uint; v2:uint; v3:uint ) : void
	pass

[extern(opengl,late,name="glUniform1uiv",library="Opengl32")]
def glUniform1uiv ( location:int; count:int; value:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform2uiv",library="Opengl32")]
def glUniform2uiv ( location:int; count:int; value:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform3uiv",library="Opengl32")]
def glUniform3uiv ( location:int; count:int; value:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform4uiv",library="Opengl32")]
def glUniform4uiv ( location:int; count:int; value:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glTexParameterIiv",library="Opengl32")]
def glTexParameterIiv ( target:uint; pname:uint; params:int const? implicit ) : void
	pass

[extern(opengl,late,name="glTexParameterIuiv",library="Opengl32")]
def glTexParameterIuiv ( target:uint; pname:uint; params:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glGetTexParameterIiv",library="Opengl32")]
def glGetTexParameterIiv ( target:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetTexParameterIuiv",library="Opengl32")]
def glGetTexParameterIuiv ( target:uint; pname:uint; params:uint? implicit ) : void
	pass

[extern(opengl,late,name="glClearBufferiv",library="Opengl32")]
def glClearBufferiv ( buffer:uint; drawbuffer:int; value:int const? implicit ) : void
	pass

[extern(opengl,late,name="glClearBufferuiv",library="Opengl32")]
def glClearBufferuiv ( buffer:uint; drawbuffer:int; value:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glClearBufferfv",library="Opengl32")]
def glClearBufferfv ( buffer:uint; drawbuffer:int; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glClearBufferfi",library="Opengl32")]
def glClearBufferfi ( buffer:uint; drawbuffer:int; depth:float; stencil:int ) : void
	pass

[extern(opengl,late,name="glGetStringi",library="Opengl32")]
def glGetStringi ( name:uint; index:uint ) : uint8 const?
	return [[uint8 const?]]

[extern(opengl,late,name="glIsRenderbuffer",library="Opengl32")]
def glIsRenderbuffer ( renderbuffer:uint ) : bool
	return [[bool]]

[extern(opengl,late,name="glBindRenderbuffer",library="Opengl32")]
def glBindRenderbuffer ( target:uint; renderbuffer:uint ) : void
	pass

[extern(opengl,late,name="glDeleteRenderbuffers",library="Opengl32")]
def glDeleteRenderbuffers ( n:int; renderbuffers:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glGenRenderbuffers",library="Opengl32")]
def glGenRenderbuffers ( n:int; renderbuffers:uint? implicit ) : void
	pass

[extern(opengl,late,name="glRenderbufferStorage",library="Opengl32")]
def glRenderbufferStorage ( target:uint; internalformat:uint; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glGetRenderbufferParameteriv",library="Opengl32")]
def glGetRenderbufferParameteriv ( target:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glIsFramebuffer",library="Opengl32")]
def glIsFramebuffer ( framebuffer:uint ) : bool
	return [[bool]]

[extern(opengl,late,name="glBindFramebuffer",library="Opengl32")]
def glBindFramebuffer ( target:uint; framebuffer:uint ) : void
	pass

[extern(opengl,late,name="glDeleteFramebuffers",library="Opengl32")]
def glDeleteFramebuffers ( n:int; framebuffers:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glGenFramebuffers",library="Opengl32")]
def glGenFramebuffers ( n:int; framebuffers:uint? implicit ) : void
	pass

[extern(opengl,late,name="glCheckFramebufferStatus",library="Opengl32")]
def glCheckFramebufferStatus ( target:uint ) : uint
	return [[uint]]

[extern(opengl,late,name="glFramebufferTexture1D",library="Opengl32")]
def glFramebufferTexture1D ( target:uint; attachment:uint; textarget:uint; texture:uint; level:int ) : void
	pass

[extern(opengl,late,name="glFramebufferTexture2D",library="Opengl32")]
def glFramebufferTexture2D ( target:uint; attachment:uint; textarget:uint; texture:uint; level:int ) : void
	pass

[extern(opengl,late,name="glFramebufferTexture3D",library="Opengl32")]
def glFramebufferTexture3D ( target:uint; attachment:uint; textarget:uint; texture:uint; level:int; zoffset:int ) : void
	pass

[extern(opengl,late,name="glFramebufferRenderbuffer",library="Opengl32")]
def glFramebufferRenderbuffer ( target:uint; attachment:uint; renderbuffertarget:uint; renderbuffer:uint ) : void
	pass

[extern(opengl,late,name="glGetFramebufferAttachmentParameteriv",library="Opengl32")]
def glGetFramebufferAttachmentParameteriv ( target:uint; attachment:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGenerateMipmap",library="Opengl32")]
def glGenerateMipmap ( target:uint ) : void
	pass

[extern(opengl,late,name="glBlitFramebuffer",library="Opengl32")]
def glBlitFramebuffer ( srcX0:int; srcY0:int; srcX1:int; srcY1:int; dstX0:int; dstY0:int; dstX1:int; dstY1:int; mask:uint; filter:uint ) : void
	pass

[extern(opengl,late,name="glRenderbufferStorageMultisample",library="Opengl32")]
def glRenderbufferStorageMultisample ( target:uint; samples:int; internalformat:uint; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glFramebufferTextureLayer",library="Opengl32")]
def glFramebufferTextureLayer ( target:uint; attachment:uint; texture:uint; level:int; layer:int ) : void
	pass

[extern(opengl,late,name="glMapBufferRange",library="Opengl32")]
def glMapBufferRange ( target:uint; offset:int64; length:int64; access:uint ) : void?
	return [[void?]]

[extern(opengl,late,name="glFlushMappedBufferRange",library="Opengl32")]
def glFlushMappedBufferRange ( target:uint; offset:int64; length:int64 ) : void
	pass

[extern(opengl,late,name="glBindVertexArray",library="Opengl32")]
def glBindVertexArray ( _array:uint ) : void
	pass

[extern(opengl,late,name="glDeleteVertexArrays",library="Opengl32")]
def glDeleteVertexArrays ( n:int; arrays:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glGenVertexArrays",library="Opengl32")]
def glGenVertexArrays ( n:int; arrays:uint? implicit ) : void
	pass

[extern(opengl,late,name="glIsVertexArray",library="Opengl32")]
def glIsVertexArray ( _array:uint ) : bool
	return [[bool]]

[extern(opengl,late,name="glDrawArraysInstanced",library="Opengl32")]
def glDrawArraysInstanced ( mode:uint; first:int; count:int; instancecount:int ) : void
	pass

[extern(opengl,late,name="glDrawElementsInstanced",library="Opengl32")]
def glDrawElementsInstanced ( mode:uint; count:int; _type:uint; indices:void const? implicit; instancecount:int ) : void
	pass

[extern(opengl,late,name="glTexBuffer",library="Opengl32")]
def glTexBuffer ( target:uint; internalformat:uint; buffer:uint ) : void
	pass

[extern(opengl,late,name="glPrimitiveRestartIndex",library="Opengl32")]
def glPrimitiveRestartIndex ( index:uint ) : void
	pass

[extern(opengl,late,name="glCopyBufferSubData",library="Opengl32")]
def glCopyBufferSubData ( readTarget:uint; writeTarget:uint; readOffset:int64; writeOffset:int64; size:int64 ) : void
	pass

[extern(opengl,late,name="glGetUniformIndices",library="Opengl32")]
def glGetUniformIndices ( program:uint; uniformCount:int; uniformNames:string const? implicit; uniformIndices:uint? implicit ) : void
	pass

[extern(opengl,late,name="glGetActiveUniformsiv",library="Opengl32")]
def glGetActiveUniformsiv ( program:uint; uniformCount:int; uniformIndices:uint const? implicit; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetActiveUniformName",library="Opengl32")]
def glGetActiveUniformName ( program:uint; uniformIndex:uint; bufSize:int; length:int? implicit; uniformName:string const ) : void
	pass

[extern(opengl,late,name="glGetUniformBlockIndex",library="Opengl32")]
def glGetUniformBlockIndex ( program:uint; uniformBlockName:string ) : uint
	return [[uint]]

[extern(opengl,late,name="glGetActiveUniformBlockiv",library="Opengl32")]
def glGetActiveUniformBlockiv ( program:uint; uniformBlockIndex:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetActiveUniformBlockName",library="Opengl32")]
def glGetActiveUniformBlockName ( program:uint; uniformBlockIndex:uint; bufSize:int; length:int? implicit; uniformBlockName:string const ) : void
	pass

[extern(opengl,late,name="glUniformBlockBinding",library="Opengl32")]
def glUniformBlockBinding ( program:uint; uniformBlockIndex:uint; uniformBlockBinding:uint ) : void
	pass

[extern(opengl,late,name="glDrawElementsBaseVertex",library="Opengl32")]
def glDrawElementsBaseVertex ( mode:uint; count:int; _type:uint; indices:void const? implicit; basevertex:int ) : void
	pass

[extern(opengl,late,name="glDrawRangeElementsBaseVertex",library="Opengl32")]
def glDrawRangeElementsBaseVertex ( mode:uint; start:uint; end:uint; count:int; _type:uint; indices:void const? implicit; basevertex:int ) : void
	pass

[extern(opengl,late,name="glDrawElementsInstancedBaseVertex",library="Opengl32")]
def glDrawElementsInstancedBaseVertex ( mode:uint; count:int; _type:uint; indices:void const? implicit; instancecount:int; basevertex:int ) : void
	pass

[extern(opengl,late,name="glMultiDrawElementsBaseVertex",library="Opengl32")]
def glMultiDrawElementsBaseVertex ( mode:uint; count:int const? implicit; _type:uint; indices:void const? implicit; drawcount:int; basevertex:int const? implicit ) : void
	pass

[extern(opengl,late,name="glProvokingVertex",library="Opengl32")]
def glProvokingVertex ( mode:uint ) : void
	pass

[extern(opengl,late,name="glFenceSync",library="Opengl32")]
def glFenceSync ( condition:uint; flags:uint ) : void?
	return [[void?]]

[extern(opengl,late,name="glIsSync",library="Opengl32")]
def glIsSync ( sync:void? implicit ) : bool
	return [[bool]]

[extern(opengl,late,name="glDeleteSync",library="Opengl32")]
def glDeleteSync ( sync:void? implicit ) : void
	pass

[extern(opengl,late,name="glClientWaitSync",library="Opengl32")]
def glClientWaitSync ( sync:void? implicit; flags:uint; timeout:uint64 ) : uint
	return [[uint]]

[extern(opengl,late,name="glWaitSync",library="Opengl32")]
def glWaitSync ( sync:void? implicit; flags:uint; timeout:uint64 ) : void
	pass

[extern(opengl,late,name="glGetInteger64v",library="Opengl32")]
def glGetInteger64v ( pname:uint; data:int64? implicit ) : void
	pass

[extern(opengl,late,name="glGetSynciv",library="Opengl32")]
def glGetSynciv ( sync:void? implicit; pname:uint; count:int; length:int? implicit; values:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetInteger64i_v",library="Opengl32")]
def glGetInteger64i_v ( target:uint; index:uint; data:int64? implicit ) : void
	pass

[extern(opengl,late,name="glGetBufferParameteri64v",library="Opengl32")]
def glGetBufferParameteri64v ( target:uint; pname:uint; params:int64? implicit ) : void
	pass

[extern(opengl,late,name="glFramebufferTexture",library="Opengl32")]
def glFramebufferTexture ( target:uint; attachment:uint; texture:uint; level:int ) : void
	pass

[extern(opengl,late,name="glTexImage2DMultisample",library="Opengl32")]
def glTexImage2DMultisample ( target:uint; samples:int; internalformat:uint; width:int; height:int; fixedsamplelocations:bool ) : void
	pass

[extern(opengl,late,name="glTexImage3DMultisample",library="Opengl32")]
def glTexImage3DMultisample ( target:uint; samples:int; internalformat:uint; width:int; height:int; depth:int; fixedsamplelocations:bool ) : void
	pass

[extern(opengl,late,name="glGetMultisamplefv",library="Opengl32")]
def glGetMultisamplefv ( pname:uint; index:uint; val:float? implicit ) : void
	pass

[extern(opengl,late,name="glSampleMaski",library="Opengl32")]
def glSampleMaski ( maskNumber:uint; mask:uint ) : void
	pass

[extern(opengl,late,name="glBindFragDataLocationIndexed",library="Opengl32")]
def glBindFragDataLocationIndexed ( program:uint; colorNumber:uint; index:uint; name:string ) : void
	pass

[extern(opengl,late,name="glGetFragDataIndex",library="Opengl32")]
def glGetFragDataIndex ( program:uint; name:string ) : int
	return [[int]]

[extern(opengl,late,name="glGenSamplers",library="Opengl32")]
def glGenSamplers ( count:int; samplers:uint? implicit ) : void
	pass

[extern(opengl,late,name="glDeleteSamplers",library="Opengl32")]
def glDeleteSamplers ( count:int; samplers:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glIsSampler",library="Opengl32")]
def glIsSampler ( sampler:uint ) : bool
	return [[bool]]

[extern(opengl,late,name="glBindSampler",library="Opengl32")]
def glBindSampler ( unit:uint; sampler:uint ) : void
	pass

[extern(opengl,late,name="glSamplerParameteri",library="Opengl32")]
def glSamplerParameteri ( sampler:uint; pname:uint; param:int ) : void
	pass

[extern(opengl,late,name="glSamplerParameteriv",library="Opengl32")]
def glSamplerParameteriv ( sampler:uint; pname:uint; param:int const? implicit ) : void
	pass

[extern(opengl,late,name="glSamplerParameterf",library="Opengl32")]
def glSamplerParameterf ( sampler:uint; pname:uint; param:float ) : void
	pass

[extern(opengl,late,name="glSamplerParameterfv",library="Opengl32")]
def glSamplerParameterfv ( sampler:uint; pname:uint; param:float const? implicit ) : void
	pass

[extern(opengl,late,name="glSamplerParameterIiv",library="Opengl32")]
def glSamplerParameterIiv ( sampler:uint; pname:uint; param:int const? implicit ) : void
	pass

[extern(opengl,late,name="glSamplerParameterIuiv",library="Opengl32")]
def glSamplerParameterIuiv ( sampler:uint; pname:uint; param:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glGetSamplerParameteriv",library="Opengl32")]
def glGetSamplerParameteriv ( sampler:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetSamplerParameterIiv",library="Opengl32")]
def glGetSamplerParameterIiv ( sampler:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetSamplerParameterfv",library="Opengl32")]
def glGetSamplerParameterfv ( sampler:uint; pname:uint; params:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetSamplerParameterIuiv",library="Opengl32")]
def glGetSamplerParameterIuiv ( sampler:uint; pname:uint; params:uint? implicit ) : void
	pass

[extern(opengl,late,name="glQueryCounter",library="Opengl32")]
def glQueryCounter ( id:uint; target:uint ) : void
	pass

[extern(opengl,late,name="glGetQueryObjecti64v",library="Opengl32")]
def glGetQueryObjecti64v ( id:uint; pname:uint; params:int64? implicit ) : void
	pass

[extern(opengl,late,name="glGetQueryObjectui64v",library="Opengl32")]
def glGetQueryObjectui64v ( id:uint; pname:uint; params:uint64? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribDivisor",library="Opengl32")]
def glVertexAttribDivisor ( index:uint; divisor:uint ) : void
	pass

[extern(opengl,late,name="glVertexAttribP1ui",library="Opengl32")]
def glVertexAttribP1ui ( index:uint; _type:uint; normalized:bool; value:uint ) : void
	pass

[extern(opengl,late,name="glVertexAttribP1uiv",library="Opengl32")]
def glVertexAttribP1uiv ( index:uint; _type:uint; normalized:bool; value:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribP2ui",library="Opengl32")]
def glVertexAttribP2ui ( index:uint; _type:uint; normalized:bool; value:uint ) : void
	pass

[extern(opengl,late,name="glVertexAttribP2uiv",library="Opengl32")]
def glVertexAttribP2uiv ( index:uint; _type:uint; normalized:bool; value:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribP3ui",library="Opengl32")]
def glVertexAttribP3ui ( index:uint; _type:uint; normalized:bool; value:uint ) : void
	pass

[extern(opengl,late,name="glVertexAttribP3uiv",library="Opengl32")]
def glVertexAttribP3uiv ( index:uint; _type:uint; normalized:bool; value:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribP4ui",library="Opengl32")]
def glVertexAttribP4ui ( index:uint; _type:uint; normalized:bool; value:uint ) : void
	pass

[extern(opengl,late,name="glVertexAttribP4uiv",library="Opengl32")]
def glVertexAttribP4uiv ( index:uint; _type:uint; normalized:bool; value:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glMinSampleShading",library="Opengl32")]
def glMinSampleShading ( value:float ) : void
	pass

[extern(opengl,late,name="glBlendEquationi",library="Opengl32")]
def glBlendEquationi ( buf:uint; mode:uint ) : void
	pass

[extern(opengl,late,name="glBlendEquationSeparatei",library="Opengl32")]
def glBlendEquationSeparatei ( buf:uint; modeRGB:uint; modeAlpha:uint ) : void
	pass

[extern(opengl,late,name="glBlendFunci",library="Opengl32")]
def glBlendFunci ( buf:uint; src:uint; dst:uint ) : void
	pass

[extern(opengl,late,name="glBlendFuncSeparatei",library="Opengl32")]
def glBlendFuncSeparatei ( buf:uint; srcRGB:uint; dstRGB:uint; srcAlpha:uint; dstAlpha:uint ) : void
	pass

[extern(opengl,late,name="glDrawArraysIndirect",library="Opengl32")]
def glDrawArraysIndirect ( mode:uint; indirect:void const? implicit ) : void
	pass

[extern(opengl,late,name="glDrawElementsIndirect",library="Opengl32")]
def glDrawElementsIndirect ( mode:uint; _type:uint; indirect:void const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform1d",library="Opengl32")]
def glUniform1d ( location:int; x:double ) : void
	pass

[extern(opengl,late,name="glUniform2d",library="Opengl32")]
def glUniform2d ( location:int; x:double; y:double ) : void
	pass

[extern(opengl,late,name="glUniform3d",library="Opengl32")]
def glUniform3d ( location:int; x:double; y:double; z:double ) : void
	pass

[extern(opengl,late,name="glUniform4d",library="Opengl32")]
def glUniform4d ( location:int; x:double; y:double; z:double; w:double ) : void
	pass

[extern(opengl,late,name="glUniform1dv",library="Opengl32")]
def glUniform1dv ( location:int; count:int; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform2dv",library="Opengl32")]
def glUniform2dv ( location:int; count:int; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform3dv",library="Opengl32")]
def glUniform3dv ( location:int; count:int; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform4dv",library="Opengl32")]
def glUniform4dv ( location:int; count:int; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glUniformMatrix2dv",library="Opengl32")]
def glUniformMatrix2dv ( location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glUniformMatrix3dv",library="Opengl32")]
def glUniformMatrix3dv ( location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glUniformMatrix4dv",library="Opengl32")]
def glUniformMatrix4dv ( location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glUniformMatrix2x3dv",library="Opengl32")]
def glUniformMatrix2x3dv ( location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glUniformMatrix2x4dv",library="Opengl32")]
def glUniformMatrix2x4dv ( location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glUniformMatrix3x2dv",library="Opengl32")]
def glUniformMatrix3x2dv ( location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glUniformMatrix3x4dv",library="Opengl32")]
def glUniformMatrix3x4dv ( location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glUniformMatrix4x2dv",library="Opengl32")]
def glUniformMatrix4x2dv ( location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glUniformMatrix4x3dv",library="Opengl32")]
def glUniformMatrix4x3dv ( location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glGetUniformdv",library="Opengl32")]
def glGetUniformdv ( program:uint; location:int; params:double? implicit ) : void
	pass

[extern(opengl,late,name="glGetSubroutineUniformLocation",library="Opengl32")]
def glGetSubroutineUniformLocation ( program:uint; shadertype:uint; name:string ) : int
	return [[int]]

[extern(opengl,late,name="glGetSubroutineIndex",library="Opengl32")]
def glGetSubroutineIndex ( program:uint; shadertype:uint; name:string ) : uint
	return [[uint]]

[extern(opengl,late,name="glGetActiveSubroutineUniformiv",library="Opengl32")]
def glGetActiveSubroutineUniformiv ( program:uint; shadertype:uint; index:uint; pname:uint; values:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetActiveSubroutineUniformName",library="Opengl32")]
def glGetActiveSubroutineUniformName ( program:uint; shadertype:uint; index:uint; bufSize:int; length:int? implicit; name:string const ) : void
	pass

[extern(opengl,late,name="glGetActiveSubroutineName",library="Opengl32")]
def glGetActiveSubroutineName ( program:uint; shadertype:uint; index:uint; bufSize:int; length:int? implicit; name:string const ) : void
	pass

[extern(opengl,late,name="glUniformSubroutinesuiv",library="Opengl32")]
def glUniformSubroutinesuiv ( shadertype:uint; count:int; indices:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glGetUniformSubroutineuiv",library="Opengl32")]
def glGetUniformSubroutineuiv ( shadertype:uint; location:int; params:uint? implicit ) : void
	pass

[extern(opengl,late,name="glGetProgramStageiv",library="Opengl32")]
def glGetProgramStageiv ( program:uint; shadertype:uint; pname:uint; values:int? implicit ) : void
	pass

[extern(opengl,late,name="glPatchParameteri",library="Opengl32")]
def glPatchParameteri ( pname:uint; value:int ) : void
	pass

[extern(opengl,late,name="glPatchParameterfv",library="Opengl32")]
def glPatchParameterfv ( pname:uint; values:float const? implicit ) : void
	pass

[extern(opengl,late,name="glBindTransformFeedback",library="Opengl32")]
def glBindTransformFeedback ( target:uint; id:uint ) : void
	pass

[extern(opengl,late,name="glDeleteTransformFeedbacks",library="Opengl32")]
def glDeleteTransformFeedbacks ( n:int; ids:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glGenTransformFeedbacks",library="Opengl32")]
def glGenTransformFeedbacks ( n:int; ids:uint? implicit ) : void
	pass

[extern(opengl,late,name="glIsTransformFeedback",library="Opengl32")]
def glIsTransformFeedback ( id:uint ) : bool
	return [[bool]]

[extern(opengl,late,name="glPauseTransformFeedback",library="Opengl32")]
def glPauseTransformFeedback (  ) : void
	pass

[extern(opengl,late,name="glResumeTransformFeedback",library="Opengl32")]
def glResumeTransformFeedback (  ) : void
	pass

[extern(opengl,late,name="glDrawTransformFeedback",library="Opengl32")]
def glDrawTransformFeedback ( mode:uint; id:uint ) : void
	pass

[extern(opengl,late,name="glDrawTransformFeedbackStream",library="Opengl32")]
def glDrawTransformFeedbackStream ( mode:uint; id:uint; stream:uint ) : void
	pass

[extern(opengl,late,name="glBeginQueryIndexed",library="Opengl32")]
def glBeginQueryIndexed ( target:uint; index:uint; id:uint ) : void
	pass

[extern(opengl,late,name="glEndQueryIndexed",library="Opengl32")]
def glEndQueryIndexed ( target:uint; index:uint ) : void
	pass

[extern(opengl,late,name="glGetQueryIndexediv",library="Opengl32")]
def glGetQueryIndexediv ( target:uint; index:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glReleaseShaderCompiler",library="Opengl32")]
def glReleaseShaderCompiler (  ) : void
	pass

[extern(opengl,late,name="glShaderBinary",library="Opengl32")]
def glShaderBinary ( count:int; shaders:uint const? implicit; binaryFormat:uint; binary:void const? implicit; length:int ) : void
	pass

[extern(opengl,late,name="glGetShaderPrecisionFormat",library="Opengl32")]
def glGetShaderPrecisionFormat ( shadertype:uint; precisiontype:uint; _range:int? implicit; precision:int? implicit ) : void
	pass

[extern(opengl,late,name="glDepthRangef",library="Opengl32")]
def glDepthRangef ( n:float; f:float ) : void
	pass

[extern(opengl,late,name="glClearDepthf",library="Opengl32")]
def glClearDepthf ( d:float ) : void
	pass

[extern(opengl,late,name="glGetProgramBinary",library="Opengl32")]
def glGetProgramBinary ( program:uint; bufSize:int; length:int? implicit; binaryFormat:uint? implicit; binary:void? implicit ) : void
	pass

[extern(opengl,late,name="glProgramBinary",library="Opengl32")]
def glProgramBinary ( program:uint; binaryFormat:uint; binary:void const? implicit; length:int ) : void
	pass

[extern(opengl,late,name="glProgramParameteri",library="Opengl32")]
def glProgramParameteri ( program:uint; pname:uint; value:int ) : void
	pass

[extern(opengl,late,name="glUseProgramStages",library="Opengl32")]
def glUseProgramStages ( pipeline:uint; stages:uint; program:uint ) : void
	pass

[extern(opengl,late,name="glActiveShaderProgram",library="Opengl32")]
def glActiveShaderProgram ( pipeline:uint; program:uint ) : void
	pass

[extern(opengl,late,name="glCreateShaderProgramv",library="Opengl32")]
def glCreateShaderProgramv ( _type:uint; count:int; strings:string const? implicit ) : uint
	return [[uint]]

[extern(opengl,late,name="glBindProgramPipeline",library="Opengl32")]
def glBindProgramPipeline ( pipeline:uint ) : void
	pass

[extern(opengl,late,name="glDeleteProgramPipelines",library="Opengl32")]
def glDeleteProgramPipelines ( n:int; pipelines:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glGenProgramPipelines",library="Opengl32")]
def glGenProgramPipelines ( n:int; pipelines:uint? implicit ) : void
	pass

[extern(opengl,late,name="glIsProgramPipeline",library="Opengl32")]
def glIsProgramPipeline ( pipeline:uint ) : bool
	return [[bool]]

[extern(opengl,late,name="glGetProgramPipelineiv",library="Opengl32")]
def glGetProgramPipelineiv ( pipeline:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform1i",library="Opengl32")]
def glProgramUniform1i ( program:uint; location:int; v0:int ) : void
	pass

[extern(opengl,late,name="glProgramUniform1iv",library="Opengl32")]
def glProgramUniform1iv ( program:uint; location:int; count:int; value:int const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform1f",library="Opengl32")]
def glProgramUniform1f ( program:uint; location:int; v0:float ) : void
	pass

[extern(opengl,late,name="glProgramUniform1fv",library="Opengl32")]
def glProgramUniform1fv ( program:uint; location:int; count:int; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform1d",library="Opengl32")]
def glProgramUniform1d ( program:uint; location:int; v0:double ) : void
	pass

[extern(opengl,late,name="glProgramUniform1dv",library="Opengl32")]
def glProgramUniform1dv ( program:uint; location:int; count:int; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform1ui",library="Opengl32")]
def glProgramUniform1ui ( program:uint; location:int; v0:uint ) : void
	pass

[extern(opengl,late,name="glProgramUniform1uiv",library="Opengl32")]
def glProgramUniform1uiv ( program:uint; location:int; count:int; value:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform2i",library="Opengl32")]
def glProgramUniform2i ( program:uint; location:int; v0:int; v1:int ) : void
	pass

[extern(opengl,late,name="glProgramUniform2iv",library="Opengl32")]
def glProgramUniform2iv ( program:uint; location:int; count:int; value:int const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform2f",library="Opengl32")]
def glProgramUniform2f ( program:uint; location:int; v0:float; v1:float ) : void
	pass

[extern(opengl,late,name="glProgramUniform2fv",library="Opengl32")]
def glProgramUniform2fv ( program:uint; location:int; count:int; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform2d",library="Opengl32")]
def glProgramUniform2d ( program:uint; location:int; v0:double; v1:double ) : void
	pass

[extern(opengl,late,name="glProgramUniform2dv",library="Opengl32")]
def glProgramUniform2dv ( program:uint; location:int; count:int; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform2ui",library="Opengl32")]
def glProgramUniform2ui ( program:uint; location:int; v0:uint; v1:uint ) : void
	pass

[extern(opengl,late,name="glProgramUniform2uiv",library="Opengl32")]
def glProgramUniform2uiv ( program:uint; location:int; count:int; value:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform3i",library="Opengl32")]
def glProgramUniform3i ( program:uint; location:int; v0:int; v1:int; v2:int ) : void
	pass

[extern(opengl,late,name="glProgramUniform3iv",library="Opengl32")]
def glProgramUniform3iv ( program:uint; location:int; count:int; value:int const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform3f",library="Opengl32")]
def glProgramUniform3f ( program:uint; location:int; v0:float; v1:float; v2:float ) : void
	pass

[extern(opengl,late,name="glProgramUniform3fv",library="Opengl32")]
def glProgramUniform3fv ( program:uint; location:int; count:int; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform3d",library="Opengl32")]
def glProgramUniform3d ( program:uint; location:int; v0:double; v1:double; v2:double ) : void
	pass

[extern(opengl,late,name="glProgramUniform3dv",library="Opengl32")]
def glProgramUniform3dv ( program:uint; location:int; count:int; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform3ui",library="Opengl32")]
def glProgramUniform3ui ( program:uint; location:int; v0:uint; v1:uint; v2:uint ) : void
	pass

[extern(opengl,late,name="glProgramUniform3uiv",library="Opengl32")]
def glProgramUniform3uiv ( program:uint; location:int; count:int; value:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform4i",library="Opengl32")]
def glProgramUniform4i ( program:uint; location:int; v0:int; v1:int; v2:int; v3:int ) : void
	pass

[extern(opengl,late,name="glProgramUniform4iv",library="Opengl32")]
def glProgramUniform4iv ( program:uint; location:int; count:int; value:int const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform4f",library="Opengl32")]
def glProgramUniform4f ( program:uint; location:int; v0:float; v1:float; v2:float; v3:float ) : void
	pass

[extern(opengl,late,name="glProgramUniform4fv",library="Opengl32")]
def glProgramUniform4fv ( program:uint; location:int; count:int; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform4d",library="Opengl32")]
def glProgramUniform4d ( program:uint; location:int; v0:double; v1:double; v2:double; v3:double ) : void
	pass

[extern(opengl,late,name="glProgramUniform4dv",library="Opengl32")]
def glProgramUniform4dv ( program:uint; location:int; count:int; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform4ui",library="Opengl32")]
def glProgramUniform4ui ( program:uint; location:int; v0:uint; v1:uint; v2:uint; v3:uint ) : void
	pass

[extern(opengl,late,name="glProgramUniform4uiv",library="Opengl32")]
def glProgramUniform4uiv ( program:uint; location:int; count:int; value:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix2fv",library="Opengl32")]
def glProgramUniformMatrix2fv ( program:uint; location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix3fv",library="Opengl32")]
def glProgramUniformMatrix3fv ( program:uint; location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix4fv",library="Opengl32")]
def glProgramUniformMatrix4fv ( program:uint; location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix2dv",library="Opengl32")]
def glProgramUniformMatrix2dv ( program:uint; location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix3dv",library="Opengl32")]
def glProgramUniformMatrix3dv ( program:uint; location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix4dv",library="Opengl32")]
def glProgramUniformMatrix4dv ( program:uint; location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix2x3fv",library="Opengl32")]
def glProgramUniformMatrix2x3fv ( program:uint; location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix3x2fv",library="Opengl32")]
def glProgramUniformMatrix3x2fv ( program:uint; location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix2x4fv",library="Opengl32")]
def glProgramUniformMatrix2x4fv ( program:uint; location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix4x2fv",library="Opengl32")]
def glProgramUniformMatrix4x2fv ( program:uint; location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix3x4fv",library="Opengl32")]
def glProgramUniformMatrix3x4fv ( program:uint; location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix4x3fv",library="Opengl32")]
def glProgramUniformMatrix4x3fv ( program:uint; location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix2x3dv",library="Opengl32")]
def glProgramUniformMatrix2x3dv ( program:uint; location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix3x2dv",library="Opengl32")]
def glProgramUniformMatrix3x2dv ( program:uint; location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix2x4dv",library="Opengl32")]
def glProgramUniformMatrix2x4dv ( program:uint; location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix4x2dv",library="Opengl32")]
def glProgramUniformMatrix4x2dv ( program:uint; location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix3x4dv",library="Opengl32")]
def glProgramUniformMatrix3x4dv ( program:uint; location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix4x3dv",library="Opengl32")]
def glProgramUniformMatrix4x3dv ( program:uint; location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glValidateProgramPipeline",library="Opengl32")]
def glValidateProgramPipeline ( pipeline:uint ) : void
	pass

[extern(opengl,late,name="glGetProgramPipelineInfoLog",library="Opengl32")]
def glGetProgramPipelineInfoLog ( pipeline:uint; bufSize:int; length:int? implicit; infoLog:string const ) : void
	pass

[extern(opengl,late,name="glVertexAttribL1d",library="Opengl32")]
def glVertexAttribL1d ( index:uint; x:double ) : void
	pass

[extern(opengl,late,name="glVertexAttribL2d",library="Opengl32")]
def glVertexAttribL2d ( index:uint; x:double; y:double ) : void
	pass

[extern(opengl,late,name="glVertexAttribL3d",library="Opengl32")]
def glVertexAttribL3d ( index:uint; x:double; y:double; z:double ) : void
	pass

[extern(opengl,late,name="glVertexAttribL4d",library="Opengl32")]
def glVertexAttribL4d ( index:uint; x:double; y:double; z:double; w:double ) : void
	pass

[extern(opengl,late,name="glVertexAttribL1dv",library="Opengl32")]
def glVertexAttribL1dv ( index:uint; v:double const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribL2dv",library="Opengl32")]
def glVertexAttribL2dv ( index:uint; v:double const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribL3dv",library="Opengl32")]
def glVertexAttribL3dv ( index:uint; v:double const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribL4dv",library="Opengl32")]
def glVertexAttribL4dv ( index:uint; v:double const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribLPointer",library="Opengl32")]
def glVertexAttribLPointer ( index:uint; size:int; _type:uint; stride:int; pointer:void const? implicit ) : void
	pass

[extern(opengl,late,name="glGetVertexAttribLdv",library="Opengl32")]
def glGetVertexAttribLdv ( index:uint; pname:uint; params:double? implicit ) : void
	pass

[extern(opengl,late,name="glViewportArrayv",library="Opengl32")]
def glViewportArrayv ( first:uint; count:int; v:float const? implicit ) : void
	pass

[extern(opengl,late,name="glViewportIndexedf",library="Opengl32")]
def glViewportIndexedf ( index:uint; x:float; y:float; w:float; h:float ) : void
	pass

[extern(opengl,late,name="glViewportIndexedfv",library="Opengl32")]
def glViewportIndexedfv ( index:uint; v:float const? implicit ) : void
	pass

[extern(opengl,late,name="glScissorArrayv",library="Opengl32")]
def glScissorArrayv ( first:uint; count:int; v:int const? implicit ) : void
	pass

[extern(opengl,late,name="glScissorIndexed",library="Opengl32")]
def glScissorIndexed ( index:uint; left:int; bottom:int; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glScissorIndexedv",library="Opengl32")]
def glScissorIndexedv ( index:uint; v:int const? implicit ) : void
	pass

[extern(opengl,late,name="glDepthRangeArrayv",library="Opengl32")]
def glDepthRangeArrayv ( first:uint; count:int; v:double const? implicit ) : void
	pass

[extern(opengl,late,name="glDepthRangeIndexed",library="Opengl32")]
def glDepthRangeIndexed ( index:uint; n:double; f:double ) : void
	pass

[extern(opengl,late,name="glGetFloati_v",library="Opengl32")]
def glGetFloati_v ( target:uint; index:uint; data:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetDoublei_v",library="Opengl32")]
def glGetDoublei_v ( target:uint; index:uint; data:double? implicit ) : void
	pass

[extern(opengl,late,name="glDrawArraysInstancedBaseInstance",library="Opengl32")]
def glDrawArraysInstancedBaseInstance ( mode:uint; first:int; count:int; instancecount:int; baseinstance:uint ) : void
	pass

[extern(opengl,late,name="glDrawElementsInstancedBaseInstance",library="Opengl32")]
def glDrawElementsInstancedBaseInstance ( mode:uint; count:int; _type:uint; indices:void const? implicit; instancecount:int; baseinstance:uint ) : void
	pass

[extern(opengl,late,name="glDrawElementsInstancedBaseVertexBaseInstance",library="Opengl32")]
def glDrawElementsInstancedBaseVertexBaseInstance ( mode:uint; count:int; _type:uint; indices:void const? implicit; instancecount:int; basevertex:int; baseinstance:uint ) : void
	pass

[extern(opengl,late,name="glGetInternalformativ",library="Opengl32")]
def glGetInternalformativ ( target:uint; internalformat:uint; pname:uint; count:int; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetActiveAtomicCounterBufferiv",library="Opengl32")]
def glGetActiveAtomicCounterBufferiv ( program:uint; bufferIndex:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glBindImageTexture",library="Opengl32")]
def glBindImageTexture ( unit:uint; texture:uint; level:int; layered:bool; layer:int; access:uint; format:uint ) : void
	pass

[extern(opengl,late,name="glMemoryBarrier",library="Opengl32")]
def glMemoryBarrier ( barriers:uint ) : void
	pass

[extern(opengl,late,name="glTexStorage1D",library="Opengl32")]
def glTexStorage1D ( target:uint; levels:int; internalformat:uint; width:int ) : void
	pass

[extern(opengl,late,name="glTexStorage2D",library="Opengl32")]
def glTexStorage2D ( target:uint; levels:int; internalformat:uint; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glTexStorage3D",library="Opengl32")]
def glTexStorage3D ( target:uint; levels:int; internalformat:uint; width:int; height:int; depth:int ) : void
	pass

[extern(opengl,late,name="glDrawTransformFeedbackInstanced",library="Opengl32")]
def glDrawTransformFeedbackInstanced ( mode:uint; id:uint; instancecount:int ) : void
	pass

[extern(opengl,late,name="glDrawTransformFeedbackStreamInstanced",library="Opengl32")]
def glDrawTransformFeedbackStreamInstanced ( mode:uint; id:uint; stream:uint; instancecount:int ) : void
	pass

[extern(opengl,late,name="glClearBufferData",library="Opengl32")]
def glClearBufferData ( target:uint; internalformat:uint; format:uint; _type:uint; data:void const? implicit ) : void
	pass

[extern(opengl,late,name="glClearBufferSubData",library="Opengl32")]
def glClearBufferSubData ( target:uint; internalformat:uint; offset:int64; size:int64; format:uint; _type:uint; data:void const? implicit ) : void
	pass

[extern(opengl,late,name="glDispatchCompute",library="Opengl32")]
def glDispatchCompute ( num_groups_x:uint; num_groups_y:uint; num_groups_z:uint ) : void
	pass

[extern(opengl,late,name="glDispatchComputeIndirect",library="Opengl32")]
def glDispatchComputeIndirect ( indirect:int64 ) : void
	pass

[extern(opengl,late,name="glCopyImageSubData",library="Opengl32")]
def glCopyImageSubData ( srcName:uint; srcTarget:uint; srcLevel:int; srcX:int; srcY:int; srcZ:int; dstName:uint; dstTarget:uint; dstLevel:int; dstX:int; dstY:int; dstZ:int; srcWidth:int; srcHeight:int; srcDepth:int ) : void
	pass

[extern(opengl,late,name="glFramebufferParameteri",library="Opengl32")]
def glFramebufferParameteri ( target:uint; pname:uint; param:int ) : void
	pass

[extern(opengl,late,name="glGetFramebufferParameteriv",library="Opengl32")]
def glGetFramebufferParameteriv ( target:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetInternalformati64v",library="Opengl32")]
def glGetInternalformati64v ( target:uint; internalformat:uint; pname:uint; count:int; params:int64? implicit ) : void
	pass

[extern(opengl,late,name="glInvalidateTexSubImage",library="Opengl32")]
def glInvalidateTexSubImage ( texture:uint; level:int; xoffset:int; yoffset:int; zoffset:int; width:int; height:int; depth:int ) : void
	pass

[extern(opengl,late,name="glInvalidateTexImage",library="Opengl32")]
def glInvalidateTexImage ( texture:uint; level:int ) : void
	pass

[extern(opengl,late,name="glInvalidateBufferSubData",library="Opengl32")]
def glInvalidateBufferSubData ( buffer:uint; offset:int64; length:int64 ) : void
	pass

[extern(opengl,late,name="glInvalidateBufferData",library="Opengl32")]
def glInvalidateBufferData ( buffer:uint ) : void
	pass

[extern(opengl,late,name="glInvalidateFramebuffer",library="Opengl32")]
def glInvalidateFramebuffer ( target:uint; numAttachments:int; attachments:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glInvalidateSubFramebuffer",library="Opengl32")]
def glInvalidateSubFramebuffer ( target:uint; numAttachments:int; attachments:uint const? implicit; x:int; y:int; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glMultiDrawArraysIndirect",library="Opengl32")]
def glMultiDrawArraysIndirect ( mode:uint; indirect:void const? implicit; drawcount:int; stride:int ) : void
	pass

[extern(opengl,late,name="glMultiDrawElementsIndirect",library="Opengl32")]
def glMultiDrawElementsIndirect ( mode:uint; _type:uint; indirect:void const? implicit; drawcount:int; stride:int ) : void
	pass

[extern(opengl,late,name="glGetProgramInterfaceiv",library="Opengl32")]
def glGetProgramInterfaceiv ( program:uint; programInterface:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetProgramResourceIndex",library="Opengl32")]
def glGetProgramResourceIndex ( program:uint; programInterface:uint; name:string ) : uint
	return [[uint]]

[extern(opengl,late,name="glGetProgramResourceName",library="Opengl32")]
def glGetProgramResourceName ( program:uint; programInterface:uint; index:uint; bufSize:int; length:int? implicit; name:string const ) : void
	pass

[extern(opengl,late,name="glGetProgramResourceiv",library="Opengl32")]
def glGetProgramResourceiv ( program:uint; programInterface:uint; index:uint; propCount:int; props:uint const? implicit; count:int; length:int? implicit; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetProgramResourceLocation",library="Opengl32")]
def glGetProgramResourceLocation ( program:uint; programInterface:uint; name:string ) : int
	return [[int]]

[extern(opengl,late,name="glGetProgramResourceLocationIndex",library="Opengl32")]
def glGetProgramResourceLocationIndex ( program:uint; programInterface:uint; name:string ) : int
	return [[int]]

[extern(opengl,late,name="glShaderStorageBlockBinding",library="Opengl32")]
def glShaderStorageBlockBinding ( program:uint; storageBlockIndex:uint; storageBlockBinding:uint ) : void
	pass

[extern(opengl,late,name="glTexBufferRange",library="Opengl32")]
def glTexBufferRange ( target:uint; internalformat:uint; buffer:uint; offset:int64; size:int64 ) : void
	pass

[extern(opengl,late,name="glTexStorage2DMultisample",library="Opengl32")]
def glTexStorage2DMultisample ( target:uint; samples:int; internalformat:uint; width:int; height:int; fixedsamplelocations:bool ) : void
	pass

[extern(opengl,late,name="glTexStorage3DMultisample",library="Opengl32")]
def glTexStorage3DMultisample ( target:uint; samples:int; internalformat:uint; width:int; height:int; depth:int; fixedsamplelocations:bool ) : void
	pass

[extern(opengl,late,name="glTextureView",library="Opengl32")]
def glTextureView ( texture:uint; target:uint; origtexture:uint; internalformat:uint; minlevel:uint; numlevels:uint; minlayer:uint; numlayers:uint ) : void
	pass

[extern(opengl,late,name="glBindVertexBuffer",library="Opengl32")]
def glBindVertexBuffer ( bindingindex:uint; buffer:uint; offset:int64; stride:int ) : void
	pass

[extern(opengl,late,name="glVertexAttribFormat",library="Opengl32")]
def glVertexAttribFormat ( attribindex:uint; size:int; _type:uint; normalized:bool; relativeoffset:uint ) : void
	pass

[extern(opengl,late,name="glVertexAttribIFormat",library="Opengl32")]
def glVertexAttribIFormat ( attribindex:uint; size:int; _type:uint; relativeoffset:uint ) : void
	pass

[extern(opengl,late,name="glVertexAttribLFormat",library="Opengl32")]
def glVertexAttribLFormat ( attribindex:uint; size:int; _type:uint; relativeoffset:uint ) : void
	pass

[extern(opengl,late,name="glVertexAttribBinding",library="Opengl32")]
def glVertexAttribBinding ( attribindex:uint; bindingindex:uint ) : void
	pass

[extern(opengl,late,name="glVertexBindingDivisor",library="Opengl32")]
def glVertexBindingDivisor ( bindingindex:uint; divisor:uint ) : void
	pass

[extern(opengl,late,name="glDebugMessageControl",library="Opengl32")]
def glDebugMessageControl ( source:uint; _type:uint; severity:uint; count:int; ids:uint const? implicit; enabled:bool ) : void
	pass

[extern(opengl,late,name="glDebugMessageInsert",library="Opengl32")]
def glDebugMessageInsert ( source:uint; _type:uint; id:uint; severity:uint; length:int; buf:string ) : void
	pass

[extern(opengl,late,name="glDebugMessageCallback",library="Opengl32")]
def glDebugMessageCallback ( userParam:void const? implicit ) : void
	pass

[extern(opengl,late,name="glGetDebugMessageLog",library="Opengl32")]
def glGetDebugMessageLog ( count:uint; bufSize:int; sources:uint? implicit; types:uint? implicit; ids:uint? implicit; severities:uint? implicit; lengths:int? implicit; messageLog:string const ) : uint
	return [[uint]]

[extern(opengl,late,name="glPushDebugGroup",library="Opengl32")]
def glPushDebugGroup ( source:uint; id:uint; length:int; message:string ) : void
	pass

[extern(opengl,late,name="glPopDebugGroup",library="Opengl32")]
def glPopDebugGroup (  ) : void
	pass

[extern(opengl,late,name="glObjectLabel",library="Opengl32")]
def glObjectLabel ( identifier:uint; name:uint; length:int; _label:string ) : void
	pass

[extern(opengl,late,name="glGetObjectLabel",library="Opengl32")]
def glGetObjectLabel ( identifier:uint; name:uint; bufSize:int; length:int? implicit; _label:string const ) : void
	pass

[extern(opengl,late,name="glObjectPtrLabel",library="Opengl32")]
def glObjectPtrLabel ( ptr:void const? implicit; length:int; _label:string ) : void
	pass

[extern(opengl,late,name="glGetObjectPtrLabel",library="Opengl32")]
def glGetObjectPtrLabel ( ptr:void const? implicit; bufSize:int; length:int? implicit; _label:string const ) : void
	pass

[extern(opengl,late,name="glBufferStorage",library="Opengl32")]
def glBufferStorage ( target:uint; size:int64; data:void const? implicit; flags:uint ) : void
	pass

[extern(opengl,late,name="glClearTexImage",library="Opengl32")]
def glClearTexImage ( texture:uint; level:int; format:uint; _type:uint; data:void const? implicit ) : void
	pass

[extern(opengl,late,name="glClearTexSubImage",library="Opengl32")]
def glClearTexSubImage ( texture:uint; level:int; xoffset:int; yoffset:int; zoffset:int; width:int; height:int; depth:int; format:uint; _type:uint; data:void const? implicit ) : void
	pass

[extern(opengl,late,name="glBindBuffersBase",library="Opengl32")]
def glBindBuffersBase ( target:uint; first:uint; count:int; buffers:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glBindBuffersRange",library="Opengl32")]
def glBindBuffersRange ( target:uint; first:uint; count:int; buffers:uint const? implicit; offsets:int64 const? implicit; sizes:int64 const? implicit ) : void
	pass

[extern(opengl,late,name="glBindTextures",library="Opengl32")]
def glBindTextures ( first:uint; count:int; textures:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glBindSamplers",library="Opengl32")]
def glBindSamplers ( first:uint; count:int; samplers:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glBindImageTextures",library="Opengl32")]
def glBindImageTextures ( first:uint; count:int; textures:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glBindVertexBuffers",library="Opengl32")]
def glBindVertexBuffers ( first:uint; count:int; buffers:uint const? implicit; offsets:int64 const? implicit; strides:int const? implicit ) : void
	pass

[extern(opengl,late,name="glClipControl",library="Opengl32")]
def glClipControl ( origin:uint; depth:uint ) : void
	pass

[extern(opengl,late,name="glCreateTransformFeedbacks",library="Opengl32")]
def glCreateTransformFeedbacks ( n:int; ids:uint? implicit ) : void
	pass

[extern(opengl,late,name="glTransformFeedbackBufferBase",library="Opengl32")]
def glTransformFeedbackBufferBase ( xfb:uint; index:uint; buffer:uint ) : void
	pass

[extern(opengl,late,name="glTransformFeedbackBufferRange",library="Opengl32")]
def glTransformFeedbackBufferRange ( xfb:uint; index:uint; buffer:uint; offset:int64; size:int64 ) : void
	pass

[extern(opengl,late,name="glGetTransformFeedbackiv",library="Opengl32")]
def glGetTransformFeedbackiv ( xfb:uint; pname:uint; param:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetTransformFeedbacki_v",library="Opengl32")]
def glGetTransformFeedbacki_v ( xfb:uint; pname:uint; index:uint; param:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetTransformFeedbacki64_v",library="Opengl32")]
def glGetTransformFeedbacki64_v ( xfb:uint; pname:uint; index:uint; param:int64? implicit ) : void
	pass

[extern(opengl,late,name="glCreateBuffers",library="Opengl32")]
def glCreateBuffers ( n:int; buffers:uint? implicit ) : void
	pass

[extern(opengl,late,name="glNamedBufferStorage",library="Opengl32")]
def glNamedBufferStorage ( buffer:uint; size:int64; data:void const? implicit; flags:uint ) : void
	pass

[extern(opengl,late,name="glNamedBufferData",library="Opengl32")]
def glNamedBufferData ( buffer:uint; size:int64; data:void const? implicit; usage:uint ) : void
	pass

[extern(opengl,late,name="glNamedBufferSubData",library="Opengl32")]
def glNamedBufferSubData ( buffer:uint; offset:int64; size:int64; data:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCopyNamedBufferSubData",library="Opengl32")]
def glCopyNamedBufferSubData ( readBuffer:uint; writeBuffer:uint; readOffset:int64; writeOffset:int64; size:int64 ) : void
	pass

[extern(opengl,late,name="glClearNamedBufferData",library="Opengl32")]
def glClearNamedBufferData ( buffer:uint; internalformat:uint; format:uint; _type:uint; data:void const? implicit ) : void
	pass

[extern(opengl,late,name="glClearNamedBufferSubData",library="Opengl32")]
def glClearNamedBufferSubData ( buffer:uint; internalformat:uint; offset:int64; size:int64; format:uint; _type:uint; data:void const? implicit ) : void
	pass

[extern(opengl,late,name="glMapNamedBuffer",library="Opengl32")]
def glMapNamedBuffer ( buffer:uint; access:uint ) : void?
	return [[void?]]

[extern(opengl,late,name="glMapNamedBufferRange",library="Opengl32")]
def glMapNamedBufferRange ( buffer:uint; offset:int64; length:int64; access:uint ) : void?
	return [[void?]]

[extern(opengl,late,name="glUnmapNamedBuffer",library="Opengl32")]
def glUnmapNamedBuffer ( buffer:uint ) : bool
	return [[bool]]

[extern(opengl,late,name="glFlushMappedNamedBufferRange",library="Opengl32")]
def glFlushMappedNamedBufferRange ( buffer:uint; offset:int64; length:int64 ) : void
	pass

[extern(opengl,late,name="glGetNamedBufferParameteriv",library="Opengl32")]
def glGetNamedBufferParameteriv ( buffer:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetNamedBufferParameteri64v",library="Opengl32")]
def glGetNamedBufferParameteri64v ( buffer:uint; pname:uint; params:int64? implicit ) : void
	pass

[extern(opengl,late,name="glGetNamedBufferPointerv",library="Opengl32")]
def glGetNamedBufferPointerv ( buffer:uint; pname:uint; params:void? implicit ) : void
	pass

[extern(opengl,late,name="glGetNamedBufferSubData",library="Opengl32")]
def glGetNamedBufferSubData ( buffer:uint; offset:int64; size:int64; data:void? implicit ) : void
	pass

[extern(opengl,late,name="glCreateFramebuffers",library="Opengl32")]
def glCreateFramebuffers ( n:int; framebuffers:uint? implicit ) : void
	pass

[extern(opengl,late,name="glNamedFramebufferRenderbuffer",library="Opengl32")]
def glNamedFramebufferRenderbuffer ( framebuffer:uint; attachment:uint; renderbuffertarget:uint; renderbuffer:uint ) : void
	pass

[extern(opengl,late,name="glNamedFramebufferParameteri",library="Opengl32")]
def glNamedFramebufferParameteri ( framebuffer:uint; pname:uint; param:int ) : void
	pass

[extern(opengl,late,name="glNamedFramebufferTexture",library="Opengl32")]
def glNamedFramebufferTexture ( framebuffer:uint; attachment:uint; texture:uint; level:int ) : void
	pass

[extern(opengl,late,name="glNamedFramebufferTextureLayer",library="Opengl32")]
def glNamedFramebufferTextureLayer ( framebuffer:uint; attachment:uint; texture:uint; level:int; layer:int ) : void
	pass

[extern(opengl,late,name="glNamedFramebufferDrawBuffer",library="Opengl32")]
def glNamedFramebufferDrawBuffer ( framebuffer:uint; buf:uint ) : void
	pass

[extern(opengl,late,name="glNamedFramebufferDrawBuffers",library="Opengl32")]
def glNamedFramebufferDrawBuffers ( framebuffer:uint; n:int; bufs:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glNamedFramebufferReadBuffer",library="Opengl32")]
def glNamedFramebufferReadBuffer ( framebuffer:uint; src:uint ) : void
	pass

[extern(opengl,late,name="glInvalidateNamedFramebufferData",library="Opengl32")]
def glInvalidateNamedFramebufferData ( framebuffer:uint; numAttachments:int; attachments:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glInvalidateNamedFramebufferSubData",library="Opengl32")]
def glInvalidateNamedFramebufferSubData ( framebuffer:uint; numAttachments:int; attachments:uint const? implicit; x:int; y:int; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glClearNamedFramebufferiv",library="Opengl32")]
def glClearNamedFramebufferiv ( framebuffer:uint; buffer:uint; drawbuffer:int; value:int const? implicit ) : void
	pass

[extern(opengl,late,name="glClearNamedFramebufferuiv",library="Opengl32")]
def glClearNamedFramebufferuiv ( framebuffer:uint; buffer:uint; drawbuffer:int; value:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glClearNamedFramebufferfv",library="Opengl32")]
def glClearNamedFramebufferfv ( framebuffer:uint; buffer:uint; drawbuffer:int; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glClearNamedFramebufferfi",library="Opengl32")]
def glClearNamedFramebufferfi ( framebuffer:uint; buffer:uint; drawbuffer:int; depth:float; stencil:int ) : void
	pass

[extern(opengl,late,name="glBlitNamedFramebuffer",library="Opengl32")]
def glBlitNamedFramebuffer ( readFramebuffer:uint; drawFramebuffer:uint; srcX0:int; srcY0:int; srcX1:int; srcY1:int; dstX0:int; dstY0:int; dstX1:int; dstY1:int; mask:uint; filter:uint ) : void
	pass

[extern(opengl,late,name="glCheckNamedFramebufferStatus",library="Opengl32")]
def glCheckNamedFramebufferStatus ( framebuffer:uint; target:uint ) : uint
	return [[uint]]

[extern(opengl,late,name="glGetNamedFramebufferParameteriv",library="Opengl32")]
def glGetNamedFramebufferParameteriv ( framebuffer:uint; pname:uint; param:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetNamedFramebufferAttachmentParameteriv",library="Opengl32")]
def glGetNamedFramebufferAttachmentParameteriv ( framebuffer:uint; attachment:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glCreateRenderbuffers",library="Opengl32")]
def glCreateRenderbuffers ( n:int; renderbuffers:uint? implicit ) : void
	pass

[extern(opengl,late,name="glNamedRenderbufferStorage",library="Opengl32")]
def glNamedRenderbufferStorage ( renderbuffer:uint; internalformat:uint; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glNamedRenderbufferStorageMultisample",library="Opengl32")]
def glNamedRenderbufferStorageMultisample ( renderbuffer:uint; samples:int; internalformat:uint; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glGetNamedRenderbufferParameteriv",library="Opengl32")]
def glGetNamedRenderbufferParameteriv ( renderbuffer:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glCreateTextures",library="Opengl32")]
def glCreateTextures ( target:uint; n:int; textures:uint? implicit ) : void
	pass

[extern(opengl,late,name="glTextureBuffer",library="Opengl32")]
def glTextureBuffer ( texture:uint; internalformat:uint; buffer:uint ) : void
	pass

[extern(opengl,late,name="glTextureBufferRange",library="Opengl32")]
def glTextureBufferRange ( texture:uint; internalformat:uint; buffer:uint; offset:int64; size:int64 ) : void
	pass

[extern(opengl,late,name="glTextureStorage1D",library="Opengl32")]
def glTextureStorage1D ( texture:uint; levels:int; internalformat:uint; width:int ) : void
	pass

[extern(opengl,late,name="glTextureStorage2D",library="Opengl32")]
def glTextureStorage2D ( texture:uint; levels:int; internalformat:uint; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glTextureStorage3D",library="Opengl32")]
def glTextureStorage3D ( texture:uint; levels:int; internalformat:uint; width:int; height:int; depth:int ) : void
	pass

[extern(opengl,late,name="glTextureStorage2DMultisample",library="Opengl32")]
def glTextureStorage2DMultisample ( texture:uint; samples:int; internalformat:uint; width:int; height:int; fixedsamplelocations:bool ) : void
	pass

[extern(opengl,late,name="glTextureStorage3DMultisample",library="Opengl32")]
def glTextureStorage3DMultisample ( texture:uint; samples:int; internalformat:uint; width:int; height:int; depth:int; fixedsamplelocations:bool ) : void
	pass

[extern(opengl,late,name="glTextureSubImage1D",library="Opengl32")]
def glTextureSubImage1D ( texture:uint; level:int; xoffset:int; width:int; format:uint; _type:uint; pixels:void const? implicit ) : void
	pass

[extern(opengl,late,name="glTextureSubImage2D",library="Opengl32")]
def glTextureSubImage2D ( texture:uint; level:int; xoffset:int; yoffset:int; width:int; height:int; format:uint; _type:uint; pixels:void const? implicit ) : void
	pass

[extern(opengl,late,name="glTextureSubImage3D",library="Opengl32")]
def glTextureSubImage3D ( texture:uint; level:int; xoffset:int; yoffset:int; zoffset:int; width:int; height:int; depth:int; format:uint; _type:uint; pixels:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCompressedTextureSubImage1D",library="Opengl32")]
def glCompressedTextureSubImage1D ( texture:uint; level:int; xoffset:int; width:int; format:uint; imageSize:int; data:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCompressedTextureSubImage2D",library="Opengl32")]
def glCompressedTextureSubImage2D ( texture:uint; level:int; xoffset:int; yoffset:int; width:int; height:int; format:uint; imageSize:int; data:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCompressedTextureSubImage3D",library="Opengl32")]
def glCompressedTextureSubImage3D ( texture:uint; level:int; xoffset:int; yoffset:int; zoffset:int; width:int; height:int; depth:int; format:uint; imageSize:int; data:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCopyTextureSubImage1D",library="Opengl32")]
def glCopyTextureSubImage1D ( texture:uint; level:int; xoffset:int; x:int; y:int; width:int ) : void
	pass

[extern(opengl,late,name="glCopyTextureSubImage2D",library="Opengl32")]
def glCopyTextureSubImage2D ( texture:uint; level:int; xoffset:int; yoffset:int; x:int; y:int; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glCopyTextureSubImage3D",library="Opengl32")]
def glCopyTextureSubImage3D ( texture:uint; level:int; xoffset:int; yoffset:int; zoffset:int; x:int; y:int; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glTextureParameterf",library="Opengl32")]
def glTextureParameterf ( texture:uint; pname:uint; param:float ) : void
	pass

[extern(opengl,late,name="glTextureParameterfv",library="Opengl32")]
def glTextureParameterfv ( texture:uint; pname:uint; param:float const? implicit ) : void
	pass

[extern(opengl,late,name="glTextureParameteri",library="Opengl32")]
def glTextureParameteri ( texture:uint; pname:uint; param:int ) : void
	pass

[extern(opengl,late,name="glTextureParameterIiv",library="Opengl32")]
def glTextureParameterIiv ( texture:uint; pname:uint; params:int const? implicit ) : void
	pass

[extern(opengl,late,name="glTextureParameterIuiv",library="Opengl32")]
def glTextureParameterIuiv ( texture:uint; pname:uint; params:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glTextureParameteriv",library="Opengl32")]
def glTextureParameteriv ( texture:uint; pname:uint; param:int const? implicit ) : void
	pass

[extern(opengl,late,name="glGenerateTextureMipmap",library="Opengl32")]
def glGenerateTextureMipmap ( texture:uint ) : void
	pass

[extern(opengl,late,name="glBindTextureUnit",library="Opengl32")]
def glBindTextureUnit ( unit:uint; texture:uint ) : void
	pass

[extern(opengl,late,name="glGetTextureImage",library="Opengl32")]
def glGetTextureImage ( texture:uint; level:int; format:uint; _type:uint; bufSize:int; pixels:void? implicit ) : void
	pass

[extern(opengl,late,name="glGetCompressedTextureImage",library="Opengl32")]
def glGetCompressedTextureImage ( texture:uint; level:int; bufSize:int; pixels:void? implicit ) : void
	pass

[extern(opengl,late,name="glGetTextureLevelParameterfv",library="Opengl32")]
def glGetTextureLevelParameterfv ( texture:uint; level:int; pname:uint; params:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetTextureLevelParameteriv",library="Opengl32")]
def glGetTextureLevelParameteriv ( texture:uint; level:int; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetTextureParameterfv",library="Opengl32")]
def glGetTextureParameterfv ( texture:uint; pname:uint; params:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetTextureParameterIiv",library="Opengl32")]
def glGetTextureParameterIiv ( texture:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetTextureParameterIuiv",library="Opengl32")]
def glGetTextureParameterIuiv ( texture:uint; pname:uint; params:uint? implicit ) : void
	pass

[extern(opengl,late,name="glGetTextureParameteriv",library="Opengl32")]
def glGetTextureParameteriv ( texture:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glCreateVertexArrays",library="Opengl32")]
def glCreateVertexArrays ( n:int; arrays:uint? implicit ) : void
	pass

[extern(opengl,late,name="glDisableVertexArrayAttrib",library="Opengl32")]
def glDisableVertexArrayAttrib ( vaobj:uint; index:uint ) : void
	pass

[extern(opengl,late,name="glEnableVertexArrayAttrib",library="Opengl32")]
def glEnableVertexArrayAttrib ( vaobj:uint; index:uint ) : void
	pass

[extern(opengl,late,name="glVertexArrayElementBuffer",library="Opengl32")]
def glVertexArrayElementBuffer ( vaobj:uint; buffer:uint ) : void
	pass

[extern(opengl,late,name="glVertexArrayVertexBuffer",library="Opengl32")]
def glVertexArrayVertexBuffer ( vaobj:uint; bindingindex:uint; buffer:uint; offset:int64; stride:int ) : void
	pass

[extern(opengl,late,name="glVertexArrayVertexBuffers",library="Opengl32")]
def glVertexArrayVertexBuffers ( vaobj:uint; first:uint; count:int; buffers:uint const? implicit; offsets:int64 const? implicit; strides:int const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexArrayAttribBinding",library="Opengl32")]
def glVertexArrayAttribBinding ( vaobj:uint; attribindex:uint; bindingindex:uint ) : void
	pass

[extern(opengl,late,name="glVertexArrayAttribFormat",library="Opengl32")]
def glVertexArrayAttribFormat ( vaobj:uint; attribindex:uint; size:int; _type:uint; normalized:bool; relativeoffset:uint ) : void
	pass

[extern(opengl,late,name="glVertexArrayAttribIFormat",library="Opengl32")]
def glVertexArrayAttribIFormat ( vaobj:uint; attribindex:uint; size:int; _type:uint; relativeoffset:uint ) : void
	pass

[extern(opengl,late,name="glVertexArrayAttribLFormat",library="Opengl32")]
def glVertexArrayAttribLFormat ( vaobj:uint; attribindex:uint; size:int; _type:uint; relativeoffset:uint ) : void
	pass

[extern(opengl,late,name="glVertexArrayBindingDivisor",library="Opengl32")]
def glVertexArrayBindingDivisor ( vaobj:uint; bindingindex:uint; divisor:uint ) : void
	pass

[extern(opengl,late,name="glGetVertexArrayiv",library="Opengl32")]
def glGetVertexArrayiv ( vaobj:uint; pname:uint; param:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetVertexArrayIndexediv",library="Opengl32")]
def glGetVertexArrayIndexediv ( vaobj:uint; index:uint; pname:uint; param:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetVertexArrayIndexed64iv",library="Opengl32")]
def glGetVertexArrayIndexed64iv ( vaobj:uint; index:uint; pname:uint; param:int64? implicit ) : void
	pass

[extern(opengl,late,name="glCreateSamplers",library="Opengl32")]
def glCreateSamplers ( n:int; samplers:uint? implicit ) : void
	pass

[extern(opengl,late,name="glCreateProgramPipelines",library="Opengl32")]
def glCreateProgramPipelines ( n:int; pipelines:uint? implicit ) : void
	pass

[extern(opengl,late,name="glCreateQueries",library="Opengl32")]
def glCreateQueries ( target:uint; n:int; ids:uint? implicit ) : void
	pass

[extern(opengl,late,name="glGetQueryBufferObjecti64v",library="Opengl32")]
def glGetQueryBufferObjecti64v ( id:uint; buffer:uint; pname:uint; offset:int64 ) : void
	pass

[extern(opengl,late,name="glGetQueryBufferObjectiv",library="Opengl32")]
def glGetQueryBufferObjectiv ( id:uint; buffer:uint; pname:uint; offset:int64 ) : void
	pass

[extern(opengl,late,name="glGetQueryBufferObjectui64v",library="Opengl32")]
def glGetQueryBufferObjectui64v ( id:uint; buffer:uint; pname:uint; offset:int64 ) : void
	pass

[extern(opengl,late,name="glGetQueryBufferObjectuiv",library="Opengl32")]
def glGetQueryBufferObjectuiv ( id:uint; buffer:uint; pname:uint; offset:int64 ) : void
	pass

[extern(opengl,late,name="glMemoryBarrierByRegion",library="Opengl32")]
def glMemoryBarrierByRegion ( barriers:uint ) : void
	pass

[extern(opengl,late,name="glGetTextureSubImage",library="Opengl32")]
def glGetTextureSubImage ( texture:uint; level:int; xoffset:int; yoffset:int; zoffset:int; width:int; height:int; depth:int; format:uint; _type:uint; bufSize:int; pixels:void? implicit ) : void
	pass

[extern(opengl,late,name="glGetCompressedTextureSubImage",library="Opengl32")]
def glGetCompressedTextureSubImage ( texture:uint; level:int; xoffset:int; yoffset:int; zoffset:int; width:int; height:int; depth:int; bufSize:int; pixels:void? implicit ) : void
	pass

[extern(opengl,late,name="glGetGraphicsResetStatus",library="Opengl32")]
def glGetGraphicsResetStatus (  ) : uint
	return [[uint]]

[extern(opengl,late,name="glGetnCompressedTexImage",library="Opengl32")]
def glGetnCompressedTexImage ( target:uint; lod:int; bufSize:int; pixels:void? implicit ) : void
	pass

[extern(opengl,late,name="glGetnTexImage",library="Opengl32")]
def glGetnTexImage ( target:uint; level:int; format:uint; _type:uint; bufSize:int; pixels:void? implicit ) : void
	pass

[extern(opengl,late,name="glGetnUniformdv",library="Opengl32")]
def glGetnUniformdv ( program:uint; location:int; bufSize:int; params:double? implicit ) : void
	pass

[extern(opengl,late,name="glGetnUniformfv",library="Opengl32")]
def glGetnUniformfv ( program:uint; location:int; bufSize:int; params:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetnUniformiv",library="Opengl32")]
def glGetnUniformiv ( program:uint; location:int; bufSize:int; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetnUniformuiv",library="Opengl32")]
def glGetnUniformuiv ( program:uint; location:int; bufSize:int; params:uint? implicit ) : void
	pass

[extern(opengl,late,name="glReadnPixels",library="Opengl32")]
def glReadnPixels ( x:int; y:int; width:int; height:int; format:uint; _type:uint; bufSize:int; data:void? implicit ) : void
	pass

[extern(opengl,late,name="glTextureBarrier",library="Opengl32")]
def glTextureBarrier (  ) : void
	pass

[extern(opengl,late,name="glSpecializeShader",library="Opengl32")]
def glSpecializeShader ( shader:uint; pEntryPoint:string; numSpecializationConstants:uint; pConstantIndex:uint const? implicit; pConstantValue:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glMultiDrawArraysIndirectCount",library="Opengl32")]
def glMultiDrawArraysIndirectCount ( mode:uint; indirect:void const? implicit; drawcount:int64; maxdrawcount:int; stride:int ) : void
	pass

[extern(opengl,late,name="glMultiDrawElementsIndirectCount",library="Opengl32")]
def glMultiDrawElementsIndirectCount ( mode:uint; _type:uint; indirect:void const? implicit; drawcount:int64; maxdrawcount:int; stride:int ) : void
	pass

[extern(opengl,late,name="glPolygonOffsetClamp",library="Opengl32")]
def glPolygonOffsetClamp ( factor:float; units:float; clamp:float ) : void
	pass

[extern(opengl,late,name="glPrimitiveBoundingBoxARB",library="Opengl32")]
def glPrimitiveBoundingBoxARB ( minX:float; minY:float; minZ:float; minW:float; maxX:float; maxY:float; maxZ:float; maxW:float ) : void
	pass

[extern(opengl,late,name="glGetTextureHandleARB",library="Opengl32")]
def glGetTextureHandleARB ( texture:uint ) : uint64
	return [[uint64]]

[extern(opengl,late,name="glGetTextureSamplerHandleARB",library="Opengl32")]
def glGetTextureSamplerHandleARB ( texture:uint; sampler:uint ) : uint64
	return [[uint64]]

[extern(opengl,late,name="glMakeTextureHandleResidentARB",library="Opengl32")]
def glMakeTextureHandleResidentARB ( handle:uint64 ) : void
	pass

[extern(opengl,late,name="glMakeTextureHandleNonResidentARB",library="Opengl32")]
def glMakeTextureHandleNonResidentARB ( handle:uint64 ) : void
	pass

[extern(opengl,late,name="glGetImageHandleARB",library="Opengl32")]
def glGetImageHandleARB ( texture:uint; level:int; layered:bool; layer:int; format:uint ) : uint64
	return [[uint64]]

[extern(opengl,late,name="glMakeImageHandleResidentARB",library="Opengl32")]
def glMakeImageHandleResidentARB ( handle:uint64; access:uint ) : void
	pass

[extern(opengl,late,name="glMakeImageHandleNonResidentARB",library="Opengl32")]
def glMakeImageHandleNonResidentARB ( handle:uint64 ) : void
	pass

[extern(opengl,late,name="glUniformHandleui64ARB",library="Opengl32")]
def glUniformHandleui64ARB ( location:int; value:uint64 ) : void
	pass

[extern(opengl,late,name="glUniformHandleui64vARB",library="Opengl32")]
def glUniformHandleui64vARB ( location:int; count:int; value:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformHandleui64ARB",library="Opengl32")]
def glProgramUniformHandleui64ARB ( program:uint; location:int; value:uint64 ) : void
	pass

[extern(opengl,late,name="glProgramUniformHandleui64vARB",library="Opengl32")]
def glProgramUniformHandleui64vARB ( program:uint; location:int; count:int; values:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glIsTextureHandleResidentARB",library="Opengl32")]
def glIsTextureHandleResidentARB ( handle:uint64 ) : bool
	return [[bool]]

[extern(opengl,late,name="glIsImageHandleResidentARB",library="Opengl32")]
def glIsImageHandleResidentARB ( handle:uint64 ) : bool
	return [[bool]]

[extern(opengl,late,name="glVertexAttribL1ui64ARB",library="Opengl32")]
def glVertexAttribL1ui64ARB ( index:uint; x:uint64 ) : void
	pass

[extern(opengl,late,name="glVertexAttribL1ui64vARB",library="Opengl32")]
def glVertexAttribL1ui64vARB ( index:uint; v:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glGetVertexAttribLui64vARB",library="Opengl32")]
def glGetVertexAttribLui64vARB ( index:uint; pname:uint; params:uint64? implicit ) : void
	pass

[extern(opengl,late,name="glCreateSyncFromCLeventARB",library="Opengl32")]
def glCreateSyncFromCLeventARB ( flags:uint ) : void?
	return [[void?]]

[extern(opengl,late,name="glDispatchComputeGroupSizeARB",library="Opengl32")]
def glDispatchComputeGroupSizeARB ( num_groups_x:uint; num_groups_y:uint; num_groups_z:uint; group_size_x:uint; group_size_y:uint; group_size_z:uint ) : void
	pass

[extern(opengl,late,name="glDebugMessageControlARB",library="Opengl32")]
def glDebugMessageControlARB ( source:uint; _type:uint; severity:uint; count:int; ids:uint const? implicit; enabled:bool ) : void
	pass

[extern(opengl,late,name="glDebugMessageInsertARB",library="Opengl32")]
def glDebugMessageInsertARB ( source:uint; _type:uint; id:uint; severity:uint; length:int; buf:string ) : void
	pass

[extern(opengl,late,name="glDebugMessageCallbackARB",library="Opengl32")]
def glDebugMessageCallbackARB ( userParam:void const? implicit ) : void
	pass

[extern(opengl,late,name="glGetDebugMessageLogARB",library="Opengl32")]
def glGetDebugMessageLogARB ( count:uint; bufSize:int; sources:uint? implicit; types:uint? implicit; ids:uint? implicit; severities:uint? implicit; lengths:int? implicit; messageLog:string const ) : uint
	return [[uint]]

[extern(opengl,late,name="glBlendEquationiARB",library="Opengl32")]
def glBlendEquationiARB ( buf:uint; mode:uint ) : void
	pass

[extern(opengl,late,name="glBlendEquationSeparateiARB",library="Opengl32")]
def glBlendEquationSeparateiARB ( buf:uint; modeRGB:uint; modeAlpha:uint ) : void
	pass

[extern(opengl,late,name="glBlendFunciARB",library="Opengl32")]
def glBlendFunciARB ( buf:uint; src:uint; dst:uint ) : void
	pass

[extern(opengl,late,name="glBlendFuncSeparateiARB",library="Opengl32")]
def glBlendFuncSeparateiARB ( buf:uint; srcRGB:uint; dstRGB:uint; srcAlpha:uint; dstAlpha:uint ) : void
	pass

[extern(opengl,late,name="glDrawArraysInstancedARB",library="Opengl32")]
def glDrawArraysInstancedARB ( mode:uint; first:int; count:int; primcount:int ) : void
	pass

[extern(opengl,late,name="glDrawElementsInstancedARB",library="Opengl32")]
def glDrawElementsInstancedARB ( mode:uint; count:int; _type:uint; indices:void const? implicit; primcount:int ) : void
	pass

[extern(opengl,late,name="glProgramParameteriARB",library="Opengl32")]
def glProgramParameteriARB ( program:uint; pname:uint; value:int ) : void
	pass

[extern(opengl,late,name="glFramebufferTextureARB",library="Opengl32")]
def glFramebufferTextureARB ( target:uint; attachment:uint; texture:uint; level:int ) : void
	pass

[extern(opengl,late,name="glFramebufferTextureLayerARB",library="Opengl32")]
def glFramebufferTextureLayerARB ( target:uint; attachment:uint; texture:uint; level:int; layer:int ) : void
	pass

[extern(opengl,late,name="glFramebufferTextureFaceARB",library="Opengl32")]
def glFramebufferTextureFaceARB ( target:uint; attachment:uint; texture:uint; level:int; face:uint ) : void
	pass

[extern(opengl,late,name="glSpecializeShaderARB",library="Opengl32")]
def glSpecializeShaderARB ( shader:uint; pEntryPoint:string; numSpecializationConstants:uint; pConstantIndex:uint const? implicit; pConstantValue:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform1i64ARB",library="Opengl32")]
def glUniform1i64ARB ( location:int; x:int64 ) : void
	pass

[extern(opengl,late,name="glUniform2i64ARB",library="Opengl32")]
def glUniform2i64ARB ( location:int; x:int64; y:int64 ) : void
	pass

[extern(opengl,late,name="glUniform3i64ARB",library="Opengl32")]
def glUniform3i64ARB ( location:int; x:int64; y:int64; z:int64 ) : void
	pass

[extern(opengl,late,name="glUniform4i64ARB",library="Opengl32")]
def glUniform4i64ARB ( location:int; x:int64; y:int64; z:int64; w:int64 ) : void
	pass

[extern(opengl,late,name="glUniform1i64vARB",library="Opengl32")]
def glUniform1i64vARB ( location:int; count:int; value:int64 const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform2i64vARB",library="Opengl32")]
def glUniform2i64vARB ( location:int; count:int; value:int64 const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform3i64vARB",library="Opengl32")]
def glUniform3i64vARB ( location:int; count:int; value:int64 const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform4i64vARB",library="Opengl32")]
def glUniform4i64vARB ( location:int; count:int; value:int64 const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform1ui64ARB",library="Opengl32")]
def glUniform1ui64ARB ( location:int; x:uint64 ) : void
	pass

[extern(opengl,late,name="glUniform2ui64ARB",library="Opengl32")]
def glUniform2ui64ARB ( location:int; x:uint64; y:uint64 ) : void
	pass

[extern(opengl,late,name="glUniform3ui64ARB",library="Opengl32")]
def glUniform3ui64ARB ( location:int; x:uint64; y:uint64; z:uint64 ) : void
	pass

[extern(opengl,late,name="glUniform4ui64ARB",library="Opengl32")]
def glUniform4ui64ARB ( location:int; x:uint64; y:uint64; z:uint64; w:uint64 ) : void
	pass

[extern(opengl,late,name="glUniform1ui64vARB",library="Opengl32")]
def glUniform1ui64vARB ( location:int; count:int; value:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform2ui64vARB",library="Opengl32")]
def glUniform2ui64vARB ( location:int; count:int; value:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform3ui64vARB",library="Opengl32")]
def glUniform3ui64vARB ( location:int; count:int; value:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform4ui64vARB",library="Opengl32")]
def glUniform4ui64vARB ( location:int; count:int; value:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glGetUniformi64vARB",library="Opengl32")]
def glGetUniformi64vARB ( program:uint; location:int; params:int64? implicit ) : void
	pass

[extern(opengl,late,name="glGetUniformui64vARB",library="Opengl32")]
def glGetUniformui64vARB ( program:uint; location:int; params:uint64? implicit ) : void
	pass

[extern(opengl,late,name="glGetnUniformi64vARB",library="Opengl32")]
def glGetnUniformi64vARB ( program:uint; location:int; bufSize:int; params:int64? implicit ) : void
	pass

[extern(opengl,late,name="glGetnUniformui64vARB",library="Opengl32")]
def glGetnUniformui64vARB ( program:uint; location:int; bufSize:int; params:uint64? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform1i64ARB",library="Opengl32")]
def glProgramUniform1i64ARB ( program:uint; location:int; x:int64 ) : void
	pass

[extern(opengl,late,name="glProgramUniform2i64ARB",library="Opengl32")]
def glProgramUniform2i64ARB ( program:uint; location:int; x:int64; y:int64 ) : void
	pass

[extern(opengl,late,name="glProgramUniform3i64ARB",library="Opengl32")]
def glProgramUniform3i64ARB ( program:uint; location:int; x:int64; y:int64; z:int64 ) : void
	pass

[extern(opengl,late,name="glProgramUniform4i64ARB",library="Opengl32")]
def glProgramUniform4i64ARB ( program:uint; location:int; x:int64; y:int64; z:int64; w:int64 ) : void
	pass

[extern(opengl,late,name="glProgramUniform1i64vARB",library="Opengl32")]
def glProgramUniform1i64vARB ( program:uint; location:int; count:int; value:int64 const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform2i64vARB",library="Opengl32")]
def glProgramUniform2i64vARB ( program:uint; location:int; count:int; value:int64 const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform3i64vARB",library="Opengl32")]
def glProgramUniform3i64vARB ( program:uint; location:int; count:int; value:int64 const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform4i64vARB",library="Opengl32")]
def glProgramUniform4i64vARB ( program:uint; location:int; count:int; value:int64 const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform1ui64ARB",library="Opengl32")]
def glProgramUniform1ui64ARB ( program:uint; location:int; x:uint64 ) : void
	pass

[extern(opengl,late,name="glProgramUniform2ui64ARB",library="Opengl32")]
def glProgramUniform2ui64ARB ( program:uint; location:int; x:uint64; y:uint64 ) : void
	pass

[extern(opengl,late,name="glProgramUniform3ui64ARB",library="Opengl32")]
def glProgramUniform3ui64ARB ( program:uint; location:int; x:uint64; y:uint64; z:uint64 ) : void
	pass

[extern(opengl,late,name="glProgramUniform4ui64ARB",library="Opengl32")]
def glProgramUniform4ui64ARB ( program:uint; location:int; x:uint64; y:uint64; z:uint64; w:uint64 ) : void
	pass

[extern(opengl,late,name="glProgramUniform1ui64vARB",library="Opengl32")]
def glProgramUniform1ui64vARB ( program:uint; location:int; count:int; value:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform2ui64vARB",library="Opengl32")]
def glProgramUniform2ui64vARB ( program:uint; location:int; count:int; value:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform3ui64vARB",library="Opengl32")]
def glProgramUniform3ui64vARB ( program:uint; location:int; count:int; value:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform4ui64vARB",library="Opengl32")]
def glProgramUniform4ui64vARB ( program:uint; location:int; count:int; value:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glMultiDrawArraysIndirectCountARB",library="Opengl32")]
def glMultiDrawArraysIndirectCountARB ( mode:uint; indirect:void const? implicit; drawcount:int64; maxdrawcount:int; stride:int ) : void
	pass

[extern(opengl,late,name="glMultiDrawElementsIndirectCountARB",library="Opengl32")]
def glMultiDrawElementsIndirectCountARB ( mode:uint; _type:uint; indirect:void const? implicit; drawcount:int64; maxdrawcount:int; stride:int ) : void
	pass

[extern(opengl,late,name="glVertexAttribDivisorARB",library="Opengl32")]
def glVertexAttribDivisorARB ( index:uint; divisor:uint ) : void
	pass

[extern(opengl,late,name="glMaxShaderCompilerThreadsARB",library="Opengl32")]
def glMaxShaderCompilerThreadsARB ( count:uint ) : void
	pass

[extern(opengl,late,name="glGetGraphicsResetStatusARB",library="Opengl32")]
def glGetGraphicsResetStatusARB (  ) : uint
	return [[uint]]

[extern(opengl,late,name="glGetnTexImageARB",library="Opengl32")]
def glGetnTexImageARB ( target:uint; level:int; format:uint; _type:uint; bufSize:int; img:void? implicit ) : void
	pass

[extern(opengl,late,name="glReadnPixelsARB",library="Opengl32")]
def glReadnPixelsARB ( x:int; y:int; width:int; height:int; format:uint; _type:uint; bufSize:int; data:void? implicit ) : void
	pass

[extern(opengl,late,name="glGetnCompressedTexImageARB",library="Opengl32")]
def glGetnCompressedTexImageARB ( target:uint; lod:int; bufSize:int; img:void? implicit ) : void
	pass

[extern(opengl,late,name="glGetnUniformfvARB",library="Opengl32")]
def glGetnUniformfvARB ( program:uint; location:int; bufSize:int; params:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetnUniformivARB",library="Opengl32")]
def glGetnUniformivARB ( program:uint; location:int; bufSize:int; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetnUniformuivARB",library="Opengl32")]
def glGetnUniformuivARB ( program:uint; location:int; bufSize:int; params:uint? implicit ) : void
	pass

[extern(opengl,late,name="glGetnUniformdvARB",library="Opengl32")]
def glGetnUniformdvARB ( program:uint; location:int; bufSize:int; params:double? implicit ) : void
	pass

[extern(opengl,late,name="glFramebufferSampleLocationsfvARB",library="Opengl32")]
def glFramebufferSampleLocationsfvARB ( target:uint; start:uint; count:int; v:float const? implicit ) : void
	pass

[extern(opengl,late,name="glNamedFramebufferSampleLocationsfvARB",library="Opengl32")]
def glNamedFramebufferSampleLocationsfvARB ( framebuffer:uint; start:uint; count:int; v:float const? implicit ) : void
	pass

[extern(opengl,late,name="glEvaluateDepthValuesARB",library="Opengl32")]
def glEvaluateDepthValuesARB (  ) : void
	pass

[extern(opengl,late,name="glMinSampleShadingARB",library="Opengl32")]
def glMinSampleShadingARB ( value:float ) : void
	pass

[extern(opengl,late,name="glNamedStringARB",library="Opengl32")]
def glNamedStringARB ( _type:uint; namelen:int; name:string; stringlen:int; _string:string ) : void
	pass

[extern(opengl,late,name="glDeleteNamedStringARB",library="Opengl32")]
def glDeleteNamedStringARB ( namelen:int; name:string ) : void
	pass

[extern(opengl,late,name="glCompileShaderIncludeARB",library="Opengl32")]
def glCompileShaderIncludeARB ( shader:uint; count:int; path:string const? implicit; length:int const? implicit ) : void
	pass

[extern(opengl,late,name="glIsNamedStringARB",library="Opengl32")]
def glIsNamedStringARB ( namelen:int; name:string ) : bool
	return [[bool]]

[extern(opengl,late,name="glGetNamedStringARB",library="Opengl32")]
def glGetNamedStringARB ( namelen:int; name:string; bufSize:int; stringlen:int? implicit; _string:string const ) : void
	pass

[extern(opengl,late,name="glGetNamedStringivARB",library="Opengl32")]
def glGetNamedStringivARB ( namelen:int; name:string; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glBufferPageCommitmentARB",library="Opengl32")]
def glBufferPageCommitmentARB ( target:uint; offset:int64; size:int64; commit:bool ) : void
	pass

[extern(opengl,late,name="glNamedBufferPageCommitmentEXT",library="Opengl32")]
def glNamedBufferPageCommitmentEXT ( buffer:uint; offset:int64; size:int64; commit:bool ) : void
	pass

[extern(opengl,late,name="glNamedBufferPageCommitmentARB",library="Opengl32")]
def glNamedBufferPageCommitmentARB ( buffer:uint; offset:int64; size:int64; commit:bool ) : void
	pass

[extern(opengl,late,name="glTexPageCommitmentARB",library="Opengl32")]
def glTexPageCommitmentARB ( target:uint; level:int; xoffset:int; yoffset:int; zoffset:int; width:int; height:int; depth:int; commit:bool ) : void
	pass

[extern(opengl,late,name="glTexBufferARB",library="Opengl32")]
def glTexBufferARB ( target:uint; internalformat:uint; buffer:uint ) : void
	pass

[extern(opengl,late,name="glDepthRangeArraydvNV",library="Opengl32")]
def glDepthRangeArraydvNV ( first:uint; count:int; v:double const? implicit ) : void
	pass

[extern(opengl,late,name="glDepthRangeIndexeddNV",library="Opengl32")]
def glDepthRangeIndexeddNV ( index:uint; n:double; f:double ) : void
	pass

[extern(opengl,late,name="glBlendBarrierKHR",library="Opengl32")]
def glBlendBarrierKHR (  ) : void
	pass

[extern(opengl,late,name="glMaxShaderCompilerThreadsKHR",library="Opengl32")]
def glMaxShaderCompilerThreadsKHR ( count:uint ) : void
	pass

[extern(opengl,late,name="glRenderbufferStorageMultisampleAdvancedAMD",library="Opengl32")]
def glRenderbufferStorageMultisampleAdvancedAMD ( target:uint; samples:int; storageSamples:int; internalformat:uint; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glNamedRenderbufferStorageMultisampleAdvancedAMD",library="Opengl32")]
def glNamedRenderbufferStorageMultisampleAdvancedAMD ( renderbuffer:uint; samples:int; storageSamples:int; internalformat:uint; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glGetPerfMonitorGroupsAMD",library="Opengl32")]
def glGetPerfMonitorGroupsAMD ( numGroups:int? implicit; groupsSize:int; groups:uint? implicit ) : void
	pass

[extern(opengl,late,name="glGetPerfMonitorCountersAMD",library="Opengl32")]
def glGetPerfMonitorCountersAMD ( group:uint; numCounters:int? implicit; maxActiveCounters:int? implicit; counterSize:int; counters:uint? implicit ) : void
	pass

[extern(opengl,late,name="glGetPerfMonitorGroupStringAMD",library="Opengl32")]
def glGetPerfMonitorGroupStringAMD ( group:uint; bufSize:int; length:int? implicit; groupString:string const ) : void
	pass

[extern(opengl,late,name="glGetPerfMonitorCounterStringAMD",library="Opengl32")]
def glGetPerfMonitorCounterStringAMD ( group:uint; counter:uint; bufSize:int; length:int? implicit; counterString:string const ) : void
	pass

[extern(opengl,late,name="glGetPerfMonitorCounterInfoAMD",library="Opengl32")]
def glGetPerfMonitorCounterInfoAMD ( group:uint; counter:uint; pname:uint; data:void? implicit ) : void
	pass

[extern(opengl,late,name="glGenPerfMonitorsAMD",library="Opengl32")]
def glGenPerfMonitorsAMD ( n:int; monitors:uint? implicit ) : void
	pass

[extern(opengl,late,name="glDeletePerfMonitorsAMD",library="Opengl32")]
def glDeletePerfMonitorsAMD ( n:int; monitors:uint? implicit ) : void
	pass

[extern(opengl,late,name="glSelectPerfMonitorCountersAMD",library="Opengl32")]
def glSelectPerfMonitorCountersAMD ( monitor:uint; enable:bool; group:uint; numCounters:int; counterList:uint? implicit ) : void
	pass

[extern(opengl,late,name="glBeginPerfMonitorAMD",library="Opengl32")]
def glBeginPerfMonitorAMD ( monitor:uint ) : void
	pass

[extern(opengl,late,name="glEndPerfMonitorAMD",library="Opengl32")]
def glEndPerfMonitorAMD ( monitor:uint ) : void
	pass

[extern(opengl,late,name="glGetPerfMonitorCounterDataAMD",library="Opengl32")]
def glGetPerfMonitorCounterDataAMD ( monitor:uint; pname:uint; dataSize:int; data:uint? implicit; bytesWritten:int? implicit ) : void
	pass

[extern(opengl,late,name="glEGLImageTargetTexStorageEXT",library="Opengl32")]
def glEGLImageTargetTexStorageEXT ( target:uint; attrib_list:int const? implicit ) : void
	pass

[extern(opengl,late,name="glEGLImageTargetTextureStorageEXT",library="Opengl32")]
def glEGLImageTargetTextureStorageEXT ( texture:uint; attrib_list:int const? implicit ) : void
	pass

[extern(opengl,late,name="glLabelObjectEXT",library="Opengl32")]
def glLabelObjectEXT ( _type:uint; object:uint; length:int; _label:string ) : void
	pass

[extern(opengl,late,name="glGetObjectLabelEXT",library="Opengl32")]
def glGetObjectLabelEXT ( _type:uint; object:uint; bufSize:int; length:int? implicit; _label:string const ) : void
	pass

[extern(opengl,late,name="glInsertEventMarkerEXT",library="Opengl32")]
def glInsertEventMarkerEXT ( length:int; marker:string ) : void
	pass

[extern(opengl,late,name="glPushGroupMarkerEXT",library="Opengl32")]
def glPushGroupMarkerEXT ( length:int; marker:string ) : void
	pass

[extern(opengl,late,name="glPopGroupMarkerEXT",library="Opengl32")]
def glPopGroupMarkerEXT (  ) : void
	pass

[extern(opengl,late,name="glMatrixLoadfEXT",library="Opengl32")]
def glMatrixLoadfEXT ( mode:uint; m:float const? implicit ) : void
	pass

[extern(opengl,late,name="glMatrixLoaddEXT",library="Opengl32")]
def glMatrixLoaddEXT ( mode:uint; m:double const? implicit ) : void
	pass

[extern(opengl,late,name="glMatrixMultfEXT",library="Opengl32")]
def glMatrixMultfEXT ( mode:uint; m:float const? implicit ) : void
	pass

[extern(opengl,late,name="glMatrixMultdEXT",library="Opengl32")]
def glMatrixMultdEXT ( mode:uint; m:double const? implicit ) : void
	pass

[extern(opengl,late,name="glMatrixLoadIdentityEXT",library="Opengl32")]
def glMatrixLoadIdentityEXT ( mode:uint ) : void
	pass

[extern(opengl,late,name="glMatrixRotatefEXT",library="Opengl32")]
def glMatrixRotatefEXT ( mode:uint; angle:float; x:float; y:float; z:float ) : void
	pass

[extern(opengl,late,name="glMatrixRotatedEXT",library="Opengl32")]
def glMatrixRotatedEXT ( mode:uint; angle:double; x:double; y:double; z:double ) : void
	pass

[extern(opengl,late,name="glMatrixScalefEXT",library="Opengl32")]
def glMatrixScalefEXT ( mode:uint; x:float; y:float; z:float ) : void
	pass

[extern(opengl,late,name="glMatrixScaledEXT",library="Opengl32")]
def glMatrixScaledEXT ( mode:uint; x:double; y:double; z:double ) : void
	pass

[extern(opengl,late,name="glMatrixTranslatefEXT",library="Opengl32")]
def glMatrixTranslatefEXT ( mode:uint; x:float; y:float; z:float ) : void
	pass

[extern(opengl,late,name="glMatrixTranslatedEXT",library="Opengl32")]
def glMatrixTranslatedEXT ( mode:uint; x:double; y:double; z:double ) : void
	pass

[extern(opengl,late,name="glMatrixFrustumEXT",library="Opengl32")]
def glMatrixFrustumEXT ( mode:uint; left:double; right:double; bottom:double; top:double; zNear:double; zFar:double ) : void
	pass

[extern(opengl,late,name="glMatrixOrthoEXT",library="Opengl32")]
def glMatrixOrthoEXT ( mode:uint; left:double; right:double; bottom:double; top:double; zNear:double; zFar:double ) : void
	pass

[extern(opengl,late,name="glMatrixPopEXT",library="Opengl32")]
def glMatrixPopEXT ( mode:uint ) : void
	pass

[extern(opengl,late,name="glMatrixPushEXT",library="Opengl32")]
def glMatrixPushEXT ( mode:uint ) : void
	pass

[extern(opengl,late,name="glClientAttribDefaultEXT",library="Opengl32")]
def glClientAttribDefaultEXT ( mask:uint ) : void
	pass

[extern(opengl,late,name="glPushClientAttribDefaultEXT",library="Opengl32")]
def glPushClientAttribDefaultEXT ( mask:uint ) : void
	pass

[extern(opengl,late,name="glTextureParameterfEXT",library="Opengl32")]
def glTextureParameterfEXT ( texture:uint; target:uint; pname:uint; param:float ) : void
	pass

[extern(opengl,late,name="glTextureParameterfvEXT",library="Opengl32")]
def glTextureParameterfvEXT ( texture:uint; target:uint; pname:uint; params:float const? implicit ) : void
	pass

[extern(opengl,late,name="glTextureParameteriEXT",library="Opengl32")]
def glTextureParameteriEXT ( texture:uint; target:uint; pname:uint; param:int ) : void
	pass

[extern(opengl,late,name="glTextureParameterivEXT",library="Opengl32")]
def glTextureParameterivEXT ( texture:uint; target:uint; pname:uint; params:int const? implicit ) : void
	pass

[extern(opengl,late,name="glTextureImage1DEXT",library="Opengl32")]
def glTextureImage1DEXT ( texture:uint; target:uint; level:int; internalformat:int; width:int; border:int; format:uint; _type:uint; pixels:void const? implicit ) : void
	pass

[extern(opengl,late,name="glTextureImage2DEXT",library="Opengl32")]
def glTextureImage2DEXT ( texture:uint; target:uint; level:int; internalformat:int; width:int; height:int; border:int; format:uint; _type:uint; pixels:void const? implicit ) : void
	pass

[extern(opengl,late,name="glTextureSubImage1DEXT",library="Opengl32")]
def glTextureSubImage1DEXT ( texture:uint; target:uint; level:int; xoffset:int; width:int; format:uint; _type:uint; pixels:void const? implicit ) : void
	pass

[extern(opengl,late,name="glTextureSubImage2DEXT",library="Opengl32")]
def glTextureSubImage2DEXT ( texture:uint; target:uint; level:int; xoffset:int; yoffset:int; width:int; height:int; format:uint; _type:uint; pixels:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCopyTextureImage1DEXT",library="Opengl32")]
def glCopyTextureImage1DEXT ( texture:uint; target:uint; level:int; internalformat:uint; x:int; y:int; width:int; border:int ) : void
	pass

[extern(opengl,late,name="glCopyTextureImage2DEXT",library="Opengl32")]
def glCopyTextureImage2DEXT ( texture:uint; target:uint; level:int; internalformat:uint; x:int; y:int; width:int; height:int; border:int ) : void
	pass

[extern(opengl,late,name="glCopyTextureSubImage1DEXT",library="Opengl32")]
def glCopyTextureSubImage1DEXT ( texture:uint; target:uint; level:int; xoffset:int; x:int; y:int; width:int ) : void
	pass

[extern(opengl,late,name="glCopyTextureSubImage2DEXT",library="Opengl32")]
def glCopyTextureSubImage2DEXT ( texture:uint; target:uint; level:int; xoffset:int; yoffset:int; x:int; y:int; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glGetTextureImageEXT",library="Opengl32")]
def glGetTextureImageEXT ( texture:uint; target:uint; level:int; format:uint; _type:uint; pixels:void? implicit ) : void
	pass

[extern(opengl,late,name="glGetTextureParameterfvEXT",library="Opengl32")]
def glGetTextureParameterfvEXT ( texture:uint; target:uint; pname:uint; params:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetTextureParameterivEXT",library="Opengl32")]
def glGetTextureParameterivEXT ( texture:uint; target:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetTextureLevelParameterfvEXT",library="Opengl32")]
def glGetTextureLevelParameterfvEXT ( texture:uint; target:uint; level:int; pname:uint; params:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetTextureLevelParameterivEXT",library="Opengl32")]
def glGetTextureLevelParameterivEXT ( texture:uint; target:uint; level:int; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glTextureImage3DEXT",library="Opengl32")]
def glTextureImage3DEXT ( texture:uint; target:uint; level:int; internalformat:int; width:int; height:int; depth:int; border:int; format:uint; _type:uint; pixels:void const? implicit ) : void
	pass

[extern(opengl,late,name="glTextureSubImage3DEXT",library="Opengl32")]
def glTextureSubImage3DEXT ( texture:uint; target:uint; level:int; xoffset:int; yoffset:int; zoffset:int; width:int; height:int; depth:int; format:uint; _type:uint; pixels:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCopyTextureSubImage3DEXT",library="Opengl32")]
def glCopyTextureSubImage3DEXT ( texture:uint; target:uint; level:int; xoffset:int; yoffset:int; zoffset:int; x:int; y:int; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glBindMultiTextureEXT",library="Opengl32")]
def glBindMultiTextureEXT ( texunit:uint; target:uint; texture:uint ) : void
	pass

[extern(opengl,late,name="glMultiTexCoordPointerEXT",library="Opengl32")]
def glMultiTexCoordPointerEXT ( texunit:uint; size:int; _type:uint; stride:int; pointer:void const? implicit ) : void
	pass

[extern(opengl,late,name="glMultiTexEnvfEXT",library="Opengl32")]
def glMultiTexEnvfEXT ( texunit:uint; target:uint; pname:uint; param:float ) : void
	pass

[extern(opengl,late,name="glMultiTexEnvfvEXT",library="Opengl32")]
def glMultiTexEnvfvEXT ( texunit:uint; target:uint; pname:uint; params:float const? implicit ) : void
	pass

[extern(opengl,late,name="glMultiTexEnviEXT",library="Opengl32")]
def glMultiTexEnviEXT ( texunit:uint; target:uint; pname:uint; param:int ) : void
	pass

[extern(opengl,late,name="glMultiTexEnvivEXT",library="Opengl32")]
def glMultiTexEnvivEXT ( texunit:uint; target:uint; pname:uint; params:int const? implicit ) : void
	pass

[extern(opengl,late,name="glMultiTexGendEXT",library="Opengl32")]
def glMultiTexGendEXT ( texunit:uint; coord:uint; pname:uint; param:double ) : void
	pass

[extern(opengl,late,name="glMultiTexGendvEXT",library="Opengl32")]
def glMultiTexGendvEXT ( texunit:uint; coord:uint; pname:uint; params:double const? implicit ) : void
	pass

[extern(opengl,late,name="glMultiTexGenfEXT",library="Opengl32")]
def glMultiTexGenfEXT ( texunit:uint; coord:uint; pname:uint; param:float ) : void
	pass

[extern(opengl,late,name="glMultiTexGenfvEXT",library="Opengl32")]
def glMultiTexGenfvEXT ( texunit:uint; coord:uint; pname:uint; params:float const? implicit ) : void
	pass

[extern(opengl,late,name="glMultiTexGeniEXT",library="Opengl32")]
def glMultiTexGeniEXT ( texunit:uint; coord:uint; pname:uint; param:int ) : void
	pass

[extern(opengl,late,name="glMultiTexGenivEXT",library="Opengl32")]
def glMultiTexGenivEXT ( texunit:uint; coord:uint; pname:uint; params:int const? implicit ) : void
	pass

[extern(opengl,late,name="glGetMultiTexEnvfvEXT",library="Opengl32")]
def glGetMultiTexEnvfvEXT ( texunit:uint; target:uint; pname:uint; params:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetMultiTexEnvivEXT",library="Opengl32")]
def glGetMultiTexEnvivEXT ( texunit:uint; target:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetMultiTexGendvEXT",library="Opengl32")]
def glGetMultiTexGendvEXT ( texunit:uint; coord:uint; pname:uint; params:double? implicit ) : void
	pass

[extern(opengl,late,name="glGetMultiTexGenfvEXT",library="Opengl32")]
def glGetMultiTexGenfvEXT ( texunit:uint; coord:uint; pname:uint; params:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetMultiTexGenivEXT",library="Opengl32")]
def glGetMultiTexGenivEXT ( texunit:uint; coord:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glMultiTexParameteriEXT",library="Opengl32")]
def glMultiTexParameteriEXT ( texunit:uint; target:uint; pname:uint; param:int ) : void
	pass

[extern(opengl,late,name="glMultiTexParameterivEXT",library="Opengl32")]
def glMultiTexParameterivEXT ( texunit:uint; target:uint; pname:uint; params:int const? implicit ) : void
	pass

[extern(opengl,late,name="glMultiTexParameterfEXT",library="Opengl32")]
def glMultiTexParameterfEXT ( texunit:uint; target:uint; pname:uint; param:float ) : void
	pass

[extern(opengl,late,name="glMultiTexParameterfvEXT",library="Opengl32")]
def glMultiTexParameterfvEXT ( texunit:uint; target:uint; pname:uint; params:float const? implicit ) : void
	pass

[extern(opengl,late,name="glMultiTexImage1DEXT",library="Opengl32")]
def glMultiTexImage1DEXT ( texunit:uint; target:uint; level:int; internalformat:int; width:int; border:int; format:uint; _type:uint; pixels:void const? implicit ) : void
	pass

[extern(opengl,late,name="glMultiTexImage2DEXT",library="Opengl32")]
def glMultiTexImage2DEXT ( texunit:uint; target:uint; level:int; internalformat:int; width:int; height:int; border:int; format:uint; _type:uint; pixels:void const? implicit ) : void
	pass

[extern(opengl,late,name="glMultiTexSubImage1DEXT",library="Opengl32")]
def glMultiTexSubImage1DEXT ( texunit:uint; target:uint; level:int; xoffset:int; width:int; format:uint; _type:uint; pixels:void const? implicit ) : void
	pass

[extern(opengl,late,name="glMultiTexSubImage2DEXT",library="Opengl32")]
def glMultiTexSubImage2DEXT ( texunit:uint; target:uint; level:int; xoffset:int; yoffset:int; width:int; height:int; format:uint; _type:uint; pixels:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCopyMultiTexImage1DEXT",library="Opengl32")]
def glCopyMultiTexImage1DEXT ( texunit:uint; target:uint; level:int; internalformat:uint; x:int; y:int; width:int; border:int ) : void
	pass

[extern(opengl,late,name="glCopyMultiTexImage2DEXT",library="Opengl32")]
def glCopyMultiTexImage2DEXT ( texunit:uint; target:uint; level:int; internalformat:uint; x:int; y:int; width:int; height:int; border:int ) : void
	pass

[extern(opengl,late,name="glCopyMultiTexSubImage1DEXT",library="Opengl32")]
def glCopyMultiTexSubImage1DEXT ( texunit:uint; target:uint; level:int; xoffset:int; x:int; y:int; width:int ) : void
	pass

[extern(opengl,late,name="glCopyMultiTexSubImage2DEXT",library="Opengl32")]
def glCopyMultiTexSubImage2DEXT ( texunit:uint; target:uint; level:int; xoffset:int; yoffset:int; x:int; y:int; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glGetMultiTexImageEXT",library="Opengl32")]
def glGetMultiTexImageEXT ( texunit:uint; target:uint; level:int; format:uint; _type:uint; pixels:void? implicit ) : void
	pass

[extern(opengl,late,name="glGetMultiTexParameterfvEXT",library="Opengl32")]
def glGetMultiTexParameterfvEXT ( texunit:uint; target:uint; pname:uint; params:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetMultiTexParameterivEXT",library="Opengl32")]
def glGetMultiTexParameterivEXT ( texunit:uint; target:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetMultiTexLevelParameterfvEXT",library="Opengl32")]
def glGetMultiTexLevelParameterfvEXT ( texunit:uint; target:uint; level:int; pname:uint; params:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetMultiTexLevelParameterivEXT",library="Opengl32")]
def glGetMultiTexLevelParameterivEXT ( texunit:uint; target:uint; level:int; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glMultiTexImage3DEXT",library="Opengl32")]
def glMultiTexImage3DEXT ( texunit:uint; target:uint; level:int; internalformat:int; width:int; height:int; depth:int; border:int; format:uint; _type:uint; pixels:void const? implicit ) : void
	pass

[extern(opengl,late,name="glMultiTexSubImage3DEXT",library="Opengl32")]
def glMultiTexSubImage3DEXT ( texunit:uint; target:uint; level:int; xoffset:int; yoffset:int; zoffset:int; width:int; height:int; depth:int; format:uint; _type:uint; pixels:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCopyMultiTexSubImage3DEXT",library="Opengl32")]
def glCopyMultiTexSubImage3DEXT ( texunit:uint; target:uint; level:int; xoffset:int; yoffset:int; zoffset:int; x:int; y:int; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glEnableClientStateIndexedEXT",library="Opengl32")]
def glEnableClientStateIndexedEXT ( _array:uint; index:uint ) : void
	pass

[extern(opengl,late,name="glDisableClientStateIndexedEXT",library="Opengl32")]
def glDisableClientStateIndexedEXT ( _array:uint; index:uint ) : void
	pass

[extern(opengl,late,name="glGetFloatIndexedvEXT",library="Opengl32")]
def glGetFloatIndexedvEXT ( target:uint; index:uint; data:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetDoubleIndexedvEXT",library="Opengl32")]
def glGetDoubleIndexedvEXT ( target:uint; index:uint; data:double? implicit ) : void
	pass

[extern(opengl,late,name="glGetPointerIndexedvEXT",library="Opengl32")]
def glGetPointerIndexedvEXT ( target:uint; index:uint; data:void? implicit ) : void
	pass

[extern(opengl,late,name="glEnableIndexedEXT",library="Opengl32")]
def glEnableIndexedEXT ( target:uint; index:uint ) : void
	pass

[extern(opengl,late,name="glDisableIndexedEXT",library="Opengl32")]
def glDisableIndexedEXT ( target:uint; index:uint ) : void
	pass

[extern(opengl,late,name="glIsEnabledIndexedEXT",library="Opengl32")]
def glIsEnabledIndexedEXT ( target:uint; index:uint ) : bool
	return [[bool]]

[extern(opengl,late,name="glGetIntegerIndexedvEXT",library="Opengl32")]
def glGetIntegerIndexedvEXT ( target:uint; index:uint; data:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetBooleanIndexedvEXT",library="Opengl32")]
def glGetBooleanIndexedvEXT ( target:uint; index:uint; data:bool? implicit ) : void
	pass

[extern(opengl,late,name="glCompressedTextureImage3DEXT",library="Opengl32")]
def glCompressedTextureImage3DEXT ( texture:uint; target:uint; level:int; internalformat:uint; width:int; height:int; depth:int; border:int; imageSize:int; bits:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCompressedTextureImage2DEXT",library="Opengl32")]
def glCompressedTextureImage2DEXT ( texture:uint; target:uint; level:int; internalformat:uint; width:int; height:int; border:int; imageSize:int; bits:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCompressedTextureImage1DEXT",library="Opengl32")]
def glCompressedTextureImage1DEXT ( texture:uint; target:uint; level:int; internalformat:uint; width:int; border:int; imageSize:int; bits:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCompressedTextureSubImage3DEXT",library="Opengl32")]
def glCompressedTextureSubImage3DEXT ( texture:uint; target:uint; level:int; xoffset:int; yoffset:int; zoffset:int; width:int; height:int; depth:int; format:uint; imageSize:int; bits:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCompressedTextureSubImage2DEXT",library="Opengl32")]
def glCompressedTextureSubImage2DEXT ( texture:uint; target:uint; level:int; xoffset:int; yoffset:int; width:int; height:int; format:uint; imageSize:int; bits:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCompressedTextureSubImage1DEXT",library="Opengl32")]
def glCompressedTextureSubImage1DEXT ( texture:uint; target:uint; level:int; xoffset:int; width:int; format:uint; imageSize:int; bits:void const? implicit ) : void
	pass

[extern(opengl,late,name="glGetCompressedTextureImageEXT",library="Opengl32")]
def glGetCompressedTextureImageEXT ( texture:uint; target:uint; lod:int; img:void? implicit ) : void
	pass

[extern(opengl,late,name="glCompressedMultiTexImage3DEXT",library="Opengl32")]
def glCompressedMultiTexImage3DEXT ( texunit:uint; target:uint; level:int; internalformat:uint; width:int; height:int; depth:int; border:int; imageSize:int; bits:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCompressedMultiTexImage2DEXT",library="Opengl32")]
def glCompressedMultiTexImage2DEXT ( texunit:uint; target:uint; level:int; internalformat:uint; width:int; height:int; border:int; imageSize:int; bits:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCompressedMultiTexImage1DEXT",library="Opengl32")]
def glCompressedMultiTexImage1DEXT ( texunit:uint; target:uint; level:int; internalformat:uint; width:int; border:int; imageSize:int; bits:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCompressedMultiTexSubImage3DEXT",library="Opengl32")]
def glCompressedMultiTexSubImage3DEXT ( texunit:uint; target:uint; level:int; xoffset:int; yoffset:int; zoffset:int; width:int; height:int; depth:int; format:uint; imageSize:int; bits:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCompressedMultiTexSubImage2DEXT",library="Opengl32")]
def glCompressedMultiTexSubImage2DEXT ( texunit:uint; target:uint; level:int; xoffset:int; yoffset:int; width:int; height:int; format:uint; imageSize:int; bits:void const? implicit ) : void
	pass

[extern(opengl,late,name="glCompressedMultiTexSubImage1DEXT",library="Opengl32")]
def glCompressedMultiTexSubImage1DEXT ( texunit:uint; target:uint; level:int; xoffset:int; width:int; format:uint; imageSize:int; bits:void const? implicit ) : void
	pass

[extern(opengl,late,name="glGetCompressedMultiTexImageEXT",library="Opengl32")]
def glGetCompressedMultiTexImageEXT ( texunit:uint; target:uint; lod:int; img:void? implicit ) : void
	pass

[extern(opengl,late,name="glMatrixLoadTransposefEXT",library="Opengl32")]
def glMatrixLoadTransposefEXT ( mode:uint; m:float const? implicit ) : void
	pass

[extern(opengl,late,name="glMatrixLoadTransposedEXT",library="Opengl32")]
def glMatrixLoadTransposedEXT ( mode:uint; m:double const? implicit ) : void
	pass

[extern(opengl,late,name="glMatrixMultTransposefEXT",library="Opengl32")]
def glMatrixMultTransposefEXT ( mode:uint; m:float const? implicit ) : void
	pass

[extern(opengl,late,name="glMatrixMultTransposedEXT",library="Opengl32")]
def glMatrixMultTransposedEXT ( mode:uint; m:double const? implicit ) : void
	pass

[extern(opengl,late,name="glNamedBufferDataEXT",library="Opengl32")]
def glNamedBufferDataEXT ( buffer:uint; size:int64; data:void const? implicit; usage:uint ) : void
	pass

[extern(opengl,late,name="glNamedBufferSubDataEXT",library="Opengl32")]
def glNamedBufferSubDataEXT ( buffer:uint; offset:int64; size:int64; data:void const? implicit ) : void
	pass

[extern(opengl,late,name="glMapNamedBufferEXT",library="Opengl32")]
def glMapNamedBufferEXT ( buffer:uint; access:uint ) : void?
	return [[void?]]

[extern(opengl,late,name="glUnmapNamedBufferEXT",library="Opengl32")]
def glUnmapNamedBufferEXT ( buffer:uint ) : bool
	return [[bool]]

[extern(opengl,late,name="glGetNamedBufferParameterivEXT",library="Opengl32")]
def glGetNamedBufferParameterivEXT ( buffer:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetNamedBufferPointervEXT",library="Opengl32")]
def glGetNamedBufferPointervEXT ( buffer:uint; pname:uint; params:void? implicit ) : void
	pass

[extern(opengl,late,name="glGetNamedBufferSubDataEXT",library="Opengl32")]
def glGetNamedBufferSubDataEXT ( buffer:uint; offset:int64; size:int64; data:void? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform1fEXT",library="Opengl32")]
def glProgramUniform1fEXT ( program:uint; location:int; v0:float ) : void
	pass

[extern(opengl,late,name="glProgramUniform2fEXT",library="Opengl32")]
def glProgramUniform2fEXT ( program:uint; location:int; v0:float; v1:float ) : void
	pass

[extern(opengl,late,name="glProgramUniform3fEXT",library="Opengl32")]
def glProgramUniform3fEXT ( program:uint; location:int; v0:float; v1:float; v2:float ) : void
	pass

[extern(opengl,late,name="glProgramUniform4fEXT",library="Opengl32")]
def glProgramUniform4fEXT ( program:uint; location:int; v0:float; v1:float; v2:float; v3:float ) : void
	pass

[extern(opengl,late,name="glProgramUniform1iEXT",library="Opengl32")]
def glProgramUniform1iEXT ( program:uint; location:int; v0:int ) : void
	pass

[extern(opengl,late,name="glProgramUniform2iEXT",library="Opengl32")]
def glProgramUniform2iEXT ( program:uint; location:int; v0:int; v1:int ) : void
	pass

[extern(opengl,late,name="glProgramUniform3iEXT",library="Opengl32")]
def glProgramUniform3iEXT ( program:uint; location:int; v0:int; v1:int; v2:int ) : void
	pass

[extern(opengl,late,name="glProgramUniform4iEXT",library="Opengl32")]
def glProgramUniform4iEXT ( program:uint; location:int; v0:int; v1:int; v2:int; v3:int ) : void
	pass

[extern(opengl,late,name="glProgramUniform1fvEXT",library="Opengl32")]
def glProgramUniform1fvEXT ( program:uint; location:int; count:int; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform2fvEXT",library="Opengl32")]
def glProgramUniform2fvEXT ( program:uint; location:int; count:int; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform3fvEXT",library="Opengl32")]
def glProgramUniform3fvEXT ( program:uint; location:int; count:int; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform4fvEXT",library="Opengl32")]
def glProgramUniform4fvEXT ( program:uint; location:int; count:int; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform1ivEXT",library="Opengl32")]
def glProgramUniform1ivEXT ( program:uint; location:int; count:int; value:int const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform2ivEXT",library="Opengl32")]
def glProgramUniform2ivEXT ( program:uint; location:int; count:int; value:int const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform3ivEXT",library="Opengl32")]
def glProgramUniform3ivEXT ( program:uint; location:int; count:int; value:int const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform4ivEXT",library="Opengl32")]
def glProgramUniform4ivEXT ( program:uint; location:int; count:int; value:int const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix2fvEXT",library="Opengl32")]
def glProgramUniformMatrix2fvEXT ( program:uint; location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix3fvEXT",library="Opengl32")]
def glProgramUniformMatrix3fvEXT ( program:uint; location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix4fvEXT",library="Opengl32")]
def glProgramUniformMatrix4fvEXT ( program:uint; location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix2x3fvEXT",library="Opengl32")]
def glProgramUniformMatrix2x3fvEXT ( program:uint; location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix3x2fvEXT",library="Opengl32")]
def glProgramUniformMatrix3x2fvEXT ( program:uint; location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix2x4fvEXT",library="Opengl32")]
def glProgramUniformMatrix2x4fvEXT ( program:uint; location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix4x2fvEXT",library="Opengl32")]
def glProgramUniformMatrix4x2fvEXT ( program:uint; location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix3x4fvEXT",library="Opengl32")]
def glProgramUniformMatrix3x4fvEXT ( program:uint; location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix4x3fvEXT",library="Opengl32")]
def glProgramUniformMatrix4x3fvEXT ( program:uint; location:int; count:int; transpose:bool; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glTextureBufferEXT",library="Opengl32")]
def glTextureBufferEXT ( texture:uint; target:uint; internalformat:uint; buffer:uint ) : void
	pass

[extern(opengl,late,name="glMultiTexBufferEXT",library="Opengl32")]
def glMultiTexBufferEXT ( texunit:uint; target:uint; internalformat:uint; buffer:uint ) : void
	pass

[extern(opengl,late,name="glTextureParameterIivEXT",library="Opengl32")]
def glTextureParameterIivEXT ( texture:uint; target:uint; pname:uint; params:int const? implicit ) : void
	pass

[extern(opengl,late,name="glTextureParameterIuivEXT",library="Opengl32")]
def glTextureParameterIuivEXT ( texture:uint; target:uint; pname:uint; params:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glGetTextureParameterIivEXT",library="Opengl32")]
def glGetTextureParameterIivEXT ( texture:uint; target:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetTextureParameterIuivEXT",library="Opengl32")]
def glGetTextureParameterIuivEXT ( texture:uint; target:uint; pname:uint; params:uint? implicit ) : void
	pass

[extern(opengl,late,name="glMultiTexParameterIivEXT",library="Opengl32")]
def glMultiTexParameterIivEXT ( texunit:uint; target:uint; pname:uint; params:int const? implicit ) : void
	pass

[extern(opengl,late,name="glMultiTexParameterIuivEXT",library="Opengl32")]
def glMultiTexParameterIuivEXT ( texunit:uint; target:uint; pname:uint; params:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glGetMultiTexParameterIivEXT",library="Opengl32")]
def glGetMultiTexParameterIivEXT ( texunit:uint; target:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetMultiTexParameterIuivEXT",library="Opengl32")]
def glGetMultiTexParameterIuivEXT ( texunit:uint; target:uint; pname:uint; params:uint? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform1uiEXT",library="Opengl32")]
def glProgramUniform1uiEXT ( program:uint; location:int; v0:uint ) : void
	pass

[extern(opengl,late,name="glProgramUniform2uiEXT",library="Opengl32")]
def glProgramUniform2uiEXT ( program:uint; location:int; v0:uint; v1:uint ) : void
	pass

[extern(opengl,late,name="glProgramUniform3uiEXT",library="Opengl32")]
def glProgramUniform3uiEXT ( program:uint; location:int; v0:uint; v1:uint; v2:uint ) : void
	pass

[extern(opengl,late,name="glProgramUniform4uiEXT",library="Opengl32")]
def glProgramUniform4uiEXT ( program:uint; location:int; v0:uint; v1:uint; v2:uint; v3:uint ) : void
	pass

[extern(opengl,late,name="glProgramUniform1uivEXT",library="Opengl32")]
def glProgramUniform1uivEXT ( program:uint; location:int; count:int; value:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform2uivEXT",library="Opengl32")]
def glProgramUniform2uivEXT ( program:uint; location:int; count:int; value:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform3uivEXT",library="Opengl32")]
def glProgramUniform3uivEXT ( program:uint; location:int; count:int; value:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform4uivEXT",library="Opengl32")]
def glProgramUniform4uivEXT ( program:uint; location:int; count:int; value:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glNamedProgramLocalParameters4fvEXT",library="Opengl32")]
def glNamedProgramLocalParameters4fvEXT ( program:uint; target:uint; index:uint; count:int; params:float const? implicit ) : void
	pass

[extern(opengl,late,name="glNamedProgramLocalParameterI4iEXT",library="Opengl32")]
def glNamedProgramLocalParameterI4iEXT ( program:uint; target:uint; index:uint; x:int; y:int; z:int; w:int ) : void
	pass

[extern(opengl,late,name="glNamedProgramLocalParameterI4ivEXT",library="Opengl32")]
def glNamedProgramLocalParameterI4ivEXT ( program:uint; target:uint; index:uint; params:int const? implicit ) : void
	pass

[extern(opengl,late,name="glNamedProgramLocalParametersI4ivEXT",library="Opengl32")]
def glNamedProgramLocalParametersI4ivEXT ( program:uint; target:uint; index:uint; count:int; params:int const? implicit ) : void
	pass

[extern(opengl,late,name="glNamedProgramLocalParameterI4uiEXT",library="Opengl32")]
def glNamedProgramLocalParameterI4uiEXT ( program:uint; target:uint; index:uint; x:uint; y:uint; z:uint; w:uint ) : void
	pass

[extern(opengl,late,name="glNamedProgramLocalParameterI4uivEXT",library="Opengl32")]
def glNamedProgramLocalParameterI4uivEXT ( program:uint; target:uint; index:uint; params:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glNamedProgramLocalParametersI4uivEXT",library="Opengl32")]
def glNamedProgramLocalParametersI4uivEXT ( program:uint; target:uint; index:uint; count:int; params:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glGetNamedProgramLocalParameterIivEXT",library="Opengl32")]
def glGetNamedProgramLocalParameterIivEXT ( program:uint; target:uint; index:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetNamedProgramLocalParameterIuivEXT",library="Opengl32")]
def glGetNamedProgramLocalParameterIuivEXT ( program:uint; target:uint; index:uint; params:uint? implicit ) : void
	pass

[extern(opengl,late,name="glEnableClientStateiEXT",library="Opengl32")]
def glEnableClientStateiEXT ( _array:uint; index:uint ) : void
	pass

[extern(opengl,late,name="glDisableClientStateiEXT",library="Opengl32")]
def glDisableClientStateiEXT ( _array:uint; index:uint ) : void
	pass

[extern(opengl,late,name="glGetFloati_vEXT",library="Opengl32")]
def glGetFloati_vEXT ( pname:uint; index:uint; params:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetDoublei_vEXT",library="Opengl32")]
def glGetDoublei_vEXT ( pname:uint; index:uint; params:double? implicit ) : void
	pass

[extern(opengl,late,name="glGetPointeri_vEXT",library="Opengl32")]
def glGetPointeri_vEXT ( pname:uint; index:uint; params:void? implicit ) : void
	pass

[extern(opengl,late,name="glNamedProgramStringEXT",library="Opengl32")]
def glNamedProgramStringEXT ( program:uint; target:uint; format:uint; len:int; _string:void const? implicit ) : void
	pass

[extern(opengl,late,name="glNamedProgramLocalParameter4dEXT",library="Opengl32")]
def glNamedProgramLocalParameter4dEXT ( program:uint; target:uint; index:uint; x:double; y:double; z:double; w:double ) : void
	pass

[extern(opengl,late,name="glNamedProgramLocalParameter4dvEXT",library="Opengl32")]
def glNamedProgramLocalParameter4dvEXT ( program:uint; target:uint; index:uint; params:double const? implicit ) : void
	pass

[extern(opengl,late,name="glNamedProgramLocalParameter4fEXT",library="Opengl32")]
def glNamedProgramLocalParameter4fEXT ( program:uint; target:uint; index:uint; x:float; y:float; z:float; w:float ) : void
	pass

[extern(opengl,late,name="glNamedProgramLocalParameter4fvEXT",library="Opengl32")]
def glNamedProgramLocalParameter4fvEXT ( program:uint; target:uint; index:uint; params:float const? implicit ) : void
	pass

[extern(opengl,late,name="glGetNamedProgramLocalParameterdvEXT",library="Opengl32")]
def glGetNamedProgramLocalParameterdvEXT ( program:uint; target:uint; index:uint; params:double? implicit ) : void
	pass

[extern(opengl,late,name="glGetNamedProgramLocalParameterfvEXT",library="Opengl32")]
def glGetNamedProgramLocalParameterfvEXT ( program:uint; target:uint; index:uint; params:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetNamedProgramivEXT",library="Opengl32")]
def glGetNamedProgramivEXT ( program:uint; target:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetNamedProgramStringEXT",library="Opengl32")]
def glGetNamedProgramStringEXT ( program:uint; target:uint; pname:uint; _string:void? implicit ) : void
	pass

[extern(opengl,late,name="glNamedRenderbufferStorageEXT",library="Opengl32")]
def glNamedRenderbufferStorageEXT ( renderbuffer:uint; internalformat:uint; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glGetNamedRenderbufferParameterivEXT",library="Opengl32")]
def glGetNamedRenderbufferParameterivEXT ( renderbuffer:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glNamedRenderbufferStorageMultisampleEXT",library="Opengl32")]
def glNamedRenderbufferStorageMultisampleEXT ( renderbuffer:uint; samples:int; internalformat:uint; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glNamedRenderbufferStorageMultisampleCoverageEXT",library="Opengl32")]
def glNamedRenderbufferStorageMultisampleCoverageEXT ( renderbuffer:uint; coverageSamples:int; colorSamples:int; internalformat:uint; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glCheckNamedFramebufferStatusEXT",library="Opengl32")]
def glCheckNamedFramebufferStatusEXT ( framebuffer:uint; target:uint ) : uint
	return [[uint]]

[extern(opengl,late,name="glNamedFramebufferTexture1DEXT",library="Opengl32")]
def glNamedFramebufferTexture1DEXT ( framebuffer:uint; attachment:uint; textarget:uint; texture:uint; level:int ) : void
	pass

[extern(opengl,late,name="glNamedFramebufferTexture2DEXT",library="Opengl32")]
def glNamedFramebufferTexture2DEXT ( framebuffer:uint; attachment:uint; textarget:uint; texture:uint; level:int ) : void
	pass

[extern(opengl,late,name="glNamedFramebufferTexture3DEXT",library="Opengl32")]
def glNamedFramebufferTexture3DEXT ( framebuffer:uint; attachment:uint; textarget:uint; texture:uint; level:int; zoffset:int ) : void
	pass

[extern(opengl,late,name="glNamedFramebufferRenderbufferEXT",library="Opengl32")]
def glNamedFramebufferRenderbufferEXT ( framebuffer:uint; attachment:uint; renderbuffertarget:uint; renderbuffer:uint ) : void
	pass

[extern(opengl,late,name="glGetNamedFramebufferAttachmentParameterivEXT",library="Opengl32")]
def glGetNamedFramebufferAttachmentParameterivEXT ( framebuffer:uint; attachment:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGenerateTextureMipmapEXT",library="Opengl32")]
def glGenerateTextureMipmapEXT ( texture:uint; target:uint ) : void
	pass

[extern(opengl,late,name="glGenerateMultiTexMipmapEXT",library="Opengl32")]
def glGenerateMultiTexMipmapEXT ( texunit:uint; target:uint ) : void
	pass

[extern(opengl,late,name="glFramebufferDrawBufferEXT",library="Opengl32")]
def glFramebufferDrawBufferEXT ( framebuffer:uint; mode:uint ) : void
	pass

[extern(opengl,late,name="glFramebufferDrawBuffersEXT",library="Opengl32")]
def glFramebufferDrawBuffersEXT ( framebuffer:uint; n:int; bufs:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glFramebufferReadBufferEXT",library="Opengl32")]
def glFramebufferReadBufferEXT ( framebuffer:uint; mode:uint ) : void
	pass

[extern(opengl,late,name="glGetFramebufferParameterivEXT",library="Opengl32")]
def glGetFramebufferParameterivEXT ( framebuffer:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glNamedCopyBufferSubDataEXT",library="Opengl32")]
def glNamedCopyBufferSubDataEXT ( readBuffer:uint; writeBuffer:uint; readOffset:int64; writeOffset:int64; size:int64 ) : void
	pass

[extern(opengl,late,name="glNamedFramebufferTextureEXT",library="Opengl32")]
def glNamedFramebufferTextureEXT ( framebuffer:uint; attachment:uint; texture:uint; level:int ) : void
	pass

[extern(opengl,late,name="glNamedFramebufferTextureLayerEXT",library="Opengl32")]
def glNamedFramebufferTextureLayerEXT ( framebuffer:uint; attachment:uint; texture:uint; level:int; layer:int ) : void
	pass

[extern(opengl,late,name="glNamedFramebufferTextureFaceEXT",library="Opengl32")]
def glNamedFramebufferTextureFaceEXT ( framebuffer:uint; attachment:uint; texture:uint; level:int; face:uint ) : void
	pass

[extern(opengl,late,name="glTextureRenderbufferEXT",library="Opengl32")]
def glTextureRenderbufferEXT ( texture:uint; target:uint; renderbuffer:uint ) : void
	pass

[extern(opengl,late,name="glMultiTexRenderbufferEXT",library="Opengl32")]
def glMultiTexRenderbufferEXT ( texunit:uint; target:uint; renderbuffer:uint ) : void
	pass

[extern(opengl,late,name="glVertexArrayVertexOffsetEXT",library="Opengl32")]
def glVertexArrayVertexOffsetEXT ( vaobj:uint; buffer:uint; size:int; _type:uint; stride:int; offset:int64 ) : void
	pass

[extern(opengl,late,name="glVertexArrayColorOffsetEXT",library="Opengl32")]
def glVertexArrayColorOffsetEXT ( vaobj:uint; buffer:uint; size:int; _type:uint; stride:int; offset:int64 ) : void
	pass

[extern(opengl,late,name="glVertexArrayEdgeFlagOffsetEXT",library="Opengl32")]
def glVertexArrayEdgeFlagOffsetEXT ( vaobj:uint; buffer:uint; stride:int; offset:int64 ) : void
	pass

[extern(opengl,late,name="glVertexArrayIndexOffsetEXT",library="Opengl32")]
def glVertexArrayIndexOffsetEXT ( vaobj:uint; buffer:uint; _type:uint; stride:int; offset:int64 ) : void
	pass

[extern(opengl,late,name="glVertexArrayNormalOffsetEXT",library="Opengl32")]
def glVertexArrayNormalOffsetEXT ( vaobj:uint; buffer:uint; _type:uint; stride:int; offset:int64 ) : void
	pass

[extern(opengl,late,name="glVertexArrayTexCoordOffsetEXT",library="Opengl32")]
def glVertexArrayTexCoordOffsetEXT ( vaobj:uint; buffer:uint; size:int; _type:uint; stride:int; offset:int64 ) : void
	pass

[extern(opengl,late,name="glVertexArrayMultiTexCoordOffsetEXT",library="Opengl32")]
def glVertexArrayMultiTexCoordOffsetEXT ( vaobj:uint; buffer:uint; texunit:uint; size:int; _type:uint; stride:int; offset:int64 ) : void
	pass

[extern(opengl,late,name="glVertexArrayFogCoordOffsetEXT",library="Opengl32")]
def glVertexArrayFogCoordOffsetEXT ( vaobj:uint; buffer:uint; _type:uint; stride:int; offset:int64 ) : void
	pass

[extern(opengl,late,name="glVertexArraySecondaryColorOffsetEXT",library="Opengl32")]
def glVertexArraySecondaryColorOffsetEXT ( vaobj:uint; buffer:uint; size:int; _type:uint; stride:int; offset:int64 ) : void
	pass

[extern(opengl,late,name="glVertexArrayVertexAttribOffsetEXT",library="Opengl32")]
def glVertexArrayVertexAttribOffsetEXT ( vaobj:uint; buffer:uint; index:uint; size:int; _type:uint; normalized:bool; stride:int; offset:int64 ) : void
	pass

[extern(opengl,late,name="glVertexArrayVertexAttribIOffsetEXT",library="Opengl32")]
def glVertexArrayVertexAttribIOffsetEXT ( vaobj:uint; buffer:uint; index:uint; size:int; _type:uint; stride:int; offset:int64 ) : void
	pass

[extern(opengl,late,name="glEnableVertexArrayEXT",library="Opengl32")]
def glEnableVertexArrayEXT ( vaobj:uint; _array:uint ) : void
	pass

[extern(opengl,late,name="glDisableVertexArrayEXT",library="Opengl32")]
def glDisableVertexArrayEXT ( vaobj:uint; _array:uint ) : void
	pass

[extern(opengl,late,name="glEnableVertexArrayAttribEXT",library="Opengl32")]
def glEnableVertexArrayAttribEXT ( vaobj:uint; index:uint ) : void
	pass

[extern(opengl,late,name="glDisableVertexArrayAttribEXT",library="Opengl32")]
def glDisableVertexArrayAttribEXT ( vaobj:uint; index:uint ) : void
	pass

[extern(opengl,late,name="glGetVertexArrayIntegervEXT",library="Opengl32")]
def glGetVertexArrayIntegervEXT ( vaobj:uint; pname:uint; param:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetVertexArrayPointervEXT",library="Opengl32")]
def glGetVertexArrayPointervEXT ( vaobj:uint; pname:uint; param:void? implicit ) : void
	pass

[extern(opengl,late,name="glGetVertexArrayIntegeri_vEXT",library="Opengl32")]
def glGetVertexArrayIntegeri_vEXT ( vaobj:uint; index:uint; pname:uint; param:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetVertexArrayPointeri_vEXT",library="Opengl32")]
def glGetVertexArrayPointeri_vEXT ( vaobj:uint; index:uint; pname:uint; param:void? implicit ) : void
	pass

[extern(opengl,late,name="glMapNamedBufferRangeEXT",library="Opengl32")]
def glMapNamedBufferRangeEXT ( buffer:uint; offset:int64; length:int64; access:uint ) : void?
	return [[void?]]

[extern(opengl,late,name="glFlushMappedNamedBufferRangeEXT",library="Opengl32")]
def glFlushMappedNamedBufferRangeEXT ( buffer:uint; offset:int64; length:int64 ) : void
	pass

[extern(opengl,late,name="glNamedBufferStorageEXT",library="Opengl32")]
def glNamedBufferStorageEXT ( buffer:uint; size:int64; data:void const? implicit; flags:uint ) : void
	pass

[extern(opengl,late,name="glClearNamedBufferDataEXT",library="Opengl32")]
def glClearNamedBufferDataEXT ( buffer:uint; internalformat:uint; format:uint; _type:uint; data:void const? implicit ) : void
	pass

[extern(opengl,late,name="glClearNamedBufferSubDataEXT",library="Opengl32")]
def glClearNamedBufferSubDataEXT ( buffer:uint; internalformat:uint; offset:int64; size:int64; format:uint; _type:uint; data:void const? implicit ) : void
	pass

[extern(opengl,late,name="glNamedFramebufferParameteriEXT",library="Opengl32")]
def glNamedFramebufferParameteriEXT ( framebuffer:uint; pname:uint; param:int ) : void
	pass

[extern(opengl,late,name="glGetNamedFramebufferParameterivEXT",library="Opengl32")]
def glGetNamedFramebufferParameterivEXT ( framebuffer:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform1dEXT",library="Opengl32")]
def glProgramUniform1dEXT ( program:uint; location:int; x:double ) : void
	pass

[extern(opengl,late,name="glProgramUniform2dEXT",library="Opengl32")]
def glProgramUniform2dEXT ( program:uint; location:int; x:double; y:double ) : void
	pass

[extern(opengl,late,name="glProgramUniform3dEXT",library="Opengl32")]
def glProgramUniform3dEXT ( program:uint; location:int; x:double; y:double; z:double ) : void
	pass

[extern(opengl,late,name="glProgramUniform4dEXT",library="Opengl32")]
def glProgramUniform4dEXT ( program:uint; location:int; x:double; y:double; z:double; w:double ) : void
	pass

[extern(opengl,late,name="glProgramUniform1dvEXT",library="Opengl32")]
def glProgramUniform1dvEXT ( program:uint; location:int; count:int; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform2dvEXT",library="Opengl32")]
def glProgramUniform2dvEXT ( program:uint; location:int; count:int; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform3dvEXT",library="Opengl32")]
def glProgramUniform3dvEXT ( program:uint; location:int; count:int; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform4dvEXT",library="Opengl32")]
def glProgramUniform4dvEXT ( program:uint; location:int; count:int; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix2dvEXT",library="Opengl32")]
def glProgramUniformMatrix2dvEXT ( program:uint; location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix3dvEXT",library="Opengl32")]
def glProgramUniformMatrix3dvEXT ( program:uint; location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix4dvEXT",library="Opengl32")]
def glProgramUniformMatrix4dvEXT ( program:uint; location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix2x3dvEXT",library="Opengl32")]
def glProgramUniformMatrix2x3dvEXT ( program:uint; location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix2x4dvEXT",library="Opengl32")]
def glProgramUniformMatrix2x4dvEXT ( program:uint; location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix3x2dvEXT",library="Opengl32")]
def glProgramUniformMatrix3x2dvEXT ( program:uint; location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix3x4dvEXT",library="Opengl32")]
def glProgramUniformMatrix3x4dvEXT ( program:uint; location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix4x2dvEXT",library="Opengl32")]
def glProgramUniformMatrix4x2dvEXT ( program:uint; location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformMatrix4x3dvEXT",library="Opengl32")]
def glProgramUniformMatrix4x3dvEXT ( program:uint; location:int; count:int; transpose:bool; value:double const? implicit ) : void
	pass

[extern(opengl,late,name="glTextureBufferRangeEXT",library="Opengl32")]
def glTextureBufferRangeEXT ( texture:uint; target:uint; internalformat:uint; buffer:uint; offset:int64; size:int64 ) : void
	pass

[extern(opengl,late,name="glTextureStorage1DEXT",library="Opengl32")]
def glTextureStorage1DEXT ( texture:uint; target:uint; levels:int; internalformat:uint; width:int ) : void
	pass

[extern(opengl,late,name="glTextureStorage2DEXT",library="Opengl32")]
def glTextureStorage2DEXT ( texture:uint; target:uint; levels:int; internalformat:uint; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glTextureStorage3DEXT",library="Opengl32")]
def glTextureStorage3DEXT ( texture:uint; target:uint; levels:int; internalformat:uint; width:int; height:int; depth:int ) : void
	pass

[extern(opengl,late,name="glTextureStorage2DMultisampleEXT",library="Opengl32")]
def glTextureStorage2DMultisampleEXT ( texture:uint; target:uint; samples:int; internalformat:uint; width:int; height:int; fixedsamplelocations:bool ) : void
	pass

[extern(opengl,late,name="glTextureStorage3DMultisampleEXT",library="Opengl32")]
def glTextureStorage3DMultisampleEXT ( texture:uint; target:uint; samples:int; internalformat:uint; width:int; height:int; depth:int; fixedsamplelocations:bool ) : void
	pass

[extern(opengl,late,name="glVertexArrayBindVertexBufferEXT",library="Opengl32")]
def glVertexArrayBindVertexBufferEXT ( vaobj:uint; bindingindex:uint; buffer:uint; offset:int64; stride:int ) : void
	pass

[extern(opengl,late,name="glVertexArrayVertexAttribFormatEXT",library="Opengl32")]
def glVertexArrayVertexAttribFormatEXT ( vaobj:uint; attribindex:uint; size:int; _type:uint; normalized:bool; relativeoffset:uint ) : void
	pass

[extern(opengl,late,name="glVertexArrayVertexAttribIFormatEXT",library="Opengl32")]
def glVertexArrayVertexAttribIFormatEXT ( vaobj:uint; attribindex:uint; size:int; _type:uint; relativeoffset:uint ) : void
	pass

[extern(opengl,late,name="glVertexArrayVertexAttribLFormatEXT",library="Opengl32")]
def glVertexArrayVertexAttribLFormatEXT ( vaobj:uint; attribindex:uint; size:int; _type:uint; relativeoffset:uint ) : void
	pass

[extern(opengl,late,name="glVertexArrayVertexAttribBindingEXT",library="Opengl32")]
def glVertexArrayVertexAttribBindingEXT ( vaobj:uint; attribindex:uint; bindingindex:uint ) : void
	pass

[extern(opengl,late,name="glVertexArrayVertexBindingDivisorEXT",library="Opengl32")]
def glVertexArrayVertexBindingDivisorEXT ( vaobj:uint; bindingindex:uint; divisor:uint ) : void
	pass

[extern(opengl,late,name="glVertexArrayVertexAttribLOffsetEXT",library="Opengl32")]
def glVertexArrayVertexAttribLOffsetEXT ( vaobj:uint; buffer:uint; index:uint; size:int; _type:uint; stride:int; offset:int64 ) : void
	pass

[extern(opengl,late,name="glTexturePageCommitmentEXT",library="Opengl32")]
def glTexturePageCommitmentEXT ( texture:uint; level:int; xoffset:int; yoffset:int; zoffset:int; width:int; height:int; depth:int; commit:bool ) : void
	pass

[extern(opengl,late,name="glVertexArrayVertexAttribDivisorEXT",library="Opengl32")]
def glVertexArrayVertexAttribDivisorEXT ( vaobj:uint; index:uint; divisor:uint ) : void
	pass

[extern(opengl,late,name="glDrawArraysInstancedEXT",library="Opengl32")]
def glDrawArraysInstancedEXT ( mode:uint; start:int; count:int; primcount:int ) : void
	pass

[extern(opengl,late,name="glDrawElementsInstancedEXT",library="Opengl32")]
def glDrawElementsInstancedEXT ( mode:uint; count:int; _type:uint; indices:void const? implicit; primcount:int ) : void
	pass

[extern(opengl,late,name="glPolygonOffsetClampEXT",library="Opengl32")]
def glPolygonOffsetClampEXT ( factor:float; units:float; clamp:float ) : void
	pass

[extern(opengl,late,name="glRasterSamplesEXT",library="Opengl32")]
def glRasterSamplesEXT ( samples:uint; fixedsamplelocations:bool ) : void
	pass

[extern(opengl,late,name="glUseShaderProgramEXT",library="Opengl32")]
def glUseShaderProgramEXT ( _type:uint; program:uint ) : void
	pass

[extern(opengl,late,name="glActiveProgramEXT",library="Opengl32")]
def glActiveProgramEXT ( program:uint ) : void
	pass

[extern(opengl,late,name="glCreateShaderProgramEXT",library="Opengl32")]
def glCreateShaderProgramEXT ( _type:uint; _string:string ) : uint
	return [[uint]]

[extern(opengl,late,name="glFramebufferFetchBarrierEXT",library="Opengl32")]
def glFramebufferFetchBarrierEXT (  ) : void
	pass

[extern(opengl,late,name="glWindowRectanglesEXT",library="Opengl32")]
def glWindowRectanglesEXT ( mode:uint; count:int; box:int const? implicit ) : void
	pass

[extern(opengl,late,name="glApplyFramebufferAttachmentCMAAINTEL",library="Opengl32")]
def glApplyFramebufferAttachmentCMAAINTEL (  ) : void
	pass

[extern(opengl,late,name="glBeginPerfQueryINTEL",library="Opengl32")]
def glBeginPerfQueryINTEL ( queryHandle:uint ) : void
	pass

[extern(opengl,late,name="glCreatePerfQueryINTEL",library="Opengl32")]
def glCreatePerfQueryINTEL ( queryId:uint; queryHandle:uint? implicit ) : void
	pass

[extern(opengl,late,name="glDeletePerfQueryINTEL",library="Opengl32")]
def glDeletePerfQueryINTEL ( queryHandle:uint ) : void
	pass

[extern(opengl,late,name="glEndPerfQueryINTEL",library="Opengl32")]
def glEndPerfQueryINTEL ( queryHandle:uint ) : void
	pass

[extern(opengl,late,name="glGetFirstPerfQueryIdINTEL",library="Opengl32")]
def glGetFirstPerfQueryIdINTEL ( queryId:uint? implicit ) : void
	pass

[extern(opengl,late,name="glGetNextPerfQueryIdINTEL",library="Opengl32")]
def glGetNextPerfQueryIdINTEL ( queryId:uint; nextQueryId:uint? implicit ) : void
	pass

[extern(opengl,late,name="glGetPerfCounterInfoINTEL",library="Opengl32")]
def glGetPerfCounterInfoINTEL ( queryId:uint; counterId:uint; counterNameLength:uint; counterName:string const; counterDescLength:uint; counterDesc:string const; counterOffset:uint? implicit; counterDataSize:uint? implicit; counterTypeEnum:uint? implicit; counterDataTypeEnum:uint? implicit; rawCounterMaxValue:uint64? implicit ) : void
	pass

[extern(opengl,late,name="glGetPerfQueryDataINTEL",library="Opengl32")]
def glGetPerfQueryDataINTEL ( queryHandle:uint; flags:uint; dataSize:int; data:void? implicit; bytesWritten:uint? implicit ) : void
	pass

[extern(opengl,late,name="glGetPerfQueryIdByNameINTEL",library="Opengl32")]
def glGetPerfQueryIdByNameINTEL ( queryName:string const; queryId:uint? implicit ) : void
	pass

[extern(opengl,late,name="glGetPerfQueryInfoINTEL",library="Opengl32")]
def glGetPerfQueryInfoINTEL ( queryId:uint; queryNameLength:uint; queryName:string const; dataSize:uint? implicit; noCounters:uint? implicit; noInstances:uint? implicit; capsMask:uint? implicit ) : void
	pass

[extern(opengl,late,name="glFramebufferParameteriMESA",library="Opengl32")]
def glFramebufferParameteriMESA ( target:uint; pname:uint; param:int ) : void
	pass

[extern(opengl,late,name="glGetFramebufferParameterivMESA",library="Opengl32")]
def glGetFramebufferParameterivMESA ( target:uint; pname:uint; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glMultiDrawArraysIndirectBindlessNV",library="Opengl32")]
def glMultiDrawArraysIndirectBindlessNV ( mode:uint; indirect:void const? implicit; drawCount:int; stride:int; vertexBufferCount:int ) : void
	pass

[extern(opengl,late,name="glMultiDrawElementsIndirectBindlessNV",library="Opengl32")]
def glMultiDrawElementsIndirectBindlessNV ( mode:uint; _type:uint; indirect:void const? implicit; drawCount:int; stride:int; vertexBufferCount:int ) : void
	pass

[extern(opengl,late,name="glMultiDrawArraysIndirectBindlessCountNV",library="Opengl32")]
def glMultiDrawArraysIndirectBindlessCountNV ( mode:uint; indirect:void const? implicit; drawCount:int; maxDrawCount:int; stride:int; vertexBufferCount:int ) : void
	pass

[extern(opengl,late,name="glMultiDrawElementsIndirectBindlessCountNV",library="Opengl32")]
def glMultiDrawElementsIndirectBindlessCountNV ( mode:uint; _type:uint; indirect:void const? implicit; drawCount:int; maxDrawCount:int; stride:int; vertexBufferCount:int ) : void
	pass

[extern(opengl,late,name="glGetTextureHandleNV",library="Opengl32")]
def glGetTextureHandleNV ( texture:uint ) : uint64
	return [[uint64]]

[extern(opengl,late,name="glGetTextureSamplerHandleNV",library="Opengl32")]
def glGetTextureSamplerHandleNV ( texture:uint; sampler:uint ) : uint64
	return [[uint64]]

[extern(opengl,late,name="glMakeTextureHandleResidentNV",library="Opengl32")]
def glMakeTextureHandleResidentNV ( handle:uint64 ) : void
	pass

[extern(opengl,late,name="glMakeTextureHandleNonResidentNV",library="Opengl32")]
def glMakeTextureHandleNonResidentNV ( handle:uint64 ) : void
	pass

[extern(opengl,late,name="glGetImageHandleNV",library="Opengl32")]
def glGetImageHandleNV ( texture:uint; level:int; layered:bool; layer:int; format:uint ) : uint64
	return [[uint64]]

[extern(opengl,late,name="glMakeImageHandleResidentNV",library="Opengl32")]
def glMakeImageHandleResidentNV ( handle:uint64; access:uint ) : void
	pass

[extern(opengl,late,name="glMakeImageHandleNonResidentNV",library="Opengl32")]
def glMakeImageHandleNonResidentNV ( handle:uint64 ) : void
	pass

[extern(opengl,late,name="glUniformHandleui64NV",library="Opengl32")]
def glUniformHandleui64NV ( location:int; value:uint64 ) : void
	pass

[extern(opengl,late,name="glUniformHandleui64vNV",library="Opengl32")]
def glUniformHandleui64vNV ( location:int; count:int; value:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformHandleui64NV",library="Opengl32")]
def glProgramUniformHandleui64NV ( program:uint; location:int; value:uint64 ) : void
	pass

[extern(opengl,late,name="glProgramUniformHandleui64vNV",library="Opengl32")]
def glProgramUniformHandleui64vNV ( program:uint; location:int; count:int; values:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glIsTextureHandleResidentNV",library="Opengl32")]
def glIsTextureHandleResidentNV ( handle:uint64 ) : bool
	return [[bool]]

[extern(opengl,late,name="glIsImageHandleResidentNV",library="Opengl32")]
def glIsImageHandleResidentNV ( handle:uint64 ) : bool
	return [[bool]]

[extern(opengl,late,name="glBlendParameteriNV",library="Opengl32")]
def glBlendParameteriNV ( pname:uint; value:int ) : void
	pass

[extern(opengl,late,name="glBlendBarrierNV",library="Opengl32")]
def glBlendBarrierNV (  ) : void
	pass

[extern(opengl,late,name="glViewportPositionWScaleNV",library="Opengl32")]
def glViewportPositionWScaleNV ( index:uint; xcoeff:float; ycoeff:float ) : void
	pass

[extern(opengl,late,name="glCreateStatesNV",library="Opengl32")]
def glCreateStatesNV ( n:int; states:uint? implicit ) : void
	pass

[extern(opengl,late,name="glDeleteStatesNV",library="Opengl32")]
def glDeleteStatesNV ( n:int; states:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glIsStateNV",library="Opengl32")]
def glIsStateNV ( state:uint ) : bool
	return [[bool]]

[extern(opengl,late,name="glStateCaptureNV",library="Opengl32")]
def glStateCaptureNV ( state:uint; mode:uint ) : void
	pass

[extern(opengl,late,name="glGetCommandHeaderNV",library="Opengl32")]
def glGetCommandHeaderNV ( tokenID:uint; size:uint ) : uint
	return [[uint]]

[extern(opengl,late,name="glGetStageIndexNV",library="Opengl32")]
def glGetStageIndexNV ( shadertype:uint ) : uint16
	return [[uint16]]

[extern(opengl,late,name="glDrawCommandsNV",library="Opengl32")]
def glDrawCommandsNV ( primitiveMode:uint; buffer:uint; indirects:int64 const? implicit; sizes:int const? implicit; count:uint ) : void
	pass

[extern(opengl,late,name="glDrawCommandsAddressNV",library="Opengl32")]
def glDrawCommandsAddressNV ( primitiveMode:uint; indirects:uint64 const? implicit; sizes:int const? implicit; count:uint ) : void
	pass

[extern(opengl,late,name="glDrawCommandsStatesNV",library="Opengl32")]
def glDrawCommandsStatesNV ( buffer:uint; indirects:int64 const? implicit; sizes:int const? implicit; states:uint const? implicit; fbos:uint const? implicit; count:uint ) : void
	pass

[extern(opengl,late,name="glDrawCommandsStatesAddressNV",library="Opengl32")]
def glDrawCommandsStatesAddressNV ( indirects:uint64 const? implicit; sizes:int const? implicit; states:uint const? implicit; fbos:uint const? implicit; count:uint ) : void
	pass

[extern(opengl,late,name="glCreateCommandListsNV",library="Opengl32")]
def glCreateCommandListsNV ( n:int; lists:uint? implicit ) : void
	pass

[extern(opengl,late,name="glDeleteCommandListsNV",library="Opengl32")]
def glDeleteCommandListsNV ( n:int; lists:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glIsCommandListNV",library="Opengl32")]
def glIsCommandListNV ( list:uint ) : bool
	return [[bool]]

[extern(opengl,late,name="glListDrawCommandsStatesClientNV",library="Opengl32")]
def glListDrawCommandsStatesClientNV ( list:uint; segment:uint; indirects:void const? implicit; sizes:int const? implicit; states:uint const? implicit; fbos:uint const? implicit; count:uint ) : void
	pass

[extern(opengl,late,name="glCommandListSegmentsNV",library="Opengl32")]
def glCommandListSegmentsNV ( list:uint; segments:uint ) : void
	pass

[extern(opengl,late,name="glCompileCommandListNV",library="Opengl32")]
def glCompileCommandListNV ( list:uint ) : void
	pass

[extern(opengl,late,name="glCallCommandListNV",library="Opengl32")]
def glCallCommandListNV ( list:uint ) : void
	pass

[extern(opengl,late,name="glBeginConditionalRenderNV",library="Opengl32")]
def glBeginConditionalRenderNV ( id:uint; mode:uint ) : void
	pass

[extern(opengl,late,name="glEndConditionalRenderNV",library="Opengl32")]
def glEndConditionalRenderNV (  ) : void
	pass

[extern(opengl,late,name="glSubpixelPrecisionBiasNV",library="Opengl32")]
def glSubpixelPrecisionBiasNV ( xbits:uint; ybits:uint ) : void
	pass

[extern(opengl,late,name="glConservativeRasterParameterfNV",library="Opengl32")]
def glConservativeRasterParameterfNV ( pname:uint; value:float ) : void
	pass

[extern(opengl,late,name="glConservativeRasterParameteriNV",library="Opengl32")]
def glConservativeRasterParameteriNV ( pname:uint; param:int ) : void
	pass

[extern(opengl,late,name="glDepthRangedNV",library="Opengl32")]
def glDepthRangedNV ( zNear:double; zFar:double ) : void
	pass

[extern(opengl,late,name="glClearDepthdNV",library="Opengl32")]
def glClearDepthdNV ( depth:double ) : void
	pass

[extern(opengl,late,name="glDepthBoundsdNV",library="Opengl32")]
def glDepthBoundsdNV ( zmin:double; zmax:double ) : void
	pass

[extern(opengl,late,name="glDrawVkImageNV",library="Opengl32")]
def glDrawVkImageNV ( vkImage:uint64; sampler:uint; x0:float; y0:float; x1:float; y1:float; z:float; s0:float; t0:float; s1:float; t1:float ) : void
	pass

[extern(opengl,late,name="glWaitVkSemaphoreNV",library="Opengl32")]
def glWaitVkSemaphoreNV ( vkSemaphore:uint64 ) : void
	pass

[extern(opengl,late,name="glSignalVkSemaphoreNV",library="Opengl32")]
def glSignalVkSemaphoreNV ( vkSemaphore:uint64 ) : void
	pass

[extern(opengl,late,name="glSignalVkFenceNV",library="Opengl32")]
def glSignalVkFenceNV ( vkFence:uint64 ) : void
	pass

[extern(opengl,late,name="glFragmentCoverageColorNV",library="Opengl32")]
def glFragmentCoverageColorNV ( color:uint ) : void
	pass

[extern(opengl,late,name="glCoverageModulationTableNV",library="Opengl32")]
def glCoverageModulationTableNV ( n:int; v:float const? implicit ) : void
	pass

[extern(opengl,late,name="glGetCoverageModulationTableNV",library="Opengl32")]
def glGetCoverageModulationTableNV ( bufSize:int; v:float? implicit ) : void
	pass

[extern(opengl,late,name="glCoverageModulationNV",library="Opengl32")]
def glCoverageModulationNV ( components:uint ) : void
	pass

[extern(opengl,late,name="glRenderbufferStorageMultisampleCoverageNV",library="Opengl32")]
def glRenderbufferStorageMultisampleCoverageNV ( target:uint; coverageSamples:int; colorSamples:int; internalformat:uint; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glUniform1i64NV",library="Opengl32")]
def glUniform1i64NV ( location:int; x:int64 ) : void
	pass

[extern(opengl,late,name="glUniform2i64NV",library="Opengl32")]
def glUniform2i64NV ( location:int; x:int64; y:int64 ) : void
	pass

[extern(opengl,late,name="glUniform3i64NV",library="Opengl32")]
def glUniform3i64NV ( location:int; x:int64; y:int64; z:int64 ) : void
	pass

[extern(opengl,late,name="glUniform4i64NV",library="Opengl32")]
def glUniform4i64NV ( location:int; x:int64; y:int64; z:int64; w:int64 ) : void
	pass

[extern(opengl,late,name="glUniform1i64vNV",library="Opengl32")]
def glUniform1i64vNV ( location:int; count:int; value:int64 const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform2i64vNV",library="Opengl32")]
def glUniform2i64vNV ( location:int; count:int; value:int64 const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform3i64vNV",library="Opengl32")]
def glUniform3i64vNV ( location:int; count:int; value:int64 const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform4i64vNV",library="Opengl32")]
def glUniform4i64vNV ( location:int; count:int; value:int64 const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform1ui64NV",library="Opengl32")]
def glUniform1ui64NV ( location:int; x:uint64 ) : void
	pass

[extern(opengl,late,name="glUniform2ui64NV",library="Opengl32")]
def glUniform2ui64NV ( location:int; x:uint64; y:uint64 ) : void
	pass

[extern(opengl,late,name="glUniform3ui64NV",library="Opengl32")]
def glUniform3ui64NV ( location:int; x:uint64; y:uint64; z:uint64 ) : void
	pass

[extern(opengl,late,name="glUniform4ui64NV",library="Opengl32")]
def glUniform4ui64NV ( location:int; x:uint64; y:uint64; z:uint64; w:uint64 ) : void
	pass

[extern(opengl,late,name="glUniform1ui64vNV",library="Opengl32")]
def glUniform1ui64vNV ( location:int; count:int; value:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform2ui64vNV",library="Opengl32")]
def glUniform2ui64vNV ( location:int; count:int; value:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform3ui64vNV",library="Opengl32")]
def glUniform3ui64vNV ( location:int; count:int; value:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glUniform4ui64vNV",library="Opengl32")]
def glUniform4ui64vNV ( location:int; count:int; value:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glGetUniformi64vNV",library="Opengl32")]
def glGetUniformi64vNV ( program:uint; location:int; params:int64? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform1i64NV",library="Opengl32")]
def glProgramUniform1i64NV ( program:uint; location:int; x:int64 ) : void
	pass

[extern(opengl,late,name="glProgramUniform2i64NV",library="Opengl32")]
def glProgramUniform2i64NV ( program:uint; location:int; x:int64; y:int64 ) : void
	pass

[extern(opengl,late,name="glProgramUniform3i64NV",library="Opengl32")]
def glProgramUniform3i64NV ( program:uint; location:int; x:int64; y:int64; z:int64 ) : void
	pass

[extern(opengl,late,name="glProgramUniform4i64NV",library="Opengl32")]
def glProgramUniform4i64NV ( program:uint; location:int; x:int64; y:int64; z:int64; w:int64 ) : void
	pass

[extern(opengl,late,name="glProgramUniform1i64vNV",library="Opengl32")]
def glProgramUniform1i64vNV ( program:uint; location:int; count:int; value:int64 const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform2i64vNV",library="Opengl32")]
def glProgramUniform2i64vNV ( program:uint; location:int; count:int; value:int64 const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform3i64vNV",library="Opengl32")]
def glProgramUniform3i64vNV ( program:uint; location:int; count:int; value:int64 const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform4i64vNV",library="Opengl32")]
def glProgramUniform4i64vNV ( program:uint; location:int; count:int; value:int64 const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform1ui64NV",library="Opengl32")]
def glProgramUniform1ui64NV ( program:uint; location:int; x:uint64 ) : void
	pass

[extern(opengl,late,name="glProgramUniform2ui64NV",library="Opengl32")]
def glProgramUniform2ui64NV ( program:uint; location:int; x:uint64; y:uint64 ) : void
	pass

[extern(opengl,late,name="glProgramUniform3ui64NV",library="Opengl32")]
def glProgramUniform3ui64NV ( program:uint; location:int; x:uint64; y:uint64; z:uint64 ) : void
	pass

[extern(opengl,late,name="glProgramUniform4ui64NV",library="Opengl32")]
def glProgramUniform4ui64NV ( program:uint; location:int; x:uint64; y:uint64; z:uint64; w:uint64 ) : void
	pass

[extern(opengl,late,name="glProgramUniform1ui64vNV",library="Opengl32")]
def glProgramUniform1ui64vNV ( program:uint; location:int; count:int; value:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform2ui64vNV",library="Opengl32")]
def glProgramUniform2ui64vNV ( program:uint; location:int; count:int; value:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform3ui64vNV",library="Opengl32")]
def glProgramUniform3ui64vNV ( program:uint; location:int; count:int; value:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniform4ui64vNV",library="Opengl32")]
def glProgramUniform4ui64vNV ( program:uint; location:int; count:int; value:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glGetInternalformatSampleivNV",library="Opengl32")]
def glGetInternalformatSampleivNV ( target:uint; internalformat:uint; samples:int; pname:uint; count:int; params:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetMemoryObjectDetachedResourcesuivNV",library="Opengl32")]
def glGetMemoryObjectDetachedResourcesuivNV ( memory:uint; pname:uint; first:int; count:int; params:uint? implicit ) : void
	pass

[extern(opengl,late,name="glResetMemoryObjectParameterNV",library="Opengl32")]
def glResetMemoryObjectParameterNV ( memory:uint; pname:uint ) : void
	pass

[extern(opengl,late,name="glTexAttachMemoryNV",library="Opengl32")]
def glTexAttachMemoryNV ( target:uint; memory:uint; offset:uint64 ) : void
	pass

[extern(opengl,late,name="glBufferAttachMemoryNV",library="Opengl32")]
def glBufferAttachMemoryNV ( target:uint; memory:uint; offset:uint64 ) : void
	pass

[extern(opengl,late,name="glTextureAttachMemoryNV",library="Opengl32")]
def glTextureAttachMemoryNV ( texture:uint; memory:uint; offset:uint64 ) : void
	pass

[extern(opengl,late,name="glNamedBufferAttachMemoryNV",library="Opengl32")]
def glNamedBufferAttachMemoryNV ( buffer:uint; memory:uint; offset:uint64 ) : void
	pass

[extern(opengl,late,name="glBufferPageCommitmentMemNV",library="Opengl32")]
def glBufferPageCommitmentMemNV ( target:uint; offset:int64; size:int64; memory:uint; memOffset:uint64; commit:bool ) : void
	pass

[extern(opengl,late,name="glTexPageCommitmentMemNV",library="Opengl32")]
def glTexPageCommitmentMemNV ( target:uint; layer:int; level:int; xoffset:int; yoffset:int; zoffset:int; width:int; height:int; depth:int; memory:uint; offset:uint64; commit:bool ) : void
	pass

[extern(opengl,late,name="glNamedBufferPageCommitmentMemNV",library="Opengl32")]
def glNamedBufferPageCommitmentMemNV ( buffer:uint; offset:int64; size:int64; memory:uint; memOffset:uint64; commit:bool ) : void
	pass

[extern(opengl,late,name="glTexturePageCommitmentMemNV",library="Opengl32")]
def glTexturePageCommitmentMemNV ( texture:uint; layer:int; level:int; xoffset:int; yoffset:int; zoffset:int; width:int; height:int; depth:int; memory:uint; offset:uint64; commit:bool ) : void
	pass

[extern(opengl,late,name="glDrawMeshTasksNV",library="Opengl32")]
def glDrawMeshTasksNV ( first:uint; count:uint ) : void
	pass

[extern(opengl,late,name="glDrawMeshTasksIndirectNV",library="Opengl32")]
def glDrawMeshTasksIndirectNV ( indirect:int64 ) : void
	pass

[extern(opengl,late,name="glMultiDrawMeshTasksIndirectNV",library="Opengl32")]
def glMultiDrawMeshTasksIndirectNV ( indirect:int64; drawcount:int; stride:int ) : void
	pass

[extern(opengl,late,name="glMultiDrawMeshTasksIndirectCountNV",library="Opengl32")]
def glMultiDrawMeshTasksIndirectCountNV ( indirect:int64; drawcount:int64; maxdrawcount:int; stride:int ) : void
	pass

[extern(opengl,late,name="glGenPathsNV",library="Opengl32")]
def glGenPathsNV ( _range:int ) : uint
	return [[uint]]

[extern(opengl,late,name="glDeletePathsNV",library="Opengl32")]
def glDeletePathsNV ( path:uint; _range:int ) : void
	pass

[extern(opengl,late,name="glIsPathNV",library="Opengl32")]
def glIsPathNV ( path:uint ) : bool
	return [[bool]]

[extern(opengl,late,name="glPathCommandsNV",library="Opengl32")]
def glPathCommandsNV ( path:uint; numCommands:int; commands:uint8 const? implicit; numCoords:int; coordType:uint; coords:void const? implicit ) : void
	pass

[extern(opengl,late,name="glPathCoordsNV",library="Opengl32")]
def glPathCoordsNV ( path:uint; numCoords:int; coordType:uint; coords:void const? implicit ) : void
	pass

[extern(opengl,late,name="glPathSubCommandsNV",library="Opengl32")]
def glPathSubCommandsNV ( path:uint; commandStart:int; commandsToDelete:int; numCommands:int; commands:uint8 const? implicit; numCoords:int; coordType:uint; coords:void const? implicit ) : void
	pass

[extern(opengl,late,name="glPathSubCoordsNV",library="Opengl32")]
def glPathSubCoordsNV ( path:uint; coordStart:int; numCoords:int; coordType:uint; coords:void const? implicit ) : void
	pass

[extern(opengl,late,name="glPathStringNV",library="Opengl32")]
def glPathStringNV ( path:uint; format:uint; length:int; pathString:void const? implicit ) : void
	pass

[extern(opengl,late,name="glPathGlyphsNV",library="Opengl32")]
def glPathGlyphsNV ( firstPathName:uint; fontTarget:uint; fontName:void const? implicit; fontStyle:uint; numGlyphs:int; _type:uint; charcodes:void const? implicit; handleMissingGlyphs:uint; pathParameterTemplate:uint; emScale:float ) : void
	pass

[extern(opengl,late,name="glPathGlyphRangeNV",library="Opengl32")]
def glPathGlyphRangeNV ( firstPathName:uint; fontTarget:uint; fontName:void const? implicit; fontStyle:uint; firstGlyph:uint; numGlyphs:int; handleMissingGlyphs:uint; pathParameterTemplate:uint; emScale:float ) : void
	pass

[extern(opengl,late,name="glWeightPathsNV",library="Opengl32")]
def glWeightPathsNV ( resultPath:uint; numPaths:int; paths:uint const? implicit; weights:float const? implicit ) : void
	pass

[extern(opengl,late,name="glCopyPathNV",library="Opengl32")]
def glCopyPathNV ( resultPath:uint; srcPath:uint ) : void
	pass

[extern(opengl,late,name="glInterpolatePathsNV",library="Opengl32")]
def glInterpolatePathsNV ( resultPath:uint; pathA:uint; pathB:uint; weight:float ) : void
	pass

[extern(opengl,late,name="glTransformPathNV",library="Opengl32")]
def glTransformPathNV ( resultPath:uint; srcPath:uint; transformType:uint; transformValues:float const? implicit ) : void
	pass

[extern(opengl,late,name="glPathParameterivNV",library="Opengl32")]
def glPathParameterivNV ( path:uint; pname:uint; value:int const? implicit ) : void
	pass

[extern(opengl,late,name="glPathParameteriNV",library="Opengl32")]
def glPathParameteriNV ( path:uint; pname:uint; value:int ) : void
	pass

[extern(opengl,late,name="glPathParameterfvNV",library="Opengl32")]
def glPathParameterfvNV ( path:uint; pname:uint; value:float const? implicit ) : void
	pass

[extern(opengl,late,name="glPathParameterfNV",library="Opengl32")]
def glPathParameterfNV ( path:uint; pname:uint; value:float ) : void
	pass

[extern(opengl,late,name="glPathDashArrayNV",library="Opengl32")]
def glPathDashArrayNV ( path:uint; dashCount:int; dashArray:float const? implicit ) : void
	pass

[extern(opengl,late,name="glPathStencilFuncNV",library="Opengl32")]
def glPathStencilFuncNV ( func:uint; ref:int; mask:uint ) : void
	pass

[extern(opengl,late,name="glPathStencilDepthOffsetNV",library="Opengl32")]
def glPathStencilDepthOffsetNV ( factor:float; units:float ) : void
	pass

[extern(opengl,late,name="glStencilFillPathNV",library="Opengl32")]
def glStencilFillPathNV ( path:uint; fillMode:uint; mask:uint ) : void
	pass

[extern(opengl,late,name="glStencilStrokePathNV",library="Opengl32")]
def glStencilStrokePathNV ( path:uint; reference:int; mask:uint ) : void
	pass

[extern(opengl,late,name="glStencilFillPathInstancedNV",library="Opengl32")]
def glStencilFillPathInstancedNV ( numPaths:int; pathNameType:uint; paths:void const? implicit; pathBase:uint; fillMode:uint; mask:uint; transformType:uint; transformValues:float const? implicit ) : void
	pass

[extern(opengl,late,name="glStencilStrokePathInstancedNV",library="Opengl32")]
def glStencilStrokePathInstancedNV ( numPaths:int; pathNameType:uint; paths:void const? implicit; pathBase:uint; reference:int; mask:uint; transformType:uint; transformValues:float const? implicit ) : void
	pass

[extern(opengl,late,name="glPathCoverDepthFuncNV",library="Opengl32")]
def glPathCoverDepthFuncNV ( func:uint ) : void
	pass

[extern(opengl,late,name="glCoverFillPathNV",library="Opengl32")]
def glCoverFillPathNV ( path:uint; coverMode:uint ) : void
	pass

[extern(opengl,late,name="glCoverStrokePathNV",library="Opengl32")]
def glCoverStrokePathNV ( path:uint; coverMode:uint ) : void
	pass

[extern(opengl,late,name="glCoverFillPathInstancedNV",library="Opengl32")]
def glCoverFillPathInstancedNV ( numPaths:int; pathNameType:uint; paths:void const? implicit; pathBase:uint; coverMode:uint; transformType:uint; transformValues:float const? implicit ) : void
	pass

[extern(opengl,late,name="glCoverStrokePathInstancedNV",library="Opengl32")]
def glCoverStrokePathInstancedNV ( numPaths:int; pathNameType:uint; paths:void const? implicit; pathBase:uint; coverMode:uint; transformType:uint; transformValues:float const? implicit ) : void
	pass

[extern(opengl,late,name="glGetPathParameterivNV",library="Opengl32")]
def glGetPathParameterivNV ( path:uint; pname:uint; value:int? implicit ) : void
	pass

[extern(opengl,late,name="glGetPathParameterfvNV",library="Opengl32")]
def glGetPathParameterfvNV ( path:uint; pname:uint; value:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetPathCommandsNV",library="Opengl32")]
def glGetPathCommandsNV ( path:uint; commands:uint8? implicit ) : void
	pass

[extern(opengl,late,name="glGetPathCoordsNV",library="Opengl32")]
def glGetPathCoordsNV ( path:uint; coords:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetPathDashArrayNV",library="Opengl32")]
def glGetPathDashArrayNV ( path:uint; dashArray:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetPathMetricsNV",library="Opengl32")]
def glGetPathMetricsNV ( metricQueryMask:uint; numPaths:int; pathNameType:uint; paths:void const? implicit; pathBase:uint; stride:int; metrics:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetPathMetricRangeNV",library="Opengl32")]
def glGetPathMetricRangeNV ( metricQueryMask:uint; firstPathName:uint; numPaths:int; stride:int; metrics:float? implicit ) : void
	pass

[extern(opengl,late,name="glGetPathSpacingNV",library="Opengl32")]
def glGetPathSpacingNV ( pathListMode:uint; numPaths:int; pathNameType:uint; paths:void const? implicit; pathBase:uint; advanceScale:float; kerningScale:float; transformType:uint; returnedSpacing:float? implicit ) : void
	pass

[extern(opengl,late,name="glIsPointInFillPathNV",library="Opengl32")]
def glIsPointInFillPathNV ( path:uint; mask:uint; x:float; y:float ) : bool
	return [[bool]]

[extern(opengl,late,name="glIsPointInStrokePathNV",library="Opengl32")]
def glIsPointInStrokePathNV ( path:uint; x:float; y:float ) : bool
	return [[bool]]

[extern(opengl,late,name="glGetPathLengthNV",library="Opengl32")]
def glGetPathLengthNV ( path:uint; startSegment:int; numSegments:int ) : float
	return [[float]]

[extern(opengl,late,name="glPointAlongPathNV",library="Opengl32")]
def glPointAlongPathNV ( path:uint; startSegment:int; numSegments:int; distance:float; x:float? implicit; y:float? implicit; tangentX:float? implicit; tangentY:float? implicit ) : bool
	return [[bool]]

[extern(opengl,late,name="glMatrixLoad3x2fNV",library="Opengl32")]
def glMatrixLoad3x2fNV ( matrixMode:uint; m:float const? implicit ) : void
	pass

[extern(opengl,late,name="glMatrixLoad3x3fNV",library="Opengl32")]
def glMatrixLoad3x3fNV ( matrixMode:uint; m:float const? implicit ) : void
	pass

[extern(opengl,late,name="glMatrixLoadTranspose3x3fNV",library="Opengl32")]
def glMatrixLoadTranspose3x3fNV ( matrixMode:uint; m:float const? implicit ) : void
	pass

[extern(opengl,late,name="glMatrixMult3x2fNV",library="Opengl32")]
def glMatrixMult3x2fNV ( matrixMode:uint; m:float const? implicit ) : void
	pass

[extern(opengl,late,name="glMatrixMult3x3fNV",library="Opengl32")]
def glMatrixMult3x3fNV ( matrixMode:uint; m:float const? implicit ) : void
	pass

[extern(opengl,late,name="glMatrixMultTranspose3x3fNV",library="Opengl32")]
def glMatrixMultTranspose3x3fNV ( matrixMode:uint; m:float const? implicit ) : void
	pass

[extern(opengl,late,name="glStencilThenCoverFillPathNV",library="Opengl32")]
def glStencilThenCoverFillPathNV ( path:uint; fillMode:uint; mask:uint; coverMode:uint ) : void
	pass

[extern(opengl,late,name="glStencilThenCoverStrokePathNV",library="Opengl32")]
def glStencilThenCoverStrokePathNV ( path:uint; reference:int; mask:uint; coverMode:uint ) : void
	pass

[extern(opengl,late,name="glStencilThenCoverFillPathInstancedNV",library="Opengl32")]
def glStencilThenCoverFillPathInstancedNV ( numPaths:int; pathNameType:uint; paths:void const? implicit; pathBase:uint; fillMode:uint; mask:uint; coverMode:uint; transformType:uint; transformValues:float const? implicit ) : void
	pass

[extern(opengl,late,name="glStencilThenCoverStrokePathInstancedNV",library="Opengl32")]
def glStencilThenCoverStrokePathInstancedNV ( numPaths:int; pathNameType:uint; paths:void const? implicit; pathBase:uint; reference:int; mask:uint; coverMode:uint; transformType:uint; transformValues:float const? implicit ) : void
	pass

[extern(opengl,late,name="glPathGlyphIndexRangeNV",library="Opengl32")]
def glPathGlyphIndexRangeNV ( fontTarget:uint; fontName:void const? implicit; fontStyle:uint; pathParameterTemplate:uint; emScale:float; baseAndCount:uint? implicit ) : uint
	return [[uint]]

[extern(opengl,late,name="glPathGlyphIndexArrayNV",library="Opengl32")]
def glPathGlyphIndexArrayNV ( firstPathName:uint; fontTarget:uint; fontName:void const? implicit; fontStyle:uint; firstGlyphIndex:uint; numGlyphs:int; pathParameterTemplate:uint; emScale:float ) : uint
	return [[uint]]

[extern(opengl,late,name="glPathMemoryGlyphIndexArrayNV",library="Opengl32")]
def glPathMemoryGlyphIndexArrayNV ( firstPathName:uint; fontTarget:uint; fontSize:int64; fontData:void const? implicit; faceIndex:int; firstGlyphIndex:uint; numGlyphs:int; pathParameterTemplate:uint; emScale:float ) : uint
	return [[uint]]

[extern(opengl,late,name="glProgramPathFragmentInputGenNV",library="Opengl32")]
def glProgramPathFragmentInputGenNV ( program:uint; location:int; genMode:uint; components:int; coeffs:float const? implicit ) : void
	pass

[extern(opengl,late,name="glGetProgramResourcefvNV",library="Opengl32")]
def glGetProgramResourcefvNV ( program:uint; programInterface:uint; index:uint; propCount:int; props:uint const? implicit; count:int; length:int? implicit; params:float? implicit ) : void
	pass

[extern(opengl,late,name="glFramebufferSampleLocationsfvNV",library="Opengl32")]
def glFramebufferSampleLocationsfvNV ( target:uint; start:uint; count:int; v:float const? implicit ) : void
	pass

[extern(opengl,late,name="glNamedFramebufferSampleLocationsfvNV",library="Opengl32")]
def glNamedFramebufferSampleLocationsfvNV ( framebuffer:uint; start:uint; count:int; v:float const? implicit ) : void
	pass

[extern(opengl,late,name="glResolveDepthValuesNV",library="Opengl32")]
def glResolveDepthValuesNV (  ) : void
	pass

[extern(opengl,late,name="glScissorExclusiveNV",library="Opengl32")]
def glScissorExclusiveNV ( x:int; y:int; width:int; height:int ) : void
	pass

[extern(opengl,late,name="glScissorExclusiveArrayvNV",library="Opengl32")]
def glScissorExclusiveArrayvNV ( first:uint; count:int; v:int const? implicit ) : void
	pass

[extern(opengl,late,name="glMakeBufferResidentNV",library="Opengl32")]
def glMakeBufferResidentNV ( target:uint; access:uint ) : void
	pass

[extern(opengl,late,name="glMakeBufferNonResidentNV",library="Opengl32")]
def glMakeBufferNonResidentNV ( target:uint ) : void
	pass

[extern(opengl,late,name="glIsBufferResidentNV",library="Opengl32")]
def glIsBufferResidentNV ( target:uint ) : bool
	return [[bool]]

[extern(opengl,late,name="glMakeNamedBufferResidentNV",library="Opengl32")]
def glMakeNamedBufferResidentNV ( buffer:uint; access:uint ) : void
	pass

[extern(opengl,late,name="glMakeNamedBufferNonResidentNV",library="Opengl32")]
def glMakeNamedBufferNonResidentNV ( buffer:uint ) : void
	pass

[extern(opengl,late,name="glIsNamedBufferResidentNV",library="Opengl32")]
def glIsNamedBufferResidentNV ( buffer:uint ) : bool
	return [[bool]]

[extern(opengl,late,name="glGetBufferParameterui64vNV",library="Opengl32")]
def glGetBufferParameterui64vNV ( target:uint; pname:uint; params:uint64? implicit ) : void
	pass

[extern(opengl,late,name="glGetNamedBufferParameterui64vNV",library="Opengl32")]
def glGetNamedBufferParameterui64vNV ( buffer:uint; pname:uint; params:uint64? implicit ) : void
	pass

[extern(opengl,late,name="glGetIntegerui64vNV",library="Opengl32")]
def glGetIntegerui64vNV ( value:uint; result:uint64? implicit ) : void
	pass

[extern(opengl,late,name="glUniformui64NV",library="Opengl32")]
def glUniformui64NV ( location:int; value:uint64 ) : void
	pass

[extern(opengl,late,name="glUniformui64vNV",library="Opengl32")]
def glUniformui64vNV ( location:int; count:int; value:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glGetUniformui64vNV",library="Opengl32")]
def glGetUniformui64vNV ( program:uint; location:int; params:uint64? implicit ) : void
	pass

[extern(opengl,late,name="glProgramUniformui64NV",library="Opengl32")]
def glProgramUniformui64NV ( program:uint; location:int; value:uint64 ) : void
	pass

[extern(opengl,late,name="glProgramUniformui64vNV",library="Opengl32")]
def glProgramUniformui64vNV ( program:uint; location:int; count:int; value:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glBindShadingRateImageNV",library="Opengl32")]
def glBindShadingRateImageNV ( texture:uint ) : void
	pass

[extern(opengl,late,name="glGetShadingRateImagePaletteNV",library="Opengl32")]
def glGetShadingRateImagePaletteNV ( viewport:uint; entry:uint; rate:uint? implicit ) : void
	pass

[extern(opengl,late,name="glGetShadingRateSampleLocationivNV",library="Opengl32")]
def glGetShadingRateSampleLocationivNV ( rate:uint; samples:uint; index:uint; location:int? implicit ) : void
	pass

[extern(opengl,late,name="glShadingRateImageBarrierNV",library="Opengl32")]
def glShadingRateImageBarrierNV ( synchronize:bool ) : void
	pass

[extern(opengl,late,name="glShadingRateImagePaletteNV",library="Opengl32")]
def glShadingRateImagePaletteNV ( viewport:uint; first:uint; count:int; rates:uint const? implicit ) : void
	pass

[extern(opengl,late,name="glShadingRateSampleOrderNV",library="Opengl32")]
def glShadingRateSampleOrderNV ( order:uint ) : void
	pass

[extern(opengl,late,name="glShadingRateSampleOrderCustomNV",library="Opengl32")]
def glShadingRateSampleOrderCustomNV ( rate:uint; samples:uint; locations:int const? implicit ) : void
	pass

[extern(opengl,late,name="glTextureBarrierNV",library="Opengl32")]
def glTextureBarrierNV (  ) : void
	pass

[extern(opengl,late,name="glVertexAttribL1i64NV",library="Opengl32")]
def glVertexAttribL1i64NV ( index:uint; x:int64 ) : void
	pass

[extern(opengl,late,name="glVertexAttribL2i64NV",library="Opengl32")]
def glVertexAttribL2i64NV ( index:uint; x:int64; y:int64 ) : void
	pass

[extern(opengl,late,name="glVertexAttribL3i64NV",library="Opengl32")]
def glVertexAttribL3i64NV ( index:uint; x:int64; y:int64; z:int64 ) : void
	pass

[extern(opengl,late,name="glVertexAttribL4i64NV",library="Opengl32")]
def glVertexAttribL4i64NV ( index:uint; x:int64; y:int64; z:int64; w:int64 ) : void
	pass

[extern(opengl,late,name="glVertexAttribL1i64vNV",library="Opengl32")]
def glVertexAttribL1i64vNV ( index:uint; v:int64 const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribL2i64vNV",library="Opengl32")]
def glVertexAttribL2i64vNV ( index:uint; v:int64 const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribL3i64vNV",library="Opengl32")]
def glVertexAttribL3i64vNV ( index:uint; v:int64 const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribL4i64vNV",library="Opengl32")]
def glVertexAttribL4i64vNV ( index:uint; v:int64 const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribL1ui64NV",library="Opengl32")]
def glVertexAttribL1ui64NV ( index:uint; x:uint64 ) : void
	pass

[extern(opengl,late,name="glVertexAttribL2ui64NV",library="Opengl32")]
def glVertexAttribL2ui64NV ( index:uint; x:uint64; y:uint64 ) : void
	pass

[extern(opengl,late,name="glVertexAttribL3ui64NV",library="Opengl32")]
def glVertexAttribL3ui64NV ( index:uint; x:uint64; y:uint64; z:uint64 ) : void
	pass

[extern(opengl,late,name="glVertexAttribL4ui64NV",library="Opengl32")]
def glVertexAttribL4ui64NV ( index:uint; x:uint64; y:uint64; z:uint64; w:uint64 ) : void
	pass

[extern(opengl,late,name="glVertexAttribL1ui64vNV",library="Opengl32")]
def glVertexAttribL1ui64vNV ( index:uint; v:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribL2ui64vNV",library="Opengl32")]
def glVertexAttribL2ui64vNV ( index:uint; v:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribL3ui64vNV",library="Opengl32")]
def glVertexAttribL3ui64vNV ( index:uint; v:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribL4ui64vNV",library="Opengl32")]
def glVertexAttribL4ui64vNV ( index:uint; v:uint64 const? implicit ) : void
	pass

[extern(opengl,late,name="glGetVertexAttribLi64vNV",library="Opengl32")]
def glGetVertexAttribLi64vNV ( index:uint; pname:uint; params:int64? implicit ) : void
	pass

[extern(opengl,late,name="glGetVertexAttribLui64vNV",library="Opengl32")]
def glGetVertexAttribLui64vNV ( index:uint; pname:uint; params:uint64? implicit ) : void
	pass

[extern(opengl,late,name="glVertexAttribLFormatNV",library="Opengl32")]
def glVertexAttribLFormatNV ( index:uint; size:int; _type:uint; stride:int ) : void
	pass

[extern(opengl,late,name="glBufferAddressRangeNV",library="Opengl32")]
def glBufferAddressRangeNV ( pname:uint; index:uint; address:uint64; length:int64 ) : void
	pass

[extern(opengl,late,name="glVertexFormatNV",library="Opengl32")]
def glVertexFormatNV ( size:int; _type:uint; stride:int ) : void
	pass

[extern(opengl,late,name="glNormalFormatNV",library="Opengl32")]
def glNormalFormatNV ( _type:uint; stride:int ) : void
	pass

[extern(opengl,late,name="glColorFormatNV",library="Opengl32")]
def glColorFormatNV ( size:int; _type:uint; stride:int ) : void
	pass

[extern(opengl,late,name="glIndexFormatNV",library="Opengl32")]
def glIndexFormatNV ( _type:uint; stride:int ) : void
	pass

[extern(opengl,late,name="glTexCoordFormatNV",library="Opengl32")]
def glTexCoordFormatNV ( size:int; _type:uint; stride:int ) : void
	pass

[extern(opengl,late,name="glEdgeFlagFormatNV",library="Opengl32")]
def glEdgeFlagFormatNV ( stride:int ) : void
	pass

[extern(opengl,late,name="glSecondaryColorFormatNV",library="Opengl32")]
def glSecondaryColorFormatNV ( size:int; _type:uint; stride:int ) : void
	pass

[extern(opengl,late,name="glFogCoordFormatNV",library="Opengl32")]
def glFogCoordFormatNV ( _type:uint; stride:int ) : void
	pass

[extern(opengl,late,name="glVertexAttribFormatNV",library="Opengl32")]
def glVertexAttribFormatNV ( index:uint; size:int; _type:uint; normalized:bool; stride:int ) : void
	pass

[extern(opengl,late,name="glVertexAttribIFormatNV",library="Opengl32")]
def glVertexAttribIFormatNV ( index:uint; size:int; _type:uint; stride:int ) : void
	pass

[extern(opengl,late,name="glGetIntegerui64i_vNV",library="Opengl32")]
def glGetIntegerui64i_vNV ( value:uint; index:uint; result:uint64? implicit ) : void
	pass

[extern(opengl,late,name="glViewportSwizzleNV",library="Opengl32")]
def glViewportSwizzleNV ( index:uint; swizzlex:uint; swizzley:uint; swizzlez:uint; swizzlew:uint ) : void
	pass

[extern(opengl,late,name="glFramebufferTextureMultiviewOVR",library="Opengl32")]
def glFramebufferTextureMultiviewOVR ( target:uint; attachment:uint; texture:uint; level:int; baseViewIndex:int; numViews:int ) : void
	pass

