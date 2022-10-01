/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2017-2022 Inviwo Foundation
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

#include <modules/python3/processors/numpymeshcreatetest.h>

#include <pybind11/cast.h>     // for object::cast
#include <pybind11/pytypes.h>  // for object, accessor, dict, objec...

#include <inviwo/core/common/inviwoapplication.h>           // for InviwoApplication
#include <inviwo/core/common/modulepath.h>                  // for ModulePath, ModulePath::Scripts
#include <inviwo/core/datastructures/geometry/typedmesh.h>  // for BasicMesh
#include <inviwo/core/ports/meshport.h>                     // for MeshOutport
#include <inviwo/core/processors/processor.h>               // for Processor
#include <inviwo/core/processors/processorinfo.h>           // for ProcessorInfo
#include <inviwo/core/processors/processorstate.h>          // for CodeState, CodeState::Experim...
#include <inviwo/core/properties/invalidationlevel.h>       // for InvalidationLevel, Invalidati...
#include <modules/python3/python3module.h>                  // for Python3Module
#include <modules/python3/pythonscript.h>                   // for PythonScriptDisk

#include <functional>   // for __base
#include <memory>       // for shared_ptr
#include <string>       // for operator+, string
#include <string_view>  // for string_view

namespace inviwo {

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo NumPyMeshCreateTest::processorInfo_{
    "org.inviwo.NumPyMeshCreateTest",  // Class identifier
    "NumPy Mesh Create Test",          // Display name
    "NumPy",                           // Category
    CodeState::Experimental,           // Code state
    {"Python"},                        // Tags
};
const ProcessorInfo NumPyMeshCreateTest::getProcessorInfo() const { return processorInfo_; }

NumPyMeshCreateTest::NumPyMeshCreateTest()
    : Processor()
    , script_(InviwoApplication::getPtr()->getModuleByType<Python3Module>()->getPath(
                  ModulePath::Scripts) +
              "/numpymeshcreatetest.py")
    , mesh_("mesh") {

    script_.onChange([this]() { invalidate(InvalidationLevel::InvalidOutput); });

    addPort(mesh_);
}

void NumPyMeshCreateTest::process() {
    script_.run([&](pybind11::dict dict) {
        auto pyMesh = dict["mesh"];
        auto mesh = std::shared_ptr<BasicMesh>(pyMesh.cast<BasicMesh*>());
        pyMesh.cast<pybind11::object>().release();
        mesh_.setData(mesh);
    });
}

}  // namespace inviwo
