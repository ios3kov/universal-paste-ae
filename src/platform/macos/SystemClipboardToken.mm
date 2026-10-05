#include "universal_paste/SystemClipboardToken.hpp"

#import <AppKit/NSPasteboard.h>

namespace universal_paste {

std::optional<ClipboardChangeToken> ReadSystemClipboardChangeToken() {
  @autoreleasepool {
    NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
    const NSInteger change_count = [pasteboard changeCount];
    return static_cast<ClipboardChangeToken>(change_count);
  }
}

}  // namespace universal_paste
