#ifndef ROUTE_LOADER_H
#define ROUTE_LOADER_H

#include <set>
#include <string>
#include <vector>

#include <vsg/all.h>

struct object_ref_t
{
    std::string label;
    std::string model_path;
    std::string texture_path;
    bool mipmap = false;
    bool smooth = false;
};

struct vertex_t
{
    vsg::vec3 pos;
    vsg::vec3 normal;
    vsg::vec2 tex_coord;
};

struct model_t
{
    std::vector<vertex_t> vertices;
    std::vector<std::uint32_t> indices;
};

struct model_pair_t
{
    std::string path;
    model_t model;
};

class RouteLoader
{
public:
    static void load(const std::string& route_path);

public:
    static const std::vector<object_ref_t>& get_object_refs() { return object_refs; }
    static const std::set<std::string>& get_unique_model_paths() { return unique_model_paths; }
    static const std::set<std::string>& get_unique_texture_paths() { return unique_texture_paths; }
    static const std::vector<model_pair_t>& get_model_pairs() { return model_pairs; }

private:
    static void load_objects_ref(const std::string& route_path);
    static void load_objects_ref_optimized(const std::string& route_path);

    static void load_models(const std::string& route_path);
    static void load_models_optimized(const std::string& route_path);

    static void load_model(const std::string& model_path);
    static void load_model_optimized(const std::string& model_path);

private:
    static std::vector<object_ref_t> object_refs;
    static std::set<std::string> unique_model_paths;
    static std::set<std::string> unique_texture_paths;
    static std::vector<model_pair_t> model_pairs;
};

#endif // ROUTE_LOADER_H
