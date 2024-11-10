/**
 *  Copyright 2024 Niklas Meyer
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * @file
 * @author Niklas Meyer <nik_mey@uni-bremen.de>
 *
 * @section Description
 *
 * Functions that convert LwM2M definitions to the Semantic Definition Format.
 */

#ifndef LWM2M_CPP_EDGE_SRC_LWM2M_TO_SDF_H_
#define LWM2M_CPP_EDGE_SRC_LWM2M_TO_SDF_H_

#include <sdf/sdf_cpp_core.h>
#include "lwm2m_cpp_edge.h"

//! @brief Map a LwM2M definition definition to a SDF model as well as a SDF mapping.
//!
//! This function a LwM2M definition definition to a SDF model as well as a SDF mapping.
//!
//! @param lwm2m The input LwM2M definition.
//! @param sdf_model The resulting SDF model.
//! @param sdf_mapping The resulting SDF mapping.
void MapLwm2mToSdf(const lwm2m::Lwm2m& lwm2m, sdf::SdfModel& sdf_model, sdf::SdfMapping& sdf_mapping);

#endif //LWM2M_CPP_EDGE_SRC_LWM2M_TO_SDF_H_
