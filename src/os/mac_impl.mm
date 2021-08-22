#include "mac_impl.h"

#include <AppKit/AppKit.h>

bool setDesktopBackground(const string& uri) {
  @autoreleasepool {
    NSString* NSuri = [NSString stringWithUTF8String:uri.c_str()];
    NSURL *url = [NSURL URLWithString:NSuri];
    if( ![[NSWorkspace sharedWorkspace] openURL:url] )
      NSLog(@"Failed to open url: %@",[url description]);
    return true;
  }
}

