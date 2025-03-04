#include "../include/map.h"


Map::Map(/* args */)
{
    tablicaZnakow.resize(SIZEY, std::vector<char>(SIZEX, 'Z'));
}

Map::~Map()
{
}

unsigned int Map::getWidth() const{
    return tablicaZnakow[0].size();
}

unsigned int Map::getHeight() const{
    return tablicaZnakow.size();
}

int Map::calculateCost(int x, int y, int targetX, int targetY) const {
    int cost = 1; // Base cost

    switch (getTerrain(x, y)) {
        case TerrainType::S: //Swamp
            cost += 2; // Moving through swamps is more costly
            break;
        case TerrainType::F: //Forest
            cost += 1; // Forest slightly more costly.  Could be different depending on unit type
            break;
        default:
            break;
    }

    //Heuristic Component (Manhattan Distance)
    cost += std::abs(x - targetX) + std::abs(y - targetY);
    return cost;
}

bool Map::isTraversable(int x, int y) const {
    TerrainType terrain = getTerrain(x, y);
    return terrain != TerrainType::M && terrain != TerrainType::W; // Mountains and Water are impassable by default
}

TerrainType Map::getTerrain(int x, int y) const{
    return static_cast<TerrainType>(tablicaZnakow[y][x]);
}


void Map::showMap(sf::RenderWindow &window)
{
    // Definiowanie kolorów dla poszczególnych znaków
    std::map<char, sf::Color> colorMap = {

        {'F', sf::Color(34, 139, 34)},   // las - ciemnozielony (Forest Green)
        {'W', sf::Color(0, 191, 255)},   // woda - jasnoniebieski (Deep Sky Blue)
        {'Z', sf::Color(139, 69, 19)},   // ziemia - brązowy (Saddle Brown)
        {'M', sf::Color(169, 169, 169)}, // góry - szary (Dark Gray)
        {'B', sf::Color(255, 0, 0)},     // bazy - czerwony (Red)
        {'S', sf::Color(107, 152, 35)}   // bagna - oliwkowozielony (Olive Drab)

        // Dodaj więcej znaków i kolorów w razie potrzeby
    };

    // Rozmiar pojedynczego kafelka

    // Iteracja po tablicy znaków i rysowanie kafelków
    for (size_t y = 0; y < tablicaZnakow.size(); ++y)
    {
        for (size_t x = 0; x < tablicaZnakow[y].size(); ++x)
        {
            char znak = tablicaZnakow[y][x];
            sf::RectangleShape tile(sf::Vector2f(TILESIZE, TILESIZE));
            tile.setPosition({x * TILESIZE, y * TILESIZE});

            // Ustawienie koloru na podstawie znaku
            auto it = colorMap.find(znak);
            if (it != colorMap.end())
            {
                tile.setFillColor(it->second);
            }
            else
            {
                tile.setFillColor(sf::Color::White); // Domyślny kolor, jeśli znak nie jest zMapowany
            }

            // Opcjonalnie: Dodanie obramowania
            tile.setOutlineThickness(1.0f);
            tile.setOutlineColor(sf::Color::Black);

            // Rysowanie kafelka na oknie
            window.draw(tile);
        }
    }
}

void Map::showMap()
{
    for (const auto &wiersz : tablicaZnakow)
    {
        for (const auto &znak : wiersz)
        {
            switch (znak)
            {
            case 'Z':
                std::cout << BROWN << znak << RESET;
                break;
            case 'W':
                std::cout << BLUE << znak << RESET;
                break;
            case 'F':
                std::cout << GREEN << znak << RESET;
                break;
            case 'M':
                std::cout << GREY << znak << RESET;
                break;
            // Dodaj więcej przypadków dla innych typów terenu
            default:
                std::cout << znak;
                break;
            }
        }
        std::cout << std::endl; // Przejście do nowej linii po każdym wierszu
    }
}

void Map::generateForest()
{
    std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> distY(0, SIZEY - 1);
    std::uniform_int_distribution<int> distX(0, SIZEX - 1);

    std::bernoulli_distribution distribution(FOREST_CHANCE);
    std::uniform_int_distribution<int> distribution2(1, NUM_FOREST_CLUSTERS);

    for (int i = 0; i < distribution2(generator); ++i)
    {
        int seedY = distY(generator);
        int seedX = distX(generator);

        if (tablicaZnakow[seedY][seedX] == 'Z')
        {
            tablicaZnakow[seedY][seedX] = 'F';

            // Rozprzestrzenianie się klastrów
            std::queue<std::pair<int, int>> q;
            q.push({seedY, seedX});
            int clusterSize = 1;

            while (!q.empty() && clusterSize < MAX_FOREST_CLUSTER_SIZE)
            {
                auto [y, x] = q.front();
                q.pop();

                // Sprawdzenie sąsiadów
                std::vector<std::pair<int, int>> neighbors = {
                    {y - 1, x}, {y + 1, x}, {y, x - 1}, {y, x + 1}, {y - 1, x - 1}, {y - 1, x + 1}, {y + 1, x - 1}, {y + 1, x + 1}};

                for (auto &[ny, nx] : neighbors)
                {
                    // Sprawdzenie granic Mapy
                    if (ny >= 0 && ny < SIZEY && nx >= 0 && nx < SIZEX)
                    {
                        if (tablicaZnakow[ny][nx] == 'Z')
                        {
                            // Prawdopodobieństwo dodania do klastru
                            if (distribution(generator))
                            {
                                tablicaZnakow[ny][nx] = 'F';
                                q.push({ny, nx});
                                clusterSize++;
                                if (clusterSize >= MAX_FOREST_CLUSTER_SIZE)
                                    break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void Map::generateMountains()
{
    std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> distY(0, SIZEY - 1);
    std::uniform_int_distribution<int> distX(0, SIZEX - 1);

    std::bernoulli_distribution distribution(MOUNTAIN_CHANCE);
    std::uniform_int_distribution<int> distribution2(1, NUM_MOUNTAIN_CLUSTERS);

    for (int i = 0; i < distribution2(generator); ++i)
    {
        int seedY = distY(generator);
        int seedX = distX(generator);

        if (tablicaZnakow[seedY][seedX] == 'Z')
        {
            tablicaZnakow[seedY][seedX] = 'M';

            // Rozprzestrzenianie się klastrów
            std::queue<std::pair<int, int>> q;
            q.push({seedY, seedX});
            int clusterSize = 1;

            while (!q.empty() && clusterSize < MAX_MOUNTAIN_CLUSTER_SIZE)
            {
                auto [y, x] = q.front();
                q.pop();

                // Sprawdzenie sąsiadów
                std::vector<std::pair<int, int>> neighbors = {
                    {y - 1, x}, {y + 1, x}, {y, x - 1}, {y, x + 1}, {y - 1, x - 1}, {y - 1, x + 1}, {y + 1, x - 1}, {y + 1, x + 1}};

                for (auto &[ny, nx] : neighbors)
                {
                    // Sprawdzenie granic Mapy
                    if (ny >= 0 && ny < SIZEY && nx >= 0 && nx < SIZEX)
                    {
                        if (tablicaZnakow[ny][nx] == 'Z')
                        {
                            // Prawdopodobieństwo dodania do klastru
                            if (distribution(generator))
                            {
                                tablicaZnakow[ny][nx] = 'M';
                                q.push({ny, nx});
                                clusterSize++;
                                if (clusterSize >= MAX_MOUNTAIN_CLUSTER_SIZE)
                                    break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void Map::generateWater()
{
    std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> distY(0, SIZEY - 1);
    std::uniform_int_distribution<int> distX(0, SIZEX - 1);

    std::bernoulli_distribution distribution(WATER_CHANCE);
    std::uniform_int_distribution<int> distribution2(1, NUM_WATER_CLUSTERS);

    for (int i = 0; i < distribution2(generator); ++i)
    {
        int seedY = distY(generator);
        int seedX = distX(generator);

        if (tablicaZnakow[seedY][seedX] == 'Z')
        {
            tablicaZnakow[seedY][seedX] = 'W';

            // Rozprzestrzenianie się klastrów
            std::queue<std::pair<int, int>> q;
            q.push({seedY, seedX});
            int clusterSize = 1;

            while (!q.empty() && clusterSize < MAX_WATER_CLUSTER_SIZE)
            {
                auto [y, x] = q.front();
                q.pop();

                // Sprawdzenie sąsiadów
                std::vector<std::pair<int, int>> neighbors = {
                    {y - 1, x}, {y + 1, x}, {y, x - 1}, {y, x + 1}, {y - 1, x - 1}, {y - 1, x + 1}, {y + 1, x - 1}, {y + 1, x + 1}};

                for (auto &[ny, nx] : neighbors)
                {
                    // Sprawdzenie granic Mapy
                    if (ny >= 0 && ny < SIZEY && nx >= 0 && nx < SIZEX)
                    {
                        if (tablicaZnakow[ny][nx] == 'Z')
                        {
                            // Prawdopodobieństwo dodania do klastru
                            if (distribution(generator))
                            {
                                tablicaZnakow[ny][nx] = 'W';
                                q.push({ny, nx});
                                clusterSize++;
                                if (clusterSize >= MAX_WATER_CLUSTER_SIZE)
                                    break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void Map::makeSwamp()
{
    std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr)));
    std::bernoulli_distribution distribution(0.1);

    for (size_t i = 0; i < SIZEY; i++)
    {
        for (size_t j = 0; j < SIZEX; j++)
        {

            if (tablicaZnakow[i][j] == 'F') //&&distribution(generator)
            {
                // tablicaZnakow[i][j] = 'B';

                // Rozprzestrzenianie się klastrów
                std::queue<std::pair<int, int>> q;
                q.push({i, j});
                int clusterSize = 1;

                while (!q.empty() && clusterSize < MAX_WATER_CLUSTER_SIZE)
                {
                    auto [y, x] = q.front();
                    q.pop();

                    // Sprawdzenie sąsiadów
                    std::vector<std::pair<int, int>> neighbors = {
                        {y - 1, x}, {y + 1, x}, {y, x - 1}, {y, x + 1}, {y - 1, x - 1}, {y - 1, x + 1}, {y + 1, x - 1}, {y + 1, x + 1}

                    };

                    for (auto &[ny, nx] : neighbors)
                    {
                        // Sprawdzenie granic Mapy
                        if (ny >= 0 && ny < SIZEY && nx >= 0 && nx < SIZEX)
                        {
                            if ((tablicaZnakow[ny][nx] == 'Z'))
                            {
                                // Prawdopodobieństwo dodania do klastru
                                if (distribution(generator))
                                {
                                    tablicaZnakow[ny][nx] = 'S';
                                    q.push({ny, nx});
                                    clusterSize++;
                                    if (clusterSize >= MAX_WATER_CLUSTER_SIZE)
                                        break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

bool Map::ifFarEnough(std::vector<std::pair<int, int>> &bazy, int x1, int y1,int playerNum)
{

    for (size_t i = 0; i < bazy.size(); i++)
    {
        if ([](int x1, int y1, int x2, int y2)
            { return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)); }
            (bazy[i].first, bazy[i].second, x1, y1) < MIN_DISTANCE_BETWEEN_BASES/playerNum)
        {

            return false;
        }
    }

    return true;
}

bool Map::isPossibleRouteBetweenPoint(const std::vector<std::pair<int, int>> &bases, int x1, int y1)
{

        Point cel = {x1, y1};
    
        for (const auto &base : bases)
        {
            Point start = {base.first, base.second};
            if (!AStar(tablicaZnakow, start, cel)){
                return false;
            }
        }
        return true;

}

std::vector<std::pair<int, int>> Map::makeBases(int playerNum)
{
    std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> distY(0, SIZEY - 1);
    std::uniform_int_distribution<int> distX(0, SIZEX - 1);

    std::bernoulli_distribution distribution(0.1);

    std::vector<std::pair<int, int>> bazy;
    const int MAX_ATTEMPTS = 1000*SIZEY*SIZEX;
    int attempts = 0;

    while (bazy.size() < static_cast<size_t>(playerNum) && attempts < MAX_ATTEMPTS)
    {
        int seedY = distY(generator);
        int seedX = distX(generator);
        attempts++;

        if (tablicaZnakow[seedY][seedX] == 'Z' && distribution(generator))
        {
            if (bazy.empty())
            {
                tablicaZnakow[seedY][seedX] = 'B';
                bazy.emplace_back(seedX, seedY);
            }
            else
            {
                if (ifFarEnough(bazy, seedX, seedY,playerNum) &&
                    isPossibleRouteBetweenPoint(bazy, seedX, seedY))
                {
                    tablicaZnakow[seedY][seedX] = 'B';
                    bazy.emplace_back(seedX, seedY);
                }
            }
        }
    }


    return bazy;
}

std::vector<std::pair<int, int>> Map::inicializeMap(int playerNum)
{
    generateForest();
    generateMountains();
    generateWater();
    makeSwamp();

    return makeBases(playerNum);
}
