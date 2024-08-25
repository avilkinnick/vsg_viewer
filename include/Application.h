#ifndef APPLICATION_H
#define APPLICATION_H

#include <thread>

#include <vsg/app/Camera.h>
#include <vsg/app/CommandGraph.h>
#include <vsg/app/ProjectionMatrix.h>
#include <vsg/app/RenderGraph.h>
#include <vsg/app/View.h>
#include <vsg/app/Viewer.h>
#include <vsg/app/ViewMatrix.h>
#include <vsg/app/Window.h>
#include <vsg/app/WindowTraits.h>
#include <vsg/core/ref_ptr.h>
#include <vsg/io/Options.h>
#include <vsg/maths/vec3.h>
#include <vsg/nodes/Group.h>
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
    void create_window();
    void create_viewer();
    void create_camera();
    vsg::ref_ptr<vsg::Perspective> create_perspective(double radius);
    vsg::ref_ptr<vsg::LookAt> create_look_at(const vsg::dvec3& center, double radius);
    void create_command_graph();
    void create_render_graph();
    void create_view();
    void create_render_im_gui();

    void initialize_viewer();

private:
    std::thread load_route_thread;

    vsg::ref_ptr<vsg::Options> options;
    vsg::ref_ptr<vsg::WindowTraits> window_traits;
    vsg::ref_ptr<vsg::Group> scene;
    vsg::ref_ptr<vsg::Window> window;
    vsg::ref_ptr<vsg::Viewer> viewer;
    vsg::ref_ptr<vsg::LookAt> look_at;
    vsg::ref_ptr<vsg::Camera> camera;
    vsg::ref_ptr<vsg::CommandGraph> command_graph;
    vsg::ref_ptr<vsg::RenderGraph> render_graph;
    vsg::ref_ptr<vsg::View> view;
    vsg::ref_ptr<vsgImGui::RenderImGui> render_im_gui;
};

#endif // APPLICATION_H
