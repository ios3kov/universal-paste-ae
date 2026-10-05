#pragma once

#include <optional>
#include <string_view>

#include "universal_paste/ClipboardTypes.hpp"

namespace universal_paste {

std::optional<Color> ParseColor(std::string_view input);

}  // namespace universal_paste
