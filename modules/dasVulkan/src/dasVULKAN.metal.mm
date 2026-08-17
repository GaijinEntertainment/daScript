// macOS Metal surface helper. GLFW hands us the NSWindow (glfwGetCocoaWindow); a
// VkSurfaceKHR on MoltenVK is created from a CAMetalLayer via VK_EXT_metal_surface.
// GLFW does not layer-back the content view itself when the window is created with
// GLFW_NO_API, so we attach a CAMetalLayer here (idempotent) and hand it back to
// the C++ side, which calls vkCreateMetalSurfaceEXT. Objective-C++ keeps the Cocoa
// / QuartzCore dependency isolated to this one translation unit.

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>

namespace das {

// Attach (once) a CAMetalLayer to the NSWindow's content view and return it. The
// contentsScale is set to the window's backing scale so the drawable is sized in
// pixels (retina-correct). Must be called on the main thread (Cocoa requirement).
void * das_metal_layer_for_nswindow(void * ns_window) {
    NSWindow * window = (NSWindow *) ns_window;
    NSView * view = [window contentView];
    if (![view.layer isKindOfClass:[CAMetalLayer class]]) {
        CAMetalLayer * layer = [CAMetalLayer layer];
        layer.contentsScale = window.backingScaleFactor;
        [view setWantsLayer:YES];
        [view setLayer:layer];
    }
    return (void *) view.layer;
}

}
