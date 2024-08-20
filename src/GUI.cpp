#include "GUI.h"

#include <vsgImGui/imgui.h>

void GUI::record(vsg::CommandBuffer& cb) const
{
    ImGui::Begin("FPS");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::ShowDemoWindow();
}
