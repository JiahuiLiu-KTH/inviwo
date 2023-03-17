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
#include <fstream>

namespace inviwo {

WingMeshReader::WingMeshReader() : DataReaderType<Mesh>() {
    addExtension(FileExtension("bin", "Wing Mesh binary file"));
}

WingMeshReader* WingMeshReader::clone() const { return new WingMeshReader(*this); }

std::shared_ptr<Mesh> WingMeshReader::readData(std::string_view filePath) {
    // initialize vertex buffer and index buffer
    auto vertexBuffer = std::make_shared<Buffer<vec3>>();
    auto vertices = vertexBuffer->getEditableRAMRepresentation();
    auto indexBuffer = std::make_shared<IndexBuffer>();
    auto indices = indexBuffer->getEditableRAMRepresentation();
    auto indexBuffer_phy = std::make_shared<IndexBuffer>();
    auto indices_phy = indexBuffer_phy->getEditableRAMRepresentation();

    // read from file
    auto vertexList = readMesh_binary(std::string(filePath));

    // add vertex
    vertices->reserve(vertexList.size());
    for (int i = 0; i < vertexList.size(); ++i) // add physical coordinates
        vertices->add(vec3(vertexList[i].x_Phy, vertexList[i].y_Phy, vertexList[i].z_Phy));

    for (int i = 0; i < vertexList.size(); ++i) // add deformed coordinates
        vertices->add(vec3(vertexList[i].x, vertexList[i].y, vertexList[i].z));

    // add index
    indices_phy->reserve(vertexList.size());
    for (int i = 0; i < vertexList.size(); ++i)
        indices_phy->add((uint32_t)(i));

    indices->reserve(vertexList.size());
    for (int i = 0; i < vertexList.size(); ++i)
        indices->add((uint32_t)(i + vertexList.size()));


    // construct mesh
    auto mesh = std::make_shared<Mesh>();
    mesh->addBuffer(BufferType::PositionAttrib, vertexBuffer);
//    mesh->addIndices(Mesh::MeshInfo(DrawType::Points, ConnectivityType::None), indexBuffer_phy);
    mesh->addIndices(Mesh::MeshInfo(DrawType::Points, ConnectivityType::None), indexBuffer);

    return mesh;
}

std::vector<Vertex> WingMeshReader::readMesh_binary(const std::string& filePath) {
    std::ifstream rf(filePath, std::ios::in | std::ios::binary);
    if(!rf) {
        std::cout << "Cannot open file!" << std::endl;
    }
    rf.seekg (0, rf.end);
    int size = rf.tellg()/56;
    rf.seekg (0, rf.beg);
    std::vector<Vertex> vertices;
    vertices.resize(size);
    for(int i = 0; i < size; i++)
        rf.read((char *) &vertices[i], sizeof(Vertex));
    rf.close();
    if(!rf.good()) {
        std::cout << "Error occurred at reading time!" << std::endl;
    }

    return vertices;
}

}  // namespace inviwo
