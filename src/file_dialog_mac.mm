#import "file_dialog.h"
#import <Cocoa/Cocoa.h>


std::string FileDialog::openFile() {
  NSOpenPanel* dialog = [NSOpenPanel openPanel];
  if ([dialog runModal] == NSModalResponseOK) {
    NSArray* urls = [dialog URLs];
    NSString* str = [[urls objectAtIndex:0] path];
    return [str cStringUsingEncoding:NSUTF8StringEncoding];
  }
  return "";
}
