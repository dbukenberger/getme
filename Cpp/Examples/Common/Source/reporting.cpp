/*
Functions for command line reporting of smoothing results.

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

#include "Mathematics/bounding_box.h"
#include "Mathematics/vector2d_algorithms.h"
#include "Mesh/mesh_quality.h"
#include "Mesh/polygonal_mesh.h"
#include "Mesh/polygonal_mesh_algorithms.h"
#include "Smoothing/getme_result.h"
#include "Smoothing/smoothing_result.h"
#include "Utility/exception_handling.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>

namespace {
struct ZeroDefaultedCount {
  std::size_t count = 0;
};

void printPolygonInformation(const std::size_t numberOfNodes,
                             const std::size_t count,
                             const bool isLast) {
  std::cout << count << " ";
  switch (numberOfNodes) {
    case 3:
      std::cout << "triangle";
      break;
    case 4:
      std::cout << "quadrilateral";
      break;
    case 5:
      std::cout << "pentagon";
      break;
    case 6:
      std::cout << "hexagon";
      break;
    case 8:
      std::cout << "octagon";
      break;
    case 12:
      std::cout << "dodecagon";
      break;
    default:
      std::cout << numberOfNodes << "-gon";
  }
  std::cout << (count == 1 ? "" : "s") << (isLast ? "" : ", ");
}

void printElementStatistics(const Mesh::PolygonalMesh& mesh) {
  const auto& polygons = mesh.getPolygons();
  std::map<std::size_t, ZeroDefaultedCount> numberOfNodesToCount;
  for (const auto& polygon : polygons) {
    numberOfNodesToCount[polygon.getNumberOfNodes()].count++;
  }
  std::cout << "  elements: ";
  for (const auto [numberOfNodes, count] : numberOfNodesToCount) {
    const bool isLastEntry =
        numberOfNodes == numberOfNodesToCount.rbegin()->first;
    printPolygonInformation(numberOfNodes, count.count, isLastEntry);
  }
  std::cout << "\n";
}

void printMeshDimension(const Mesh::PolygonalMesh& mesh) {
  const auto boundingBox = Mathematics::getBoundingBox(mesh.getNodes());
  std::cout << "  x-dimension: " << boundingBox.getXDimension()
            << ", y-dimension: " << boundingBox.getYDimension() << "\n";
}

void printMeshQuality(const Mesh::MeshQuality& meshQuality,
                      const std::string& descriptionPrefix) {
  const std::string prefix =
      descriptionPrefix.empty() ? "M" : descriptionPrefix + "m";
  std::cout << prefix << "esh quality: ";
  if (meshQuality.getQMin() < 0) {
    if (meshQuality.getNumberOfInvalidElements().has_value()) {
      std::cout << meshQuality.getNumberOfInvalidElements().value()
                << " invalid elements\n";
    } else {
      std::cout << "invalid mesh\n";
    }
  } else {
    std::cout << std::fixed << std::setprecision(4)
              << "qmin=" << meshQuality.getQMin();
    if (meshQuality.getQMinStar().has_value()) {
      std::cout << ", qmin*=" << meshQuality.getQMinStar().value();
    }
    std::cout << ", qmean=" << meshQuality.getQMean() << "\n";
  }
}

void printSmoothingResultBase(const Smoothing::SmoothingResultBase& result) {
  std::cout << result.algorithmName << " smoothing result\n";
  printMeshQuality(result.meshQuality, "  ");
  std::cout << "  smoothing time: " << std::fixed << std::setprecision(3)
            << result.smoothingWallClockTimeInSeconds << "s\n";
}
}  // namespace

void Common::printInitialMeshInformation(const Mesh::PolygonalMesh& mesh) {
  std::cout << "Initial mesh consist of " << mesh.getNumberOfNodes()
            << " nodes (" << mesh.getFixedNodeIndices().size() << " fixed) and "
            << mesh.getNumberOfPolygons() << " polygons.\n";
  printElementStatistics(mesh);
  printMeshDimension(mesh);
  const Mesh::MeshQuality meshQuality(mesh);
  printMeshQuality(meshQuality, "  ");
}

void Common::printSmoothingResult(const Smoothing::SmoothingResult& result) {
  printSmoothingResultBase(result);
  std::cout << "  iterations: " << result.iterations << "\n";
}

void Common::printSmoothingResult(const Smoothing::GetmeResult& result) {
  printSmoothingResultBase(result);
  std::cout << "  iterations: " << result.getmeSimultaneousIterations << "/"
            << result.getmeSequentialIterations << "\n";
}

void Common::writeResultMesh(const Mesh::PolygonalMesh &mesh, const std::filesystem::path &initialMeshPath, const std::string &meshName) {
    auto pathString = initialMeshPath.string();

    std::string fExt = initialMeshPath.extension();
    const auto index = pathString.find(fExt);
    Utility::throwExceptionIfTrue(std::string::npos == index, "File has an unexpected file extension.");

    pathString = pathString.erase(index) + "_" + meshName + fExt;
    std::cout << "Writing result mesh file " << pathString << "\n";
    if (fExt == ".mesh") {
        Mesh::writeMeshFile(mesh, pathString);
    } else if (fExt == ".obj") {
        Mesh::writeObjFile(mesh, pathString);
    }
}

Mesh::PolygonalMesh Common::readInitialMesh(const std::filesystem::path &initialMeshPath) {
    std::string fExt = initialMeshPath.extension();
    const auto index = initialMeshPath.string().find(fExt);

    if (fExt == ".mesh") {
        return Mesh::readMeshFile( initialMeshPath);
    } else if (fExt == ".obj") {
        return Mesh::readObjFile(initialMeshPath);
    }
    Utility::throwException("Unexpected file extension.");
}
