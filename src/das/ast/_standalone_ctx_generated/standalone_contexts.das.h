namespace das {
namespace standalone_contexts {


class Standalone : public Context {
public:
    Standalone();
    auto standalone_aot ( char * const  input, char * const  output_dir, bool isAotLib, bool cross_platform, bool paranoid_validation, CodeOfPolicies const  & cop ) -> char *;
};
} // namespace standalone_contexts
} // namespace das
