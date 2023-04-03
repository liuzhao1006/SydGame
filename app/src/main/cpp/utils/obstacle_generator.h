//
// Created by aosp on 2023/2/26.
//

#ifndef GAME_OBSTACLE_GENERATOR_H
#define GAME_OBSTACLE_GENERATOR_H

#include "../engine/engine.h"
#include "obstacle.h"

// Generates obstacles given a difficulty level.
class ObstacleGenerator {
private:
    int mDifficulty;

public:
    ObstacleGenerator() { mDifficulty = 0; }

    void SetDifficulty(int dif) { mDifficulty = dif; }

    // generate a new obstacle.
    void Generate(Obstacle *result) const;

private:
    static void GenEasy(Obstacle *result);
    static void GenMedium(Obstacle *result);
    static void GenIntermediate(Obstacle *result);
    static void GenHard(Obstacle *result);

    static void FillRow(Obstacle *result, int row);
    static void FillCol(Obstacle *result, int col);
};

#endif //GAME_OBSTACLE_GENERATOR_H
