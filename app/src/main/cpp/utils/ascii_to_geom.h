//
// Created by aosp on 2023/2/26.
//

#ifndef GAME_ASCII_TO_GEOM_H
#define GAME_ASCII_TO_GEOM_H

#include "../engine/engine.h"

/* Converts ASCII art into a Vbo/Ibo pair. Useful for retro-looking
 * drawings/text! scale is the size of each character. The center of the
 * rendering will be 0,0.
 *
 * Examples:
 *
 * Square:
 *     +---+
 *     |   |
 *     +---+
 *
 * Triangle:
 *     +-----+
 *      `   /
 *       ` /
 *        +
 *
 * The + sign represents a vertex; lines are represented by -, /, ` and |.
 */
SimpleGeom* AsciiArtToGeom(const char* art, float scale);

#endif //GAME_ASCII_TO_GEOM_H
