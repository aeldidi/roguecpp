#ifndef PRELUDE_OPTION_HPP
#define PRELUDE_OPTION_HPP

#include "macros.hpp"
#include "panic.hpp"

namespace prelude {

// A type which may contain a value of type `T`, or may not.
template <typename T>
struct Option {
  // Returns an option containing no value.
  static Option<T> None() { return Option(); }

  // Returns an `Option` containing the value x.
  static Option<T> Some(T&& x) { return Option(x); }

  // Returns the value contained within, panicking if the value is
  // `Option.None`.
  T unwrap() const {
    if (!this->set) {
      panic("tried to unwrap an optional which did not contain a value");
    }

    return MOV(this->value);
  }

  // Returns true if the option contains a value.
  bool isSome() const { return this->set; }

 private:
  Option(T&& x) : value(x), set(true) {}
  Option() = default;
  bool set = false;
  T value;
};

}  // namespace prelude

#endif  // PRELUDE_OPTION_HPP
