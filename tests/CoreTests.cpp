#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

#include "universal_paste/ClipboardClassifier.hpp"
#include "universal_paste/ClipboardOwnershipTracker.hpp"
#include "universal_paste/ColorParser.hpp"

using universal_paste::ClassifyClipboard;
using universal_paste::ClipboardKind;
using universal_paste::ClipboardOwnershipState;
using universal_paste::ClipboardOwnershipTracker;
using universal_paste::ClipboardSnapshot;
using universal_paste::ParseColor;

namespace {

bool Near(double a, double b, double epsilon = 1e-6) {
  return std::fabs(a - b) <= epsilon;
}

void TestHex() {
  auto c = ParseColor("#f0a");
  assert(c);
  assert(Near(c->r, 1.0));
  assert(Near(c->g, 0.0));
  assert(Near(c->b, 170.0 / 255.0));
  assert(Near(c->a, 1.0));

  c = ParseColor("#33669980");
  assert(c);
  assert(Near(c->r, 0x33 / 255.0));
  assert(Near(c->g, 0x66 / 255.0));
  assert(Near(c->b, 0x99 / 255.0));
  assert(Near(c->a, 0x80 / 255.0));
}

void TestRgb() {
  auto c = ParseColor("rgb(255, 0, 128)");
  assert(c);
  assert(Near(c->r, 1.0));
  assert(Near(c->g, 0.0));
  assert(Near(c->b, 128.0 / 255.0));

  c = ParseColor(" RGBA(100%, 0%, 50%, 25%) ");
  assert(c);
  assert(Near(c->r, 1.0));
  assert(Near(c->g, 0.0));
  assert(Near(c->b, 0.5));
  assert(Near(c->a, 0.25));
}

void TestHsl() {
  auto c = ParseColor("hsl(0, 100%, 50%)");
  assert(c);
  assert(Near(c->r, 1.0));
  assert(Near(c->g, 0.0));
  assert(Near(c->b, 0.0));

  c = ParseColor("hsl(120, 100%, 25%)");
  assert(c);
  assert(Near(c->r, 0.0));
  assert(Near(c->g, 0.5));
  assert(Near(c->b, 0.0));

  c = ParseColor("hsla(-240, 100%, 50%, 0.5)");
  assert(c);
  assert(Near(c->r, 0.0));
  assert(Near(c->g, 1.0));
  assert(Near(c->b, 0.0));
  assert(Near(c->a, 0.5));
}

void TestRejects() {
  assert(!ParseColor(""));
  assert(!ParseColor("hello #fff"));
  assert(!ParseColor("rgb(999, 0, 0)"));
  assert(!ParseColor("rgba(0, 0, 0, 2)"));
  assert(!ParseColor("hsl(0, 100, 50)"));
  assert(!ParseColor("hsl(0, 101%, 50%)"));
  assert(!ParseColor("#12"));
  assert(!ParseColor("#ggg"));
}

void TestClassification() {
  ClipboardSnapshot snapshot;
  auto result = ClassifyClipboard(snapshot);
  assert(result.primary == ClipboardKind::None);

  snapshot.text = std::string("Hello");
  result = ClassifyClipboard(snapshot);
  assert(result.primary == ClipboardKind::Text);

  snapshot.text = std::string("#ff0000");
  result = ClassifyClipboard(snapshot);
  assert(result.primary == ClipboardKind::Color);
  assert(result.parsed_color);
  assert(result.alternatives.size() == 1);
  assert(result.alternatives[0] == ClipboardKind::Text);

  snapshot.has_image = true;
  result = ClassifyClipboard(snapshot);
  assert(result.primary == ClipboardKind::Image);
  assert(result.alternatives.size() == 2);
  assert(result.alternatives[0] == ClipboardKind::Color);
  assert(result.alternatives[1] == ClipboardKind::Text);

  snapshot.files = {"a.png", "b.png"};
  result = ClassifyClipboard(snapshot);
  assert(result.primary == ClipboardKind::Files);
  assert(result.alternatives.size() == 3);
  assert(result.alternatives[0] == ClipboardKind::Image);
  assert(result.alternatives[1] == ClipboardKind::Color);
  assert(result.alternatives[2] == ClipboardKind::Text);
}

void TestNativeClipboardOwnership() {
  ClipboardOwnershipTracker tracker;

  assert(tracker.state() == ClipboardOwnershipState::ExternalEligible);
  assert(!tracker.ShouldPreferNativePaste(10));

  tracker.OnAeCopyOrCutObserved();
  assert(tracker.state() == ClipboardOwnershipState::AwaitingNativeCopySettle);
  assert(tracker.ShouldPreferNativePaste(10));

  tracker.OnAeCopyOrCutSettled(11);
  assert(tracker.state() == ClipboardOwnershipState::NativePastePreferred);
  assert(tracker.native_token() == 11);
  assert(tracker.ShouldPreferNativePaste(11));
  assert(tracker.ShouldPreferNativePaste(11));

  assert(!tracker.ShouldPreferNativePaste(12));
  assert(tracker.state() == ClipboardOwnershipState::ExternalEligible);
  assert(!tracker.native_token());
}

void TestNativeClipboardOwnershipUnknownTokenFailsSafe() {
  ClipboardOwnershipTracker tracker;

  tracker.OnAeCopyOrCutObserved();
  tracker.OnAeCopyOrCutSettled(std::nullopt);
  assert(tracker.state() == ClipboardOwnershipState::NativeTokenUnavailable);
  assert(tracker.ShouldPreferNativePaste(std::nullopt));
  assert(tracker.ShouldPreferNativePaste(50));

  tracker.Reset();
  assert(tracker.state() == ClipboardOwnershipState::ExternalEligible);
  assert(!tracker.ShouldPreferNativePaste(50));
}

void TestTokenLossAfterNativeCopyFailsSafe() {
  ClipboardOwnershipTracker tracker;

  tracker.OnAeCopyOrCutObserved();
  tracker.OnAeCopyOrCutSettled(100);
  assert(tracker.ShouldPreferNativePaste(std::nullopt));
  assert(tracker.state() == ClipboardOwnershipState::NativeTokenUnavailable);
}

}  // namespace

int main() {
  TestHex();
  TestRgb();
  TestHsl();
  TestRejects();
  TestClassification();
  TestNativeClipboardOwnership();
  TestNativeClipboardOwnershipUnknownTokenFailsSafe();
  TestTokenLossAfterNativeCopyFailsSafe();
  std::cout << "Universal Paste core tests passed\n";
  return 0;
}
