#include "universal_paste/ColorParser.hpp"

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>

namespace universal_paste {
namespace {

std::string Trim(std::string value) {
  auto not_space = [](unsigned char ch) { return !std::isspace(ch); };
  value.erase(value.begin(), std::find_if(value.begin(), value.end(), not_space));
  value.erase(std::find_if(value.rbegin(), value.rend(), not_space).base(), value.end());
  return value;
}

std::string Lower(std::string value) {
  std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
    return static_cast<char>(std::tolower(ch));
  });
  return value;
}

bool ParseDoubleStrict(const std::string& token, double* out) {
  if (!out || token.empty()) {
    return false;
  }
  errno = 0;
  char* end = nullptr;
  const double value = std::strtod(token.c_str(), &end);
  if (errno == ERANGE || end == token.c_str() || *end != '\0' || !std::isfinite(value)) {
    return false;
  }
  *out = value;
  return true;
}

bool EndsWithPercent(const std::string& token) {
  return !token.empty() && token.back() == '%';
}

bool ParsePercent01(const std::string& token, double* out) {
  if (!EndsWithPercent(token)) {
    return false;
  }
  double value = 0.0;
  if (!ParseDoubleStrict(Trim(token.substr(0, token.size() - 1)), &value) || value < 0.0 || value > 100.0) {
    return false;
  }
  *out = value / 100.0;
  return true;
}

bool ParseRgbComponent(const std::string& token, double* out) {
  if (EndsWithPercent(token)) {
    return ParsePercent01(token, out);
  }
  double value = 0.0;
  if (!ParseDoubleStrict(token, &value) || value < 0.0 || value > 255.0) {
    return false;
  }
  *out = value / 255.0;
  return true;
}

bool ParseAlpha(const std::string& token, double* out) {
  if (EndsWithPercent(token)) {
    return ParsePercent01(token, out);
  }
  double value = 0.0;
  if (!ParseDoubleStrict(token, &value) || value < 0.0 || value > 1.0) {
    return false;
  }
  *out = value;
  return true;
}

std::vector<std::string> SplitCommaList(const std::string& body) {
  std::vector<std::string> parts;
  std::size_t start = 0;
  while (start <= body.size()) {
    const auto comma = body.find(',', start);
    const auto end = comma == std::string::npos ? body.size() : comma;
    parts.push_back(Trim(body.substr(start, end - start)));
    if (comma == std::string::npos) {
      break;
    }
    start = comma + 1;
  }
  return parts;
}

int HexDigit(char ch) {
  if (ch >= '0' && ch <= '9') return ch - '0';
  ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
  if (ch >= 'a' && ch <= 'f') return 10 + (ch - 'a');
  return -1;
}

bool ParseHexByte(char hi, char lo, double* out) {
  const int h = HexDigit(hi);
  const int l = HexDigit(lo);
  if (h < 0 || l < 0) return false;
  *out = static_cast<double>((h << 4) | l) / 255.0;
  return true;
}

std::optional<Color> ParseHex(const std::string& input) {
  if (input.empty() || input.front() != '#') {
    return std::nullopt;
  }
  const std::string hex = input.substr(1);
  Color color;
  if (hex.size() == 3 || hex.size() == 4) {
    const int r = HexDigit(hex[0]);
    const int g = HexDigit(hex[1]);
    const int b = HexDigit(hex[2]);
    if (r < 0 || g < 0 || b < 0) return std::nullopt;
    color.r = static_cast<double>((r << 4) | r) / 255.0;
    color.g = static_cast<double>((g << 4) | g) / 255.0;
    color.b = static_cast<double>((b << 4) | b) / 255.0;
    if (hex.size() == 4) {
      const int a = HexDigit(hex[3]);
      if (a < 0) return std::nullopt;
      color.a = static_cast<double>((a << 4) | a) / 255.0;
    }
    return color;
  }
  if (hex.size() == 6 || hex.size() == 8) {
    if (!ParseHexByte(hex[0], hex[1], &color.r) ||
        !ParseHexByte(hex[2], hex[3], &color.g) ||
        !ParseHexByte(hex[4], hex[5], &color.b)) {
      return std::nullopt;
    }
    if (hex.size() == 8 && !ParseHexByte(hex[6], hex[7], &color.a)) {
      return std::nullopt;
    }
    return color;
  }
  return std::nullopt;
}

double HueToRgb(double p, double q, double t) {
  if (t < 0.0) t += 1.0;
  if (t > 1.0) t -= 1.0;
  if (t < 1.0 / 6.0) return p + (q - p) * 6.0 * t;
  if (t < 1.0 / 2.0) return q;
  if (t < 2.0 / 3.0) return p + (q - p) * (2.0 / 3.0 - t) * 6.0;
  return p;
}

Color HslToRgb(double hue_degrees, double saturation, double lightness, double alpha) {
  double hue = std::fmod(hue_degrees, 360.0);
  if (hue < 0.0) hue += 360.0;
  hue /= 360.0;

  Color color;
  color.a = alpha;
  if (saturation == 0.0) {
    color.r = color.g = color.b = lightness;
    return color;
  }

  const double q = lightness < 0.5
      ? lightness * (1.0 + saturation)
      : lightness + saturation - lightness * saturation;
  const double p = 2.0 * lightness - q;
  color.r = HueToRgb(p, q, hue + 1.0 / 3.0);
  color.g = HueToRgb(p, q, hue);
  color.b = HueToRgb(p, q, hue - 1.0 / 3.0);
  return color;
}

std::optional<Color> ParseFunctionColor(const std::string& input) {
  const auto open = input.find('(');
  if (open == std::string::npos || input.back() != ')') {
    return std::nullopt;
  }

  const std::string name = Lower(Trim(input.substr(0, open)));
  const std::string body = input.substr(open + 1, input.size() - open - 2);
  const auto parts = SplitCommaList(body);

  if (name == "rgb" || name == "rgba") {
    const std::size_t expected = name == "rgb" ? 3 : 4;
    if (parts.size() != expected) return std::nullopt;
    Color color;
    if (!ParseRgbComponent(parts[0], &color.r) ||
        !ParseRgbComponent(parts[1], &color.g) ||
        !ParseRgbComponent(parts[2], &color.b)) {
      return std::nullopt;
    }
    if (expected == 4 && !ParseAlpha(parts[3], &color.a)) {
      return std::nullopt;
    }
    return color;
  }

  if (name == "hsl" || name == "hsla") {
    const std::size_t expected = name == "hsl" ? 3 : 4;
    if (parts.size() != expected) return std::nullopt;
    double hue = 0.0;
    double saturation = 0.0;
    double lightness = 0.0;
    double alpha = 1.0;
    if (!ParseDoubleStrict(parts[0], &hue) ||
        !ParsePercent01(parts[1], &saturation) ||
        !ParsePercent01(parts[2], &lightness)) {
      return std::nullopt;
    }
    if (expected == 4 && !ParseAlpha(parts[3], &alpha)) {
      return std::nullopt;
    }
    return HslToRgb(hue, saturation, lightness, alpha);
  }

  return std::nullopt;
}

}  // namespace

std::optional<Color> ParseColor(std::string_view input) {
  const std::string trimmed = Trim(std::string(input));
  if (trimmed.empty()) {
    return std::nullopt;
  }
  if (trimmed.front() == '#') {
    return ParseHex(trimmed);
  }
  return ParseFunctionColor(trimmed);
}

}  // namespace universal_paste
