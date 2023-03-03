/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2015-2023 Inviwo Foundation
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

#include <modules/basegl/processors/imageprocessing/imagebinary.h>

#include <inviwo/core/processors/processorinfo.h>                        // for ProcessorInfo
#include <inviwo/core/processors/processorstate.h>                       // for CodeState, CodeS...
#include <inviwo/core/processors/processortags.h>                        // for Tags, Tags::GL
#include <inviwo/core/properties/ordinalproperty.h>                      // for FloatProperty
#include <modules/basegl/processors/imageprocessing/imageglprocessor.h>  // for ImageGLProcessor
#include <modules/opengl/shader/shaderutils.h>                           // for setUniforms

#include <string>       // for string
#include <string_view>  // for string_view

namespace inviwo {
class TextureUnitContainer;

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo ImageBinary::processorInfo_{
    "org.inviwo.ImageBinary",  // Class identifier
    "Image Binary",            // Display name
    "Image Operation",         // Category
    CodeState::Stable,         // Code state
    Tags::GL,                  // Tags
};
const ProcessorInfo ImageBinary::getProcessorInfo() const { return processorInfo_; }

ImageBinary::ImageBinary()
    : ImageGLProcessor("img_binary.frag"), threshold_("threshold", "Threshold", 0.5) {
    addProperty(threshold_);
}

void ImageBinary::preProcess(TextureUnitContainer&) { utilgl::setUniforms(shader_, threshold_); }

}  // namespace inviwo
