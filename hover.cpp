#define STB_IMAGE_IMPLEMENTATION
#include <GL/glut.h>
#include <string>
#include "stb_image.h"
#include <stdlib.h>
#include <time.h>
#include <vector>
#include<iostream>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"
// Window dimensions
int windowWidth = 900;
int windowHeight = 400;
GLuint textureID; // Texture ID for the loaded image

// Function prototypes
void init();
void display();
void mouse(int button, int state, int x, int y);
void createNewWindow();
void passiveMotion(int x, int y);

// Flag to determine if the button is clicked
bool buttonClicked = false;
// Flag to determine if the mouse is hovering over the button
bool isHovering = false;

//image sizes
int imgWidth, imgHeight;
//image texture
//GLuint texture;




struct Image {
    GLuint texture;
    int posX;
    int posY;
    int width;
    int height;
};
std::vector<Image> images;

//load images
void loadTexture(const char* filename, GLuint& texture, int& width, int& height, int desiredWidth, int desiredHeight) {
    stbi_set_flip_vertically_on_load(true); // Set before loading the image data
    int channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);

    // Resize the image if desiredWidth and desiredHeight are provided
    if (desiredWidth != -1 && desiredHeight != -1) {
        unsigned char* resized_data = (unsigned char*)malloc(desiredWidth * desiredHeight * 4);
        stbir_resize_uint8(data, width, height, 0, resized_data, desiredWidth, desiredHeight, 0, 4);
        stbi_image_free(data);
        data = resized_data;
        width = desiredWidth;
        height = desiredHeight;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}








void drawText(float x, float y, std::string text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}







void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Load  button images
    Image image1, image2; // Example images
    loadTexture("assets/Large Buttons/Large Buttons/Play Button.png", image1.texture, image1.width, image1.height, 200, 100);
    loadTexture("assets/Large Buttons/Large Buttons/Exit Button.png", image2.texture, image2.width, image2.height, 200, 100);
    //load hovering button images

    // Set positions for each image
    image1.posX = 350;
    image1.posY = 200;
    image2.posX = 350;
    image2.posY = 80;

    // Add images to the vector
    images.push_back(image1);
    images.push_back(image2);
}









int mouseX = 0;
int mouseY = 0;
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowWidth, 0, windowHeight, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Loop through images and draw each one
    for (const auto& image : images) {
        glBindTexture(GL_TEXTURE_2D, image.texture);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex2f(image.posX, image.posY);
        glTexCoord2f(1.0, 0.0); glVertex2f(image.posX + image.width, image.posY);
        glTexCoord2f(1.0, 1.0); glVertex2f(image.posX + image.width, image.posY + image.height);
        glTexCoord2f(0.0, 1.0); glVertex2f(image.posX, image.posY + image.height);
        glEnd();
    }

    glutSwapBuffers();
}




void display1() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the button rectangle
    glColor3f(0.1035, 0.9023, 0.69432);
    glBegin(GL_POLYGON);
    // Draw text inside the button
    glColor3f(0.1035, 0.9023, 0.69432); // White color for the text
    drawText(380, 250, "WELCOME TO THE GAME");
    glEnd();

}

void mouse(int button, int state, int x, int y) {
    // Check if left mouse button is pressed
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // if we click on the start game button
        if (x >= 350 && x <= 580 && y >= 100 && y <= 200) {
            // Set the flag indicating the button is clicked
            buttonClicked = true;
            // Close the current window
            glutDestroyWindow(glutGetWindow());
            // Create a new window
            createNewWindow();
        }
        //if we click on the exist button
        if (x >= 350 && x <= 580 && y >= 210 && y <= 310) {
            // Set the flag indicating the button is clicked
            buttonClicked = true;
            // Close the current window
            glutDestroyWindow(glutGetWindow());
        }
    }
}

void passiveMotion(int x, int y) {
    mouseX = x;
    mouseY = y;
    // Check if the mouse is within the button area
    if (x >= 350 && x <= 580 && y >= 100 && y <= 200) {
        // Set the flag indicating the mouse is hovering over the button
        isHovering = true;
        //create a hovering image
        Image hover_play_button;
        loadTexture("assets/Large Buttons/Colored Large Buttons/Play col_Button.png", hover_play_button.texture, hover_play_button.width, hover_play_button.height, 200, 100);
        hover_play_button.posX = 350;
        hover_play_button.posY = 200;
        images.push_back(hover_play_button);

    }
    else {
        // Set the flag indicating the mouse is not hovering over the button
        isHovering = false;
        Image image1;
        loadTexture("assets/Large Buttons/Large Buttons/Play Button.png", image1.texture, image1.width, image1.height, 200, 100);
        image1.posX = 350;
        image1.posY = 200;
        images.push_back(image1);
    }

    if (x >= 350 && x <= 580 && y >= 210 && y <= 310) {
        // Set the flag indicating the mouse is hovering over the button
        isHovering = true;
        //create a hovering image
        Image hover_exist_button;
        loadTexture("assets/Large Buttons/Colored Large Buttons/Exit  col_Button.png", hover_exist_button.texture, hover_exist_button.width, hover_exist_button.height, 200, 100);
        hover_exist_button.posX = 350;
        hover_exist_button.posY = 80;
        images.push_back(hover_exist_button);

    }
    else {
        // Set the flag indicating the mouse is not hovering over the button
        isHovering = false;
        Image image2;
        loadTexture("assets/Large Buttons/Large Buttons/Exit Button.png", image2.texture, image2.width, image2.height, 200, 100);
        image2.posX = 350;
        image2.posY = 80;
        images.push_back(image2);
    }

    // Redisplay the window
    glutPostRedisplay();
}

void createNewWindow() {
    // Create a new window
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Game Window");

    // Register callback functions
    glutDisplayFunc(display1);

    // Initialize OpenGL settings
    init();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Menu");
    init();

    // Register callback functions
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(passiveMotion); // Register passive motion function

    // Initialize OpenGL settings

    glutMainLoop();
    return 0;
}
