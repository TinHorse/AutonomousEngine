#pragma once
#include "Components.h"

class Environment : public Entity
{
public:
	Environment(EntityManager& manager, EnvironmentType env) : Entity(manager)
	{
		environment_type = env;
	}

private:
	EnvironmentType environment_type = NoEnvironment;
};