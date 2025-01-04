# SimpleECS-cpp-lib
> Simple ECS C++ library still under process. To add : error handling

### Steps to build and run example:

1. Clone the repo:
```bash
git clone https://github.com/MailiTruong/SimpleECS-cpp-lib.git
cd SimpleECS

```

2. Run *make* to build the static library:
```bash
make

```

3. To run the example program:
```bash
make run

```
### Steps to use:

1. Include the headers:
```cpp
#include "ECS.hpp"

```

2. Then you can write components:
```cpp
struct Transform
{
    float x, y, z;
};

struct Rigidbody
{
    float mass;
};

```

3. Write systems:
```cpp

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

```
**4. Then you have to create an instance of ECS in your main loop before anything**

5. You can then create a scene:
```cpp
ECS ecs;

Scene menu = ecs.create_scene();
ecs.select_scene(menu);

```

5. You can create your entities:
```cpp
ECS ecs;
Entity player = ecs.create_entity();

```

6. You can register and add components:
```cpp
ecs.register_component<Transform>(); 
ecs.register_component<Rigidbody>();

ecs.add_component<Transform>(player, { .x=10, .y=20, .z=30 });
ecs.add_component<Rigidbody>(player, { .mass = 50.0f }); 

```

7. You can access a component: 
```cpp
Transform *t = ecs.get_component<Transform>(player);

```

8. You can create a signature and register a system:
```cpp
ComponentMask signature = ecs.create_signature<Transform, Rigidbody>();
ComponentMask signature2 = ecs.create_signature<Rigidbody>();
std::cout << "Generated Signature: " << signature << std::endl;

ecs.register_system(mvt_system, signature, nullptr);
ecs.register_system(mvt2_system, signature2, nullptr);

```
9. You can call the system:
```cpp
ecs.call_system(mvt_system);
ecs.call_system(mvt2_system);

```

10. And finally you can remove a component, remove an entity and delete a scene:
```cpp
ecs.remove_component<Transform>(player);
ecs.remove_component<Rigidbody>(player);

ecs.delete_entity(player); 
ecs.delete_entity(player2); 
  
ecs.delete_scene(menu);

```

> I have commented out some logs to debug when I work on it, but if you wish to vizualize what is happening feel free to uncomment those parts !
