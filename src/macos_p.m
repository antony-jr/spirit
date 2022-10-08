#include <Carbon/Carbon.h>
#import <objc/Object.h>
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <AppKit/AppKit.h>
#import <ApplicationServices/ApplicationServices.h>

#include <string.h>

CFDictionaryRef bounds = NULL;

void hide_from_dock() {
  ProcessSerialNumber psn = {0, kCurrentProcess};
  TransformProcessType(&psn, kProcessTransformToUIElementApplication);
}

// Derived From
// MaCoPiX : Mascot Constructive Pilot for X
//    macos_getwin.c
//       Get Window Info in macOS
//                                                     2019.6 K.Chimarie
void macos_get_active_window(int *x, int *y, int *w, int *h, char **window_title) {
  *x=-1;
  *y=-1;
  *w=-1;
  *h=-1;
  *window_title = NULL;

  CFArrayRef windowArray = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
  NSArray* windows = CFBridgingRelease(windowArray);
  NSUInteger count = [windows count];
  static CGRect rect;
 
  for (NSUInteger i = 0; i < count; i++){
    NSDictionary*   nswindowsdescription = [windows objectAtIndex:i];
    NSNumber* windowid = (NSNumber*)[nswindowsdescription objectForKey:@"kCGWindowNumber"];
    NSString* windowName = (NSString*)[nswindowsdescription objectForKey:@"kCGWindowOwnerName"];
    CFDictionaryRef windowdescription = (CFDictionaryRef)CFArrayGetValueAtIndex ((CFArrayRef)windowArray, i);
    
    if(windowid){
	int layer;
	CFNumberGetValue(CFDictionaryGetValue(windowdescription, kCGWindowLayer), kCFNumberIntType, &layer);
	if(layer==0){
	  CFDictionaryRef bounds = (CFDictionaryRef)CFDictionaryGetValue (windowdescription, kCGWindowBounds);
	  if(bounds){
	    CGRectMakeWithDictionaryRepresentation(bounds, &rect);
	    *x=(int)rect.origin.x;
	    *y=(int)rect.origin.y;
	    *w=(int)rect.size.width;
	    *h=(int)rect.size.height;
	    const char *source = [windowName UTF8String];
	    *window_title = strdup(source);
 
	    if((x!=0)||(y!=0)){
	      break;
	    }
	    CFRelease(bounds);
	  }
	}
    }
  }
}
