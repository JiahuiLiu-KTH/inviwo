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

#include <KTH/wing/datastructures/feature.h>

namespace inviwo {

feature::feature(int id) : ID(id) {}

feature::feature(int id, const std::unordered_set<int> &successor) : ID(id), successor(successor) {}

feature::feature(std::vector<std::string> lineTokens) {
    std::string subStr;
    std::vector<std::string> splitTokens;
    this->ID = std::stoi(lineTokens[0]);
    this->volume = std::stod(lineTokens[1]);
    this->centroid = {std::stod(lineTokens[2].substr(2)), std::stod(lineTokens[3]), std::stod(lineTokens[4].substr(1, lineTokens[4].length()-2))};
    this->centroidPhy = {std::stod(lineTokens[5].substr(2)), std::stod(lineTokens[6]), std::stod(lineTokens[7].substr(1, lineTokens[7].length()-2))};

    subStr = lineTokens[8].substr(2, lineTokens[8].length() - 3);
    splitTokens = split(subStr);
    this->center = Eigen::MatrixXd{{std::stod(splitTokens[0]), std::stod(splitTokens[1]), std::stod(splitTokens[2])}};

    subStr = lineTokens[9].substr(2, lineTokens[9].length() - 3);
    splitTokens = split(subStr);
    this->A = Eigen::MatrixXd{{std::stod(splitTokens[0]), std::stod(splitTokens[1]), std::stod(splitTokens[2])},
                              {std::stod(splitTokens[3]), std::stod(splitTokens[4]), std::stod(splitTokens[5])},
                              {std::stod(splitTokens[6]), std::stod(splitTokens[7]), std::stod(splitTokens[8])}};

    subStr = lineTokens[10].substr(2, lineTokens[10].length() - 3);
    splitTokens = split(subStr);
    this->center_Phy = Eigen::MatrixXd{{std::stod(splitTokens[0]), std::stod(splitTokens[1]), std::stod(splitTokens[2])}};

    subStr = lineTokens[11].substr(2, lineTokens[11].length() - 3);
    splitTokens = split(subStr);
    this->A_Phy = Eigen::MatrixXd{{std::stod(splitTokens[0]), std::stod(splitTokens[1]), std::stod(splitTokens[2])},
                                  {std::stod(splitTokens[3]), std::stod(splitTokens[4]), std::stod(splitTokens[5])},
                                  {std::stod(splitTokens[6]), std::stod(splitTokens[7]), std::stod(splitTokens[8])}};

    subStr = lineTokens[12].substr(5, lineTokens[12].length() - 6);
    splitTokens = split(subStr);
    this->successor.reserve(splitTokens.size());
    for (int i = 0; i < splitTokens.size(); ++i) {
        this->successor.insert(std::stoi(splitTokens[i]));
    }

    subStr = lineTokens[13].substr(5, lineTokens[13].length() - 6);
    splitTokens = split(subStr);
    this->regionPointSet.reserve(splitTokens.size());
    for (int i = 0; i < splitTokens.size(); ++i) {
        this->regionPointSet.insert(std::stoi(splitTokens[i]));
    }

}

int feature::getId() const {
    return ID;
}

void feature::setId(int id) {
    ID = id;
}

double feature::getVolume() const {
    return volume;
}

void feature::setVolume(double volume) {
    feature::volume = volume;
}

const std::tuple<double, double, double> &feature::getCentroid() const {
    return centroid;
}

void feature::setCentroid(const std::tuple<double, double, double> &centroid) {
    feature::centroid = centroid;
}

const std::tuple<double, double, double> &feature::getCentroidPhy() const {
    return centroidPhy;
}

void feature::setCentroidPhy(const std::tuple<double, double, double> &centroidPhy) {
    feature::centroidPhy = centroidPhy;
}

const Eigen::MatrixXd &feature::getCenter() const {
    return center;
}

void feature::setCenter(const Eigen::MatrixXd &center) {
    feature::center = center;
}

const Eigen::MatrixXd &feature::getA() const {
    return A;
}

void feature::setA(const Eigen::MatrixXd &a) {
    A = a;
}

const Eigen::MatrixXd &feature::getCenterPhy() const {
    return center_Phy;
}

void feature::setCenterPhy(const Eigen::MatrixXd &centerPhy) {
    center_Phy = centerPhy;
}

const Eigen::MatrixXd &feature::getAPhy() const {
    return A_Phy;
}

void feature::setAPhy(const Eigen::MatrixXd &aPhy) {
    A_Phy = aPhy;
}

const std::unordered_set<int> &feature::getSuccessor() const {
    return successor;
}

void feature::setSuccessor(const std::unordered_set<int> &successor) {
    feature::successor = successor;
}

const std::unordered_set<int> &feature::getRegionPointSet() const {
    return regionPointSet;
}

void feature::setRegionPointSet(const std::unordered_set<int> &regionPointSet) {
    feature::regionPointSet = regionPointSet;
}

std::string feature::toCsvString() {

    ////----------------------------------------------------------------------------------------------------------------
    std::ostringstream ss;
    ss << std::scientific;

    ss << ID;
    ss << ", ";

    ss << volume;
    ss << ", ";

    ss << "(";
    ss << std::get<0>(centroid) << ", " <<std::get<1>(centroid) << ", " << std::get<2>(centroid);
    ss << "), ";

    ss << "(";
    ss << std::get<0>(centroidPhy) << ", " <<std::get<1>(centroidPhy) << ", " << std::get<2>(centroidPhy);
    ss << "), ";

    ss << "<";
    for (int i = 0; i < 2; ++i) {
        ss << center.data()[i];
        ss << " ";
    }
    ss << center.data()[2] << ">, ";

    ss << "<";
    for (int i = 0; i < 8; ++i) {
        ss << A.data()[i];
        ss << " ";
    }
    ss << A.data()[8] << ">, ";

    ss << "<";
    for (int i = 0; i < 2; ++i) {
        ss << center_Phy.data()[i];
        ss << " ";
    }
    ss << center_Phy.data()[2] << ">, ";

    ss << "<";
    for (int i = 0; i < 8; ++i) {
        ss << A_Phy.data()[i];
        ss << " ";
    }
    ss << A_Phy.data()[8] << ">, ";

    ss << "set(";
    for (auto const &s : successor) {
        ss << s << " ";
    }
    ss << "), ";

    ss << "set(";
    for (auto const &i : regionPointSet) {
        ss << i << " ";
    }
    ss << ")" << std::endl;

    return ss.str();
    ////----------------------------------------------------------------------------------------------------------------
    std::string csvString;

    csvString.append(std::to_string(ID));
    csvString.append(", ");

    csvString.append(std::to_string(volume));
    csvString.append(", ");

    csvString.append("(");
    csvString.append(std::to_string(std::get<0>(centroid)));
    csvString.append("  ");
    csvString.append(std::to_string(std::get<1>(centroid)));
    csvString.append("  ");
    csvString.append(std::to_string(std::get<2>(centroid)));
    csvString.append("), ");

    csvString.append("<");
    for (int i = 0; i < 3; ++i) {
        csvString.append(std::to_string(center.data()[i]));
        csvString.append(" ");
    }
    csvString.append(">, ");

    csvString.append("<");
    for (int i = 0; i < 9; ++i) {
        csvString.append(std::to_string(A.data()[i]));
        csvString.append(" ");
    }
    csvString.append(">, ");

    csvString.append("set(");
    for (auto const &s : successor) {
        csvString.append(std::to_string(s));
        csvString.append(" ");
    }
    csvString.append("), ");

    csvString.append("set(");
    for (auto const &i : regionPointSet) {
        csvString.append(std::to_string(i));
        csvString.append(" ");
    }
    csvString.append(")\n");

    return csvString;
}

}  // namespace inviwo
