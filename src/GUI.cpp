#include "GUI.h"

#include <cstdint>
#include <stdexcept>

GUI::GUI(vsg::ref_ptr<vsg::PhysicalDevice> physical_device_in)
    : physical_device(physical_device_in)
    , object_refs(RouteLoader::get_object_refs())
    , unique_model_paths(RouteLoader::get_unique_model_paths())
    , unique_texture_paths(RouteLoader::get_unique_texture_paths())
    , model_pairs(RouteLoader::get_model_pairs())
{
    ImGui::CreateContext();
    load_font("../../fonts/SpaceMono-Bold.ttf");
    set_colors();
}

void GUI::record(vsg::CommandBuffer& cb) const
{
    render_debug_window();
    render_loader_window();
    ImGui::ShowDemoWindow();
}

void GUI::load_font(const std::string& font_path)
{
    ImGuiIO& io = ImGui::GetIO();
    auto font = io.Fonts->AddFontFromFileTTF(font_path.c_str(), 24.0f);
    if (!font)
    {
        throw std::runtime_error("Failed to open file \"" + font_path + "\"!");
    }
}

void GUI::set_colors()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.5f, 0.5f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.8f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.1f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.2f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.6f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 0.0f, 0.0f, 0.8f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.0f, 0.0f, 0.0f, 0.1f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.0f, 0.0f, 0.8f);
    style.Colors[ImGuiCol_Button] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1.0f, 0.0f, 0.0f, 0.6f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.0f, 0.0f, 0.0f, 0.8f);
    style.Colors[ImGuiCol_Header] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.5f, 0.5f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1f, 0.4f, 0.75f, 0.78f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1f, 0.4f, 0.75f, 1.0f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.0f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.43f, 0.35f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.9f, 0.7f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
}

void GUI::render_debug_window() const
{
    ImGui::SetNextWindowPos(ImVec2(5.0f, 5.0f));
    ImGui::SetNextWindowSize(ImVec2(485.0f, 130.0f));

    ImGui::Begin("Debug");

    ImGui::TextUnformatted(physical_device->getProperties().deviceName);

    const std::uint32_t api_version = physical_device->getProperties().apiVersion;
    ImGui::Text("Vulkan API %i.%i.%i", VK_API_VERSION_MAJOR(api_version), VK_API_VERSION_MINOR(api_version), VK_API_VERSION_PATCH(api_version));

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();
}

void GUI::render_loader_window() const
{
    ImGui::SetNextWindowPos(ImVec2(5.0f, 140.0f));
    ImGui::SetNextWindowSize(ImVec2(1800.0f, 1295.0f));

    ImGui::Begin("Loader");

    render_objects_ref();
    render_unique_model_paths();
    render_unique_texture_paths();
    render_model_pairs();

    ImGui::End();
}

void GUI::render_objects_ref() const
{
    const std::size_t object_refs_size = object_refs.size();
    const std::string header = "object_refs[" + std::to_string(object_refs_size) + "]###object_refs";

    if (ImGui::CollapsingHeader(header.c_str()))
    {

        if (ImGui::BeginTable("object_refs_table", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_ScrollY))
        {
            ImGui::TableSetupScrollFreeze(1, 1);
            ImGui::TableSetupColumn("index");
            ImGui::TableSetupColumn("label");
            ImGui::TableSetupColumn("model_path");
            ImGui::TableSetupColumn("texture_path");
            ImGui::TableSetupColumn("mipmap");
            ImGui::TableSetupColumn("smooth");
            ImGui::TableHeadersRow();

            for (std::size_t row = 0; row < object_refs_size; ++row)
            {
                const object_ref_t& object_ref = object_refs[row];
                const std::string label = std::to_string(row);

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Selectable(label.c_str(), false, ImGuiSelectableFlags_SpanAllColumns);

                ImGui::TableSetColumnIndex(1);
                ImGui::TextUnformatted(object_ref.label.c_str());
                ImGui::TableSetColumnIndex(2);
                ImGui::TextUnformatted(object_ref.model_path.c_str());
                ImGui::TableSetColumnIndex(3);
                ImGui::TextUnformatted(object_ref.texture_path.c_str());

                ImGui::TableSetColumnIndex(4);
                ImU32 cell_bg_color = ImGui::GetColorU32(object_ref.mipmap ? ImVec4(0.0f, 1.0f, 0.0f, 0.4f) : ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);

                ImGui::TableSetColumnIndex(5);
                cell_bg_color = ImGui::GetColorU32(object_ref.smooth ? ImVec4(0.0f, 1.0f, 0.0f, 0.4f) : ImVec4(1.0f, 0.0f, 0.0f, 0.4f));
                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
            }
            ImGui::EndTable();
        }
    }
}

void GUI::render_unique_model_paths() const
{
    const std::size_t unique_model_paths_size = unique_model_paths.size();
    const std::string header = "unique_model_paths[" + std::to_string(unique_model_paths_size) + "]###unique_model_paths";

    if (ImGui::CollapsingHeader(header.c_str()))
    {
        if (ImGui::BeginTable("unique_model_paths", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_ScrollY))
        {
            ImGui::TableSetupScrollFreeze(1, 1);
            ImGui::TableSetupColumn("index");
            ImGui::TableSetupColumn("model_path");
            ImGui::TableHeadersRow();

            for (std::size_t row = 0; row < unique_model_paths_size; ++row)
            {
                ImGui::TableNextRow();

                auto it = unique_model_paths.cbegin();
                std::advance(it, row);

                ImGui::TableSetColumnIndex(0);
                const std::string label = std::to_string(row);
                ImGui::Selectable(label.c_str(), false, ImGuiSelectableFlags_SpanAllColumns);

                ImGui::TableSetColumnIndex(1);
                ImGui::TextUnformatted(it->c_str());
            }
            ImGui::EndTable();
        }
    }
}

void GUI::render_unique_texture_paths() const
{
    const std::size_t unique_texture_paths_size = unique_texture_paths.size();
    const std::string header = "unique_texture_paths[" + std::to_string(unique_texture_paths_size) + "]###unique_texture_paths";

    if (ImGui::CollapsingHeader(header.c_str()))
    {
        if (ImGui::BeginTable("unique_texture_paths", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_ScrollY))
        {
            ImGui::TableSetupScrollFreeze(1, 1);
            ImGui::TableSetupColumn("index");
            ImGui::TableSetupColumn("texture_path");
            ImGui::TableHeadersRow();

            for (std::size_t row = 0; row < unique_texture_paths_size; ++row)
            {
                ImGui::TableNextRow();

                auto it = unique_texture_paths.cbegin();
                std::advance(it, row);

                ImGui::TableSetColumnIndex(0);
                const std::string label = std::to_string(row);
                ImGui::Selectable(label.c_str(), false, ImGuiSelectableFlags_SpanAllColumns);

                ImGui::TableSetColumnIndex(1);
                ImGui::TextUnformatted(it->c_str());
            }
            ImGui::EndTable();
        }
    }
}

void GUI::render_model_pairs() const
{
    const std::size_t model_pairs_size = model_pairs.size();
    const std::string header = "model_pairs[" + std::to_string(model_pairs_size) + "]###model_pairs";

    if (ImGui::CollapsingHeader(header.c_str()))
    {
        if (ImGui::BeginTable("model_pairs", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_ScrollY))
        {
            ImGui::TableSetupScrollFreeze(1, 1);
            ImGui::TableSetupColumn("index");
            ImGui::TableSetupColumn("model_path");
            ImGui::TableSetupColumn("model");
            ImGui::TableHeadersRow();

            for (std::size_t row_1 = 0; row_1 < model_pairs_size; ++row_1)
            {
                const model_pair_t& model_pair = model_pairs[row_1];
                const model_t& model = model_pair.model;

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%zu", row_1);

                ImGui::TableSetColumnIndex(1);
                ImGui::TextUnformatted(model_pair.path.c_str());

                ImGui::TableSetColumnIndex(2);
                std::string label = "model##model" + std::to_string(row_1);
                if (ImGui::CollapsingHeader(label.c_str()))
                {
                    const auto& vertices = model.vertices;

                    ImGui::Indent();

                    label = "vertices[" + std::to_string(vertices.size()) + "]###model" + std::to_string(row_1) + "vertices";
                    if (ImGui::CollapsingHeader(label.c_str()))
                    {
                        if (ImGui::BeginTable(label.c_str(), 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_ScrollY))
                        {
                            ImGui::TableSetupScrollFreeze(1, 1);
                            ImGui::TableSetupColumn("index");
                            ImGui::TableSetupColumn("pos");
                            ImGui::TableSetupColumn("normal");
                            ImGui::TableSetupColumn("tex_coord");
                            ImGui::TableHeadersRow();

                            for (std::size_t row_2 = 0; row_2 < vertices.size(); ++row_2)
                            {
                                ImGui::TableNextRow();

                                ImGui::TableSetColumnIndex(0);
                                label = std::to_string(row_2);
                                ImGui::Selectable(label.c_str(), false, ImGuiSelectableFlags_SpanAllColumns);

                                ImGui::TableSetColumnIndex(1);
                                ImGui::Text("%.3f %.3f %.3f", vertices[row_2].pos.x, vertices[row_2].pos.y, vertices[row_2].pos.z);

                                ImGui::TableSetColumnIndex(2);
                                ImGui::Text("%.3f %.3f %.3f", vertices[row_2].pos.x, vertices[row_2].pos.y, vertices[row_2].pos.z);

                                ImGui::TableSetColumnIndex(3);
                                ImGui::Text("%.3f %.3f %.3f", vertices[row_2].pos.x, vertices[row_2].pos.y, vertices[row_2].pos.z);
                            }
                            ImGui::EndTable();
                        }
                    }
                }
            }
            ImGui::EndTable();
        }
    }
}
