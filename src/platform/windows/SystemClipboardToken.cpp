#include "universal_paste/SystemClipboardToken.hpp"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

namespace universal_paste {

std::optional<ClipboardChangeToken> ReadSystemClipboardChangeToken() {
  // GetClipboardSequenceNumber does not require opening the clipboard.
  // A zero value can mean access is unavailable, but treating it as an opaque
  // token is conservative: native AE paste remains preferred after AE Copy/Cut
  // until a distinct value is observed.
  return static_cast<ClipboardChangeToken>(GetClipboardSequenceNumber());
}

}  // namespace universal_paste
