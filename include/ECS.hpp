/**
 * @author      : mailitg (mailitg@$HOSTNAME)
 * @file        : ECS
 * @created     : Wednesday Jan 01, 2025 20:04:41 CET
 */

#ifndef ECS_HPP
#define ECS_HPP

#include <unordered_map>
#include <vector>
#include <queue>

#include "ComponentManager.hpp"
#include "SystemManager.hpp"

class ECS
{
    public:
        ECS();
        virtual ~ECS();

        Entity create_entity();
        void delete_entity(Entity entity);
        ComponentMask get_entity_mask(Entity entity);

        template<typename T>
            void register_component()
            {
                system_manager.component_manager->register_component<T>(); 
            }

        template<typename T>
            void unregister_component()
            {

                system_manager.component_manager->unregister_component<T>(); 
            }

        template<typename T>
            void add_component(Entity entity, T default_value = {})
            {    
                if (system_manager.component_manager->add_component<T>(entity, default_value) == -1)
                {
                    return;
                }
                ComponentMask updated_mask = get_entity_mask(entity);
                int component_ind = system_manager.component_manager->type_to_index_map.find(typeid(T))->second;
                updated_mask.set(component_ind);
                update_entity_mask(entity, updated_mask);
            }

        template<typename T>
            void remove_component(Entity entity)
            {
                if (system_manager.component_manager->remove_component<T>(entity) == -1)
                {
                    return;
                }
                ComponentMask updated_mask = get_entity_mask(entity);
                int component_ind = system_manager.component_manager->type_to_index_map.find(typeid(T))->second;

                updated_mask.reset(component_ind);
                update_entity_mask(entity, updated_mask);
            }

        template<typename T>
            T *get_component(Entity entity)
            {
                return system_manager.component_manager->get_component<T>(entity);
            }


        template<typename... Components>
            ComponentMask create_signature()
            {
                return system_manager.create_signature<Components...>();
            }       

        void register_system(System system, ComponentMask mask, void *args);
        void call_system(System system);

        std::queue<Entity> entities;
        std::unordered_map<Entity, ComponentMask> entity_to_mask;


    private:
        Entity next_entity = 0;
        std::vector<Entity> recycled_entities;

        ComponentManager component_manager;
        SystemManager system_manager;

        void update_entity_mask(Entity entity, ComponentMask mask);
};

#endif 

