/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2014-2018 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

#ifndef IVW_BUFFER_OBJECT_H
#define IVW_BUFFER_OBJECT_H

#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/glformats.h>
#include <modules/opengl/buffer/bufferobjectobserver.h>
#include <inviwo/core/datastructures/buffer/buffer.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API BufferObject : public Observable<BufferObjectObserver> {
public:
    BufferObject(size_t sizeInBytes, const DataFormatBase* format,
                 BufferUsage usage, BufferTarget target = BufferTarget::Data);
    BufferObject(const BufferObject& rhs);
    BufferObject(BufferObject&& rhs);
    BufferObject& operator=(const BufferObject& other);
    BufferObject& operator=(BufferObject&& other);
    virtual ~BufferObject();
    virtual BufferObject* clone() const;

    GLenum getFormatType() const;
    GLenum getTarget() const;
    GLuint getId() const;

    GLFormats::GLFormat getGLFormat() const;
    const DataFormatBase* getDataFormat() const;

    void bind() const;
    void unbind() const;

    /**
     * \brief bind the buffer object and set the vertex attribute pointer
     *
     * @param location   used to set the vertex attribute location
     */
    void bindAndSetAttribPointer(GLuint location) const;

    /**
     * Set the size of the buffer in bytes.
     * Convenience function for calling initialize(nullptr, sizeInBytes)
     * @param sizeInBytes
     */
    void setSize(GLsizeiptr sizeInBytes);

    void upload(const void* data, GLsizeiptr sizeInBytes);

    void download(void* data) const;

private:
    void initialize(const void* data, GLsizeiptr sizeInBytes);

    GLuint id_;
    GLenum usageGL_;
    GLenum target_;
    GLFormats::GLFormat glFormat_;
    GLsizeiptr sizeInBytes_;
    const DataFormatBase* dataFormat_;

};

inline GLFormats::GLFormat BufferObject::getGLFormat() const { return glFormat_; }
inline const DataFormatBase* BufferObject::getDataFormat() const { return dataFormat_; }


}  // namespace

#endif  // IVW_BUFFER_OBJECT_H
