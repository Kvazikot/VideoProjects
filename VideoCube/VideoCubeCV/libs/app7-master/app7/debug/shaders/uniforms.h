#version 430

//----------------------------------------------------------------------------------
// File:        es3aep-kepler/TerrainTessellation/assets/shaders/uniforms.h
// SDK Version: v2.11 
// Email:       gameworks@nvidia.com
// Site:        http://developer.nvidia.com/
//
// Copyright (c) 2014-2015, NVIDIA CORPORATION. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//----------------------------------------------------------------------------------

#ifndef __cplusplus
//precision mediump float;

#define sdk_bool bool
// Standard SDK defines
#define SDK_BOOL  bool
#define SDK_VEC2  vec2
#define SDK_VEC3  vec3
#define SDK_VEC4  vec4
#define SDK_MAT4  mat4

uniform vec4 eyePosWorld;
uniform float invNoise3DSize;
uniform mat4 InvProjection;
uniform mat4 InvView;
uniform vec3 lightDirWorld;
//uniform vec3 lightDir;
uniform float time;
uniform vec2 translate;
uniform float m_metersToUnits;


layout(std140, binding=1) uniform
#else
struct
#endif
    
TessellationParams
{
    //float invNoise3DSize;
    //SDK_MAT4 InvProjection;
    //SDK_MAT4 InvView;
    //SDK_VEC3 lightDirWorld;
    //SDK_VEC4 eyePosWorld;
    //float time;
    SDK_VEC2 translate1;
    
#ifdef CPP
    TessellationParams() :
        innerTessFactor(32.0f),
        outerTessFactor(32.0f), 
        noiseFreq(0.25f), 
        heightScale(1.5f),
        noiseOctaves(8),
        triSize(5.0f),
        cull(true),
        lod(true),
        lightDir(0.0f, 0.0f, -1.0f),
        time(0.0f),
        gridOrigin(-8.0f, 0.0f, -8.0f),
        gridW(16), gridH(16),
        tileSize(1.0f, 0.0f, 1.0f)
        {
            nv::vec3f halfTileSize = nv::vec3f(tileSize.x, heightScale, tileSize.z)*0.5f;
            tileBoundingSphereR = length(halfTileSize);
        }
#endif
};
