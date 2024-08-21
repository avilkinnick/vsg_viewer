#include "Application.h"

#include <stdexcept>

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
    create_options();
    create_window_traits();
    create_scene();
    create_viewer();
    create_window();
    create_look_at();
    create_camera();
    create_command_graph();
    create_render_graph();
    create_view();
    create_render_im_gui();

    initialize_viewer();

    load_route_thread = std::thread(&RouteLoader::load, "../../routes/rostov-kavkazskaya");
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

void Application::create_viewer()
{
    viewer = vsg::Viewer::create();
}

void Application::create_window()
{
    window = vsg::Window::create(window_traits);
    if (!window)
    {
        throw std::runtime_error("Failed to create VSG window!");
    }

    viewer->addWindow(window);
}

void Application::create_look_at()
{
    vsg::ComputeBounds compute_bounds;
    scene->accept(compute_bounds);

    const vsg::dvec3 center = (compute_bounds.bounds.min + compute_bounds.bounds.max) * 0.5;
    radius = vsg::length(compute_bounds.bounds.max - compute_bounds.bounds.min) * 0.6;

    look_at = vsg::LookAt::create(center + vsg::dvec3(0.0, -radius * 3.5, 0.0), center, vsg::dvec3(0.0, 0.0, 1.0));
}

void Application::create_camera()
{
    constexpr double near_far_ratio = 0.01;

    auto perspective = vsg::Perspective::create(30.0, static_cast<double>(window->extent2D().width) / static_cast<double>(window->extent2D().height), near_far_ratio * radius, radius * 400.5);

    camera = vsg::Camera::create(perspective, look_at, vsg::ViewportState::create(window->extent2D()));
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
