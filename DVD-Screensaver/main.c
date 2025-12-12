#include <zephyr/device.h>
#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(dvd_app);

#include <lvgl.h>

LV_IMG_DECLARE(beagle);

int main(void)
{
	int32_t display_width = lv_display_get_horizontal_resolution(NULL);
	int32_t display_height = lv_display_get_vertical_resolution(NULL);

	/* Create UI */
	lv_obj_t *scr = lv_scr_act();
//	lv_obj_set_style_bg_color(scr, lv_color_white(), 0);

	/* Create Image Object */
	lv_obj_t *img = lv_img_create(scr);
	lv_img_set_src(img, &beagle);

	int32_t img_width = lv_image_get_src_width(img);
	int32_t img_height = lv_image_get_src_height(img);

	/* Initial Position and Velocity */
	int x = 0;
	int y = 0;
	int dx = 20; // Speed X
	int dy = 20; // Speed Y

	while (1) {
		/* Apply Position */
		lv_obj_set_pos(img, x, y);

		/* Update display */
		lv_task_handler();

		/* Adjust for speed */
		k_sleep(K_MSEC(500));

		/* Update Position */
		x += dx;
		y += dy;

		/* Bounce Logic */
		if (x <= 0) {
			x = 0;
			dx = -dx;
		} else if (x + img_width >= display_width) {
			x = display_width - img_width;
			dx = -dx;
		}

		if (y <= 0) {
			y = 0;
			dy = -dy;
		} else if (y + img_height >= display_height) {
			y = display_height - img_height;
			dy = -dy;
		}
	}

	return 0;
}
