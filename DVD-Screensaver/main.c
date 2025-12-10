#include <lvgl.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(dvd_app);
LV_IMG_DECLARE(khasim);

/* Screen Dimensions */
#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 300
#define IMG_WIDTH 155
#define IMG_HEIGHT 155

int main(void)
{
	const struct device *display_dev;

	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device not ready, aborting test");
		return -1;
	}

	/* Create UI */
	LOG_INF("Line: %d", __LINE__);
	lv_obj_t *scr = lv_scr_act();
	LOG_INF("Line: %d", __LINE__);
	lv_obj_set_style_bg_color(scr, lv_color_white(), 0);

	/* Create Image Object */
	LOG_INF("Line: %d", __LINE__);
	lv_obj_t *img = lv_img_create(scr);
	LOG_INF("Line: %d", __LINE__);
	lv_img_set_src(img, &khasim);
	LOG_INF("Line: %d", __LINE__);
	lv_obj_set_size(img, IMG_WIDTH, IMG_HEIGHT);
	LOG_INF("Line: %d", __LINE__);

	/* Initial Position and Velocity */
	int x = 0;
	int y = 0;
	int dx = 2; // Speed X
	int dy = 2; // Speed Y

	lv_obj_set_pos(img, x, y);
	LOG_INF("Line: %d", __LINE__);

	lv_task_handler();
	LOG_INF("Line: %d", __LINE__);
	display_blanking_off(display_dev);
	LOG_INF("Line: %d", __LINE__);

	while (1) {
		/* Update Position */
		x += dx;
		y += dy;

		/* Bounce Logic */
		if (x <= 0) {
			x = 0;
			dx = -dx;
		} else if (x + IMG_WIDTH >= SCREEN_WIDTH) {
			x = SCREEN_WIDTH - IMG_WIDTH;
			dx = -dx;
		}

		if (y <= 0) {
			y = 0;
			dy = -dy;
		} else if (y + IMG_HEIGHT >= SCREEN_HEIGHT) {
			y = SCREEN_HEIGHT - IMG_HEIGHT;
			dy = -dy;
		}

		/* Apply Position */
		LOG_INF("Line: %d", __LINE__);
		lv_obj_set_pos(img, x, y);

		lv_task_handler();
		LOG_INF("Line: %d", __LINE__);
		k_sleep(K_MSEC(200)); // Adjust for speed
	}

	return 0;
}
