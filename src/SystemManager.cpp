/**
 * @author      : mailitg (mailitg@$HOSTNAME)
 * @file        : SystemManager
 * @created     : Friday Jan 03, 2025 13:45:41 CET
 */

#include "../src/SystemManager.hpp"
#include <cstring>

SystemManager::SystemManager()
{    

}

SystemManager::~SystemManager()
{
}

void SystemManager::register_system(System system, ComponentMask mask, int argc, void *args[])
{
    SystemInfo sys_info = { .system = system, .component_mask = mask };
    sys_info.args = new void *[argc];
    std::memcpy(sys_info.args, args, argc * sizeof(void *));

    /* std::cout << "Registering system with mask: " << mask << std::endl; */

    for (const std::pair<Entity, ComponentMask> &item : *entities) {
        if ((sys_info.component_mask & item.second) == sys_info.component_mask) 
        {
            sys_info.entities.insert(item.first);
            /* std::cout << "Entity " << item.first << " matches the system mask." << std::endl; */
        }
    }

    systems.push_back(sys_info);

    std::uintptr_t system_id = reinterpret_cast<std::uintptr_t>(system);
    system_to_index[system_id] = systems.size() - 1;

    /* std::cout << "System registered, total systems: " << systems.size() << std::endl; */
}

void SystemManager::call_system(System system)
{
    std::uintptr_t system_id = reinterpret_cast<std::uintptr_t>(system);

    if (system_to_index.find(system_id) == system_to_index.end())
    {
        /* std::cerr << "System not found!" << std::endl; */
        return;
    }

    int system_index = system_to_index[system_id];

    SystemInfo &sys_info = systems[system_index];
    std::vector<Entity> entities_vector(sys_info.entities.begin(), sys_info.entities.end());

    /* // Print out entities to be processed */
    /* std::cout << "Entities to process: "; */
    /* for (Entity e : entities_vector) { */
    /*     std::cout << e << " "; */
    /* } */

    sys_info.system(entities_vector, sys_info.args);
}
