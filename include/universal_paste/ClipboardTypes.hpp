#pragma once

#include <optional>
#include <string>
#include <vector>

namespace universal_paste {

struct Color {
  double r{0.0};
  double g{0.0};
  double b{0.0};
  double a{1.0};
};

enum class ClipboardKind {
  None,
  Files,
  Image,
  Color,
  Text,
};

struct ClipboardSnapshot {
  std::vector<std::string> files;
  bool has_image{false};
  std::optional<std::string> text;
};

struct Classification {
  ClipboardKind primary{ClipboardKind::None};
  std::vector<ClipboardKind> alternatives;
  std::optional<Color> parsed_color;
};

}  // namespace universal_paste
