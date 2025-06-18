/**
 * @file Application.cpp
 * @brief Implementation of the Application class for managing the application lifecycle.
 */

#include "Application.h"
#include "config.h"

/** @brief Logging tag for the Application class. */
static const char *TAG = "Application";

/**
 * @brief Constructs a new Application object.
 *
 * Initializes the Application instance.
 */
Application::Application() {}

/**
 * @brief Initializes the Non-Volatile Storage (NVS) partition.
 *
 * Sets up NVS for persistent storage of key-value data, such as Wi-Fi credentials.
 * If NVS is corrupted or in a new version, it is erased and reinitialized.
 */
void Application::initializeNVS()
{
    ESP_LOGI(TAG, "Initializing NVS...");
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_LOGW(TAG, "NVS formatting required...");
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "NVS successfully initialized.");
}

/**
 * @brief Initializes and mounts the LittleFS filesystem.
 *
 * Configures and registers the LittleFS driver with the Virtual File System (VFS)
 * using the base path and partition label defined in config.h. Handles mounting
 * or formatting if necessary, and logs partition information.
 */
void Application::initializeFS()
{
    ESP_LOGI(TAG, "Initializing LittleFS...");
    esp_vfs_littlefs_conf_t conf = {
        .base_path = LFS_BASE_PATH,            
        .partition_label = LFS_PARTITION_LABEL, 
        .partition = NULL,                     
        .format_if_mount_failed = true,        
        .read_only = false,                    
        .dont_mount = false,                   
        .grow_on_mount = false,                
    };

    esp_err_t ret = esp_vfs_littlefs_register(&conf);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            ESP_LOGE(TAG, "Failed to find LittleFS partition. Ensure '%s' exists in partition_custom.csv", LFS_PARTITION_LABEL);
        }
        else
        {
            ESP_LOGE(TAG, "Failed to initialize LittleFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_littlefs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to retrieve LittleFS partition info (%s)", esp_err_to_name(ret));
    }
    else
    {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }
    ESP_LOGI(TAG, "LittleFS successfully mounted at path: %s", LFS_BASE_PATH);
}

/**
 * @brief Executes the main application logic.
 *
 * Initializes NVS and LittleFS, starts the Wi-Fi manager, and enters an infinite loop
 * to monitor connection status and perform application tasks.
 */
void Application::run()
{
    ESP_LOGI(TAG, "Application started.");

    initializeNVS();
    initializeFS();

    m_wifi.start();

    while (true)
    {
        if (m_wifi.isConnected())
        {
            ESP_LOGI(TAG, "Device connected. IP: %s", m_wifi.getIpAddress().c_str());

        }
        else
        {
            ESP_LOGI(TAG, "Device in provisioning mode...");
        }
        vTaskDelay(pdMS_TO_TICKS(10000)); 
    }
}