/**
 * @file Application.h
 * @brief Declaration of the main Application class for managing application lifecycle.
 */

#pragma once

#include "sdk_compat.h"
#include "WifiManager.h"

/**
 * @class Application
 * @brief Main class responsible for orchestrating the application's lifecycle.
 *
 * This class handles the initialization of core services, including Non-Volatile Storage (NVS)
 * and LittleFS filesystem, and manages Wi-Fi connectivity through the WifiManager.
 */
class Application {
public:
    /**
     * @brief Constructs a new Application object.
     *
     * Initializes the Application instance, preparing it for execution.
     */
    Application();

    /**
     * @brief Executes the main application logic.
     *
     * This method initializes necessary components and enters an infinite loop
     * to keep the application running.
     */
    void run();

private:
    /**
     * @brief Initializes the Non-Volatile Storage (NVS) partition.
     *
     * Sets up NVS for persistent storage of key-value data, such as Wi-Fi credentials.
     * Handles formatting of NVS in case of corruption or first boot.
     */
    void initializeNVS();

    /**
     * @brief Initializes and mounts the LittleFS filesystem.
     *
     * Registers the LittleFS driver with the Virtual File System (VFS) using the
     * base path defined in config.h, enabling file operations on the storage partition.
     */
    void initializeFS();

    /** @brief Instance of WifiManager for handling Wi-Fi connectivity. */
    WifiManager m_wifi;
};