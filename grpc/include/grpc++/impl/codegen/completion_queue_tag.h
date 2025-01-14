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

#ifndef GRPCXX_IMPL_CODEGEN_COMPLETION_QUEUE_TAG_H
#define GRPCXX_IMPL_CODEGEN_COMPLETION_QUEUE_TAG_H

namespace grpc {

namespace internal {
/// An interface allowing implementors to process and filter event tags.
class CompletionQueueTag {
 public:
  virtual ~CompletionQueueTag() {}
  /// Called prior to returning from Next(), return value is the status of the
  /// operation (return status is the default thing to do). If this function
  /// returns false, the tag is dropped and not returned from the completion
  /// queue
  virtual bool FinalizeResult(void** tag, bool* status) = 0;
};
}  // namespace internal

}  // namespace grpc

#endif  // GRPCXX_IMPL_CODEGEN_COMPLETION_QUEUE_TAG_H
