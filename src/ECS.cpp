/**
 * @author      : mailitg (mailitg@$HOSTNAME)
 * @file        : ECS
 * @created     : Wednesday Jan 01, 2025 21:24:47 CET
 */

#include "ComponentManager.hpp"
#include "ECS.hpp"
#include "SystemManager.hpp"

ECS::ECS() : component_manager(), system_manager(&component_manager)
{
    system_manager.entities = &entity_to_mask;
}

ECS::~ECS()
{
}

Entity ECS::create_entity()
{
    Entity entity;
    if (!recycled_entities.empty())
    {
        entity = recycled_entities.back();
        recycled_entities.pop_back();
    }
    else
    {
        entity = next_entity++;
    }

    entities.push(entity);
    entity_to_mask[entity] = ComponentMask();  
    return entity;
}

void ECS::delete_entity(Entity entity)
{
    entity_to_mask.erase(entity);
    recycled_entities.push_back(entity);

    for (SystemInfo &sys_info : system_manager.systems) {
        if ((sys_info.component_mask & get_entity_mask(entity)) == sys_info.component_mask) {
            sys_info.entities.erase(entity);
        }
    }
}

ComponentMask ECS::get_entity_mask(Entity entity)
{
    return entity_to_mask[entity];
}

void ECS::update_entity_mask(Entity entity, ComponentMask mask)
{
    entity_to_mask[entity] = mask;

    for (SystemInfo &sys_info : system_manager.systems) {
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

void ECS::register_system(System system, ComponentMask mask, void *args)
{
    system_manager.register_system(system, mask, args);
}

void ECS::call_system(System system)
{
    system_manager.call_system(system);
}
