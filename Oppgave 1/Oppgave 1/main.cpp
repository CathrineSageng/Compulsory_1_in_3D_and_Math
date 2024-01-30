#include <iostream>
#include <fstream>
#include <cmath>
#include<vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
using namespace std;

//Stores the coordinates x, y
vector<float> verticesPositions;
//Stores the color coordinates for every single vertex 
vector<float> colors;
//Stores the results of the derivative
vector<float> derivativeResults;

int numberOfDataPoints = 41;

//Definition quantity
double a = -2.0; //The start of the definition quantity
double b = 2.0; //The end of the definition quantitity 

//Intervals n+1
int n = 40;

//Calculates the dissolution of h
double h = (b - a) / n;

// Opens the text file for writing
ofstream file("Data.txt");

//Is a string literal that contains the source code for a vertex shader. 
const char* vertexShaderSource = 
//Version 3,3 core
" #version 330 core\n"
//Represents the positon of the vertex. Locatioon 0 is the location of the data 
" layout (location = 0) in vec2 aPos;\n"
//Represents the color attributes of the vertex. Locatioon 1 is the location of the data 
" layout (location = 1) in vec3 aColor;\n"
//A variable called ourColor that will be used to pass color information from the vertex shader to the fragment shader. 
" out vec3 ourColor;\n"
//Calculates the final position of the vertex and assigns color to the vertex 
"void main() {\n"
"    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
"    ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource =
//Version 3,3 core
" #version 330 core\n"
//This variable recevies the color information passed from the vertex shader. 
" in vec3 ourColor;\n"
//The final color of the fragment
" out vec4 FragColor;\n"
//Sets the final color of the fragment. It takes the RBG values from the variable 'our color' and sets the transparency to 1.
"void main() {\n"
"    FragColor = vec4(ourColor, 1.0);\n"
"}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

double function(double x);
double differenceQuotient(double x);
void calculatefunction();

int main(void)
{
    // GLFW holds the information of a window (size, position etc.)
    // window is a pointer that stores adress where a 'GLFWwindow is located. 
    GLFWwindow* window;

    /* Initialize the GLFW library */
    if (!glfwInit())
    {
        cout << "Failed to initialize GLFW library" << endl;
        return -1;
    }
      
    //Specifies that the major and minor version of OpenGL is 3,3. 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //Specifies that the core profile of OpenGL is used. 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creates a window and checks if it was successful. 
    window = glfwCreateWindow(800, 600, "Graph of the function x^2", nullptr, nullptr);
    if (window == nullptr) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    // OpenGL is a state machine and when creating a window with GLFW, it comes with its own OpenGL context.
    //This line of code spesifices that the attributes given to 'window' should be rendered 
    glfwMakeContextCurrent(window);
   
    //Checks if the loading of the function pointers using GLAD is successful.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failied to load openGL pointers!" << endl;
        glfwTerminate();
        return -1;
    }

    //Sets up a window with the size 800 in width, 600 in height and 0,0 (lower left corner)
    glViewport(0, 0, 800, 600);
    //This function is called automatecally when the window is resized. 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Declares three variables 
    unsigned int vertexShader, fragmentShader, shaderProgram;
    //Creates a vertex shader object and the variable 'vertexShader' is assigned to the vartex shader object. 
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Sets the source code of the shader. vertexshader is the indentifier of the vertex shader to which are assigning the source code. 
    //1 is number of strings in the vertexShaderSource array. 
    //&vertexShaderSource is the pointer to a string containing the vertex shader source. 
    //0- the source code is null terminated. 
    glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
    //Compiles the shader code associated with the specified shader object. 
    glCompileShader(vertexShader);
    //Checks for compilation errors. 
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, 0, infoLog);
        cout << "Failed to compile the vertex shader" << infoLog << endl;
    }

    //The three lines of code below assignes the 'fragmentShader' variable the identifier for the compiled fragment shader object. 
    //Creates a fragment shader object and the variable 'fragmentShader' is assigned to the fragment shader object.
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);
    glCompileShader(fragmentShader);
    //Checks for compilation errors. 
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, 0, infoLog);
        cout << "Failed to compile the fragment shader" << infoLog << endl;
    }

    //Here we create a shader program. Attaching the compiled vertex shader and fragment shader, linking the program
    //and activates it for use in rendering. 
    //The shaderProgram variable has assigned the newly created shader program object. 
    shaderProgram = glCreateProgram();
    //The two functions below attaches the 'vertexShader' and the 'fragmentshader' to the shader program. 
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    //Links the shaders attached to the shader program. 
    glLinkProgram(shaderProgram);
    //Checks for linking errors. 
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, 0, infoLog);
        cout << "Failed to link the shader program" << infoLog << endl;
    }
    //Activates the shader program for rendering. 
    glUseProgram(shaderProgram);

    //Here we delete the individual shader objects after they have been attached and linked to the shader program. 
    //The shader program retains a reference to the compiled code, so deleting the shader objects do not affect the shader program. 
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    calculatefunction();

    //Creates a VAO and binds it 
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //Creates two VBO for the position and color to the graph
    unsigned int VBO[2];
    glGenBuffers(2, VBO);

    //Copies the x and y position to the graph to VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, verticesPositions.size() * sizeof(float), verticesPositions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Copies the color data for the graph to the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    //Closes the text file
    file.close();

    cout<< "The data points has been created and saved in the file 'Data.txt'"<<endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //Escape key closes the program 
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        // Bind VAO
        glBindVertexArray(VAO);

        glDrawArrays(GL_LINE_STRIP, 0, numberOfDataPoints);

        // Unbind VAO
        glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Cleans up and stops the program
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

//Closes the window when esc kay is pressed. 
void processInput(GLFWwindow* window)
{
    //Closes the window with the esc key
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

//This is a callback function that adjusts the dimentions it should render in when the window is resized.  
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    cout << "The window size is: " << width << " x " << height << endl;
}

// Function f(x)= x^2
double function(double x)
{
    return pow(x, 2);
}

double differenceQuotient(double x)
{
    //Calculates the Newtons quotient
    double h = 0.01;
    return (function(x + h) - function(x)) / h;
}

void calculatefunction()
{
    for (int i = 0; i < numberOfDataPoints; ++i)
    {   
        double x = a + i * h;
        double y = function(x);
        double derivative = differenceQuotient(x);

        //The two first lines stores the coordinates for x anf y. 
        //The third line stores the derivative for each vertex 
        verticesPositions.push_back(x);
        verticesPositions.push_back(y);
        derivativeResults.push_back(derivative);

        float red, green, blue;
        if (derivative > 0) {
            //If derivative is greater than 0, the vertex color is green 
            red = 0.0f;
            green = 1.0f;
            blue = 0.0f;
        }
        else {
            //If the derivative is below 0, the vertex color is red. 
            red = 1.0f;
            green = 0.0f;
            blue = 0.0f;
        }

        //These three lines are used to store the calculated values for r, g, b and put in the end of the vector
        colors.push_back(red);
        colors.push_back(green);
        colors.push_back(blue);

        file << "x: " << x << " y: " << " derivative: " << derivative << y << " r: "
            << red << " g: " << green << " b: " << blue << endl;
    }
}