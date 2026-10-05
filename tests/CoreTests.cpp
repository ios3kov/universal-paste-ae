#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

#include "universal_paste/ClipboardClassifier.hpp"
#include "universal_paste/ColorParser.hpp"

using universal_paste::ClassifyClipboard;
using universal_paste::ClipboardKind;
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

  c = ParseColor("rgba(100%, 0%, 50%, 25%)");
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
}

void TestRejects() {
  assert(!ParseColor("hello #fff"));
  assert(!ParseColor("rgb(999, 0, 0)"));
  assert(!ParseColor("hsl(0, 100, 50)"));
  assert(!ParseColor("#12"));
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

}  // namespace

int main() {
  TestHex();
  TestRgb();
  TestHsl();
  TestRejects();
  TestClassification();
  std::cout << "Universal Paste core tests passed\n";
  return 0;
}
