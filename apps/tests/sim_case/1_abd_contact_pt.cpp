#include <catch.hpp>
#include <app/asset_dir.h>
#include <uipc/uipc.h>
#include <uipc/constitutions/affine_body.h>
#include <filesystem>
#include <fstream>

TEST_CASE("1_abd_contact_pt", "[abd]")
{
    using namespace uipc;
    using namespace uipc::geometry;
    using namespace uipc::world;
    using namespace uipc::constitution;
    using namespace uipc::engine;
    namespace fs = std::filesystem;

    std::string tetmesh_dir{AssetDir::tetmesh_path()};
    auto        this_output_path = AssetDir::output_path(__FILE__);


    UIPCEngine engine{"cuda", this_output_path};
    World      world{engine};

    auto config       = Scene::default_config();
    config["gravity"] = Vector3{0, -10, 0};

    {  // dump config
        std::ofstream ofs(fmt::format("{}/config.json", this_output_path));
        ofs << config.dump(4);
    }

    Scene scene{config};
    {
        // create constitution and contact model
        auto& abd = scene.constitution_tabular().create<AffineBodyConstitution>();
        auto& default_contact = scene.contact_tabular().default_element();

        // create object
        auto object = scene.objects().create("tets");

        vector<Vector4i> Ts = {Vector4i{0, 1, 2, 3}};

        {
            vector<Vector3> Vs = {Vector3{0, 1, 0},
                                  Vector3{0, 0, 1},
                                  Vector3{-std::sqrt(3) / 2, 0, -0.5},
                                  Vector3{std::sqrt(3) / 2, 0, -0.5}};

            std::transform(Vs.begin(),
                           Vs.end(),
                           Vs.begin(),
                           [&](auto& v) { return v * 0.3; });

            auto mesh1 = tetmesh(Vs, Ts);

            label_surface(mesh1);
            label_triangle_orient(mesh1);

            mesh1.instances().resize(1);
            abd.apply_to(mesh1, 100.0_MPa);
            default_contact.apply_to(mesh1);

            auto trans_view = view(mesh1.transforms());
            auto is_fixed   = mesh1.instances().find<IndexT>(builtin::is_fixed);
            auto is_fixed_view = view(*is_fixed);

            {
                Transform t      = Transform::Identity();
                t.translation()  = Vector3::UnitY() * 0.35;
                trans_view[0]    = t.matrix();
                is_fixed_view[0] = 0;
            }

            object->geometries().create(mesh1);
        }

        {

            vector<Vector3> Vs = {Vector3{0, 1, 0},
                                  Vector3{0, 0, 1},
                                  Vector3{-std::sqrt(3) / 2, 0, -0.5},
                                  Vector3{std::sqrt(3) / 2, 0, -0.5}};

            std::transform(Vs.begin(),
                           Vs.end(),
                           Vs.begin(),
                           [&](auto& v) { return v * 0.3; });


            auto mesh2 = tetmesh(Vs, Ts);

            label_surface(mesh2);
            label_triangle_orient(mesh2);

            mesh2.instances().resize(1);
            // apply constitution and contact model to the geometry
            abd.apply_to(mesh2, 100.0_MPa);
            default_contact.apply_to(mesh2);

            auto trans_view = view(mesh2.transforms());
            auto is_fixed   = mesh2.instances().find<IndexT>(builtin::is_fixed);
            auto is_fixed_view = view(*is_fixed);

            {
                Transform t      = Transform::Identity();
                trans_view[0]    = t.matrix();
                is_fixed_view[0] = 1;
            }

            object->geometries().create(mesh2);
        }
    }

    world.init(scene);
    SceneIO sio{scene};
    sio.write_surface(fmt::format("{}/scene_surface{}.obj", this_output_path, 0));

    for(int i = 1; i < 30; i++)
    {
        world.advance();
        world.sync();
        world.retrieve();
        sio.write_surface(fmt::format("{}/scene_surface{}.obj", this_output_path, i));
    }
}