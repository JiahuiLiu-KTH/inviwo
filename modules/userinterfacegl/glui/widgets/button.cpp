/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2017 Inviwo Foundation
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

#include <modules/userinterfacegl/glui/widgets/button.h>
#include <modules/userinterfacegl/glui/renderer.h>

#include <inviwo/core/util/moduleutils.h>
#include <modules/opengl/texture/textureunit.h>
#include <modules/opengl/texture/texture2darray.h>
#include <modules/opengl/shader/shader.h>
#include <modules/opengl/shader/shaderutils.h>
#include <modules/opengl/rendering/meshdrawergl.h>

#include <vector>

namespace inviwo {

namespace glui {

Button::Button(Renderer *uiRenderer, const std::string &label, const ivec2 &extent)
    : Element(ItemType::Button, label, uiRenderer) {
    widgetExtent_ = extent;
    action_ = [&]() { LogInfo("UI button " << getLabel() << " triggered."); };

    std::vector<std::string> btnFiles = {"button-normal.png",  "button-pressed.png",
                                         "button-checked.png", "button-halo.png",
                                         "button-halo.png",    "button-halo.png"};
    uiRenderer_->createUITextures("button", btnFiles,
                                  module::getModulePath("UserInterfaceGL", ModulePath::Images));
    uiTextures_ = uiRenderer_->getUITextures("button");
    if (!uiTextures_) {
        LogWarn("Could not create UI textures for a button");
    }
}

void Button::renderWidget(const ivec2 &origin, const PickingMapper &pickingMapper) {
    TextureUnit texUnit;
    texUnit.activate();
    uiTextures_->bind();

    // bind textures
    auto &uiShader = uiRenderer_->getShader();
    uiShader.setUniform("arrayTexSampler", texUnit.getUnitNumber());

    uiShader.setUniform("origin_", vec2(origin + widgetPos_));
    uiShader.setUniform("extent_", vec2(widgetExtent_));

    // set up picking color
    uiShader.setUniform("pickingColor_", pickingMapper.getColor(pickingIDs_.front()));
    uiShader.setUniform("uiState_", ivec2(uiState(), (hovered_ ? 1 : 0)));
    uiShader.setUniform("marginScale_", marginScale());

    // render quad
    uiRenderer_->getMeshDrawer()->draw();
}

ivec2 Button::computeLabelPos(int descent) const {
    // align label to be vertically and horizontally centered within the button
    if (glm::all(glm::greaterThan(labelExtent_, ivec2(0)))) {
        vec2 labelSize(labelExtent_);
        labelSize.y -= descent;
        ivec2 labelOrigin(ivec2(glm::floor(vec2(widgetExtent_) * 0.5f + 0.5f)));
        // compute offset for vertical alignment in the center
        vec2 labelOffset(-labelSize.x * 0.5f, -labelSize.y * 0.5f);

        return ivec2(labelOrigin + ivec2(labelOffset + 0.5f));
    }
    return ivec2(0);
}

Element::UIState Button::uiState() const {
    return (checked_ ? UIState::Checked : pushed_ ? UIState::Pressed : UIState::Normal);
}

vec2 Button::marginScale() const {
    if (uiTextures_) {
        return (vec2(uiTextures_->getDimensions()) / vec2(widgetExtent_));
    }
    return vec2(1.0f);
}

}  // namespace glui

}  // namespace inviwo
