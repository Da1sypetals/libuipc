#pragma once
#include <type_define.h>
#include <uipc/backend/macro.h>
#include <uipc/engine/engine.h>
#include <sstream>
#include <sim_system_collection.h>
#include <sim_action.h>
#include <sim_engine_state.h>
#include <i_sim_system.h>

namespace uipc::backend::cuda
{
class GlobalVertexManager;
class GlobalSimpicialSurfaceManager;
class GlobalContactManager;
class GlobalDCDFilter;
class GlobalCCDFilter;

class DoFPredictor;
class LineSearcher;
class GradientHessianComputer;
class GlobalLinearSystem;

class UIPC_BACKEND_API SimEngine : public engine::IEngine
{
    class DeviceImpl;
    friend class SimSystem;

  public:
    SimEngine();
    ~SimEngine();

    SimEngine(const SimEngine&)            = delete;
    SimEngine& operator=(const SimEngine&) = delete;

    WorldVisitor&  world() noexcept;
    SimEngineState state() const noexcept;

  protected:
    virtual void do_init(backend::WorldVisitor v) override;
    virtual void do_advance() override;
    virtual void do_sync() override;
    virtual void do_retrieve() override;
    virtual Json do_to_json() const override;

  private:
    void build();
    void init_scene();
    void register_all_systems();  // called in do_init() only.

    DeviceImpl&         device_impl() noexcept;
    U<DeviceImpl>       m_device_impl;
    std::stringstream   m_string_stream;
    U<WorldVisitor>     m_world_visitor;
    SimSystemCollection m_system_collection;
    SimEngineState      m_state = SimEngineState::None;

    // Events
    list<SimAction> m_on_init_scene;
    void            event_init_scene();
    list<SimAction> m_on_rebuild_scene;
    void            event_rebuild_scene();
    list<SimAction> m_on_write_scene;
    void            event_write_scene();

    // Utilities
    template <std::derived_from<ISimSystem> T>
    T* find();

  private:
    // Aware Top Systems
    GlobalVertexManager*           m_global_vertex_manager  = nullptr;
    GlobalSimpicialSurfaceManager* m_global_surface_manager = nullptr;
    GlobalContactManager*          m_global_contact_manager = nullptr;
    GlobalDCDFilter*               m_global_dcd_filter      = nullptr;
    GlobalCCDFilter*               m_global_ccd_filter      = nullptr;

    DoFPredictor*            m_dof_predictor             = nullptr;
    LineSearcher*            m_line_searcher             = nullptr;
    GradientHessianComputer* m_gradient_hessian_computer = nullptr;
    GlobalLinearSystem*      m_global_linear_system      = nullptr;


    Float m_abs_tol         = 0.0;
    Float m_newton_tol      = 1e-3;
    SizeT m_newton_max_iter = 1000;
    SizeT m_current_frame   = 0;
};
}  // namespace uipc::backend::cuda

#include "details/sim_engine.inl"