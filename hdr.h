#ifndef HDR_H
#define HDR_H

#include <stdio.h>
#include <cmath>
#include <iostream>
#include "vec3.h"
#include "image.h"

void ReadRLE(unsigned char* channel, FILE* fp, int length) {
    int index = 0;
    while (index < length) {
        unsigned char buf[1];
        fread(buf, 1, 1, fp);
        int n = buf[0];
        if (n > 128) {
            fread(buf, 1, 1, fp);
            for (int k = 0; k < (n & 127); k++) {
                channel[index++] = buf[0];
            }
        } else {
            for (int k = 0; k < n; k++) {
                fread(buf, 1, 1, fp);
                channel[index++] = buf[0];
            }
        }
    }
    
}

int LoadHDRImage(const char* fileName, Image<Vec3>& image) {

    FILE* fp;
    fopen_s(&fp, fileName, "rb");

    if (fileName == nullptr || fp == nullptr) return 0;

    unsigned char buf[16];

    while (1) {
        fread(buf, 1, 1, fp);
        if (buf[0] == 0x0A) {
            fread(buf, 1, 1, fp);
            if (buf[0] == 0x0A) {
                break;
            }
        }
    }

    int width = 0;
    int height = 0;
    int line;
    fread(buf, 3, 1, fp);
    if (buf[1] == 'X') {
        while (1) {
            fread(buf, 1, 1, fp);
            if (buf[0] == 0x20) {
                break;
            }
            width = width * 10 + (buf[0] - '0');
            std::cout << width << std::endl;
        }
        fread(buf, 3, 1, fp);
        while (1) {
            fread(buf, 1, 1, fp);
            if (buf[0] == 0x0A) {
                break;
            }
            height = height * 10 + (buf[0] - '0');
        }
    } else if (buf[1] == 'Y') {
        while (1) {
            fread(buf, 1, 1, fp);
            if (buf[0] == 0x20) {
                break;
            }
            height = height * 10 + (buf[0] - '0');
        }
        fread(buf, 3, 1, fp);
        while (1) {
            fread(buf, 1, 1, fp);
            if (buf[0] == 0x0A) {
                break;
            }
            width = width * 10 + (buf[0] - '0');
        }
        line = height;
    }

    if (image.lines == NULL) {
        image.create(width, height);
    }

    if (image.lines != nullptr) {
        std::cout << "w:" << image.width << std::endl;
        std::cout << "h:" << image.height << std::endl;
        // image.lines[0][359] = Vec3(0, 0, 0);
        std::cout << image.lines[0][359] << std::endl;
        
    }

    for (int j = 0; j < 359; j++) {
        image.lines[0][j] = Vec3(0, 0, 0);
    }

    unsigned char* r = new unsigned char[width];
    unsigned char* g = new unsigned char[width];
    unsigned char* b = new unsigned char[width];
    unsigned char* e = new unsigned char[width];

    for (int i = 0; i < line; i++) {
        fread(buf, 2, 1, fp);
        if (buf[0] != 0x2 || buf[1] != 0x2) {
            std::cout << "error" << ftell(fp) << std::endl;
            break;
        }
        fread(buf, 2, 1, fp);
        int count = (buf[0] << 8) + buf[1];

        // std::cout << i << "-" << count << ":" << ftell(fp) << std::endl;

        ReadRLE(r, fp, count);
        ReadRLE(g, fp, count);
        ReadRLE(b, fp, count);
        ReadRLE(e, fp, count);

        for (int j = 0; j < count; j++) {
            double p = pow(2, e[j] - 128);
            image.lines[i][j] = Vec3(r[j] * p, g[j] * p, b[j] * p);
        }
        // break;
    }

    delete[] r;
    delete[] g;
    delete[] b;
    delete[] e;

    return 1;
}

#endif