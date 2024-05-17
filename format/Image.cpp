#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdint>

#define Matrix vector<vector<vector<unsigned char>>>
 
using namespace std;

class Image
{
protected:
    Matrix matrix;
    string imagePath;

public:
    Image(string imagePath);
    virtual void load(string imagePath) = 0;
    virtual string save() = 0;
    virtual void resize(Matrix &) = 0;

    int getWidth() const;
    int getHeight() const;
};

Image::Image(string imagePath)
{
    this->imagePath = imagePath;
}

int Image::getWidth() const {
    if (matrix.empty()) {
        return 0; 
    }
    return matrix[0].size();
}

int Image::getHeight() const {
    return matrix.size();
}
