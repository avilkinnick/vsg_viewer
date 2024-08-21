#include "RouteLoader.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

std::vector<object_ref_t> RouteLoader::object_refs;
std::set<std::string> RouteLoader::unique_model_paths;
std::set<std::string> RouteLoader::unique_texture_paths;
std::vector<model_pair_t> RouteLoader::model_pairs;

void RouteLoader::load(const std::string& route_path)
{
    const bool optimized_exists = std::filesystem::exists(route_path + "/optimized");
    if (optimized_exists)
    {
        load_objects_ref_optimized(route_path + "/optimized");
        load_models_optimized(route_path + "/optimized");
    }
    else
    {
        load_objects_ref(route_path);
        load_models(route_path);
    }
}

void RouteLoader::load_objects_ref(const std::string& route_path)
{
    std::ifstream inf(route_path + "/objects.ref");
    if (!inf)
    {
        throw std::runtime_error("Failed to open file \"" + route_path + "/objects.ref\"!");
    }

    std::string buffers[3];
    std::size_t current_buffer_index = 0;

    bool mipmap = false;
    bool smooth = false;

    while (inf >> buffers[current_buffer_index])
    {
        const std::string& current_buffer = buffers[current_buffer_index];

        if (current_buffer[0] == '3' && current_buffer[1] == '3')
        {
            int a = 10;
        }

        if (current_buffer == "[mipmap]")
        {
            mipmap = true;
        }
        else if (current_buffer == "[not_mipmap]")
        {
            mipmap = false;
        }
        else if (current_buffer == "[smooth]")
        {
            smooth = true;
        }
        else if (current_buffer == "[not_smooth]")
        {
            smooth = false;
        }

        switch (current_buffer_index)
        {
        case 0:
        {
            if (current_buffer[0] != '/')
            {
                ++current_buffer_index;
            }
            break;
        }
        case 1:
        {
            if (current_buffer[0] == '/')
            {
                ++current_buffer_index;
            }
            else
            {
                buffers[0] = current_buffer;
            }
            break;
        }
        case 2:
        {
            if (current_buffer[0] == '/')
            {
                object_refs.emplace_back(object_ref_t{buffers[0], buffers[1], buffers[2], mipmap, smooth});
                unique_model_paths.insert(buffers[1]);
                unique_texture_paths.insert(buffers[2]);
                current_buffer_index = 0;
            }
            else
            {
                buffers[0] = current_buffer;
                current_buffer_index = 1;
            }
            break;
        }
        default:
        {
            break;
        }
        }
    }
}

void RouteLoader::load_objects_ref_optimized(const std::string& route_path)
{

}

void RouteLoader::load_models(const std::string& route_path)
{
    for (const std::string& model_path : unique_model_paths)
    {
        load_model(route_path + model_path);
    }
}

void RouteLoader::load_models_optimized(const std::string& route_path)
{

}

void RouteLoader::load_model(const std::string& model_path)
{
    std::ifstream inf(model_path);
    if (!inf)
    {
        std::cout << "Failed to open file \"" + model_path + "\"!\n";
        return;
    }

    std::string buffer;
    while (buffer != "TriMesh()")
    {
        inf >> buffer;
        if (inf.eof())
        {
            std::cout << "Failed to load model from file \"" + model_path + "\"!\n";
            return;
        }
    }

    inf >> buffer >> buffer;

    std::uint32_t temp_vertices_size, temp_vertex_faces_size;
    inf >> temp_vertices_size >> temp_vertex_faces_size;

    inf >> buffer >> buffer;

    std::vector<vsg::vec3> temp_vertices(temp_vertices_size);
    for (vsg::vec3& temp_vertex : temp_vertices)
    {
        inf >> buffer;
        if (buffer.back() == 'N')
        {
            std::cout << "Failed to load model from file \"" + model_path + "\"!\n";
            return;
        }
        temp_vertex.x = std::stof(buffer);

        inf >> buffer;
        if (buffer.back() == 'N')
        {
            std::cout << "Failed to load model from file \"" + model_path + "\"!\n";
            return;
        }
        temp_vertex.y = std::stof(buffer);

        inf >> buffer;
        if (buffer.back() == 'N')
        {
            std::cout << "Failed to load model from file \"" + model_path + "\"!\n";
            return;
        }
        temp_vertex.z = std::stof(buffer);
    }

    model_t model;
    for (const vsg::vec3& temp_vertex : temp_vertices)
    {
        model.vertices.emplace_back(vertex_t{temp_vertex});
    }

    model_pairs.emplace_back(model_pair_t{model_path, model});
}

void RouteLoader::load_model_optimized(const std::string& model_path)
{

}
