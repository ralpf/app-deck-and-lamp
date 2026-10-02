An Entity is a piece of code that is intended to support serialization and regular updates of state.
It consist of 2 concepts:
    - ISetting - a struct that holds user defined data, serialization and versioning
    - IEntity  - a class that supports Update(ISetting *setting) with a pointer to setting

Entities are created an used in main.cpp
The idea is to hardcode their declaration, and add them to an array. The desired order is achieved by placing into correct array position.

Overall, simplicty is the key. In the end it looks as simple as declaring the desired entities, store in array and update in main.cpp
We prefer C style, and use pointers to pass settings and entities around

The 2 arrays in main holds abstract refs to IEntity, and to ISetting. It updates them one by one, pluging in 