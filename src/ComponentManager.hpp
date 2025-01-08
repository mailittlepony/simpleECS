/**
 * @author      : mailitg (mailitg@$HOSTNAME)
 * @file        : ComponentManager
 * @created     : Wednesday Jan 01, 2025 20:07:40 CET
 */

#ifndef COMPONENTMANAGER_HPP
#define COMPONENTMANAGER_HPP

#define MAX_COMPONENTS 32

#include <iostream>
#include <typeindex>
#include <unordered_map>
#include <vector>

using Entity=int;

class IComponent
{
    public:
        int id;
        std::unordered_map<int, Entity> index_to_entity_map; 
        std::unordered_map<Entity, int> entity_to_index_map; 
};

template<typename T>
class ComponentInfo : public IComponent
{
    public:
        std::vector<T> array;
};

class ComponentManager
{
    public:
        ~ComponentManager()
        {
            for (int i = 0; i < MAX_COMPONENTS; ++i)
            {
                if (components[i])
                {
                    delete components[i];
                    components[i] = nullptr;
                }
            }
        }

        template<typename T>
            void register_component()
            {
                if (type_to_index_map.find(typeid(T)) != type_to_index_map.end())
                {
                    //std::cerr << "Component already registered" << std::endl;
                    return;
                }

                for (int i = 0; i < MAX_COMPONENTS; i++)
                {
                    if (components[i] == nullptr)
                    {
                        components[i] = new ComponentInfo<T>();
                        components[i]->id = i;
                        type_to_index_map[typeid(T)] = i;
                        return;
                    }
                }

                std::cerr << "Max component reached" << std::endl;
            }

        template<typename T>
            void unregister_component()
            {
                if (type_to_index_map.find(typeid(T))== type_to_index_map.end())
                {
                    //std::cerr << "Component doesn't exist" << std::endl;
                    return;
                }

                for (int i= 0; i < MAX_COMPONENTS; i++)
                {
                    if (i == type_to_index_map[typeid(T)] && components[i] != nullptr)
                    {
                        delete components[i];
                        components[i] = nullptr;
                        type_to_index_map.erase(typeid(T));
                        //std::cout <<"Component unregistered successfully" << std::endl;
                    }
                }
            }

        template<typename T>
            int add_component(Entity entity, T default_value = {  })
            {                
                if (type_to_index_map.find(typeid(T)) == type_to_index_map.end())
                {
                    //std::cerr << "Register component first !" << std::endl;
                    return -1;
                }

                ComponentInfo<T> *comp_info = (ComponentInfo<T> *)components[type_to_index_map[typeid(T)]];
                std::cout << "entity_to_index_map for type: " << typeid(T).name() << std::endl;
                /* for (auto& entry : comp_info->entity_to_index_map) { */
                /*     std::cout << "Entity " << entry.first << " has component at index " << entry.second << std::endl; */
                /* } */
                if (comp_info->entity_to_index_map.find(entity) != comp_info->entity_to_index_map.end())
                {
                    //std::cerr << "Component already exists !" << std::endl;
                    return -1;
                }

                comp_info->array.push_back(default_value);
                comp_info->entity_to_index_map[entity] = comp_info->array.size() - 1;
                comp_info->index_to_entity_map[comp_info->array.size() - 1] = entity;
                
                //std::cout << "Successfully added component of type " << typeid(T).name() << " for entity: " << entity << std::endl;

                return 0;
            }

        template<typename T>
            int remove_component(Entity entity)
            {
                ComponentInfo<T> *comp_info = (ComponentInfo<T>*)components[type_to_index_map[typeid(T)]];

                if (comp_info->entity_to_index_map.find(entity) == comp_info->entity_to_index_map.end())
                {
                    //std::cerr << "Component does not exist !" << std::endl;
                    return -1;
                }

                int del_ind = comp_info->entity_to_index_map[entity]; 
                int last_ind = comp_info->array.size() - 1;
                comp_info->array[del_ind] = std::move(comp_info->array[last_ind]);

                comp_info->array.pop_back();

                comp_info->entity_to_index_map.erase(entity);
                comp_info->index_to_entity_map.erase(del_ind);

                //std::cout << "Successfully removed component of type " << typeid(T).name() << " for entity " << entity << std::endl;
                return 0;
            }

        template<typename T>
            T *get_component(Entity entity)
            {
                ComponentInfo<T> *comp_info = (ComponentInfo<T> *)components[type_to_index_map[typeid(T)]];

                if (comp_info->entity_to_index_map.find(entity) == comp_info->entity_to_index_map.end())
                {
                    //std::cerr << "Entity does not have the component !" << std::endl;
                    return nullptr;
                }

                int ind = comp_info->entity_to_index_map[entity];
                return &comp_info->array[ind];
            }

        IComponent *components[32] = { nullptr };
        std::unordered_map<std::type_index, int> type_to_index_map;
};

#endif 

