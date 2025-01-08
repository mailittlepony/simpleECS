/**
 * @author      : mailitg (mailitg@$HOSTNAME)
 * @file        : SystemManager
 * @created     : Wednesday Jan 01, 2025 21:53:48 CET
 */

#ifndef SYSTEMMANAGER_HPP
#define SYSTEMMANAGER_HPP

#include "../src/ComponentManager.hpp"
#include <cstdint>
#include <set>
#include <unordered_map>

typedef std::bitset<MAX_COMPONENTS> ComponentMask;
using System=void (*)(std::vector<Entity>, void *args[]);

struct SystemInfo
{
    System system;
    void **args;
    ComponentMask component_mask;
    std::set<Entity> entities;
};

class SystemManager
{
    public:
        SystemManager();
        virtual ~SystemManager();

        template <typename First, typename... Rest>
            ComponentMask create_signature() 
            {
                ComponentMask mask;
                mask.set(component_manager->type_to_index_map[typeid(First)]);  

                if constexpr (sizeof...(Rest) > 0) 
                {
                    mask |= create_signature<Rest...>();  
                }

                return mask;
            }

        void register_system(System system, ComponentMask mask, int argc = 0, void *args[] = nullptr);
        void call_system(System system);

        std::unordered_map<Entity, ComponentMask> *entities;
        ComponentManager *component_manager;
        std::vector<SystemInfo> systems;
        std::unordered_map<std::uintptr_t, int> system_to_index;

};

#endif 

