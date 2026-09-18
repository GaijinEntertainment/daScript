#pragma once

// modules/dasGlfw/ARCHITECTURE.md#touch-list
enum DasGlfwTouchField {
    DAS_TOUCH_COUNT = 0,
    DAS_TOUCH_ID = 1,
    DAS_TOUCH_X = 2,
    DAS_TOUCH_Y = 3,
    DAS_TOUCH_BEGAN = 4,
    DAS_TOUCH_ENDED = 5
};

#ifdef __cplusplus
extern "C" {
#endif
void das_glfw_touch_poll(void);
double das_glfw_touch_query(int field, int i);
int das_glfw_is_touch_device(void);
#ifdef __cplusplus
}
#endif
