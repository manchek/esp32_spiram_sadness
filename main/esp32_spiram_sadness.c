#include <stdio.h>

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <nvs_flash.h>

static const char *LOGTAG __attribute__((unused)) = "main";
static TaskHandle_t task_handle = NULL;
static bool running = true;

static void task( void* arg )
{
	ESP_LOGI(LOGTAG, "Task begins");
	while (running) {
		ESP_LOGI(LOGTAG, "Task Loop");
		vTaskDelay(pdMS_TO_TICKS(2000));
	}
	ESP_LOGI(LOGTAG, "Task ends");
	vTaskDelete(NULL);
}

void app_main(void)
{
	ESP_LOGI(LOGTAG, "Hi there");

	esp_err_t err = nvs_flash_init();   //initialize flash for later use
	if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		err = nvs_flash_init();
	}
	ESP_ERROR_CHECK(err);

	ESP_LOGI(LOGTAG, "Reset reason %d", esp_reset_reason());

	xTaskCreatePinnedToCore(task, "task", 5120, NULL, 15, &task_handle, 1);

	while (true) {
		ESP_LOGI(LOGTAG, "Loop");
		vTaskDelay(pdMS_TO_TICKS(5000));
	}
}
