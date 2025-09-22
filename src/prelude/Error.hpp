#ifndef PRELUDE_ERROR_HPP
#define PRELUDE_ERROR_HPP

#include <cstddef>

namespace prelude {

// An error type which can optionally store a payload of type `T`.
template <typename ErrorCode, typename Payload = std::nullptr_t>
struct Error {
  ErrorCode errorCode;
  Payload payload;
};

}  // namespace prelude

#endif  // PRELUDE_ERROR_HPP
