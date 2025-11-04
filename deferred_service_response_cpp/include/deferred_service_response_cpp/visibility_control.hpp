// Copyright 2025 Jasper van Brakel
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef DEFERRED_SERVICE_RESPONSE_CPP__VISIBILITY_CONTROL_HPP_
#define DEFERRED_SERVICE_RESPONSE_CPP__VISIBILITY_CONTROL_HPP_

// This logic was borrowed (then namespaced) from the examples on the gcc wiki:
//     https://gcc.gnu.org/wiki/Visibility

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define DEFERRED_SERVICE_RESPONSE_CPP_EXPORT __attribute__ ((dllexport))
    #define DEFERRED_SERVICE_RESPONSE_CPP_IMPORT __attribute__ ((dllimport))
  #else
    #define DEFERRED_SERVICE_RESPONSE_CPP_EXPORT __declspec(dllexport)
    #define DEFERRED_SERVICE_RESPONSE_CPP_IMPORT __declspec(dllimport)
  #endif
  #ifdef DEFERRED_SERVICE_RESPONSE_CPP_BUILDING_LIBRARY
    #define DEFERRED_SERVICE_RESPONSE_CPP_PUBLIC DEFERRED_SERVICE_RESPONSE_CPP_EXPORT
  #else
    #define DEFERRED_SERVICE_RESPONSE_CPP_PUBLIC DEFERRED_SERVICE_RESPONSE_CPP_IMPORT
  #endif
  #define DEFERRED_SERVICE_RESPONSE_CPP_PUBLIC_TYPE DEFERRED_SERVICE_RESPONSE_CPP_PUBLIC
  #define DEFERRED_SERVICE_RESPONSE_CPP_LOCAL
#else
  #define DEFERRED_SERVICE_RESPONSE_CPP_EXPORT __attribute__ ((visibility("default")))
  #define DEFERRED_SERVICE_RESPONSE_CPP_IMPORT
  #if __GNUC__ >= 4
    #define DEFERRED_SERVICE_RESPONSE_CPP_PUBLIC __attribute__ ((visibility("default")))
    #define DEFERRED_SERVICE_RESPONSE_CPP_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define DEFERRED_SERVICE_RESPONSE_CPP_PUBLIC
    #define DEFERRED_SERVICE_RESPONSE_CPP_LOCAL
  #endif
  #define DEFERRED_SERVICE_RESPONSE_CPP_PUBLIC_TYPE
#endif

#endif  // DEFERRED_SERVICE_RESPONSE_CPP__VISIBILITY_CONTROL_HPP_
