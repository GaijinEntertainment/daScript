// fixture for test_frame_pos_unstamp.cpp - see the .cpp for the scenario
options gen2
options gc

[export]
def stamped_position() : int {
    return frame_position()
}
