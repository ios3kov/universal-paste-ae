#pragma once

#include <cstdint>
#include <optional>

namespace universal_paste {

using ClipboardChangeToken = std::uint64_t;

enum class ClipboardOwnershipState {
  ExternalEligible,
  AwaitingNativeCopySettle,
  NativePastePreferred,
  NativeTokenUnavailable,
};

class ClipboardOwnershipTracker {
 public:
  void OnAeCopyOrCutObserved();
  void OnAeCopyOrCutSettled(std::optional<ClipboardChangeToken> token);

  bool ShouldPreferNativePaste(std::optional<ClipboardChangeToken> current_token);

  ClipboardOwnershipState state() const;
  std::optional<ClipboardChangeToken> native_token() const;
  void Reset();

 private:
  ClipboardOwnershipState state_{ClipboardOwnershipState::ExternalEligible};
  std::optional<ClipboardChangeToken> native_token_;
};

}  // namespace universal_paste
