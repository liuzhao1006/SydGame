//
// Created by aosp on 2023/2/26.
//

#include "obstacle_generator.h"

#include "../game_consts.h"

void ObstacleGenerator::Generate(Obstacle *result) const {
    static const int PROB_TABLE[] = {
            // EASY   MED  INT  HARD
            100, 0,   0,   0,   // difficulty 0
            75,  25,  0,   0,   // difficulty 1
            50,  50,  0,   0,   // difficulty 2
            25,  75,  0,   0,   // difficulty 3
            0,   100, 0,   0,   // difficulty 4
            0,   75,  25,  0,   // difficulty 5
            0,   50,  50,  0,   // difficulty 6
            0,   25,  75,  0,   // difficulty 7
            0,   0,   100, 0,   // difficulty 8
            0,   0,   75,  25,  // difficulty 9
            0,   0,   50,  50,  // difficulty 10
            0,   0,   25,  75,  // difficulty 11
            0,   0,   0,   100  // difficulty 12+
    };
    result->Reset();
    result->style = 1 + Random(7);

    int d = Clamp(mDifficulty, 0, 12);
    int easyProb = PROB_TABLE[d * 4];
    int medProb = PROB_TABLE[d * 4 + 1];
    int intermediateProb = PROB_TABLE[d * 4 + 2];
    int roll = Random(100);
    if (roll <= easyProb) {
        GenEasy(result);
    } else if (roll <= easyProb + medProb) {
        GenMedium(result);
    } else if (roll <= easyProb + medProb + intermediateProb) {
        GenIntermediate(result);
    } else {
        GenHard(result);
    }
    result->PutRandomBonus();
}

void ObstacleGenerator::FillRow(Obstacle *result, int row) {
    for (auto & i : result->grid) {
        i[row] = true;
    }
}

void ObstacleGenerator::FillCol(Obstacle *result, int col) {
    for (int i = 0; i < OBS_GRID_SIZE; ++i) {
        result->grid[col][i] = true;
    }
}

void ObstacleGenerator::GenEasy(Obstacle *result) {
    int n = Random(4);
    int i, j;
    Obstacle *o = result;  // shorthand
    switch (n) {
        case 0:
            i = Random(1, OBS_GRID_SIZE - 1);           // i is the row of the bonus
            FillRow(result, i + (Random(2) ? 1 : -1));  // horizontal bar next to i
            break;
        case 1:
            i = Random(1, OBS_GRID_SIZE - 1);  // i is the column of the bonus
            FillCol(result, i + (Random(2) ? 1 : -1));  // vertical bar next to i
            break;
        case 2:
            FillRow(result, 0);
            FillRow(result, OBS_GRID_SIZE - 1);
            FillCol(result, 0);
            FillCol(result, OBS_GRID_SIZE - 1);
            break;
        default:
            i = Random(0, OBS_GRID_SIZE - 2);  // i is the row of the bonus
            j = Random(0, OBS_GRID_SIZE - 2);  // i is the row of the bonus
            o->grid[i][j] = o->grid[i + 1][j] = o->grid[i][j + 1] =
            o->grid[i + 1][j + 1] = true;
            break;
    }
}

void ObstacleGenerator::GenMedium(Obstacle *result) {
    int n = Random(3);
    int i;
    switch (n) {
        case 0:
            i = Random(1, OBS_GRID_SIZE - 1);  // i is the row of the bonus
            FillRow(result, i + 1);
            FillRow(result, i - 1);
            break;
        case 1:
            i = Random(1, OBS_GRID_SIZE - 1);  // i is the column of the bonus
            FillCol(result, i - 1);
            FillCol(result, i + 1);
            break;
        default:
            i = Random(1, OBS_GRID_SIZE - 1);  // i is the column of the bonus
            FillRow(result, i);
            FillCol(result, i);
            break;
    }
}

void ObstacleGenerator::GenIntermediate(Obstacle *result) {
    int n = Random(3);
    int i;
    switch (n) {
        case 0:
            i = Random(0, OBS_GRID_SIZE - 2);
            FillRow(result, i);
            FillRow(result, i + 1);
            FillRow(result, i + 2);
            break;
        case 1:
            i = Random(0, OBS_GRID_SIZE - 2);  // i is the column of the bonus
            FillCol(result, i);
            FillCol(result, i + 1);
            FillCol(result, i + 2);
            break;
        default:
            i = Random(1, OBS_GRID_SIZE - 2);  // i is the column of the bonus
            FillCol(result, i - 1);
            FillCol(result, i + 1);
            FillCol(result, i + 2);
            break;
    }
}

void ObstacleGenerator::GenHard(Obstacle *result) {
    int n = Random(4);
    int i;
    int j;
    switch (n) {
        case 0:
            i = Random(0, OBS_GRID_SIZE - 3);
            FillRow(result, i);
            FillRow(result, i + 1);
            FillRow(result, i + 2);
            FillRow(result, i + 3);
            result->grid[Random(0, OBS_GRID_SIZE)][Random(0, OBS_GRID_SIZE)] = false;
            break;
        case 1:
            i = Random(0, OBS_GRID_SIZE - 3);
            FillCol(result, i);
            FillCol(result, i + 1);
            FillCol(result, i + 2);
            FillCol(result, i + 3);
            result->grid[Random(0, OBS_GRID_SIZE)][Random(0, OBS_GRID_SIZE)] = false;
            break;
        case 2:
            i = Random(0, OBS_GRID_SIZE);
            for (j = 0; j < OBS_GRID_SIZE; j++) {
                if (i != j) {
                    FillCol(result, i);
                }
            }
            result->grid[Random(0, OBS_GRID_SIZE)][Random(0, OBS_GRID_SIZE)] = false;
            break;
        default:
            i = Random(0, OBS_GRID_SIZE);
            for (j = 0; j < OBS_GRID_SIZE; j++) {
                if (i != j) {
                    FillRow(result, i);
                }
            }
            result->grid[Random(0, OBS_GRID_SIZE)][Random(0, OBS_GRID_SIZE)] = false;
            break;
    }
}
