#include <catch.hpp>
#include <app/asset_dir.h>
#include <uipc/uipc.h>
#include <uipc/constitution/affine_body_constitution.h>
#include <uipc/constitution/soft_transform_constraint.h>
#include <filesystem>
#include <fstream>
#include <numbers>

TEST_CASE("25_abd_animated_rotation", "[animation]")
{
    using namespace uipc;
    using namespace uipc::geometry;
    using namespace uipc::world;
    using namespace uipc::constitution;
    using namespace uipc::engine;
    namespace fs = std::filesystem;

    std::string tetmesh_dir{AssetDir::tetmesh_path()};
    auto        this_output_path = AssetDir::output_path(__FILE__);


    Engine engine{"cuda", this_output_path};
    World  world{engine};

    auto config = Scene::default_config();

    Float dt                            = 0.01;
    config["gravity"]                   = Vector3{0, -9.8, 0};
    config["contact"]["enable"]         = true;  // disable contact
    config["line_search"]["max_iter"]   = 8;
    config["linear_system"]["tol_rate"] = 1e-3;
    config["dt"]                        = dt;

    {  // dump config
        std::ofstream ofs(fmt::format("{}config.json", this_output_path));
        ofs << config.dump(4);
    }

    SimplicialComplexIO io;

    Scene scene{config};

    // create constitution and contact model
    AffineBodyConstitution abd;
    RotatingMotor          rm;

    // create object
    auto object = scene.objects().create("cube");
    {
        auto mesh = io.read(fmt::format("{}/cube.msh", tetmesh_dir));

        auto trans_view = view(mesh.transforms());
        {
            Transform t = Transform::Identity();
            t.translate(Vector3::UnitY() * 2);
            trans_view[0] = t.matrix();
        }

        label_surface(mesh);
        label_triangle_orient(mesh);

        abd.apply_to(mesh, 10.0_MPa);
        rm.apply_to(mesh, 1e2, Vector3::UnitX(), std::numbers::pi / 1.0_s);
        object->geometries().create(mesh);
    }

    auto ground_obj = scene.objects().create("ground");
    {
        Transform pre_transform = Transform::Identity();
        pre_transform.scale(Vector3{3, 0.1, 12});

        SimplicialComplexIO io{pre_transform};
        io          = SimplicialComplexIO{pre_transform};
        auto ground = io.read(fmt::format("{}{}", tetmesh_dir, "cube.msh"));

        label_surface(ground);
        label_triangle_orient(ground);

        Transform transform = Transform::Identity();
        transform.translate(Vector3{0, 0, 4.5});
        view(ground.transforms())[0] = transform.matrix();
        abd.apply_to(ground, 10.0_MPa);

        auto is_fixed      = ground.instances().find<IndexT>(builtin::is_fixed);
        view(*is_fixed)[0] = 1;

        ground_obj->geometries().create(ground);
    }

    auto& animator = scene.animator();
    animator.insert(*object,
                    [](Animation::UpdateInfo& info)
                    {
                        auto geo_slots = info.geo_slots();
                        auto geo = geo_slots[0]->geometry().as<SimplicialComplex>();

                        auto is_constrained =
                            geo->instances().find<IndexT>(builtin::is_constrained);
                        auto is_constrained_view = view(*is_constrained);
                        is_constrained_view[0]   = 1;

                        RotatingMotor::animate(*geo, info.dt());
                    });


    world.init(scene);
    SceneIO sio{scene};
    sio.write_surface(fmt::format("{}scene_surface{}.obj", this_output_path, 0));

    for(int i = 1; i < 360; i++)
    {
        world.advance();
        world.retrieve();
        sio.write_surface(fmt::format("{}scene_surface{}.obj", this_output_path, i));
    }
}