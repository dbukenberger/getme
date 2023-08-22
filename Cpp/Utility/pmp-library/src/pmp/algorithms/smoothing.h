// Copyright 2011-2020 the Polygon Mesh Processing Library developers.
// Distributed under a MIT-style license, see LICENSE.txt for details.

#pragma once

#include "pmp/SurfaceMesh.h"

namespace pmp {

//! \brief Perform explicit Laplacian smoothing.
//! \details See \cite desbrun_1999_implicit for details
//! \param mesh The input mesh, modified in place.
//! \param iters The number of iterations performed.
//! \param use_uniform_laplace Use uniform or cotan Laplacian. Default: cotan.
//! \ingroup algorithms
void explicit_smoothing(SurfaceMesh& mesh, unsigned int iters = 10,
                        bool use_uniform_laplace = false);

//! \brief Perform implicit Laplacian smoothing.
//! \details See \cite desbrun_1999_implicit and \cite kazhdan_2012
//! \param mesh The input mesh, modified in place.
//! \param timestep The time step taken.
//! \param use_uniform_laplace Use uniform or cotan Laplacian. Default: cotan.
//! \param rescale Re-center and re-scale model after smoothing. Default: true.
//! \throw SolverException in case of a failure to solve the linear system.
//! \ingroup algorithms
void implicit_smoothing(SurfaceMesh& mesh, Scalar timestep = 0.001,
                        bool use_uniform_laplace = false, bool rescale = true);

} // namespace pmp
