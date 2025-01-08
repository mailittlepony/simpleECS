/**
 * @author      : mailitg (mailitg@$HOSTNAME)
 * @file        : ECS
 * @created     : Wednesday Jan 01, 2025 21:24:47 CET
 */

#include "../src/ComponentManager.hpp"
#include "ECS.hpp"
#include "../src/SystemManager.hpp"

std::vector<Scene> ECS::recycled_scenes = {  };
Scene ECS::next_scene = 0;
std::unordered_map<Scene, SceneInfo> ECS::scenes = {  };
SceneInfo *ECS::current_scene = nullptr;

ECS::~ECS()
{
    scenes.clear();
    recycled_scenes.clear();
    next_scene = 0;
    current_scene = nullptr;
}

Entity ECS::create_entity()
{
    Entity entity;
    if (!current_scene->recycled_entities.empty())
    {
        entity = current_scene->recycled_entities.back();
        current_scene->recycled_entities.pop_back();
    }
    else
    {
        entity = current_scene->next_entity++;
    }

    current_scene->entity_to_mask[entity] = ComponentMask();  
    return entity;
}

void ECS::delete_entity(Entity entity)
{
    for (SystemInfo &sys_info : current_scene->system_manager.systems) 
    {
        if ((sys_info.component_mask & get_entity_mask(entity)) == sys_info.component_mask) 
        {
            sys_info.entities.erase(entity);
        }
    }

    current_scene->entity_to_mask[entity].reset();
    current_scene->recycled_entities.push_back(entity);
    current_scene->entity_to_mask[entity] = std::move(current_scene->entity_to_mask[current_scene->entity_to_mask.size() - 1]);
    current_scene->entity_to_mask.erase(current_scene->entity_to_mask.size() - 1);
}

ComponentMask ECS::get_entity_mask(Entity entity)
{
    return current_scene->entity_to_mask[entity];
}

void ECS::update_entity_mask(Entity entity, ComponentMask mask)
{
    current_scene->entity_to_mask[entity] = mask;

    for (SystemInfo &sys_info : current_scene->system_manager.systems) {
        if ((mask & sys_info.component_mask) == sys_info.component_mask) 
        {
            sys_info.entities.emplace(entity);
        } 
        else 
        {
            sys_info.entities.erase(entity);
        }
    }
}

void ECS::register_system(System system, ComponentMask mask, int argc, void *args[])
{
    current_scene->system_manager.register_system(system, mask, argc, args);
}

void ECS::call_system(System system)
{
    current_scene->system_manager.call_system(system);
}

Scene ECS::create_scene()
{
    Scene new_scene;
    if (!recycled_scenes.empty())
    {
        new_scene = recycled_scenes.back();
        recycled_scenes.pop_back();
        /* std::cout << "Reusing recycled scene ID: " << new_scene << std::endl; */
    }
    else
    {
        new_scene = next_scene++;
        /* std::cout << "Creating new scene ID: " << new_scene << std::endl; */
    }

    scenes[new_scene] = {  };
    SceneInfo *new_scene_info = &scenes[new_scene];
    new_scene_info->system_manager.entities = &new_scene_info->entity_to_mask;
    new_scene_info->system_manager.component_manager = &new_scene_info->component_manager;
    new_scene_info->id = new_scene;

    return new_scene;
}

void ECS::delete_scene(Scene scene)
{
    if (scenes.find(scene) == scenes.end()) 
    {
        /* std::cerr <<"Scene doesn't exist"<< std::endl; */
        return;
    }

    if (current_scene == &scenes[scene]) 
    {
        current_scene = nullptr;
    }

    recycled_scenes.push_back(scene);
    scenes[scene] = std::move(scenes[scenes.size() - 1]);
    scenes.erase(scenes.size() - 1);
    /* std::cout << "deleted scene ID: " << scene << std::endl; */

    /* std::cout << "Recycled scene IDs: "; */
    /* for (const auto &scene : recycled_scenes) { */
    /*     std::cout << scene << " "; */
    /* } */
    /* std::cout << std::endl; */

}

void ECS::select_scene(Scene scene)
{
    if (scenes.find(scene) == scenes.end()) {
        /* std::cerr <<"Scene does not exist!"<< std::endl; */
        return;
    }
    current_scene = &scenes[scene];
    /* std::cout << "Selected scene ID: " << scene << std::endl; */
}
