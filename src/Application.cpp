#include "Application.h"

#include <stdexcept>

#include <vsg/app/CloseHandler.h>
#include <vsg/app/Trackball.h>
#include <vsg/state/ViewportState.h>
#include <vsg/utils/SharedObjects.h>
#include <vsgXchange/all.h>
#include <vsgImGui/SendEventsToImGui.h>

#include "GUI.h"
#include "RouteLoader.h"

Application::~Application()
{
    load_route_thread.detach();
}

void Application::initialize()
{
    load_route_thread = std::thread(&RouteLoader::load, "../../routes/rostov-kavkazskaya");

    create_options();
    create_window_traits();
    create_scene();
    create_window();
    create_viewer();
    create_camera();
    create_command_graph();
    create_render_graph();
    create_view();
    create_render_im_gui();

    initialize_viewer();
}

void Application::run()
{
    while (viewer->advanceToNextFrame())
    {
        viewer->handleEvents();
        viewer->update();
        viewer->recordAndSubmit();
        viewer->present();
    }
}

void Application::create_options()
{
    options = vsg::Options::create();
    options->sharedObjects = vsg::SharedObjects::create();
    options->fileCache = vsg::getEnv("VSG_FILE_CACHE");
    options->paths = vsg::getEnvPaths("VSG_FILE_PATH");
    options->add(vsgXchange::all::create());
}

void Application::create_window_traits()
{
    window_traits = vsg::WindowTraits::create();
    window_traits->windowTitle = "vsg_viewer";
    window_traits->debugLayer = true;
    window_traits->fullscreen = true;
}

void Application::create_scene()
{
    scene = vsg::Group::create();
}

void Application::create_window()
{
    window = vsg::Window::create(window_traits);
    if (!window)
    {
        throw std::runtime_error("Failed to create VSG window!");
    }
}

void Application::create_viewer()
{
    viewer = vsg::Viewer::create();
    viewer->addWindow(window);
}

void Application::create_camera()
{
    vsg::ComputeBounds compute_bounds;
    scene->accept(compute_bounds);

    const vsg::dvec3 center = (compute_bounds.bounds.min + compute_bounds.bounds.max) * 0.5;
    const double radius = vsg::length(compute_bounds.bounds.max - compute_bounds.bounds.min) * 0.6;

    auto perspective = create_perspective(radius);
    look_at = create_look_at(center, radius);
    auto viewport_state = vsg::ViewportState::create(window->extent2D());

    camera = vsg::Camera::create(perspective, look_at, viewport_state);
}

vsg::ref_ptr<vsg::Perspective> Application::create_perspective(double radius)
{
    constexpr double field_of_view = 30.0;
    const double window_width = static_cast<double>(window->extent2D().width);
    const double window_height = static_cast<double>(window->extent2D().height);
    const double aspect_ratio = window_width / window_height;
    constexpr double near_far_ratio = 0.01;
    const double near_distance = near_far_ratio * radius;
    const double far_distance = radius * 400.5;
    return vsg::Perspective::create(field_of_view, aspect_ratio, near_distance, far_distance);
}

vsg::ref_ptr<vsg::LookAt> Application::create_look_at(const vsg::dvec3& center, double radius)
{
    const vsg::dvec3 eye = center + vsg::dvec3(0.0, -radius * 3.5, 0.0);
    const vsg::dvec3 up = vsg::dvec3(0.0, 0.0, 1.0);
    return vsg::LookAt::create(eye, center, up);
}

void Application::create_command_graph()
{
    command_graph = vsg::CommandGraph::create(window);
}

void Application::create_render_graph()
{
    render_graph = vsg::RenderGraph::create(window);

    command_graph->addChild(render_graph);
}

void Application::create_view()
{
    view = vsg::View::create(camera);
    view->addChild(vsg::createHeadlight());
    view->addChild(scene);

    render_graph->addChild(view);
}

void Application::create_render_im_gui()
{
    render_im_gui = vsgImGui::RenderImGui::create(window, GUI::create(window->getPhysicalDevice()));

    render_graph->addChild(render_im_gui);
}

void Application::initialize_viewer()
{
    viewer->addEventHandler(vsgImGui::SendEventsToImGui::create());
    viewer->addEventHandler(vsg::CloseHandler::create(viewer));
    viewer->addEventHandler(vsg::Trackball::create(camera));
    viewer->assignRecordAndSubmitTaskAndPresentation({command_graph});
    viewer->compile();
}
