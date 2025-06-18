/**
 * @file main.cpp
 * @author Muhamad Arif Hidayat
 * @brief Entry point for the ESP-IDF application.
 */

#include "Application.h"

/** @brief Global instance of the Application class. */
Application app;

/**
 * @brief Main entry point for the ESP-IDF application.
 *
 * This function is called by the ESP-IDF framework to start the application.
 * It runs the Application instance's main logic.
 */
extern "C" void app_main(void) {
    app.run();
}