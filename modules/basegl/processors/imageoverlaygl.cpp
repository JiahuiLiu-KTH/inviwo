/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2014-2016 Inviwo Foundation
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

#include "imageoverlaygl.h"
#include <modules/opengl/texture/textureunit.h>
#include <inviwo/core/interaction/events/gestureevent.h>
#include <inviwo/core/interaction/events/touchevent.h>
#include <inviwo/core/interaction/interactionhandler.h>
#include <modules/opengl/texture/textureutils.h>
#include <modules/opengl/openglutils.h>

namespace inviwo {

const ProcessorInfo ImageOverlayGL::processorInfo_{
    "org.inviwo.ImageOverlayGL",  // Class identifier
    "Image Overlay",              // Display name
    "Image Operation",            // Category
    CodeState::Experimental,      // Code state
    Tags::GL,                     // Tags
};
const ProcessorInfo ImageOverlayGL::getProcessorInfo() const {
    return processorInfo_;
}

OverlayProperty::OverlayProperty(std::string identifier, std::string displayName,
                                 InvalidationLevel invalidationLevel, PropertySemantics semantics)
    : CompositeProperty(identifier, displayName, invalidationLevel, semantics)
    , pos_("position", "Position", vec2(0.25f), vec2(0.0f), vec2(1.0f), vec2(0.01f), InvalidationLevel::Valid)
    , size_("size", "Size", vec2(0.48f), vec2(0.0f), vec2(1.0f), vec2(0.01f), InvalidationLevel::Valid)
    , absolutePos_("absolutePos", "Position (absolute)", ivec2(10), ivec2(0), ivec2(2048), ivec2(1), InvalidationLevel::Valid)
    , absoluteSize_("absoluteSize", "Size (absolute)", ivec2(10), ivec2(0), ivec2(2048), ivec2(1), InvalidationLevel::Valid)
    , anchorPos_("anchor", "Anchor", vec2(0.0f), vec2(-1.0f), vec2(1.0f), vec2(0.01f), InvalidationLevel::Valid)
    , blendMode_("blendMode", "Blending Mode", InvalidationLevel::Valid)
    , positioningMode_("positioningMode", "Positioning Mode", InvalidationLevel::Valid)
    , sizeMode_("sizeMode", "Size Mode", InvalidationLevel::Valid)
    , viewDimensions_(-1, -1)
    , viewport_(0, 0, 1, 1)
    , isDeserializing_(false)
{ 
    blendMode_.addOption("replace", "Replace", BlendMode::Replace);
    blendMode_.addOption("over", "Blend", BlendMode::Over);
    blendMode_.setSelectedValue(BlendMode::Over);
    blendMode_.setCurrentStateAsDefault();

    positioningMode_.addOption("absolute", "Absolute", Positioning::Absolute);
    positioningMode_.addOption("relative", "Relative", Positioning::Relative);
    positioningMode_.setSelectedValue(Positioning::Relative);
    positioningMode_.setCurrentStateAsDefault();

    sizeMode_.addOption("absolute", "Absolute", Positioning::Absolute);
    sizeMode_.addOption("relative", "Relative", Positioning::Relative);
    sizeMode_.setSelectedValue(Positioning::Relative);
    sizeMode_.setCurrentStateAsDefault();

    auto updateCallback = [&]() { updateViewport(); };

    positioningMode_.onChange([&]() {
        pos_.setVisible(positioningMode_.get() == Positioning::Relative);
        absolutePos_.setVisible(positioningMode_.get() == Positioning::Absolute);
        updateViewport();
    });
    pos_.onChange(updateCallback);
    absolutePos_.onChange(updateCallback);
    sizeMode_.onChange([&]() {
        size_.setVisible(sizeMode_.get() == Positioning::Relative);
        absoluteSize_.setVisible(sizeMode_.get() == Positioning::Absolute);
        updateViewport();
    });
    size_.onChange(updateCallback);
    absoluteSize_.onChange(updateCallback);
    anchorPos_.onChange(updateCallback);
    blendMode_.onChange(updateCallback);

    addProperty(positioningMode_);
    addProperty(pos_);
    addProperty(absolutePos_);

    addProperty(sizeMode_);
    addProperty(size_);
    addProperty(absoluteSize_);

    addProperty(anchorPos_);
    addProperty(blendMode_);

    updateVisibilityState();
}

void OverlayProperty::setViewDimensions(ivec2 viewDim) {
    viewDimensions_ = viewDim;
    updateViewport();
}

const ivec4 & OverlayProperty::getViewport() const {
    return viewport_;
}

GLint OverlayProperty::getBlendMode() const {
    return static_cast<GLint>(blendMode_.get());
}

void OverlayProperty::deserialize(Deserializer& d) {
    isDeserializing_ = true;
    CompositeProperty::deserialize(d);
    isDeserializing_ = false;
    updateVisibilityState();
}

void OverlayProperty::updateViewport() {
    if (isDeserializing_) return;

    vec2 viewDim(viewDimensions_);

    // determine size and center position first (absolute coords)
    vec2 pos(viewDim * 0.25f);
    vec2 size(viewDim * 0.5f);

    if (positioningMode_.get() == Positioning::Absolute) {
        pos = vec2(absolutePos_.get());
    }
    else {
        pos = pos_.get() * viewDim;
    }
    if (sizeMode_.get() == Positioning::Absolute) {
        size = vec2(absoluteSize_.get());
    }
    else {
        size = size_.get() * viewDim;
    }

    // adjust max values of absolute pos and size
    // need to save and restore current values since setMaxValue() will clip data values
    Property::OnChangeBlocker block(absolutePos_);
    Property::OnChangeBlocker block2(absoluteSize_);
    auto tmp = absolutePos_.get();
    absolutePos_.setMaxValue(viewDimensions_);
    absolutePos_.set(tmp);
    tmp = absoluteSize_.get();
    absoluteSize_.setMaxValue(viewDimensions_);
    absoluteSize_.set(tmp);

    // consider anchor position
    vec2 anchor(anchorPos_.get());
    vec2 shift = 0.5f * size * (anchorPos_.get() + vec2(1.0f, 1.0f));
    pos.x -= shift.x;
    // negate y axis
    pos.y = viewDim.y - (pos.y + shift.y);
    
    // use pixel aligned positions for best results
    viewport_ = ivec4(pos.x, pos.y, size.x, size.y);
    CompositeProperty::propertyModified();
}

void OverlayProperty::updateVisibilityState() {
    size_.setVisible(sizeMode_.get() == Positioning::Relative);
    absoluteSize_.setVisible(sizeMode_.get() == Positioning::Absolute);

    pos_.setVisible(positioningMode_.get() == Positioning::Relative);
    absolutePos_.setVisible(positioningMode_.get() == Positioning::Absolute);
}


ImageOverlayGL::ImageOverlayGL()
    : Processor()
    , inport_("inport")
    , overlayPort_("overlay")
    , outport_("outport")
    , enabled_("enabled", "Overlay Enabled", true)
    , overlayInteraction_("overlayInteraction", "Overlay Interaction", false)
    , overlayProperty_("overlay", "Overlay")
    , border_("border", "Border", true)
    , borderColor_("borderColor", "Color", vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4(0.0f), vec4(1.0f))
    , borderWidth_("borderWidth", "Width", 2, 0, 100)
    , shader_("img_identity.vert", "img_overlay.frag")
    , viewManager_()
    , currentDim_(0u, 0u) 
{
    shader_.onReload([this]() { invalidate(InvalidationLevel::InvalidResources); });

    addPort(inport_);
    addPort(overlayPort_);

    overlayPort_.onConnect([this]() {
        viewManager_.push_back(overlayProperty_.getViewport());
        onStatusChange();
    });

    overlayPort_.onDisconnect([this]() {
        viewManager_.clear();
    });

    addPort(outport_);

    addProperty(enabled_);
    addProperty(overlayInteraction_);
    addProperty(overlayProperty_);

    borderColor_.setSemantics(PropertySemantics::Color);

    border_.addProperty(borderColor_);
    border_.addProperty(borderWidth_);
    addProperty(border_);

    overlayProperty_.onChange(this, &ImageOverlayGL::onStatusChange);
}

ImageOverlayGL::~ImageOverlayGL() = default;

void ImageOverlayGL::propagateEvent(Event* event, Outport* source) {
    if (event->hasVisitedProcessor(this)) return;
    event->markAsVisited(this);

    invokeEvent(event);
    if (event->hasBeenUsed()) return;

    if (event->hash() == ResizeEvent::chash()) {
        auto resizeEvent = static_cast<ResizeEvent*>(event);

        updateViewports(resizeEvent->size(), true);
        inport_.propagateEvent(resizeEvent);

        if (overlayPort_.isConnected()) {
            ResizeEvent e(uvec2(viewManager_[0].z, viewManager_[0].w));
            overlayPort_.propagateEvent(&e);
        }
    } else {
        if (overlayInteraction_.get() && overlayPort_.isConnected()) {
            std::unique_ptr<Event> newEvent(viewManager_.registerEvent(event));
            int activeView = viewManager_.getActiveView();

            if (newEvent && activeView >= 0) {
                overlayPort_.propagateEvent(newEvent.get());
                if (newEvent->hasBeenUsed()) event->markAsUsed();
                for (auto p : newEvent->getVisitedProcessors()) event->markAsVisited(p);
                return;
            }
        }

        if (event->shouldPropagateTo(&inport_, this, source)) {
            inport_.propagateEvent(event);
        }
    }
}

bool ImageOverlayGL::isReady() const { return inport_.isReady(); }

void ImageOverlayGL::onStatusChange() {
    if (overlayPort_.isConnected()) {
        // update viewport stored in view manager
        viewManager_.replace(0, overlayProperty_.getViewport());

        ResizeEvent e(uvec2(viewManager_[0].z, viewManager_[0].w));
        overlayPort_.propagateEvent(&e, overlayPort_.getConnectedOutport());
    }
}

void ImageOverlayGL::process() {
    if (!enabled_.get()) {
        outport_.setData(inport_.getData());
        return;
    }

    utilgl::activateTargetAndCopySource(outport_, inport_, inviwo::ImageType::ColorDepthPicking);

    if (overlayPort_.hasData()) {  // draw overlay
        utilgl::DepthFuncState(GL_ALWAYS);

        ivec4 viewport = overlayProperty_.getViewport();
        int borderWidth = 0;
        if (border_.isChecked()) {
            // adjust viewport by border width
            borderWidth = borderWidth_.get();
            viewport += ivec4(-borderWidth, -borderWidth, 2 * borderWidth, 2 * borderWidth);
        }

        utilgl::BlendModeState blendMode(overlayProperty_.getBlendMode(), GL_ONE_MINUS_SRC_ALPHA);

        TextureUnit colorUnit, depthUnit, pickingUnit;
        shader_.activate();
        shader_.setUniform("color_", colorUnit.getUnitNumber());
        shader_.setUniform("depth_", depthUnit.getUnitNumber());
        shader_.setUniform("picking_", pickingUnit.getUnitNumber());
        shader_.setUniform("borderWidth_", borderWidth);
        shader_.setUniform("borderColor_", borderColor_.get());
        shader_.setUniform("viewport_", vec4(viewport));

        utilgl::bindTextures(overlayPort_, colorUnit, depthUnit, pickingUnit);

        utilgl::ViewportState viewportState(viewport);
        utilgl::singleDrawImagePlaneRect();
        shader_.deactivate();
    }
    utilgl::deactivateCurrentTarget();
}

void ImageOverlayGL::updateViewports(ivec2 dim, bool force) {
    if (!force && (currentDim_ == dim)) return;  // no changes

    overlayProperty_.setViewDimensions(dim);
    currentDim_ = dim;
}

}  // namespace

