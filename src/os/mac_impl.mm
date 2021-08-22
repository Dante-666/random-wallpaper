#include "mac_impl.h"

#include <AppKit/AppKit.h>

bool setDesktopBackground(const string& uri, const char* tmpDir, const bool& isWebURL) {
  @autoreleasepool {
    NSURL *nsurl;
    NSWorkspace *workspace = [NSWorkspace sharedWorkspace];
    if(isWebURL) {
      nsurl = [NSURL URLWithString:[NSString stringWithUTF8String:uri.c_str()]];
      NSURL *tmpDirUrl = [NSURL fileURLWithPath:[NSString stringWithUTF8String:tmpDir]];
    } else {
      nsurl = [NSURL fileURLWithPath:[NSString stringWithUTF8String:uri.c_str()]];
    }
    //TODO: set only for the default screen and provide options
    NSScreen* screen = [NSScreen mainScreen];
    //TODO: setup options for this too, tile, etc
    NSDictionary<NSWorkspaceDesktopImageOptionKey, id> * dict = 
      [workspace desktopImageOptionsForScreen:screen];
    NSError* err;
    @try {
      [workspace setDesktopImageURL:nsurl forScreen:screen options:dict error:&err];
    } @catch(NSException *e) {
      NSLog(@"Exception: %@", e);
      //throw url_error{};
    }
    if(err) {
      NSLog(@"%@", [err description]);
      //throw url_error{};
    }
    return true;
  }
}

const char *url_error::what() const noexcept {
  return "couldn't access web resource";
}

