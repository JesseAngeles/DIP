// BMP.cpp

#include <Image.cpp>

#pragma pack(push, 1) // Alinear las estructuras en 1 byte
struct BmpFileHeader
{
    uint16_t fileType;   // Tipo de archivo siempre debe ser 0x4D42 ("BM")
    uint32_t fileSize;   // Tamaño del archivo en bytes
    uint16_t reserved1;  // Reservado
    uint16_t reserved2;  // Reservado
    uint32_t offsetData; // Offset a los datos de imagen
};

struct BmpInfoHeader
{
    uint32_t size;         // Tamaño de esta cabecera
    int32_t width;         // Anchura de la imagen
    int32_t height;        // Altura de la imagen
    uint16_t planes;       // Número de planos de color
    uint16_t bitCount;     // Número de bits por pixel
    uint32_t compression;  // Tipo de compresión
    uint32_t sizeImage;    // Tamaño de los datos de imagen
    int32_t xPelsPerMeter; // Resolución horizontal
    int32_t yPelsPerMeter; // Resolución vertical
    uint32_t clrUsed;      // Número de colores en la paleta
    uint32_t clrImportant; // Colores importantes
};
#pragma pack(pop) // Restaurar el alineamiento por defecto

class BMP : public Image
{
private:
    BmpFileHeader bmpFileHeader;
    BmpInfoHeader bmpInfoHeader;
public:
    BMP(string imagePath);
    void load(string imagePath) override;
    string save() override;
    void resize(Matrix &) override;
};

void BMP::load(string imagePath)
{
    fstream file(imagePath, ios::binary);
    if (!file)
    {
        cerr << "Error openning the file: " << imagePath << endl;
        return;
    }

    // Read headers
    file.read(reinterpret_cast<char *>(&bmpFileHeader), sizeof(bmpFileHeader));
    file.read(reinterpret_cast<char *>(&bmpInfoHeader), sizeof(bmpInfoHeader));

    if (bmpFileHeader.fileType != 0x4D42)
    {
        cerr << "Not a BMP file" << endl;
        return;
    }

    // Resize matrix
    matrix.resize(bmpInfoHeader.height, vector<vector<unsigned char>>(bmpInfoHeader.width, vector<unsigned char>(3)));

    // Move pointer to matrix data
    file.seekg(bmpFileHeader.offsetData, ios::beg);

    for (int y = 0; y < bmpInfoHeader.height; y++)
        for (int x = 0; x < bmpInfoHeader.width; x++)
        {
            file.read(reinterpret_cast<char *>(&matrix[y][x][2]), 1); // Blue
            file.read(reinterpret_cast<char *>(&matrix[y][x][1]), 1); // Green
            file.read(reinterpret_cast<char *>(&matrix[y][x][0]), 1); // Red
        }

    file.close();
}

string BMP::save()
{
    ofstream file(this->imagePath, ios::binary);
    if (!file)
    {
        cerr << "Error openning the file: " << imagePath << endl;
        return "";
    }

    // Escribir encabezados
    file.write(reinterpret_cast<char *>(&bmpFileHeader), sizeof(bmpFileHeader));
    file.write(reinterpret_cast<char *>(&bmpInfoHeader), sizeof(bmpInfoHeader));

    // Moverse a la matriz
    file.seekp(bmpFileHeader.offsetData);

    for (int y = 0; y < bmpInfoHeader.height; y++)
    {
        for (int x = 0; x < bmpInfoHeader.width; x++)
        {
            file.write(reinterpret_cast<char *>(&matrix[y][x][2]), 1); // Blue
            file.write(reinterpret_cast<char *>(&matrix[y][x][1]), 1); // Green
            file.write(reinterpret_cast<char *>(&matrix[y][x][0]), 1); // Red
        }

        // bytes de relleno
        for (int i = 0; i < bmpInfoHeader.width % 4; ++i)
        {
            char paddingByte = 0;
            file.write(&paddingByte, 1);
        }
    }

    file.close();

    return this->imagePath;
}

void BMP::resize(Matrix &matrix)
{
    int width = getWidth(matrix);
    int height = getHeight(matrix);

    if (width <= 0 || height <= 0)
    {
        cout << "Invalid size" << endl;
        return;
    }

    this->bmpInfoHeader.width = width;
    this->bmpInfoHeader.height = height;

    this->bmpInfoHeader.sizeImage = (width * 3) * height;
    this->bmpFileHeader.fileSize = sizeof(BmpFileHeader) + sizeof(BmpInfoHeader) + this->bmpInfoHeader.sizeImage;

    this->matrix.clear();
    this->matrix = matrix;
}
