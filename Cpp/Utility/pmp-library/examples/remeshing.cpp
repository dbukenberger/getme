// Copyright 2011-2019 the Polygon Mesh Processing Library developers.
// Distributed under a MIT-style license, see LICENSE.txt for details.

#include <pmp/visualization/MeshViewer.h>
#include <pmp/algorithms/features.h>
#include <pmp/algorithms/remeshing.h>
#include <pmp/utilities.h>

#include <imgui.h>

using namespace pmp;

class Viewer : public MeshViewer
{
public:
    Viewer(const char* title, int width, int height);

protected:
    void process_imgui() override;
};

Viewer::Viewer(const char* title, int width, int height)
    : MeshViewer(title, width, height)
{
    set_draw_mode("Hidden Line");
    crease_angle_ = 0.0;
}

void Viewer::process_imgui()
{
    MeshViewer::process_imgui();

    ImGui::Spacing();
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Remeshing", ImGuiTreeNodeFlags_DefaultOpen))
    {
        static int feature_angle = 70;
        ImGui::PushItemWidth(80);
        ImGui::SliderInt("##feature_angle", &feature_angle, 1, 180);
        ImGui::PopItemWidth();
        ImGui::SameLine();
        if (ImGui::Button("Detect Features"))
        {
            clear_features(mesh_);
            detect_features(mesh_, feature_angle);
            update_mesh();
        }

        ImGui::Text("Remeshing:");
        ImGui::SameLine();

        if (ImGui::Button("Uniform"))
        {
            Scalar l(0);
            for (auto eit : mesh_.edges())
                l += distance(mesh_.position(mesh_.vertex(eit, 0)),
                              mesh_.position(mesh_.vertex(eit, 1)));
            l /= (Scalar)mesh_.n_edges();
            try
            {
                uniform_remeshing(mesh_, l);
            }
            catch (const InvalidInputException& e)
            {
                std::cerr << e.what() << std::endl;
                return;
            }
            update_mesh();
        }

        ImGui::SameLine();

        if (ImGui::Button("Adaptive"))
        {
            auto bb = bounds(mesh_).size();
            try
            {
                adaptive_remeshing(mesh_,
                                   0.0010 * bb,  // min length
                                   0.0500 * bb,  // max length
                                   0.0005 * bb); // approx. error
            }
            catch (const InvalidInputException& e)
            {
                std::cerr << e.what() << std::endl;
                return;
            }
            update_mesh();
        }
    }
}

int main(int argc, char** argv)
{
#ifndef __EMSCRIPTEN__
    Viewer window("Remeshing", 800, 600);
    if (argc == 2)
        window.load_mesh(argv[1]);
    return window.run();
#else
    Viewer window("Remeshing", 800, 600);
    window.load_mesh(argc == 2 ? argv[1] : "input.off");
    return window.run();
#endif
}
