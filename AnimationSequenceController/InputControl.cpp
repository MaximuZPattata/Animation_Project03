#include "pch.h"

#include "cControlGameEngine.h"
#include "cAnimationSystem.h"

extern cControlGameEngine gameEngine;

const float POSITION_OFFSET = 0.0f;

cMesh* controlMeshModel;

static glm::vec3 playerForward = glm::vec3(0.f, 0.f, 1.0f);

static bool animLooped = false;

void resetCamAttributes()
{
    if (!gameEngine.mouseMoved)
        gameEngine.mouseMoved = true;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    float ROTATION_DEGREES = 5.0f;

    const float CAMERA_SPEED = 300.0f;

    const float MODEL_MOVE_SPEED = 1.0f;

    float CAMERA_MOVEMENT = CAMERA_SPEED * gameEngine.deltaTime;

    glm::vec3 CAMERA_TARGET = gameEngine.GetCurrentCameraTarget();
    glm::vec3 CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 CAMERA_POSITION = gameEngine.GetCurrentCameraPosition();

    //---------------Controls for Mesh Models----------------------------------------------------------

    if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
    {
        controlMeshModel = gameEngine.GetCurrentModelSelected();

        std::string currentModelName = controlMeshModel->friendlyName;

        glm::vec3 currentModelPosition = gameEngine.GetModelPosition(currentModelName);

        float currentModelScaleValue = gameEngine.GetModelScaleValue(currentModelName);

        if (key == GLFW_KEY_W && action) // Decrement model's Z position
        {
            gameEngine.MoveModel(currentModelName, currentModelPosition.x, currentModelPosition.y, currentModelPosition.z - MODEL_MOVE_SPEED);
        }
        if (key == GLFW_KEY_A && action) // Decrement model's X position
        {
            gameEngine.MoveModel(currentModelName, currentModelPosition.x - MODEL_MOVE_SPEED, currentModelPosition.y, currentModelPosition.z);
        }
        if (key == GLFW_KEY_S && action) // Increment model's Z position
        {
            gameEngine.MoveModel(currentModelName, currentModelPosition.x, currentModelPosition.y, currentModelPosition.z + MODEL_MOVE_SPEED);
        }
        if (key == GLFW_KEY_D && action) // Increment model's X position
        {
            gameEngine.MoveModel(currentModelName, currentModelPosition.x + MODEL_MOVE_SPEED, currentModelPosition.y, currentModelPosition.z);
        }
        if (key == GLFW_KEY_Q && action) // Increment model's Y position 
        {
            gameEngine.MoveModel(currentModelName, currentModelPosition.x, currentModelPosition.y + MODEL_MOVE_SPEED, currentModelPosition.z);
        }
        if (key == GLFW_KEY_E && action) // Decrement model's Y position
        {
            gameEngine.MoveModel(currentModelName, currentModelPosition.x, currentModelPosition.y - MODEL_MOVE_SPEED, currentModelPosition.z);
        }
        if (key == GLFW_KEY_I && action) // Rotate model's X axis - 5 degrees
        {
            gameEngine.RotateMeshModel(currentModelName, ROTATION_DEGREES, 1.f, 0.f, 0.f);
        }
        if (key == GLFW_KEY_O && action) // Rotate model's Y axis - 5 degrees
        {
            gameEngine.RotateMeshModel(currentModelName, ROTATION_DEGREES, 0.f, 1.f, 0.f);
        }
        if (key == GLFW_KEY_P && action) // Rotate model's Z axis - 5 degrees
        {
            gameEngine.RotateMeshModel(currentModelName, ROTATION_DEGREES, 0.f, 0.f, 1.f);
        }
        if (key == GLFW_KEY_C && action) // Decrement model's scale value
        {
            gameEngine.ScaleModel(currentModelName, currentModelScaleValue - 0.01f);
        }
        if (key == GLFW_KEY_V && action) // Increment model's scale value
        {
            gameEngine.ScaleModel(currentModelName, currentModelScaleValue + 0.01f);
        }
        if (key == GLFW_KEY_Z && action) // Shift to previous model
        {
            gameEngine.ShiftToPreviousMeshInList();
        }
        if (key == GLFW_KEY_X && action) // Shift to next model
        {
            gameEngine.ShiftToNextMeshInList();
        }
        if (key == GLFW_KEY_B && action) // Change to wireframe mode
        {
            gameEngine.TurnWireframeModeOn(currentModelName);
        }
    }

    if (mods == 0)
    {
        //----------------------------PLAYER CONTROLS----------------------------------------
        
        // Key to toggle between mesh bone coloring and mesh texture
        if (key == GLFW_KEY_T && action)
        {
            gameEngine.ToggleMeshBoneWeightColor("Vampire");
        }

        if (key == GLFW_KEY_UP)
        {
            std::string modelName = "Vampire";

            if (action == GLFW_PRESS)
            {
                //---------------------Rotate Player-------------------------------------------

                glm::quat rotationQuat = glm::rotation(playerForward, glm::vec3(0.f, 0.f, -1.0f));

                gameEngine.RotateMeshModelUsingQuaternion(modelName, rotationQuat);

                //--------------------Animate Player-------------------------------------------

                gameEngine.PlayAnimation(modelName, "Vampire_Jump", 0.f, false, true);
            }

            else if (action == GLFW_REPEAT)
            {
                //--------------------Loop Animation-------------------------------------------

                if (!animLooped)
                {
                    gameEngine.LoopExistingAnimation(modelName, "Vampire_Jump");
                    animLooped = true;
                }
            }

            else if(action == GLFW_RELEASE)
            {
                if (animLooped)
                {
                    gameEngine.UnloopPreviousAnimations(modelName);
                    animLooped = false;
                }

                gameEngine.PlayAnimation(modelName, "Vampire_Idle", 7.f, true, false);
            }
        }

        if (key == GLFW_KEY_LEFT && action)
        {
            //---------------------Rotate Player-------------------------------------------

            glm::quat rotationQuat = glm::rotation(playerForward, glm::vec3(-1.0f, 0.f, 0.f));  

            gameEngine.RotateMeshModelUsingQuaternion("Vampire", rotationQuat);

            //--------------------Animate Player-------------------------------------------

            gameEngine.PlayAnimation("Vampire", "Vampire_Run", 5.f, true, true);

        }
        if (key == GLFW_KEY_RIGHT && action)
        {
            //---------------------Rotate Player-------------------------------------------

            glm::quat rotationQuat = glm::rotation(playerForward, glm::vec3(1.0f, 0.f, 0.f));

            gameEngine.RotateMeshModelUsingQuaternion("Vampire", rotationQuat);

            //--------------------Animate Player-------------------------------------------

            gameEngine.PlayAnimation("Vampire", "Vampire_Run", 5.f, true, true);
        }

        if (key == GLFW_KEY_DOWN && action)
        {
            //---------------------Rotate Player-------------------------------------------

            glm::quat rotationQuat = glm::rotation(playerForward, glm::vec3(0.f, 0.f, 1.0f));

            gameEngine.RotateMeshModelUsingQuaternion("Vampire", rotationQuat);

            //--------------------Animate Player-------------------------------------------

            gameEngine.PlayAnimation("Vampire", "Vampire_Walk", 6.f, true, true);
        }

        if (key == GLFW_KEY_I && action)
        {
            //--------------------Animate Player-------------------------------------------

            gameEngine.PlayAnimation("Vampire", "Vampire_Idle", 6.f, true, true);
        }

        //------------------------------Camera Showcases------------------------------------------------

        if (key == GLFW_KEY_1 && action)
        {
            CAMERA_POSITION = glm::vec3(0.0f + POSITION_OFFSET, 50.0f + POSITION_OFFSET, 300.0f + POSITION_OFFSET);
            gameEngine.MoveCameraTarget(0.0f, 0.0f, -1.0f);
            gameEngine.yaw = -90.0f;
            gameEngine.pitch = 0.f;
            resetCamAttributes();
        }

        if (key == GLFW_KEY_2 && action)
        {
            CAMERA_POSITION = glm::vec3(-290.0f + POSITION_OFFSET, 70.0f + POSITION_OFFSET, 0.f + POSITION_OFFSET);
            gameEngine.MoveCameraTarget(1.f, -0.06f, 0.03f);
            gameEngine.yaw = -358.241f;
            gameEngine.pitch = -3.65f;
            resetCamAttributes();
        }

        if (key == GLFW_KEY_3 && action)
        {
            CAMERA_POSITION = glm::vec3(-4.25f + POSITION_OFFSET, 55.f + POSITION_OFFSET, -280.f + POSITION_OFFSET);
            gameEngine.MoveCameraTarget(-0.0115f, 0.009f, 1.0f);
            gameEngine.yaw = -269.345f;
            gameEngine.pitch = 0.5f;
            resetCamAttributes();
        }

        if (key == GLFW_KEY_4 && action)
        {
            CAMERA_POSITION = glm::vec3(270.f + POSITION_OFFSET, 53.f + POSITION_OFFSET, 6.f + POSITION_OFFSET);
            gameEngine.MoveCameraTarget(-1.f, -0.016f, -0.035f);
            gameEngine.yaw = -177.998f;
            gameEngine.pitch = -0.9f;
            resetCamAttributes();
        }

        if (key == GLFW_KEY_5 && action)
        {
            CAMERA_POSITION = glm::vec3(2.5f + POSITION_OFFSET, 450.f + POSITION_OFFSET, 100.f + POSITION_OFFSET);
            gameEngine.MoveCameraTarget(-0.0012f, -0.98f, -0.22f);
            gameEngine.yaw = -90.3f;
            gameEngine.pitch = -77.55f;
            resetCamAttributes();
        }

        if (key == GLFW_KEY_SPACE && action)
        {
            if (gameEngine.enableMouseMovement)
                gameEngine.enableMouseMovement = false;
            else
                gameEngine.enableMouseMovement = true;

            resetCamAttributes();
        }

        //------------------------------Move Camera--------------------------------------------------------

        if (key == GLFW_KEY_W && action)
        {
            CAMERA_POSITION += CAMERA_MOVEMENT * CAMERA_TARGET;
        }
        if (key == GLFW_KEY_A && action)
        {
            CAMERA_POSITION -= glm::normalize(glm::cross(CAMERA_TARGET, CAMERA_UP)) * CAMERA_MOVEMENT;
        }
        if (key == GLFW_KEY_S && action)
        {
            CAMERA_POSITION -= CAMERA_MOVEMENT * CAMERA_TARGET;
        }
        if (key == GLFW_KEY_D && action)
        {
            CAMERA_POSITION += glm::normalize(glm::cross(CAMERA_TARGET, CAMERA_UP)) * CAMERA_MOVEMENT;
        }
        if (key == GLFW_KEY_Q && action)
        {
            CAMERA_POSITION += CAMERA_MOVEMENT * CAMERA_UP;
        }
        if (key == GLFW_KEY_E && action)
        {
            CAMERA_POSITION -= CAMERA_MOVEMENT * CAMERA_UP;
        }

        gameEngine.MoveCameraPosition(CAMERA_POSITION.x, CAMERA_POSITION.y, CAMERA_POSITION.z);
    }

    return;
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    if (gameEngine.enableMouseMovement)
    {
        glm::vec3 CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 CAMERA_POSITION = gameEngine.GetCurrentCameraPosition();

        int screenWidth, screenHeight;
        glfwGetWindowSize(window, &screenWidth, &screenHeight);

        float mouseSensitivity = 0.05f;

        float xOffset = 0.0f;
        float yOffset = 0.0f;

        if (xPos >= screenWidth - 1 || xPos <= 0) // Rotate Cam when mouse moves to the edge of the screen(left or right)
        {
            xOffset = 0.0f;

            if (xPos <= 0)
                xOffset = -10.0f;
            else if (xPos >= screenWidth - 1)
                xOffset = 10.0f;

            gameEngine.yaw += mouseSensitivity * xOffset;
        }

        if (yPos >= screenHeight - 1 || yPos <= 20) // Rotate Cam when mouse moves to the edge of the screen(up or down)
        {
            yOffset = 0.0f;

            if (yPos <= 20)
                yOffset = 10.0f;
            else if (yPos >= screenHeight - 1)
                yOffset = -10.0f;

            gameEngine.pitch += mouseSensitivity * yOffset;
        }

        if (gameEngine.mouseMoved)
        {
            gameEngine.previousX = xPos;
            gameEngine.previousY = yPos;
            gameEngine.mouseMoved = false;
        }

        xOffset = xPos - gameEngine.previousX;
        yOffset = gameEngine.previousY - yPos; //Reversed Y

        xOffset *= mouseSensitivity;
        yOffset *= mouseSensitivity;

        gameEngine.yaw += xOffset;
        gameEngine.pitch += yOffset;

        if (gameEngine.pitch > 90.0f)
            gameEngine.pitch = 90.0f;

        if (gameEngine.pitch < -90.0f)
            gameEngine.pitch = -90.0f;

        glm::vec3 front;

        front.x = cos(glm::radians(gameEngine.yaw)) * cos(glm::radians(gameEngine.pitch));
        front.y = sin(glm::radians(gameEngine.pitch));
        front.z = sin(glm::radians(gameEngine.yaw)) * cos(glm::radians(gameEngine.pitch));

        glm::vec3 cameraFront = glm::normalize(front);

        gameEngine.MoveCameraTarget(cameraFront.x, cameraFront.y, cameraFront.z);

        gameEngine.previousX = xPos;
        gameEngine.previousY = yPos;
    }
}
