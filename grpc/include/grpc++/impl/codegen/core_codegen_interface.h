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

#ifndef GRPCXX_IMPL_CODEGEN_CORE_CODEGEN_INTERFACE_H
#define GRPCXX_IMPL_CODEGEN_CORE_CODEGEN_INTERFACE_H

#include <grpc++/impl/codegen/config.h>
#include <grpc++/impl/codegen/status.h>
#include <grpc/impl/codegen/byte_buffer_reader.h>
#include <grpc/impl/codegen/grpc_types.h>
#include <grpc/impl/codegen/sync.h>

extern "C" {
struct grpc_byte_buffer;
}

namespace grpc {

/// Interface between the codegen library and the minimal subset of core
/// features required by the generated code.
///
/// All undocumented methods are simply forwarding the call to their namesakes.
/// Please refer to their corresponding documentation for details.
///
/// \warning This interface should be considered internal and private.
class CoreCodegenInterface {
 public:
  /// Upon a failed assertion, log the error.
  virtual void assert_fail(const char* failed_assertion, const char* file,
                           int line) = 0;

  virtual const grpc_completion_queue_factory*
  grpc_completion_queue_factory_lookup(
      const grpc_completion_queue_attributes* attributes) = 0;
  virtual grpc_completion_queue* grpc_completion_queue_create(
      const grpc_completion_queue_factory* factory,
      const grpc_completion_queue_attributes* attributes, void* reserved) = 0;
  virtual grpc_completion_queue* grpc_completion_queue_create_for_next(
      void* reserved) = 0;
  virtual grpc_completion_queue* grpc_completion_queue_create_for_pluck(
      void* reserved) = 0;
  virtual void grpc_completion_queue_destroy(grpc_completion_queue* cq) = 0;
  virtual grpc_event grpc_completion_queue_pluck(grpc_completion_queue* cq,
                                                 void* tag,
                                                 gpr_timespec deadline,
                                                 void* reserved) = 0;

  virtual void* gpr_malloc(size_t size) = 0;
  virtual void gpr_free(void* p) = 0;

  virtual void gpr_mu_init(gpr_mu* mu) = 0;
  virtual void gpr_mu_destroy(gpr_mu* mu) = 0;
  virtual void gpr_mu_lock(gpr_mu* mu) = 0;
  virtual void gpr_mu_unlock(gpr_mu* mu) = 0;
  virtual void gpr_cv_init(gpr_cv* cv) = 0;
  virtual void gpr_cv_destroy(gpr_cv* cv) = 0;
  virtual int gpr_cv_wait(gpr_cv* cv, gpr_mu* mu,
                          gpr_timespec abs_deadline) = 0;
  virtual void gpr_cv_signal(gpr_cv* cv) = 0;
  virtual void gpr_cv_broadcast(gpr_cv* cv) = 0;

  virtual void grpc_byte_buffer_destroy(grpc_byte_buffer* bb) = 0;

  virtual int grpc_byte_buffer_reader_init(grpc_byte_buffer_reader* reader,
                                           grpc_byte_buffer* buffer)
      GRPC_MUST_USE_RESULT = 0;
  virtual void grpc_byte_buffer_reader_destroy(
      grpc_byte_buffer_reader* reader) = 0;
  virtual int grpc_byte_buffer_reader_next(grpc_byte_buffer_reader* reader,
                                           grpc_slice* slice) = 0;

  virtual grpc_byte_buffer* grpc_raw_byte_buffer_create(grpc_slice* slice,
                                                        size_t nslices) = 0;
  virtual grpc_slice grpc_slice_new_with_user_data(void* p, size_t len,
                                                   void (*destroy)(void*),
                                                   void* user_data) = 0;
  virtual void grpc_call_ref(grpc_call* call) = 0;
  virtual void grpc_call_unref(grpc_call* call) = 0;
  virtual void* grpc_call_arena_alloc(grpc_call* call, size_t length) = 0;
  virtual grpc_slice grpc_empty_slice() = 0;
  virtual grpc_slice grpc_slice_malloc(size_t length) = 0;
  virtual void grpc_slice_unref(grpc_slice slice) = 0;
  virtual grpc_slice grpc_slice_ref(grpc_slice slice) = 0;
  virtual grpc_slice grpc_slice_split_tail(grpc_slice* s, size_t split) = 0;
  virtual grpc_slice grpc_slice_split_head(grpc_slice* s, size_t split) = 0;
  virtual void grpc_slice_buffer_add(grpc_slice_buffer* sb,
                                     grpc_slice slice) = 0;
  virtual void grpc_slice_buffer_pop(grpc_slice_buffer* sb) = 0;
  virtual grpc_slice grpc_slice_from_static_buffer(const void* buffer,
                                                   size_t length) = 0;
  virtual grpc_slice grpc_slice_from_copied_buffer(const void* buffer,
                                                   size_t length) = 0;

  virtual void grpc_metadata_array_init(grpc_metadata_array* array) = 0;
  virtual void grpc_metadata_array_destroy(grpc_metadata_array* array) = 0;

  virtual const Status& ok() = 0;
  virtual const Status& cancelled() = 0;

  virtual gpr_timespec gpr_inf_future(gpr_clock_type type) = 0;
  virtual gpr_timespec gpr_time_0(gpr_clock_type type) = 0;
};

extern CoreCodegenInterface* g_core_codegen_interface;

/// Codegen specific version of \a GPR_ASSERT.
#define GPR_CODEGEN_ASSERT(x)                                              \
  do {                                                                     \
    if (!(x)) {                                                            \
      grpc::g_core_codegen_interface->assert_fail(#x, __FILE__, __LINE__); \
    }                                                                      \
  } while (0)

}  // namespace grpc

#endif  // GRPCXX_IMPL_CODEGEN_CORE_CODEGEN_INTERFACE_H
