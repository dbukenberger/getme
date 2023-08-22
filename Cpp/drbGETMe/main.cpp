/*
Smoothing example from Section 7.1.1 of the GETMe book.

Copyright (C) 2023  TWT GmbH Science & Innovation.

This file is part of GETMe Polygonal Meshes 2D.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/
#include "Common/reporting.h"
#include "Common/smoothing_headers.h"

#include <filesystem>
#include <iostream>

int main(int argc, char *argv[]) {

    if(argc < 2) {
        std::cout << "Please specify an input mesh." << std::endl;
        return 0;
    }

    char *inputFilePath = argv[1];
    std::cout << "In File: " << inputFilePath << std::endl;

    std::filesystem::path filePath(inputFilePath);
    std::string inputBaseName = filePath.filename().string();
    const size_t index = inputBaseName.find("_initial");
    if (std::string::npos != index) {
        inputBaseName = inputBaseName.erase(index) + filePath.extension().string();
    }

    std::string outputDir = "res";
    std::filesystem::create_directory(outputDir);
    std::string ouputFilePath = outputDir + "/" + inputBaseName;

    // Read inital mesh and report quality.
    const auto initialMesh = Common::readInitialMesh(inputFilePath);
    Common::printInitialMeshInformation(initialMesh);
    Common::writeResultMesh(initialMesh, ouputFilePath, "initial");

    // Set algorithm parameters.
    const Smoothing::SmartLaplaceConfig smartLaplaceConfig;
    const Smoothing::GetmeConfig getmeConfig(initialMesh.getMaximalNumberOfPolygonNodes());

    // Smooth mesh using smart Laplace.
    const auto smartLaplaceResult = Smoothing::smartLaplace(initialMesh, smartLaplaceConfig);
    Common::printSmoothingResult(smartLaplaceResult);
    Common::writeResultMesh(smartLaplaceResult.mesh, ouputFilePath, "smart_laplace");

    // Smooth mesh using GETMe.
    const auto getmeResult = Smoothing::getme(initialMesh, getmeConfig);
    Common::printSmoothingResult(getmeResult);
    Common::writeResultMesh(getmeResult.mesh, ouputFilePath, "getme");

    return 0;
}
