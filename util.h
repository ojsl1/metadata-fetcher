#ifndef UTIL_H
#define UTIL_H

#include <cstdint>
#include "png.h"
#include <vector>
#include <string>

struct PNGDisplayInfo
{
  uint32_t height;
  uint32_t width;
  int bitDepth;
  std::vector<std::string> lines;
};

/* @todo std::cout -> out.lines.emplace_back(..)
 */
PNGDisplayInfo ExtractPNGInfo(const char* filename);

/* @todo std::cout -> out.lines.emplace_back(..)
 */
void ReadTextChunks(png_structp png, png_infop info);

/* @todo std::cout -> out.lines.emplace_back(..)
 * @todo std::err -> <stdexcept>
 *
 * @note std::ifstream automatically closes the file when it goes out of scope.
 * So no need to fclose(file) repeatedly.
 */
void PrintPNGInfo(const char* filename);

extern PNGDisplayInfo pngInfo;

#endif // UTIL_H
