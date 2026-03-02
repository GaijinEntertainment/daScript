options gen2
options persistent_heap
options gc

require glfw/glfw_live

[export]
def main {
    glfw_live(
        "GLFW demo in live mode",
        "example_app.das",
        "{get_das_root()}/modules/dasGlfw/framework"
    )
}
