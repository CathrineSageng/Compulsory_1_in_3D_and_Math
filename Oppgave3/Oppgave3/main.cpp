#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

double function(double x, double y);
void calculateColor(double x, double& r, double& g, double& b);

int main() {
    //Opens a textfile
    ofstream outfile("Data.txt");

    //Definition quantity
    double a_x = -2.0; //The start of the definition quantity x- axis 
    double b_x = 2.0; //The end of the definition quantity x-axis 
    double a_y = -2.0; //The start of the definition quantity y- axis 
    double b_y = 2.0; //The end of the definition quantity y-axis 

    //Number of data points (coordinates)
    int numberOfVertices_x = 10;
    int numberOfVertices_y = 10; 

    //Calculates the dissolution of h
    double h_x = (b_x - a_x) / (numberOfVertices_x-1);
    double h_y = (b_y - a_y) / (numberOfVertices_y-1);

    //Shows in the top of the text file how many lines of data points is in the text file 
    outfile << "Number of lines: " <<  numberOfVertices_x * numberOfVertices_y << endl;

    for (int i = 0; i < numberOfVertices_x; ++i) {
        for (int j = 0; j < numberOfVertices_y; ++j) {
            // Calculates the values for x and y coordinates 
            double x = a_x + i * h_x;
            double y = a_y + j * h_y;

            double z = function(x, y);

            //Calculates the rbg values based on the x value 
            double r;
            double g;
            double b;
            calculateColor(x, r, g, b);

            // Writes out the coordinates to the text file 
            outfile << "x: " << x << " y: " << y << " z: " << z << 
            " red: "<< r << " green: " << g << " blue: " << b<<  endl;
        }
    }

    // Closes the textfile 
    outfile.close();

    cout << "The data points has been created and saved in the file 'Data.txt'" << endl;

    return 0;
}

//Used a function from the lecture notes f(x,y)=2x^2y
double function(double x, double y)
{
    return 2.0 * pow(x, 2) * y;
}

//This function map the input value x from its original number to normalized 
//value from 0 to 1 that is used for color in openGL
void calculateColor(double x, double& r, double& g, double& b) {
    
    //This calculation do so the range of value x is between -0,5 and 0,5 
    r = (x + 1.0) / 2.0; 
    g = (x + 1.0) / 2.0;
    b = (x + 1.0) / 2.0;

    //Normalizes the RBG- values to be between 0 and 1, so it will read correctly in an OpenGL project 
    //This makes the values be between 0 and 1 and not -0,5 and 0,5 
    r = max(0.0, min(1.0, r));
    g = max(0.0, min(1.0, g));
    b = max(0.0, min(1.0, b));
}
