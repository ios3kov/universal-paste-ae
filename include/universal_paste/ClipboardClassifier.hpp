#pragma once

#include "universal_paste/ClipboardTypes.hpp"

namespace universal_paste {

Classification ClassifyClipboard(const ClipboardSnapshot& snapshot);

}  // namespace universal_paste
