#include <iostream>
#include <fstream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
using namespace std;

//Generates 100 vertices (points on the graph) of the mathematical function x^2
const int numberOfVertices = 200;

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
//Calculates the final position of the vetex and assigns color to the vertex 
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

void function(double t, double& x, double& y, double& z)
{
    // Example equations for a three-dimensional spiral
    x = t * cos(t);
    y = t * sin(t);
    z = t;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

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

    ////Definition quantity
    //double a = -2.0; //The start of the definition quantity
    //double b = 2.0; //The end of the definition quantitity 

    ////Intervals n+1
    //int n = 40;

    ////Calculates the dissolution of h
    //double h = (b - a) / n;

    // Opens the text file for writing
    ofstream file("Data.txt");

    // Declares tree variables 
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

    file << numberOfVertices << endl;

    float vertices[numberOfVertices * 6];
    unsigned int indices[numberOfVertices];

    // Generates vertex data
    for (int i = 0; i < numberOfVertices; ++i)
    {
        double t = i * 0.1;

        double x, y, z;
        function(t, x, y, z);

        double r = sin(t);
        double g = cos(t);
        double b = 0.5;

        vertices[i * 6] = static_cast<float>(x);
        vertices[i * 6 + 1] = static_cast<float>(y);
        vertices[i * 6 + 2] = static_cast<float>(z);
        vertices[i * 6 + 3] = static_cast<float>(r);
        vertices[i * 6 + 4] = static_cast<float>(g);
        vertices[i * 6 + 5] = static_cast<float>(b);

        indices[i] = i;
    }

    unsigned int VAO, VBO, EBO;
    //Generates one vertex array object and assigns its ID to the variable VAO
    //VAO specify how vertex data is organized and accessed.
    glGenVertexArrays(1, &VAO);
    //Generates one vertex buffer object and assigns its ID to the variable VBO.
    //VBO is used to store vertex coordinates, color, normals and other attributes. 
    glGenBuffers(1, &VBO);
    //Generates one element buffer object and assigns its ID to the variable EBO.
    //Is used for indexed rendering. 
    glGenBuffers(1, &EBO);


    glBindVertexArray(VAO);

    // Binds VBO to the GL_ARRAY_BUFFER target
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Allocates memory for the VBO and fills it with data. Specifies the size of the data with a 
    //static draw that will not be changed frequently. 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Binds the EBO to the GL_BUFFER_DATA target. 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //Allicates memory for the EBO and fills it with data. Specifies the size of the data with a 
    //static draw that will not be changed frequently. 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Specifies the attributes to a vertex. The parameters are the location, size of the attributes (x and y), 
    //that the x and y values are float, false for normalization, byte offset and offset of where the x and y 
    //values is in the array. 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Same for color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VAO
    glBindVertexArray(0);

    //Closes the text file
    file.close();

    cout << "The data points has been created and saved in the file 'Data.txt'" << endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //Escape key closes the program 
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        // Bind VAO
        glBindVertexArray(VAO);

        //GL_LINE STRIP draws a sequence of line regments, then how many vertices to be rendered, 
        //the values in the index array and the offset into the index array, here the array starts from 0.
        glDrawElements(GL_LINE_STRIP, numberOfVertices, GL_UNSIGNED_INT, 0);

        // Unbind VAO
        glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Cleans up and stops the program
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
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


