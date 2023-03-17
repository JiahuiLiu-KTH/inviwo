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

#include <KTH/wing/processors/wingfeatureselector.h>

namespace inviwo {

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo WingFeatureSelector::processorInfo_{
    "org.inviwo.WingFeatureSelector",  // Class identifier
    "Wing Feature Selector",        // Display name
    "Mesh Operation",                   // Category
    CodeState::Experimental,       // Code state
    "Wing, Mesh",                    // Tags
    R"(<Explanation of how to use the processor.>)"_unindentHelp};

const ProcessorInfo WingFeatureSelector::getProcessorInfo() const { return processorInfo_; }

WingFeatureSelector::WingFeatureSelector()
    : Processor{}
    , inPort_{"inport", "<description of the inport data and any requirements on the data>"_help}
    , outPort_{"outport", "<description of the generated outport data>"_help}
    , propInputFilePrefix("inputFilePrefix", "Input File Path Prefix") {
    addPorts(inPort_, outPort_);
    addProperties(propInputFilePrefix);
}

void WingFeatureSelector::process() {
    auto inputMesh = inPort_.getData();
    auto mesh = inputMesh->clone();

    // TODO: fill the prefix

    // TODO: set the number of caches

    // TODO: check the file existence.

    // get features from CSV file
    auto features = readCSV(propInputFilePrefix.get());

    // update indices
    auto indexBuffer = mesh->getIndices(0);
    auto featureIndices = indexBuffer->getEditableRAMRepresentation();
    featureIndices->clear();
    for (auto & feature : features) {
        for (auto & pointId : feature.getRegionPointSet()) {
            featureIndices->add((uint32_t)(pointId));
        }
    }

    outPort_.setData(mesh);
}

std::vector<std::string> WingFeatureSelector::getNextLineAndSplitIntoTokens(std::ifstream& str) {
    std::vector<std::string>   result;
    std::string                line;
    std::getline(str,line);

    std::stringstream          lineStream(line);
    std::string                cell;

    while(std::getline(lineStream,cell, ','))
    {
        result.push_back(cell);
    }
    // This checks for a trailing comma with no data after it.
    if (!lineStream && cell.empty())
    {
        // If there was a trailing comma then add an empty element.
        result.push_back("");
    }
    return result;
}

std::vector<feature> WingFeatureSelector::readCSV(const std::string& filePath) {
    std::ifstream ifs (filePath, std::ifstream::in);
    std::vector<feature> features;
    while (ifs.good()) {
        auto lineTokens = getNextLineAndSplitIntoTokens(ifs);
        if (lineTokens.size() == 14) {
            feature inputFeature = feature(lineTokens);
            features.push_back(inputFeature);
        }
    }
    ifs.close();
    return features;
}


}  // namespace inviwo
