
#ifndef INCLUDED_U5E_CODEPOINT_HPP
#define INCLUDED_U5E_CODEPOINT_HPP

#include <u5e/codepoint_traits.hpp>

namespace u5e {
  /**
   * \brief Native representation of a codepoint
   *
   * Explicity class in order to hijack overloads, such that we only
   * build codepoints out of known encodings and we only write to
   * encodings out of known codepoints.
   */
  class codepoint {
  public:
    /**
     * A codepoint has an integer value type.
     */
    codepoint_traits::int_type value;

    /**
     * Default constructor, starts as NULL.
     */
    constexpr codepoint() : value(0) { };

    /**
     * Implicit constructor from an integer value.
     */
    constexpr codepoint(int32_t v) : value(v) { };

    /**
     * Override int operator to return the codepoint value.
     */
    constexpr operator int() const { return value; };

  	constexpr bool operator==(const codepoint& b) { return value == b.value; };
  };
}

#endif
