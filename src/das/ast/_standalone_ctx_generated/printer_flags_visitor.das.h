namespace das {
namespace printer_flags_visitor {


class Standalone : public Context {
public:
    Standalone();
    auto used_flags ( ) -> void;
};
} // namespace printer_flags_visitor
} // namespace das
