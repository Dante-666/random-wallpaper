#include "mac_impl.h"

#include <AppKit/AppKit.h>

void setDesktopBackground(const string& uri) {
  @autoreleasepool {
    NSURL *nsurl = [NSURL fileURLWithPath:[NSString stringWithUTF8String:uri.c_str()]];
    NSWorkspace *workspace = [NSWorkspace sharedWorkspace];
    //TODO: set only for the default screen and provide options
    NSScreen* screen = [NSScreen mainScreen];
    //TODO: setup options for this too, tile, etc
    NSDictionary<NSWorkspaceDesktopImageOptionKey, id> * dict = 
      [workspace desktopImageOptionsForScreen:screen];
    NSError* err;
    BOOL success;
    @try {
      success = [workspace setDesktopImageURL:nsurl forScreen:screen options:dict error:&err];
    } @catch(NSException *exp) {
      throw mac_error{[[exp description] UTF8String]};
    }
    if(not success) {
      throw mac_error{[[err description] UTF8String]};
    }
  }
}

const char *mac_error::what() const noexcept {
  return _errMsg.c_str();
}
