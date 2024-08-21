#ifndef GUI_H
#define GUI_H

#include <vector>

#include <vsg/all.h>
#include <vsgImGui/imgui.h>

#include "RouteLoader.h"

class GUI : public vsg::Inherit<vsg::Command, GUI>
{
public:
    GUI(vsg::ref_ptr<vsg::PhysicalDevice> physical_device_in);

    void record(vsg::CommandBuffer& cb) const override;

private:
    void load_font(const std::string& font_path);
    void set_colors();

    void render_debug_window() const;

    void render_loader_window() const;
    void render_objects_ref() const;
    void render_unique_model_paths() const;
    void render_unique_texture_paths() const;
    void render_model_pairs() const;

private:
    vsg::ref_ptr<vsg::PhysicalDevice> physical_device;
    const std::vector<object_ref_t>& object_refs;
    const std::set<std::string>& unique_model_paths;
    const std::set<std::string>& unique_texture_paths;
    const std::vector<model_pair_t>& model_pairs;
};

#endif // GUI_H
