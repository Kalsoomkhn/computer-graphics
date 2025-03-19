#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct Image {
    string magic_no;
    int width, height, maximum_color;
    unsigned char** red;
    unsigned char** green;
    unsigned char** blue;

    Image() : red(nullptr), green(nullptr), blue(nullptr) {}

    Image(const Image& m) {
        magic_no = m.magic_no;
        width = m.width;
        height = m.height;
        maximum_color = m.maximum_color;
        allocateMemory();
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                red[i][j] = m.red[i][j];
                green[i][j] = m.green[i][j];
                blue[i][j] = m.blue[i][j];
            }
        }
    }

    void allocateMemory() {
        red = new unsigned char* [height];
        green = new unsigned char* [height];
        blue = new unsigned char* [height];
        for (int i = 0; i < height; i++) {
            red[i] = new unsigned char[width];
            green[i] = new unsigned char[width];
            blue[i] = new unsigned char[width];
        }
    }

    void readImage(const string& filename) {
        ifstream file(filename, ios::binary);
        if (!file) {
            cerr << "Error: Cannot open file!" << endl;
            return;
        }

        string line, widthStr, heightStr;

        // Skip comment line 
        getline(file, line);
        while (line[0] == '#') {
            getline(file, line);
        }

        magic_no = line;

        getline(file, line);
        while (line[0] == '#') {
            getline(file, line);
        }

        stringstream ss(line);
        ss >> widthStr >> heightStr;
        width = stoi(widthStr);
        height = stoi(heightStr);

        getline(file, line);
        while (line[0] == '#') {
            getline(file, line);
        }

        maximum_color = stoi(line);

        allocateMemory();
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; j++) {
                file.read((char*)&(red[i][j]), sizeof(unsigned char));
                file.read((char*)&(green[i][j]), sizeof(unsigned char));
                file.read((char*)&(blue[i][j]), sizeof(unsigned char));
            }
        }

        file.close();
    }

    void writeImage(const string& filename) {
        ofstream file(filename, ios::binary);
        if (!file) {
            cerr << "Error: Cannot open file for writing!" << endl;
            return;
        }

        file << magic_no << "\n" << width << " " << height << "\n" << maximum_color << "\n";

        // Write pixel data (RGB)
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; ++j) {
                file.write((char*)&(red[i][j]), sizeof(unsigned char));
                file.write((char*)&(green[i][j]), sizeof(unsigned char));
                file.write((char*)&(blue[i][j]), sizeof(unsigned char));
            }
        }

        file.close();
        cout << "PPM Image successfully written to " << filename << endl;
    }

    void negativeFilter() {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                red[i][j] = 255 - red[i][j];
                green[i][j] = 255 - green[i][j];
                blue[i][j] = 255 - blue[i][j];
            }
        }
    }

    void colouredImage(int x) {
        switch (x) {
        case 1: //red
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    red[i][j];
                    green[i][j] = 0;
                    blue[i][j] = 0;
                }
            }
            break;
        case 2: //green
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    red[i][j] = 0;
                    green[i][j];
                    blue[i][j] = 0;
                }
            }
            break;
        case 3: //blue
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    red[i][j] = 0;
                    green[i][j] = 0;
                    blue[i][j];
                }
            }
            break;
        case 4: //yellow
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    red[i][j];
                    green[i][j];
                    blue[i][j] = 0;
                }
            }
            break;
        case 5: //cyan
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    red[i][j] = 0;
                    green[i][j];
                    blue[i][j];
                }
            }
            break;
        case 6: //magenta
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    red[i][j];
                    green[i][j] = 0;
                    blue[i][j];
                }
            }
            break;
        }
    }

    void writeGreyImage(const string& filename) {
        ofstream file(filename, ios::binary);
        if (!file) {
            cerr << "Error: Cannot open file for writing!" << endl;
            return;
        }

        file << "P5" << "\n" << width << " " << height << "\n" << maximum_color << "\n";

       
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; ++j) {
                unsigned char px = (red[i][j] + green[i][j] + blue[i][j]) / 3;
                file.write((char*)&px, sizeof(unsigned char));
            }
        }

        file.close();
        cout << "PPM Image successfully written to " << filename << endl;
    }

    void createCombineImage(Image m2, double alpha) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                red[i][j] = (alpha * red[i][j]) + ((1 - alpha) * m2.red[i][j]);
                green[i][j] = (alpha * green[i][j]) + ((1 - alpha) * m2.green[i][j]);
                blue[i][j] = (alpha * blue[i][j]) + ((1 - alpha) * m2.blue[i][j]);
            }
        }
    }

    void subAmbient(const Image& m2) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                red[i][j] = max(0, red[i][j] - m2.red[i][j]);
                green[i][j] = max(0, green[i][j] - m2.green[i][j]);
                blue[i][j] = max(0, blue[i][j] - m2.blue[i][j]);
            }
    }
    }

    ~Image() {
        if (red) {
            for (int i = 0; i < height; i++) {
                delete[] red[i];
                delete[] green[i];
                delete[] blue[i];
            }
            delete[] red;
            delete[] green;
            delete[] blue;
        }
    }
};

void make_negative_image() {
    Image m1;
    string filename;
    cout << "Input file name: ";
    cin >> filename;
    m1.readImage(filename);
    m1.negativeFilter();
    cout << "Output file name: ";
    cin >> filename;
    m1.writeImage(filename);
}

void make_colored_image() {
    Image m1;
    string filename;
    cout << "Enter File name: ";
    cin >> filename;
    m1.readImage(filename);
    int num;
    cout << "1: Red" << endl;
    cout << "2: Green" << endl;
    cout << "3: Blue" << endl;
    cout << "4: Yellow" << endl;
    cout << "5: Cyan" << endl;
    cout << "6: Magenta" << endl;
    cin >> num;
    while (num < 1 || num >6) {
        cout << "Wrong option entered, select between 1-6: " << endl;
        cin >> num;
    }
    m1.colouredImage(num);
    string filename2;
    cout << "Enter filename: ";
    cin >> filename2;
    m1.writeImage(filename2);
}

void make_grayscale_image() {
    Image m1;
    string filename;
    cout << "Input file name: ";
    cin >> filename;
    m1.readImage(filename);
    cout << "Output file name (.pgm): ";
    cin >> filename;
    m1.writeGreyImage(filename);
}

void combine_images() {
    Image m1;
    string filename;
    cout << "Input first file :";
    cin >> filename;
    m1.readImage(filename);
    Image m2;
    string filename2;
    cout << "Input second file :";
    cin >> filename2;
    m2.readImage(filename2);
    double alpha;
    cout << "Enter alpha value: ";
    cin >> alpha;
    m1.createCombineImage(m2, alpha);
    string filename3;
    cout << "Output file name :";
    cin >> filename3;
    m1.writeImage(filename3);
}
void subtract_ambient() {
    Image m1;
    string filename;
    cout << "Input first file: ";
    cin >> filename;
    m1.readImage(filename);
    Image m2;
    string filename2;
    cout << "Input second file: ";
    cin >> filename2;
    m2.readImage(filename2);
    m1.subAmbient(m2);
    cout << "Output file name: ";
    cin >> filename;
    m1.writeImage(filename);
}

void create_images_movie() {
    Image m1, m2;
    string filename1, filename2;
    cout << "Input file1 name :";
    cin >> filename1;
    m1.readImage(filename1);
    cout << "Input file2 name :";
    cin >> filename2;
    m2.readImage(filename2);
    double alpharange = 0.0;
    int Ncounter = 1;
    while (alpharange <= 1) {
        Image temp(m1);
        temp.createCombineImage(m2, alpharange);
        string file = "movie" + to_string(Ncounter) + ".ppm";
        temp.writeImage(file);
        Ncounter++;
        alpharange = alpharange + 0.1;

    }
}

int main() {
    int option;


    cout << "Id_no : Operation" << endl;
    cout << "1     :  apply negative filter on image" << endl;
    cout << "2     :  apply color effect on image" << endl;
    cout << "3     :  create grayscale image (.pgm)" << endl;
    cout << "4     :  combine two images (Alpha Screening)" << endl;
    cout << "5     :  subtract ambient from left or right image" << endl;
    cout << "6     :  create movie of transformation between two images" << endl;
    cout << endl << "Enter option from above menu :";
    cin >> option;
    switch (option) {
    case 1:
        make_negative_image();
        break;
    case 2:
        make_colored_image();
        break;
    case 3:
        make_grayscale_image();
        break;
    case 4:
        combine_images();
        break;
    case 5:
        subtract_ambient();
        break;
    case 6:
        create_images_movie();
        break;
    default:
        cout << "Invalid option,Try again!" << endl;
    }



    return 0;
}