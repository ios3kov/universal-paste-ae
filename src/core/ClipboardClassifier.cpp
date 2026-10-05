#include "universal_paste/ClipboardClassifier.hpp"

#include <algorithm>

#include "universal_paste/ColorParser.hpp"

namespace universal_paste {
namespace {

void AddAlternative(std::vector<ClipboardKind>* alternatives, ClipboardKind kind) {
  if (!alternatives || kind == ClipboardKind::None) return;
  if (std::find(alternatives->begin(), alternatives->end(), kind) == alternatives->end()) {
    alternatives->push_back(kind);
  }
}

}  // namespace

Classification ClassifyClipboard(const ClipboardSnapshot& snapshot) {
  Classification result;
  const bool has_files = !snapshot.files.empty();
  const bool has_text = snapshot.text.has_value() && !snapshot.text->empty();
  const auto parsed_color = has_text ? ParseColor(*snapshot.text) : std::nullopt;

  if (has_files) {
    result.primary = ClipboardKind::Files;
    if (snapshot.has_image) AddAlternative(&result.alternatives, ClipboardKind::Image);
    if (parsed_color) AddAlternative(&result.alternatives, ClipboardKind::Color);
    if (has_text) AddAlternative(&result.alternatives, ClipboardKind::Text);
  } else if (snapshot.has_image) {
    result.primary = ClipboardKind::Image;
    if (parsed_color) AddAlternative(&result.alternatives, ClipboardKind::Color);
    if (has_text) AddAlternative(&result.alternatives, ClipboardKind::Text);
  } else if (parsed_color) {
    result.primary = ClipboardKind::Color;
    result.parsed_color = parsed_color;
    AddAlternative(&result.alternatives, ClipboardKind::Text);
  } else if (has_text) {
    result.primary = ClipboardKind::Text;
  }

  if (parsed_color && !result.parsed_color) {
    result.parsed_color = parsed_color;
  }

  return result;
}

}  // namespace universal_paste
