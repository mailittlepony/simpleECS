/**
 * @author      : mailitg (mailitg@$HOSTNAME)
 * @file        : ECS
 * @created     : Wednesday Jan 01, 2025 20:04:41 CET
 */

#ifndef ECS_HPP
#define ECS_HPP

#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>

#include "../src/ComponentManager.hpp"
#include "../src/SystemManager.hpp"

using Scene=int;

struct SceneInfo
{
    Scene id;
    Entity next_entity = 0;
    ComponentManager component_manager;
    SystemManager system_manager;
    std::vector<Entity> recycled_entities;

    std::unordered_map<Entity, ComponentMask> entity_to_mask;
};

class ECS
{
    public:
        ECS() = delete;
        ~ECS();

        static Entity create_entity();
        static void delete_entity(Entity entity);
        static ComponentMask get_entity_mask(Entity entity);
        static Scene create_scene();
        static void delete_scene(Scene scene);
        static void select_scene(Scene scene);

        template<typename T>
            static void register_component()
            {
                current_scene->component_manager.register_component<T>(); 
            }

        template<typename T>
            static void unregister_component()
            {

                current_scene->component_manager.unregister_component<T>(); 
            }

        template<typename T>
            static void add_component(Entity entity, T default_value = {})
            { 
                if (current_scene->entity_to_mask.find(entity) == current_scene->entity_to_mask.end())
                {
                    std::cerr <<"entity doesn't exist !"<<std::endl;
                    return;
                }

                if (current_scene->component_manager.add_component<T>(entity, default_value) == -1)
                {
                    return;
                }
                ComponentMask updated_mask = get_entity_mask(entity);
                int component_ind = current_scene->component_manager.type_to_index_map.find(typeid(T))->second;
                updated_mask.set(component_ind);
                update_entity_mask(entity, updated_mask);
            }

        template<typename T>
            static void remove_component(Entity entity)
            {
                if (current_scene->component_manager.remove_component<T>(entity) == -1)
                {
                    return;
                }
                ComponentMask updated_mask = get_entity_mask(entity);
                int component_ind = current_scene->component_manager.type_to_index_map.find(typeid(T))->second;

                updated_mask.reset(component_ind);
                update_entity_mask(entity, updated_mask);
            }

        template<typename T>
            static T *get_component(Entity entity)
            {
                if (current_scene->entity_to_mask.find(entity) == current_scene->entity_to_mask.end())
                {
                    std::cerr <<"entity doesn't exist !"<<std::endl;
                    return nullptr;
                }

                return current_scene->component_manager.get_component<T>(entity);
            }


        template<typename... Components>
            static ComponentMask create_signature()
            {
                return current_scene->system_manager.create_signature<Components...>();
            }       

        static void register_system(System system, ComponentMask mask, int argc = 0, void *args[] = nullptr);
        static void call_system(System system);


    private:
        static Scene next_scene;
        static std::vector<Scene> recycled_scenes;
        static std::unordered_map<Scene, SceneInfo> scenes;
        static SceneInfo *current_scene;

        static void update_entity_mask(Entity entity, ComponentMask mask);
};

#endif 

