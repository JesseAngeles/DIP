#include "Image.cpp"
#include <cstring>
#include <zlib.h>

#pragma pack(push, 1) // Alinear las estructuras en 1 byte
struct PngFileHeader
{
    uint8_t signature[8]; // Firma PNG
};

struct PngChunkHeader
{
    uint32_t length; // Longitud del bloque de datos
    uint8_t type[4]; // Tipo de bloque de datos
};

struct PngIHDR
{
    int32_t width;          // Anchura de la imagen
    int32_t height;         // Altura de la imagen
    int8_t bitDepth;        // Profundidad de bits por canal
    int8_t colorType;       // Tipo de color (RGB, RGBA, gray-scale, etc)
    int8_t compression;     // Metodo de compresión
    int8_t filterMethod;    // Metodo de filtrado
    int8_t interlaceMethod; // Metodo de entrelazado
};
#pragma pack(pop) // Restaurar el alineamiento por defecto

struct PngChunk
{
    uint32_t length;      // Longitud del chunk
    uint8_t type[4];      // Tipo de chunk
    vector<uint8_t> data; // Datos del chunk
    uint32_t crc;         // CRC del chunk
};

class PNG : public Image
{
private:
    PngFileHeader pngFileHeader;
    PngChunkHeader pngChunkHeader;
    PngIHDR pngIHDR;

public:
    PNG(string imagePath);
    void load(string imagePath) override;
    string save() override;
    void resize(Matrix &) override;
};

PNG::PNG(string imagePath) : Image(imagePath) {}

void PNG::load(string imagePath)
{
    ifstream file(imagePath, ios::binary);

    if (!file)
    {
        cerr << "Error opening the file: " << imagePath << endl;
        return;
    }

    // Leer headers
    file.read(reinterpret_cast<char *>(&pngFileHeader), sizeof(pngFileHeader));

    while (file.read(reinterpret_cast<char *>(&pngChunkHeader), sizeof(pngChunkHeader)))
    {
        PngChunk chunk;
        chunk.length = pngChunkHeader.length;
        memcpy(chunk.type, pngChunkHeader.type, 4);

        chunk.data.resize(chunk.length);
        file.read(reinterpret_cast<char *>(chunk.data.data()), chunk.length);
        file.read(reinterpret_cast<char *>(&chunk.crc), sizeof(chunk.crc));

        if (memcmp(chunk.type, "IHDR", 4) == 0)
        {
            memcpy(&pngIHDR, chunk.data.data(), sizeof(pngIHDR));
            // Redimensionar la matriz
            matrix.resize(pngIHDR.height, vector<vector<unsigned char>>(pngIHDR.width, vector<unsigned char>(3)));
        }
        else if (memcmp(chunk.type, "IDAT", 4) == 0)
        {
            // Aquí agregaríamos la lógica para procesar los datos de imagen IDAT y descomprimirlos
            // Ejemplo de cómo almacenar los datos de la imagen comprimida
        }
        else if (memcmp(chunk.type, "IEND", 4) == 0)
        {
            break;
        }
    }

    // Añadir el algoritmo de descompresión Deflate aquí para los datos de IDAT
    // Nota: Esto requiere la implementación del descompresor y el proceso de descompresión.
}

string PNG::save()
{
    // Implementación para guardar un archivo PNG
    return "";
}

void PNG::resize(Matrix &matrix)
{
    // Implementación para redimensionar la imagen PNG
}
