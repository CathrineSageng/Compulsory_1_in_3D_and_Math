#include <iostream>
#include <fstream>
#include <cmath>
#include <GLFW/glfw3.h>
using namespace std;

// Function f(x)= x^3
double function( const double x)
{
    return pow(x, 3);
}

int main(void)
{
    //Definition quantity
    double a = -2.0; //The start of the definition quantity
    double b = 2.0; //The end of the definition quantitity 

    //Intervals n+1
    int n = 40;

    //Calculates the dissolution of h
    double h = (b - a) / n;

    // Opens the text file for writing
    ofstream outputFile("Data.txt");

    //Creates data points and saves them to the text file. 
    for (int i = 0; i <= n; i++)
    {
        double x = a + i * h;
        double y = function(x);

        //Writes the data points to the text file "Data.txt"
        outputFile <<"x: " << x << " y: " << y << endl;
    }

    //Closes the text file
    outputFile.close();

    cout<< "The data points has been created and saved in the file 'Data.txt'"<<endl;


    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 960, "Graph of the function f(x)= x^3", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}