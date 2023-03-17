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
#pragma once

#include <KTH/wing/wingmoduledefine.h>
#include <iostream>
#include <string>
#include <tuple>
#include <Eigen/Dense>
#include <unordered_set>

namespace inviwo {

/**
 * \brief VERY_BRIEFLY_DESCRIBE_THE_CLASS
 * DESCRIBE_THE_CLASS_FROM_A_DEVELOPER_PERSPECTIVE
 */
class IVW_MODULE_WING_API feature {
public:
    virtual ~feature() = default;

private:
    int ID;
    double volume;
    std::tuple<double, double, double> centroid;
    std::tuple<double, double, double> centroidPhy;
    Eigen::MatrixXd center;
    Eigen::MatrixXd A;
    Eigen::MatrixXd center_Phy;
    Eigen::MatrixXd A_Phy;
    std::unordered_set<int> successor;
    std::unordered_set<int> regionPointSet;

public:
    const std::unordered_set<int> &getRegionPointSet() const;

    void setRegionPointSet(const std::unordered_set<int> &regionPointSet);

public:
    explicit feature(int id);

    feature(int id, const std::unordered_set<int> &successor);

    explicit feature(std::vector<std::string> lineTokens);

    int getId() const;

    void setId(int id);

    const std::unordered_set<int> &getSuccessor() const;

    void setSuccessor(const std::unordered_set<int> &successor);

public:
    std::string toCsvString ();

    double getVolume() const;

    void setVolume(double volume);

    const std::tuple<double, double, double> &getCentroid() const;

    void setCentroid(const std::tuple<double, double, double> &centroid);

    const Eigen::MatrixXd &getCenter() const;

    void setCenter(const Eigen::MatrixXd &center);

    const Eigen::MatrixXd &getA() const;

    void setA(const Eigen::MatrixXd &a);

    const std::tuple<double, double, double> &getCentroidPhy() const;

    void setCentroidPhy(const std::tuple<double, double, double> &centroidPhy);

    const Eigen::MatrixXd &getCenterPhy() const;

    void setCenterPhy(const Eigen::MatrixXd &centerPhy);

    const Eigen::MatrixXd &getAPhy() const;

    void setAPhy(const Eigen::MatrixXd &aPhy);

public:
    static std::vector<std::string> split(std::string & s, char del = ' ') {
        std::vector<std::string> rtn;
        std::stringstream ss(s);
        std::string word;
        while (!ss.eof()) {
            getline(ss, word, del);
            if (!word.empty())
                rtn.push_back(word);
        }
        return rtn;
    }

};

}  // namespace inviwo
