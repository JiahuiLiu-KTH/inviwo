/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2023 Inviwo Foundation
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

#include <KTH/wing/processors/wingmeshreader.h>

namespace inviwo {

WingMeshReader::WingMeshReader() : DataReaderType<Mesh>() {
    addExtension(FileExtension("bin", "Wing Mesh binary file"));
}

WingMeshReader* WingMeshReader::clone() const { return new WingMeshReader(*this); }

std::shared_ptr<Mesh> WingMeshReader::readData(std::string_view filePath) {
    // initialize vertex buffer and index buffer
    auto vertexBuffer = std::make_shared<Buffer<vec3> >();
    auto vertices = vertexBuffer->getEditableRAMRepresentation();
    auto indexBuffer = std::make_shared<IndexBuffer>();
    auto indices = indexBuffer->getEditableRAMRepresentation();

    // add vertex
    vertices->reserve(3);
    vertices->add(vec3(0.0, 0.0, 0.0));
    vertices->add(vec3(1.0, 0.0, 0.0));
    vertices->add(vec3(0.0, 1.0, 0.0));
    vertices->add(vec3(0.0, 0.0, 1.0));

    // add index
    indices->add((uint32_t)(0));
    indices->add((uint32_t)(1));
    indices->add((uint32_t)(2));
    indices->add((uint32_t)(3));

    // construct mesh
    auto mesh = std::make_shared<Mesh>();
    mesh->addBuffer(BufferType::PositionAttrib, vertexBuffer);
    mesh->addIndices(Mesh::MeshInfo(DrawType::Points, ConnectivityType::None), indexBuffer);

    return mesh;
}

}  // namespace inviwo
