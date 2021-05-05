#pragma once
#include "SDL.h"
#include "Node.h"
#include "Entities.h"
#include "Math.h"
#include <vector>
#include "Connection.h"
#include <queue>
#include "Components.h"
#include "Player.h"

enum RequestType : std::size_t
{
	Pickup,
	Delivery,
	PickupBuilding,
	DeliveryBuilding
};


struct TransporterRequest
{
	TransporterRequest(RequestType request_type, ResourceType Resource, Node* Node)
	{
		requestType = request_type;
		resource = Resource;
		node = Node;
	}
	RequestType requestType;
	ResourceType resource;
	Node* node = nullptr;
	bool MarkedForDestruction = false;
	Transporter* transporter = nullptr;

	TransporterRequest& operator= (TransporterRequest other)
	{
		requestType = other.requestType;
		resource = other.resource;
		node = other.node;
		transporter = other.transporter;
		MarkedForDestruction = other.MarkedForDestruction;
		return *this;
	}
};

class NodeSystem
{
public:
	int NodeID = 0;

	NodeSystem()
	{
		
	}

	void InitAtFirstStorage(Vector2D position)
	{
		for (int i = 0; i < NumIdleTransports; i++)
		{
			Entity& entity = Game::assets->CreateTransporter(position);

			Transporter* transporter = dynamic_cast<Transporter*>(&entity);
			transporter->GetComponent<SpriteComponent>().Hide();
			transporters.push_back(transporter);
		}
		/*
		for (int i = 0; i < NumIdleTransportsConstruction; i++)
		{
			Entity& entity = Game::assets->CreateTransporter(position);

			Transporter* transporter = dynamic_cast<Transporter*>(&entity);
			transporter->GetComponent<SpriteComponent>().Hide();
			transportersConstruction.push_back(transporter);
		}
	*/
	}

	void AddTransporter(Transporter* transporter)
	{
		transporters.push_back(transporter);
		NumIdleTransports++;
	}


	void PartitionVectors(int Frame)
	{
		const int Frames = 60;
		int EntitiesPerFrame = 0;
		Frame = Frame % Frames;

		EntitiesPerFrame = (activeRequests.size() / Frames) + 1;
		ActivePartitionCurrentStart = Frame * EntitiesPerFrame;
		ActivePartitionCurrentEnd = ActivePartitionCurrentStart + EntitiesPerFrame;

		/*
		EntitiesPerFrame = (requestsToBeProcessedConstruction.size() / Frames) + 1;
		ActiveConstructionPartitionCurrentStart = Frame * EntitiesPerFrame;
		ActiveConstructionPartitionCurrentEnd = ActiveConstructionPartitionCurrentStart + EntitiesPerFrame;
		*/
	}

	void Update(int Time, Vector2D cameraOffset)
	{
		PartitionVectors(Time);

		// update normal requests
		ProcessNewRequests();
		UpdateActiveRequests();

		// update construction requests
		//ProcessNewRequestsConstruction();
		//UpdateActiveRequestsConstruction();

		// Update nodes
		for (Node* node : nodes)
		{
			node->Update(cameraOffset);
		}
		for (Connection* connection : connections)
		{
			connection->Update(cameraOffset);
		}
	}

	Node* AddNode(int x, int y, Entity* entity, bool bIsGlobalStorage)
	{
		Node* NewNode = new Node(x,y,entity);
		nodes.push_back(NewNode);
		NewNode->ID = NodeID++;
		AddConnections(NewNode, x, y);

		ConstructionSite* site = nullptr;
		site = dynamic_cast<ConstructionSite*>(entity);

		if (site)
		{
			site->SetNode(NewNode);
			NewNode->bIsGlobalResourceStorage = bIsGlobalStorage;
			if (bIsGlobalStorage)
			{
				storages.push_back(NewNode);
			}
			return NewNode;
		}

		Building* building = nullptr;
		building = dynamic_cast<Building*>(entity);
		if (building)
		{
			building->SetNode(NewNode);
			NewNode->bIsGlobalResourceStorage = bIsGlobalStorage;
			if (bIsGlobalStorage)
			{
				storages.push_back(NewNode);
			}
			return NewNode;
		}
	}

	void AddConnections(Node* NewNode, int x, int y)
	{
		std::vector<Node*> foundNodes = FindNodesInArea(NewNode, x, y);
		for (Node* node : foundNodes)
		{
			if (node)
			{
				AddConnection(NewNode, node, x, y, node->posX, node->posY);
				NewNode->AddConnectedNode(node);
				node->AddConnectedNode(NewNode);
			}
		}
	}

	Connection& AddConnection(Node* NewNode, Node* node, int x, int y, int x2, int y2)
	{
		Connection* connection = new Connection(x, y, x2, y2);
		connections.push_back(connection);
		connection->SetNodes(NewNode, node);

		node->AddConnection(connection);
		NewNode->AddConnection(connection);

		return *connection;
	}

	std::vector<Node*> FindNodesInArea(Node* NewNode, int x, int y, bool bOnlyStorages = false)
	{
		float minDist = INT_MAX;
		float minDist2 = INT_MAX;
		Node* closestNode = nullptr;
		Node* closestNode2 = nullptr;

		std::vector<Node*> closestNodes;

		for (Node* node : nodes)
		{
			if (node == NewNode)
				continue;
			float dist = Math::distance(x, y, node->posX, node->posY);
			if (dist < maxDist)
			{
				if (dist < minDist)
				{
					if (!bOnlyStorages)
					{
						minDist = dist;
						closestNode = node;
					}
					else if (node->bIsGlobalResourceStorage)
					{
						minDist = dist;
						closestNode = node;
					}
				}
			}
		}

		for (Node* node : nodes)
		{
			if (node == NewNode)
				continue;
			float dist = Math::distance(x, y, node->posX, node->posY);
			if (dist < maxDist)
			{
				if (dist < minDist2 && dist > minDist)
				{
					minDist2 = dist;
					closestNode2 = node;
				}
			}
		}
		closestNodes.push_back(closestNode);
		closestNodes.push_back(closestNode2);

		return closestNodes;
	}

	Node* FindClosestStorage(Node* node) // can return nullptr
	{
		// Breadth-first search

		// check if connected to storage
		if (node->IsNotConnected()) return nullptr;

		// clear/create visited map
		for (Node* n : nodes)
		{
			visited[n] = false;
		}


		Node* current = node;
		std::queue<Node*> not_visited;

		not_visited.push(node);
		while (!not_visited.empty())
		{
			current = not_visited.front();
			visited[current] = true;
			not_visited.pop();

			if (current->bIsGlobalResourceStorage)
			{
				return current;
			}

			for (Node* n : current->GetConnectedNodes())
			{
				if (n && !visited[n])
				{
					not_visited.push(n);
				}
			}
		}

		return nullptr;
	}

	void Draw(SDL_Renderer* renderer)
	{
		for (Connection* connection : connections)
		{
			connection->Draw(renderer);
		}
	}

	std::stack<Node*> FindPath(Node* Start, Node* End)
	{
		Node* Current = Start;
		Node* Target = End;

		std::stack<Node*> path;
		if (!Current || !Target)
		{
			std::cout << "Start: " << Current << std::endl;
			std::cout << "End: " << Target << std::endl;
			return path;
		}

		Init();
		
		// If the target is still the original target, push it
		path.push(Target);

		// list yet to be tested
		std::priority_queue<Node*, std::vector<Node*>, NodeCompare> not_tested;
		not_tested.push(Current);

		// Initialize currrent
		goals[Current] = Math::distanceNoSqrt(Current->posX, Current->posY, Target->posX, Target->posY);

		// while there are NavNodes not yet tested
		while (!not_tested.empty() && Current != Target)
		{
			// if the NavNode has been visited, remove it
			while (!not_tested.empty() && visited[not_tested.top()])
			{
				not_tested.pop();
			}

			// if the list is empty now, terminate
			if (not_tested.empty())
			{
				break;
			}

			// set current NavNode to front of the list
			Current = not_tested.top();
			visited[Current] = true;

			bool vis;
			long int lowestGoal;
			long int local;

			// check all neighbours
			for (Node* n : Current->GetConnectedNodes())
			{
				// check if neighbour has already been visited and make sure it is not an obstacle
				vis = visited[n];
				if (!visited[n])
				{
					not_tested.push(n);
				}

				// calculate the neighbour's local goals
				lowestGoal = goals[Current] + Math::distanceNoSqrt(n->posX, n->posY, Current->posX, Current->posY);

				local = goals[n];
				// check if the lowest local goal is smaller than the neighbour's previous local goal
				if (lowestGoal < goals[n])
				{
					// set the neighbour's parent to the current NavNode
					parents[n] = Current;

					// update the neighbour's local goal
					goals[n] = lowestGoal;

					// set the neighbour's global goal to the local goal plus the distance to the target
					n->globalDist = goals[n] + Math::distanceNoSqrt(n->posX, n->posY, Target->posX, Target->posY);
				}
			}
		}

		// Backtrack through the parent map to find the final path
		if (Current != nullptr)
		{
			while (Target != nullptr)
			{
				path.push(Target);
				Target = parents[Target];
			}
		}

		if (!path.empty())
			path.pop();

		if (path.size() == 1 && !Start->IsConnectedTo(End))
		{
			bool b = path.size() == 1 && !Start->IsConnectedTo(path.top());
			path.pop();
		}

		return path;
	}

	void Init()
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			visited[nodes[i]] = false;
			parents[nodes[i]] = nullptr;
			goals[nodes[i]] = LONG_MAX;
			nodes[i]->globalDist = LONG_MAX;
		}
	}

	// Process normal requests

	void ProcessNewRequests()
	{
		// destroy processed requests
		requestsToBeProcessed.erase(std::remove_if(requestsToBeProcessed.begin(), requestsToBeProcessed.end(), // iterates through requests
			[](TransporterRequest& mRequest) // creates unique ptr to each element
		{
			return mRequest.MarkedForDestruction; // for each element, checks if active
		}),
			requestsToBeProcessed.end());

		// Process new requests
		for (TransporterRequest& request : requestsToBeProcessed)
		{
			if (NumIdleTransports == 0) return;

			Node* closestStorage = FindClosestStorage(request.node);
			if (!closestStorage) continue;

			// determine resource or none, depending on delivery or pickup
			ResourceType resourceType = None;
			switch (request.requestType)
			{
			case Delivery:
				resourceType = request.resource;
				break;
			case Pickup:
				resourceType = None;
				break;
			}

			// Find transporter
			Transporter* transporter = nullptr;
			for (Transporter* t : transporters)
			{
				if (t->GetComponent<NodeMovementComponent>().Idle)
				{
					transporter = t;
					transporter->GetComponent<NodeMovementComponent>().Idle = false;
					break;
				}
			}
			if (!transporter) continue;

			// set transporter path
			transporter->GetComponent<NodeMovementComponent>().SetPath(Game::node_system->FindPath(closestStorage, request.node), request.node);
			transporter->GetComponent<NodeMovementComponent>().setPathStatus(Moving);

			// re-skin transporter
			std::string transporterRes = "transporter_empty";
			if (request.requestType == Delivery)
			{
				switch (resourceType)
				{
				case Mushrooms:
					transporterRes = "transporter_mushrooms";
					break;
				case Amber:
					transporterRes = "transporter_amber";
					break;
				case Iron:
					transporterRes = "transporter_iron";
					break;
				case Silicate:
					transporterRes = "transporter_silicate";
					break;
				case Obsidian:
					transporterRes = "transporter_obsidian";
					break;
				case Morphite:
					transporterRes = "transporter_morphite";
					break;
				}
			}


			transporter->GetComponent<SpriteComponent>().setTexture(transporterRes);

			request.MarkedForDestruction = true;

			// add to active requests
			TransporterRequest Request = request;
			Request.transporter = transporter;
			Request.MarkedForDestruction = false;
			activeRequests.push_back(Request);

			transporter->GetComponent<SpriteComponent>().UnHide();

			NumIdleTransports--;
		}
	}

	void UpdateActiveRequests()
	{
		// destroy finished requests
		activeRequests.erase(std::remove_if(activeRequests.begin(), activeRequests.end(), // iterates through requests
			[](TransporterRequest& mRequest) // creates unique ptr to each element
		{
			return mRequest.MarkedForDestruction; // for each element, checks if active
		}),
			activeRequests.end());

		for (int i = ActivePartitionCurrentStart; i < ActivePartitionCurrentEnd; i++)
		{
			if (i >= activeRequests.size()) return;
			TransporterRequest& request = activeRequests[i];
			Transporter* transporter = request.transporter;
			if (!transporter)
			{
				request.MarkedForDestruction = true; 
				return;
			}

			NodeMovementComponent& movement = transporter->GetComponent<NodeMovementComponent>();

			if (movement.GetPathStatus() == WaitingForReturn)
			{
				Entity* OtherEntity = nullptr;

				// determine resource or none, depending on delivery or pickup
				ResourceType resourceType = None;
				switch (request.requestType)
				{
				case Delivery:
					resourceType = None;
					break;
				case Pickup:
					resourceType = request.resource;
					break;
				}

				// get target building
				OtherEntity = movement.GetTargetNode()->GetEntity();

				// Transfer Resources
				switch (request.requestType)
				{
				case Delivery:
					OtherEntity->GetComponent<StorageComponent>().IncrementResourceBy(request.resource, 1);
					OtherEntity->GetComponent<StorageComponent>().NumActiveDeliveries--;;
					break;
				case Pickup:
					OtherEntity->GetComponent<StorageComponent>().NumActivePickups--;
					break;
				}

				// Find closest storage to building
				Node* FromNode = movement.GetTargetNode();

				Node* closestStorage = FindClosestStorage(FromNode);
				if (!closestStorage) continue;

				// find path home
				movement.SetPath(Game::node_system->FindPath(FromNode, closestStorage), closestStorage);
				movement.setPathStatus(Returning);

				// re-skin transporter
				std::string transporterRes = "transporter_empty";
				if (resourceType != None)
				{
					switch (resourceType)
					{
					case Mushrooms:
						transporterRes = "transporter_mushrooms";
						break;
					case Amber:
						transporterRes = "transporter_amber";
						break;
					case Iron:
						transporterRes = "transporter_iron";
						break;
					case Silicate:
						transporterRes = "transporter_silicate";
						break;
					case Obsidian:
						transporterRes = "transporter_obsidian";
						break;
					case Morphite:
						transporterRes = "transporter_morphite";
						break;
					}
				}


				transporter->GetComponent<SpriteComponent>().setTexture(transporterRes);

				continue;
			}

			if (movement.GetPathStatus() == ArrivedHome)
			{
				// Transfer Resources to player storage
				switch (request.requestType)
				{
				case Delivery:
					break;
				case Pickup:
					Game::player->GetComponent<PlayerResourceComponent>().IncrementResourceBy(request.resource, 1);
					break;
				}

				request.MarkedForDestruction = true;
				transporter->GetComponent<NodeMovementComponent>().Idle = true;
				NumIdleTransports++;
				transporter->GetComponent<SpriteComponent>().Hide();
				continue;
			}
		}
	}

	void AddRequest(RequestType request_type, ResourceType resource, Node* node)
	{
		requestsToBeProcessed.push_back(TransporterRequest(request_type, resource, node));
	}

	void AddRequestAtFront(RequestType request_type, ResourceType resource, Node* node)
	{
		requestsToBeProcessed.insert(requestsToBeProcessed.begin(), TransporterRequest(request_type, resource, node));
	}

	std::vector<Node*>& GetStorages()
	{
		return storages;
	}

	void UpdatePlayerResourcesFromStorages()
	{
		for (Node* storage : storages)
		{
			if (!storage->GetEntity()->HasComponent<StorageComponent>()) return;

			PlayerResourceComponent& resources = Game::player->GetComponent<PlayerResourceComponent>();

			StorageComponent& s = storage->GetEntity()->GetComponent<StorageComponent>();
			resources.rMushrooms += s.GetResourceAmount(ResourceType::Mushrooms);
			resources.rAmber += s.GetResourceAmount(ResourceType::Amber);
			resources.rIron += s.GetResourceAmount(ResourceType::Iron);
			resources.rSilicate += s.GetResourceAmount(ResourceType::Silicate);
			resources.rObsidian += s.GetResourceAmount(ResourceType::Obsidian);
			resources.rMorphite += s.GetResourceAmount(ResourceType::Morphite);

			// clear resources
			s.SetResourceAmount(ResourceType::Mushrooms, 0);
			s.SetResourceAmount(ResourceType::Amber, 0);
			s.SetResourceAmount(ResourceType::Iron, 0);
			s.SetResourceAmount(ResourceType::Silicate, 0);
			s.SetResourceAmount(ResourceType::Obsidian, 0);
			s.SetResourceAmount(ResourceType::Morphite, 0);
		}
	}

	int GetNumRegularTransporters()
	{
		return transporters.size();
	}

	int GetNumManufacturingTransporters()
	{
		return transportersManufacturing.size();
	}


private:
	std::vector<Node*> nodes;
	std::vector<Node*> storages;
	float maxDist = 100.f;
	std::vector<Connection*> connections;

	// Requests
	int NumIdleTransports = 15;
	std::vector<TransporterRequest> requestsToBeProcessed;
	std::vector<TransporterRequest> activeRequests;
	std::vector<Transporter*> transporters;

	// Manufacturing transporters
	int NumIdleTransportsManufacturing = 10;
	std::vector<TransporterRequest> requestsToBeProcessedManufacturing;
	std::vector<TransporterRequest> activeRequestsManufacturing;
	std::vector<Transporter*> transportersManufacturing;

	// Partitioning
	int ActivePartitionCurrentStart = 0;
	int ActivePartitionCurrentEnd = 0;

	int ActiveConstructionPartitionCurrentStart = 0;
	int ActiveConstructionPartitionCurrentEnd = 0;

	ConfigBuildings configB;
	
	// === Pathfinding ==============
	// visited map
	std::map<Node*, bool> visited;

	// current parent Node map
	std::map<Node*, Node*> parents;

	// fLocal map
	std::map<Node*, long int> goals;
};