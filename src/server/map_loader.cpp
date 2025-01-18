#include "map_loader.hpp"


Map_Generator::Map_Generator(unsigned int seed){
    this->seed = seed; 

    this->create_map();
}

void Map_Generator::create_map(void){
    for (auto i = 0; i < MAP_WIDTH; i++){
        for (auto j = 0; j < MAP_HEIGHT; j++){
            if (j == MAP_HEIGHT - 1){
                this->tile_map[i][j] = GROUND;
            }
            else {
                this->tile_map[i][j] = NO_TILE;
            }
        }
    }

    this->set_horizone();
}

void Map_Generator::set_horizone(void){
    int highest = 80;
    int lowest = 120;

    int x = 0;
    int y = 100;

    std::default_random_engine generator(this->seed);

    // Set next blocks (TMP)
    int t [] = {2, 2, 2, 1};
    int weights[] = {1, 2, 3, 6};

    int terrain_types = 0;
    for (auto i = 0; i < 4; i++){
        terrain_types += weights[i] * t[i];
    }
    std::uniform_int_distribution<int> dtr_1(0, terrain_types);
    while (x < MAP_WIDTH){
        int random_type = dtr_1(generator);
        if (random_type == 0 || random_type == 1){
            if (random_type == 0) {
                y += 3;
                y = std::min(y, lowest);
            }
            else {
                y -= 3;
                y = std::max(y, highest);
            }
            this->fill_dirt(x, y);
            x++;
        }
        else if (random_type >= 2 && random_type <= 5){
            std::uniform_int_distribution<int> dtr_2(1, 2);
            int times = dtr_2(generator);
            for (auto i = 0; i < times; i++){
                if (random_type <= 3){
                    y += 2;
                    y = std::min(y, lowest);
                }
                else {
                    y -= 2;
                    y = std::max(y, highest);
                }
                this->fill_dirt(x, y);
                x++;
            }
        }
        else if (random_type >= 6 && random_type <= 11){
            std::uniform_int_distribution<int> dtr_3(1, 3);
            int times = dtr_3(generator);
            for (auto i = 0; i < times; i++){
                if (random_type <= 8){
                    y += 1;
                    y = std::min(y, lowest);
                }
                else {
                    y -= 1;
                    y = std::max(y, highest);
                }
                this->fill_dirt(x, y);
                x++;
            }
        }
        else {
            std::uniform_int_distribution<int> dtr_3(1, 6);
            int times = dtr_3(generator);
            for (auto i = 0; i < times; i++){
                this->fill_dirt(x, y);
                x++;
            }
        }
    }
    

}

void Map_Generator::save_map(void){
    std::ofstream file;
    file.open("map.txt");
    for (auto i = 0; i < MAP_HEIGHT; i++){
        for (auto j = 0; j < MAP_WIDTH; j++){
            file << this->tile_map[j][i];
        }
        file << '\n';
    }
    file.close();
}

void Map_Generator::fill_dirt(int x, int y){
    if (x >= MAP_WIDTH || y >= MAP_HEIGHT){
        return;
    }

    for (auto i = y; i < MAP_HEIGHT - 1; i++){
        this->tile_map[x][i] = DIRT;
    }
}