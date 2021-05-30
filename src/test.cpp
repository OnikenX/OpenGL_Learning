//
// Created by onikenx on 28/05/21.
//
#include <iostream>
#include <cmath>
#include <vector>

void coisas() {
    std::vector<double> values;
    for (auto i = 0.0; i < 10.0; i += 0.001)
        values.push_back(sin(i));

    std::cout << *std::min_element(values.begin(), values.end()) << "  "
              << *std::max_element(values.begin(), values.end()) << "  "
              << values.size() << std::endl;

}

#define MAX_SIZE 3

int main() {
    std::string fragmentShader =
            R"(#version 330 core
layout(location = 0) out vec4 color;
void main()
{
    //rgba
    color = vec4(1.0, 0.0, 0.0, 1.0);
}
)";
    std::cout << fragmentShader << std::endl;
}