// Copyright 2024 The Manifold Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#define MANIFOLD_VERSION_MAJOR 3
#define MANIFOLD_VERSION_MINOR 0
#define MANIFOLD_VERSION_PATCH 0

// Comparable version numbers to check version in code or preprocessing.
// Check if your minimum requirements are met with e.g.
//  MANIFOLD_VERSION > MANIFOLD_VERSION_NUMBER(2, 5, 1)
#define MANIFOLD_VERSION_NUMBER(v_major, v_minor, v_patch) \
  (((v_major) * 1000000) + ((v_minor) * 1000) + (v_patch))

#define MANIFOLD_VERSION MANIFOLD_VERSION_NUMBER(MANIFOLD_VERSION_MAJOR, \
                                                 MANIFOLD_VERSION_MINOR, \
                                                 MANIFOLD_VERSION_PATCH)
