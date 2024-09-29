#include <uipc/constitution/constraint.h>
#include <uipc/builtin/attribute_name.h>

namespace uipc::constitution
{
Constraint::Constraint() noexcept {}

void Constraint::apply_to(geometry::SimplicialComplex& sc) const
{
    auto constraint_uid = sc.meta().find<U64>(builtin::constraint_uid);
    UIPC_ASSERT(!constraint_uid, "Limitation: Now libuipc only support single constraint.");
    constraint_uid = sc.meta().create<U64>(builtin::constraint_uid, get_uid());
    geometry::view(*constraint_uid).front() = uid();
}
}  // namespace uipc::constitution
