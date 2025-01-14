/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef GRPCXX_IMPL_CODEGEN_SLICE_H
#define GRPCXX_IMPL_CODEGEN_SLICE_H

#include <grpc++/impl/codegen/core_codegen_interface.h>
#include <grpc++/impl/codegen/string_ref.h>

namespace grpc {

inline grpc::string_ref StringRefFromSlice(const grpc_slice* slice) {
  return grpc::string_ref(
      reinterpret_cast<const char*>(GRPC_SLICE_START_PTR(*slice)),
      GRPC_SLICE_LENGTH(*slice));
}

inline grpc::string StringFromCopiedSlice(grpc_slice slice) {
  return grpc::string(reinterpret_cast<char*>(GRPC_SLICE_START_PTR(slice)),
                      GRPC_SLICE_LENGTH(slice));
}

inline grpc_slice SliceReferencingString(const grpc::string& str) {
  return g_core_codegen_interface->grpc_slice_from_static_buffer(str.data(),
                                                                 str.length());
}

inline grpc_slice SliceFromCopiedString(const grpc::string& str) {
  return g_core_codegen_interface->grpc_slice_from_copied_buffer(str.data(),
                                                                 str.length());
}

}  // namespace grpc

#endif  // GRPCXX_IMPL_CODEGEN_SLICE_H
