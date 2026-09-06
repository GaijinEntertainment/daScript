#include "daScript/misc/platform.h"

#include "dasStdDlgTray.h"

#import <Cocoa/Cocoa.h>
#include <ApplicationServices/ApplicationServices.h>

namespace das {
    class OsxTray;
}

@interface DasTrayTarget : NSObject
@property (nonatomic, assign) das::OsxTray * tray;
- (void)statusItemClicked:(id)sender;
- (void)menuItemClicked:(id)sender;
@end

namespace das {

    class OsxTray final : public TrayBackend {
    public:
        NSStatusItem * item = nil;
        NSMenu * menu = nil;
        DasTrayTarget * target = nil;
        vector<TrayEvent> events;

        virtual ~OsxTray() override {
            if ( item ) {
                [[NSStatusBar systemStatusBar] removeStatusItem:item];
                [item release];
            }
            [menu release];
            target.tray = nullptr;
            [target release];
        }

        virtual bool create(const char * tooltip) override {
            [NSApplication sharedApplication];
            [NSApp setActivationPolicy:NSApplicationActivationPolicyAccessory];
            [NSApp finishLaunching];
            item = [[[NSStatusBar systemStatusBar] statusItemWithLength:NSSquareStatusItemLength] retain];
            if ( !item ) return false;
            target = [[DasTrayTarget alloc] init];
            target.tray = this;
            item.button.toolTip = [NSString stringWithUTF8String:tooltip];
            item.button.target = target;
            item.button.action = @selector(statusItemClicked:);
            [item.button sendActionOn:(NSEventMaskLeftMouseUp | NSEventMaskRightMouseUp)];
            menu = [[NSMenu alloc] initWithTitle:@""];
            menu.autoenablesItems = NO;
            return true;
        }

        virtual void setIcon(const uint8_t * rgba8, int32_t width, int32_t height) override {
            NSBitmapImageRep * rep = [[NSBitmapImageRep alloc]
                initWithBitmapDataPlanes:NULL pixelsWide:width pixelsHigh:height
                bitsPerSample:8 samplesPerPixel:4 hasAlpha:YES isPlanar:NO
                colorSpaceName:NSCalibratedRGBColorSpace
                bitmapFormat:NSBitmapFormatAlphaNonpremultiplied
                bytesPerRow:width * 4 bitsPerPixel:32];
            memcpy(rep.bitmapData, rgba8, size_t(width) * size_t(height) * 4);
            NSImage * image = [[NSImage alloc] initWithSize:NSMakeSize(18, 18)];
            [image addRepresentation:rep];
            [rep release];
            item.button.image = image;
            [image release];
        }

        virtual void setTooltip(const char * text) override {
            item.button.toolTip = [NSString stringWithUTF8String:text];
        }

        virtual void setMenu(const vector<TrayMenuEntry> & entries) override {
            [menu removeAllItems];
            for ( const auto & e : entries ) {
                if ( e.separator ) {
                    [menu addItem:[NSMenuItem separatorItem]];
                    continue;
                }
                NSMenuItem * mi = [[NSMenuItem alloc]
                    initWithTitle:[NSString stringWithUTF8String:e.label.c_str()]
                    action:@selector(menuItemClicked:) keyEquivalent:@""];
                mi.target = target;
                mi.tag = e.id;
                mi.enabled = e.enabled;
                mi.state = e.checked ? NSControlStateValueOn : NSControlStateValueOff;
                [menu addItem:mi];
                [mi release];
            }
        }

        virtual void poll(vector<TrayEvent> & out) override {
            @autoreleasepool {
                for ( ;; ) {
                    NSEvent * ev = [NSApp nextEventMatchingMask:NSEventMaskAny
                        untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES];
                    if ( !ev ) break;
                    [NSApp sendEvent:ev];
                }
                [NSApp updateWindows];
            }
            out.swap(events);
            events.clear();
        }

        virtual bool notify(const char * title, const char * body) override {
            @autoreleasepool {
                NSTask * task = [[[NSTask alloc] init] autorelease];
                task.launchPath = @"/usr/bin/osascript";
                NSString * script = [NSString stringWithFormat:@"display notification \"%@\" with title \"%@\"",
                    escapeForAppleScript(body), escapeForAppleScript(title)];
                task.arguments = @[@"-e", script];
                @try {
                    [task launch];
                } @catch ( NSException * ) {
                    return false;
                }
                return true;
            }
        }

        static NSString * escapeForAppleScript(const char * text) {
            NSString * s = [NSString stringWithUTF8String:text];
            s = [s stringByReplacingOccurrencesOfString:@"\\" withString:@"\\\\"];
            return [s stringByReplacingOccurrencesOfString:@"\"" withString:@"\\\""];
        }

        void onStatusItemClicked() {
            NSEvent * ev = NSApp.currentEvent;
            NSPoint p = ev ? [NSEvent mouseLocation] : NSMakePoint(0, 0);
            NSArray<NSScreen *> * screens = [NSScreen screens];
            if ( ev && screens.count > 0 ) p.y = screens[0].frame.size.height - p.y;
            bool rightClick = ev && (ev.type == NSEventTypeRightMouseUp || (ev.modifierFlags & NSEventModifierFlagControl));
            if ( rightClick ) {
                events.push_back(TrayEvent{TrayEventKind::right_click, 0, int32_t(p.x), int32_t(p.y)});
                if ( menu.numberOfItems > 0 ) {
                    item.menu = menu;
                    [item.button performClick:nil];
                    item.menu = nil;
                }
            } else {
                TrayEventKind kind = (ev && ev.clickCount >= 2) ? TrayEventKind::double_click : TrayEventKind::click;
                events.push_back(TrayEvent{kind, 0, int32_t(p.x), int32_t(p.y)});
            }
        }

        void onMenuItemClicked(int32_t id) {
            events.push_back(TrayEvent{TrayEventKind::menu, id, 0, 0});
        }
    };

    bool TrayPlatformAvailable() {
        if ( ![NSThread isMainThread] ) return false;
        CFDictionaryRef session = CGSessionCopyCurrentDictionary();
        if ( !session ) return false;
        CFRelease(session);
        return true;
    }

    TrayBackend * TrayPlatformCreate() {
        if ( !TrayPlatformAvailable() ) return nullptr;
        return new OsxTray();
    }
}

@implementation DasTrayTarget
- (void)statusItemClicked:(id)sender {
    (void) sender;
    if ( self.tray ) self.tray->onStatusItemClicked();
}
- (void)menuItemClicked:(id)sender {
    if ( self.tray ) self.tray->onMenuItemClicked(int32_t([(NSMenuItem *) sender tag]));
}
@end
