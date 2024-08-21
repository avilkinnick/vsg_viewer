#ifndef APPLICATION_H
#define APPLICATION_H

#include <thread>

#include <vsg/all.h>
#include <vsgImGui/RenderImGui.h>

class Application
{
public:
    ~Application();

    void initialize();
    void run();

private:
    void create_options();
    void create_window_traits();
    void create_scene();
    void create_viewer();
    void create_window();
    void create_look_at();
    void create_camera();
    void create_command_graph();
    void create_render_graph();
    void create_view();
    void create_render_im_gui();

    void initialize_viewer();

private:
    vsg::ref_ptr<vsg::Options> options;
    vsg::ref_ptr<vsg::WindowTraits> window_traits;
    vsg::ref_ptr<vsg::Group> scene;
    vsg::ref_ptr<vsg::Viewer> viewer;
    vsg::ref_ptr<vsg::Window> window;
    double radius;
    vsg::ref_ptr<vsg::LookAt> look_at;
    vsg::ref_ptr<vsg::Camera> camera;
    vsg::ref_ptr<vsg::CommandGraph> command_graph;
    vsg::ref_ptr<vsg::RenderGraph> render_graph;
    vsg::ref_ptr<vsg::View> view;
    vsg::ref_ptr<vsgImGui::RenderImGui> render_im_gui;

    std::thread load_route_thread;
};

#endif // APPLICATION_H
