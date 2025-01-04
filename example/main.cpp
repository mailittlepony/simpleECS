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
    Scene menu = ECS::create_scene();
    ECS::select_scene(menu);

    ECS::register_component<Transform>(); 
    ECS::register_component<Rigidbody>(); 

    Entity player = ECS::create_entity();
    Entity player2 = ECS::create_entity();
    std::cout <<"player mask: "<< ECS::get_entity_mask(player)<< std::endl;

    ECS::add_component<Transform>(player, { .x=10, .y=20, .z=30 });
    ECS::add_component<Rigidbody>(player, { .mass = 50.0f }); 

    ECS::add_component<Rigidbody>(player2, { .mass = 50.0f }); 
    std::cout <<"player mask: "<< ECS::get_entity_mask(player)<< std::endl;

    Transform *t = ECS::get_component<Transform>(player);
    std::cout << "tx:"<< t->x << std::endl;

    ComponentMask signature = ECS::create_signature<Transform, Rigidbody>();
    ComponentMask signature2 = ECS::create_signature<Rigidbody>();
    std::cout << "Generated Signature: " << signature << std::endl;

    ECS::register_system(mvt_system, signature, nullptr);
    ECS::register_system(mvt2_system, signature2, nullptr);

    /* // Print the state before deletion */
    /* std::cout << "\nBefore deleting entity:" << std::endl; */
    /* std::cout << "Entity to Mask: " << ECS::get_entity_mask(player) << std::endl; */
    /* std::cout << "Entity to Mask2: " << ECS::get_entity_mask(player2) << std::endl; */

    ECS::call_system(mvt_system);
    ECS::call_system(mvt2_system);

    ECS::remove_component<Transform>(player);
    ECS::remove_component<Rigidbody>(player);

    /* // Print the state after removing components */
    /* std::cout << "\nAfter removing components:" << std::endl; */
    /* std::cout << "Entity to Mask: " << ECS::get_entity_mask(player) << std::endl; */

    Rigidbody *r = ECS::get_component<Rigidbody>(player);

    ECS::delete_entity(player); 
    ECS::delete_entity(player2); 

    Scene game = ECS::create_scene();
    ECS::select_scene(game);

    ECS::delete_scene(menu);
    ECS::select_scene(menu);

    return 0;
}

