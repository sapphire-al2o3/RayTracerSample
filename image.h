#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <fstream>
#include "vec3.h"
#include "util.h"

template <typename T>
class Image
{
public:
    T** lines;
    int width;
    int height;

    Image(int _width, int _height) : width(_width), height(_height)
    {
        lines = new T*[height];
        lines[0] = new T[width * height];
        for(int i = 1; i < height; i++) {
            lines[i] = &lines[0][width * i];
        }
    }

    ~Image() {
        delete[] lines[0];
        delete[] lines;
    }
};

void ppm_output(const Image<Vec3>& image, const std::string& filename)
{
    std::ofstream file(filename);
    file << "P3" << std::endl;
    file << image.width << " " << image.height << std::endl;
    file << "255" << std::endl;
    for(int j = 0; j < image.height; j++) {
        for(int i = 0; i < image.width; i++) {
            Vec3 c = image.lines[j][i];
            int r = clamp((int)(255 * c.x), 0, 255);
            int g = clamp((int)(255 * c.y), 0, 255);
            int b = clamp((int)(255 * c.z), 0, 255);
            file << r << " " << g << " " << b << std::endl;
        }
    }
    file.close();
}

void divide(const Image<Vec3>& image, double k) {
    for(int i = 0; i < image.height; i++) {
        for(int j = 0; j < image.width; j++) {
            image.lines[i][j] = image.lines[i][j] / k;
        }
    }
}

#endif
