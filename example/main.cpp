/**
 * @author      : mailitg (mailitg@$HOSTNAME)
 * @file        : main
 * @created     : Wednesday Jan 01, 2025 20:20:47 CET
 */

#include <cstdint>
#include <iostream>
#include <ostream>
#include <vector>

#include "ECS.hpp"

struct Transform
{
    float x, y, z;
};

struct Rigidbody
{
    float mass;
};

void mvt_system(std::vector<Entity> entities, void *args)
{
    for (Entity const &entity : entities)
    {
        std::cout << "mvt_system processing entity: " << entity << std::endl;
    }
}

void mvt2_system(std::vector<Entity> entities, void *args)
{
    for (Entity const &entity : entities)
    {
        std::cout << "mvt 2 system processing entity: " << entity << std::endl;
    }
}

int main(void)
{
    ECS ecs;

    Scene menu = ecs.create_scene();
    ecs.select_scene(menu);

    ecs.register_component<Transform>(); 
    ecs.register_component<Rigidbody>(); 

    Entity player = ecs.create_entity();
    Entity player2 = ecs.create_entity();
    std::cout <<"player mask: "<< ecs.get_entity_mask(player)<< std::endl;

    ecs.add_component<Transform>(player, { .x=10, .y=20, .z=30 });
    ecs.add_component<Rigidbody>(player, { .mass = 50.0f }); 

    ecs.add_component<Rigidbody>(player2, { .mass = 50.0f }); 
    std::cout <<"player mask: "<< ecs.get_entity_mask(player)<< std::endl;

    Transform *t = ecs.get_component<Transform>(player);
    std::cout << "tx:"<< t->x << std::endl;

    ComponentMask signature = ecs.create_signature<Transform, Rigidbody>();
    ComponentMask signature2 = ecs.create_signature<Rigidbody>();
    std::cout << "Generated Signature: " << signature << std::endl;

    ecs.register_system(mvt_system, signature, nullptr);
    ecs.register_system(mvt2_system, signature2, nullptr);

    /* // Print the state before deletion */
    /* std::cout << "\nBefore deleting entity:" << std::endl; */
    /* std::cout << "Entity to Mask: " << ecs.get_entity_mask(player) << std::endl; */
    /* std::cout << "Entity to Mask2: " << ecs.get_entity_mask(player2) << std::endl; */

    ecs.call_system(mvt_system);
    ecs.call_system(mvt2_system);

    ecs.remove_component<Transform>(player);
    ecs.remove_component<Rigidbody>(player);

    /* // Print the state after removing components */
    /* std::cout << "\nAfter removing components:" << std::endl; */
    /* std::cout << "Entity to Mask: " << ecs.get_entity_mask(player) << std::endl; */

    Rigidbody *r = ecs.get_component<Rigidbody>(player);

    ecs.delete_entity(player); 
    ecs.delete_entity(player2); 

    Scene game = ecs.create_scene();
    ecs.select_scene(game);

    ecs.delete_scene(menu);
    ecs.select_scene(menu);

    return 0;
}

