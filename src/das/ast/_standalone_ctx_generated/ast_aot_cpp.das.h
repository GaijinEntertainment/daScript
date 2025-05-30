namespace das {
namespace ast_aot_cpp {


class Standalone : public Context {
public:
    Standalone();
    auto aot ( char * const  input, bool isAotLib, bool paranoid_validation, bool cross_platform, CodeOfPolicies const  & cop ) -> char *;
};
} // namespace ast_aot_cpp
} // namespace das
