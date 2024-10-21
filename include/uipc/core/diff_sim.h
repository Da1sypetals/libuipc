#pragma once
#include <uipc/common/macro.h>
#include <uipc/diff_sim/parameter_collection.h>
namespace uipc::backend
{
class SceneVisitor;
}
namespace uipc::core
{
class Scene;
class UIPC_CORE_API DiffSim
{
    friend class Scene;
    friend class World;

    DiffSim()  = default;
    ~DiffSim() = default;

    // delete copy constructor and assignment operator
    DiffSim(const DiffSim&)            = delete;
    DiffSim& operator=(const DiffSim&) = delete;

  public:
    diff_sim::ParameterCollection&       parameters();
    const diff_sim::ParameterCollection& parameters() const;

  private:
    void init(backend::SceneVisitor& scene);  // only be called by World.

    diff_sim::ParameterCollection m_parameters;
};
}  // namespace uipc::core
