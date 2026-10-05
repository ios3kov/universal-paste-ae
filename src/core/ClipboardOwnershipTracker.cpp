#include "universal_paste/ClipboardOwnershipTracker.hpp"

namespace universal_paste {

void ClipboardOwnershipTracker::OnAeCopyOrCutObserved() {
  state_ = ClipboardOwnershipState::AwaitingNativeCopySettle;
  native_token_.reset();
}

void ClipboardOwnershipTracker::OnAeCopyOrCutSettled(
    std::optional<ClipboardChangeToken> token) {
  if (!token) {
    state_ = ClipboardOwnershipState::NativeTokenUnavailable;
    native_token_.reset();
    return;
  }

  state_ = ClipboardOwnershipState::NativePastePreferred;
  native_token_ = token;
}

bool ClipboardOwnershipTracker::ShouldPreferNativePaste(
    std::optional<ClipboardChangeToken> current_token) {
  switch (state_) {
    case ClipboardOwnershipState::ExternalEligible:
      return false;

    case ClipboardOwnershipState::AwaitingNativeCopySettle:
    case ClipboardOwnershipState::NativeTokenUnavailable:
      return true;

    case ClipboardOwnershipState::NativePastePreferred:
      if (!current_token) {
        state_ = ClipboardOwnershipState::NativeTokenUnavailable;
        native_token_.reset();
        return true;
      }

      if (native_token_ && *native_token_ == *current_token) {
        return true;
      }

      state_ = ClipboardOwnershipState::ExternalEligible;
      native_token_.reset();
      return false;
  }

  return true;
}

ClipboardOwnershipState ClipboardOwnershipTracker::state() const {
  return state_;
}

std::optional<ClipboardChangeToken> ClipboardOwnershipTracker::native_token() const {
  return native_token_;
}

void ClipboardOwnershipTracker::Reset() {
  state_ = ClipboardOwnershipState::ExternalEligible;
  native_token_.reset();
}

}  // namespace universal_paste
