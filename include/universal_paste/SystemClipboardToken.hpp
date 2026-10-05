#pragma once

#include <optional>

#include "universal_paste/ClipboardOwnershipTracker.hpp"

namespace universal_paste {

// Returns a monotonic-ish platform clipboard change token for equality checks.
// The value is intentionally opaque: callers must only compare tokens from the
// same process session, not assume ordering or persistence across restarts.
std::optional<ClipboardChangeToken> ReadSystemClipboardChangeToken();

}  // namespace universal_paste
