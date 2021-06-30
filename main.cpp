#include <iostream>
#include <vector>
#include <random>

enum RoomType
{
    BEDROOM,
    KITCHEN,
    BATHROOM,
    CHILDREN_ROOM,
    LIVING_ROOM,
    UNKNOWN = 99
};

enum BuildingType
{
    LIVING_HOUSE,
    GARAGE,
    BARN,
    SAUNA,
    NOT_DEFINED = 99
};

struct Room
{
    RoomType roomType;
    float area;

   explicit Room(RoomType roomType = UNKNOWN, float area = 10.0f)
    {
        this->area = area;
        this->roomType = roomType;
    }
};

struct Floor
{
    float ceilingHeight;
    std::vector<Room> rooms;

    explicit Floor(float ceilingHeight = 2.2f, int roomCount = 2)
    {
        this->ceilingHeight = ceilingHeight;
        if (roomCount < 2)
        {
            std::cerr << "Minimal rooms quantity is 2.\n";
            roomCount = 2;
        }
        else if (roomCount > 4)
        {
            std::cerr << "Maximal rooms quantity is 4.\n";
            roomCount = 4;
        }

        for (int i = 0; i < roomCount; ++i)
        {
            Room r;
            rooms.push_back(r);
        }
    }

    void addRoom(RoomType roomType, float area)
    {
        Room r(roomType, area);
        rooms.push_back(r);
    }
};

struct Building
{
private:
    bool withOven = false;

public:
    BuildingType buildingType;
    float area;
    std::vector<Floor> floors;

    explicit Building(BuildingType buildingType = NOT_DEFINED, float area = 100.0f, bool randomArea = true)
    {
        this->buildingType = buildingType;
        if (randomArea) this->area = rand() % 25 + 25;
        else this->area = area;
        //Add minimum 1 floor:
        Floor f;
        floors.push_back(f);
    }

    void setWithOven(const int value)
    {
        if (buildingType == LIVING_HOUSE || buildingType == SAUNA)
        {
            this->withOven = (value != 0);
        }
        else
        {
            std::cerr << "This type of building can't be equipped with oven.\n";
            this->withOven = false;
        }
    }

    bool getWithOven() const
    {
        return withOven;
    }

    void addFloor(float ceilingHeight, int roomCount)
    {
        Floor f(ceilingHeight, roomCount);
        floors.push_back(f);
    }
};

struct Plot
{
    int id;
    std::vector<Building> buildings;
    float area;

    explicit Plot(int id, float area = 100.0f)
    {
        //Unique id is based on plots count:
        this->id = id;
        this->area = area;
        Building b (LIVING_HOUSE);
        buildings.push_back(b);
    }

    void addBuilding(BuildingType buildingType, float area)
    {
        Building b(buildingType, area);
        buildings.push_back(b);
    }
};

std::string getBuildingType(BuildingType bType)
{
    if (bType == LIVING_HOUSE) return "LIVING_HOUSE";
    else if (bType == SAUNA) return "SAUNA";
    else if (bType == BARN) return "BARN";
    else if (bType == GARAGE) return "GARAGE";
    else return "NOT_DEFINED";
}

void printPlots(std::vector<Plot>& plots) {
    for (int i = 0; i < plots.size(); ++i) {
        std::cout << "Plot id: " << plots[i].id << " (area  " << plots[i].area << ", buildings "
                  << plots[i].buildings.size() << "):\n";
        for (int j = 0; j < plots[i].buildings.size(); ++j)
        {
            Building b = plots[i].buildings[j];
            std::cout << "\t Building " << j << ": " << getBuildingType(b.buildingType) << ", area: " << b.area << ", oven: " << b.getWithOven() << "\n";
            for (int k = 0; k < b.floors.size(); ++k)
            {
                Floor f = b.floors[k];
                std::cout << "\t\t Floor " << k << ": ceiling height " << f.ceilingHeight << " m, rooms: " << f.rooms.size() << "\n";
            }
        }

    }
}

int main() {
    srand(time(nullptr));
    std::cout << "Generating country side area...\n";

    int plotsQuantity = rand() % 10 + 10; // from 10 to 20 plots
    int quantityOfOvens = 0;
    std::vector<Plot> plots;
    for (int i = 0; i < plotsQuantity; ++i)
    {
        float plotArea = rand() % 101 + 200; //from 200 to 300
        Plot p(i, plotArea);
        float buildingsCount = rand() % 3 + 1; //from 1 to 3
        for (int j = 0; p.buildings.size() < buildingsCount; ++j)
        {
            BuildingType randomType = (BuildingType)(rand() % 4);
            float randomArea = rand() % long (p.area * 0.1) + (p.area * 0.1);
            Building b(randomType, randomArea);
            if (b.buildingType == LIVING_HOUSE || b.buildingType == SAUNA)
            {
                b.setWithOven(rand() % 2);
            }
            if (b.getWithOven()) quantityOfOvens++;
            int randomFloorsQuantity = rand() % 3 + 1; // from 1 to 3 floors
            for (int k = 0; b.floors.size() < randomFloorsQuantity; ++k)
            {
                float randomRoomsQuantity = rand() % 3 + 2; // 2 - 4 rooms
                float randomCeilingHeight = rand() % 3 + 2.2f;
                Floor f(randomCeilingHeight, randomRoomsQuantity);
                for (int m = 0; f.rooms.size() < randomRoomsQuantity; m++)
                {
                    RoomType randomRoomType = (RoomType)(rand() % 6);
                    float randomRoomArea = rand() % long (b.area * 0.2) + (b.area * 0.1);
                    Room r(randomRoomType, randomRoomArea);
                    f.rooms.push_back(r);
                }
                b.floors.push_back(f);
            }
            p.buildings.push_back(b);
        }
        plots.push_back(p);
    }

    printPlots(plots);

    std::cout << "\n-----------------------\n";
    std::cout << "\tTotal quantity of ovens: " << quantityOfOvens << "\n";

    return 0;
}
