#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include <set>
#include "Math.h"

// The Entity Component System is composed of three classes: component, entity and manager
// A Component can be anything: transform, visual representation, movement, collision etc
// An entity is an object in the game, like a character, scenery, buildings
// The manager stores all of the entities and keeps track of their interactions


// declare classes
class Component;
class Entity;
class EntityManager;


// initialize variable ComponentID to size_t, whose maximum value is equal to the biggest value that can be held by the OS
using ComponentID = std::size_t;


// Groups are used to group entities for rendering purposes. Each entity can be a part of one or more groups. 
using Group = std::size_t;

// inline function only needs to be speficied if the function is in a header or if it used for a template specialization in the same file
// NOTE on static, extern: static variables are restricted to the current source file, whereas extern variables are defined in a different file (if the referenced variable does not exist, you will get a linker error "symbol not found")
inline ComponentID MakeNewComponentTypeID()
{
	static ComponentID lastID = 0u;
	return lastID++; // increments static ComponentID lastID
}

// template function getComponentTypeID. When we pass in a component that already exists, we want to get its ID rather than a new one. 
// NOTE: This ocde will not return a unique ID for each component, but rather for each component TYPE !!
template<typename T> inline ComponentID GetComponentTypeID() noexcept
{
	static ComponentID typeID = MakeNewComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>; // like array, but only for bool. takes less space than regular array. Here, it stores bools for determining whether an entity has a specific component
using GroupBitSet = std::bitset<maxGroups>; // Stores bools for determining which groups the entity belongs to

using ComponentArray = std::array<Component*, maxComponents>; // array of components, with each component max size of 32


class Component
{
public:
	Entity *entity; // reference to its entity (owner)
	bool IsActive() const;
	void Destroy();
	virtual void LinkComponentPointers() {}; // used for re-linking components after a component pointer has changed

	virtual void Init() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual ~Component() {};

protected:
	bool active = true;
};


class Entity
{
public:
	Entity(EntityManager& mManager) : manager(mManager) 
	{
		Ticks = TickMaster++;
	}
	bool IsActive() const;
	virtual void update() {}
	virtual void refresh(std::set<Entity*>& deletedEntities) {}
	void Destroy();
	virtual ~Entity();

	bool HasGroup(Group mGroup); // checks if entity is part of a specific group
	void AddToGroup(Group mGroup); // adds entity to specified group
	void RemoveFromGroup(Group mGroup); // removes entity from specified group

	
	template<typename T> // checks if an entity has a specific component.
	inline bool HasComponent() const
	{
		return componentBitSet[GetComponentTypeID<T>()]; // Returns true if the queried component type (i.e. typename) is present. Since the BitSet stores bools for each component type, this code simply gets the component type ID of the queried component and checks the BitSet at that index.
	}


	template<typename T, typename ...TArgs> // adds a component T to the entity (TArgs are constructor arguments of the component)
	// NOTE: "typename ... var" means that this is a variadic template. variadic templates take a variable number of arguments.
	// If you want the template to only take a positive number of arguments (i.e. not 0), then you can write it like above, with the first argument "typename T," followed by the variadic arguments "typename ...TArgs"
	T & AddComponent(TArgs && ...mArgs)
	{
		T *comp(new T(std::forward<TArgs>(mArgs)...)); // make new object of the specified component
		comp->entity = this; // sets component's entity (owner) to the current entity

		componentArray[GetComponentTypeID<T>()] = comp; // add component to the entity's component array
		componentBitSet[GetComponentTypeID<T>()] = true; // set the hasComponent flag to true for this component

		comp->Init(); // intialize component
		return *comp;
	}

	template<typename T>
	T & AddColliderComponent(T* comp)
	{
		comp->entity = this; // sets component's entity (owner) to the current entity
		componentArray[GetComponentTypeID<T>()] = comp; // add component to the entity's component array
		componentBitSet[GetComponentTypeID<T>()] = true; // set the hasComponent flag to true for this component
		comp->Init();
		return *comp;
	}

	template<typename T>
	void SetComponent(T* comp)
	{
		//comp->entity = this; // sets component's entity (owner) to the current entity
		componentArray[GetComponentTypeID<T>()] = comp; // add component to the entity's component array
		//componentBitSet[GetComponentTypeID<T>()] = true; // set the hasComponent flag to true for this component
		//comp->Init();
		//return *comp;
		for (auto comp : componentArray)
		{
			if (comp)
			{
				comp->LinkComponentPointers();
			}
		}
	}
	
	template<typename T>
	inline T & GetComponent() const
	{
		auto ptr(componentArray[GetComponentTypeID<T>()]); // create pointer to transform of component in component array
		return *static_cast<T*>(ptr); // static cast the pointer, which converts the pointer to a pointer to the type of the component, rather than being a pointer to its base class Component
		// NOTE on static cast: new_type value = static_cast <new_type> (expression);
	}

	template<typename T>
	inline T * GetComponentPtr() const
	{
		auto ptr(componentArray[GetComponentTypeID<T>()]); // create pointer to transform of component in component array
		return static_cast<T*>(ptr); // static cast the pointer, which converts the pointer to a pointer to the type of the component, rather than being a pointer to its base class Component
		// NOTE on static cast: new_type value = static_cast <new_type> (expression);
	}

	void incrementTicks()
	{
		Ticks++;
	}

	const long int& getTicks()
	{
		return Ticks;
	}
	
protected:
	EntityManager& manager;
	bool active = true;

	ComponentArray componentArray{};
	ComponentBitSet componentBitSet; // each Entity has a bitset that keeps track of all active components
	GroupBitSet groupBitSet; // each Entity has a Group bitset that keeps track of which groups it is part of
	long int Ticks;
	static long int TickMaster;
};
