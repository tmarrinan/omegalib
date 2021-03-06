/******************************************************************************
* THE OMEGA LIB PROJECT
*-----------------------------------------------------------------------------
* Copyright 2010-2015		Electronic Visualization Laboratory,
*							University of Illinois at Chicago
* Authors:
*  Alessandro Febretti		febret@gmail.com
*-----------------------------------------------------------------------------
* Copyright (c) 2010-2015, Electronic Visualization Laboratory,
* University of Illinois at Chicago
* All rights reserved.
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer. Redistributions in binary
* form must reproduce the above copyright notice, this list of conditions and
* the following disclaimer in the documentation and/or other materials provided
* with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*-----------------------------------------------------------------------------
* What's in this file
*	Interface to code running on GPUs
******************************************************************************/
#ifndef __GPU_PROGRAM__
#define __GPU_PROGRAM__

#include "GpuResource.h"
#include "GpuBuffer.h"
#include "Texture.h"

namespace omega
{
    ///////////////////////////////////////////////////////////////////////////
    class OMEGA_API GpuProgram : public GpuResource
    {
        friend class GpuContext;
    public:
        enum ShaderType
        {
            VertexShader,
            FragmentShader,
            GeometryShader,
            ShaderTypes
        };

    public:
        virtual void dispose();

        bool build();

        bool setShader(ShaderType type, const String& filename);
        void setShaderSource(ShaderType type, const String& source);

        unsigned int getUniformLocation(const String& name);
        unsigned int getAttributeLocation(const String& name);

        GLuint getId() { return myId; }

        bool use();

    protected:
        // Only Renderer can create GpuPrograms.
        GpuProgram(GpuContext* context);

    private:
        GLuint myId;

        String myShaderFilename[ShaderTypes];
        String myShaderSource[ShaderTypes];
        bool myShaderDirty[ShaderTypes];
        bool myDirty;
        GLuint myShader[ShaderTypes];
    };

    ///////////////////////////////////////////////////////////////////////////
    class OMEGA_API GpuDrawCall : public ReferenceType
    {
    public:
        static const int MaxParams = 64;
        enum PrimType { PrimNone, PrimPoints, PrimLines, PrimTriangles, PrimTriangleStrip };

        struct TextureBinding
        {
            Ref<Texture> texture;
            String name;
            GLuint location;
        };

    public:
        GpuDrawCall(GpuProgram* program):
            myProgram(program) { }


        void setVertexArray(VertexArray* va);
        void addTexture(const String& name, Texture* tx);
        void clearTextures();
        Uniform* addUniform(const String& name);
        void clearUniforms();

        void run();

        PrimType primType;
        unsigned int items;
    private:
        Ref<VertexArray> myVertexArray;
        Ref<GpuProgram> myProgram;
        List<TextureBinding*> myTextureBindings;
        List< Ref<Uniform> > myUniforms;
    };
}; // namespace omega

#endif
