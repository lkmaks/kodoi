#ifndef CONFIG_H
#define CONFIG_H


struct Config
{
    int board_size = 15;
    int board_scene_cell_size = 30;
    int board_scene_coordinates_padding = 100;

    int bar_scene_bar_width = 50;
    int bar_scene_half_bar_height = 400;
    int bar_scene_value_vertical_padding = 15;

    int board_layout_bar_width = 50;
    int board_layout_vpad = 50;
    int board_layout_hpad1 = 50;
    int board_layout_hpad2 = 50;
    int board_layout_info_widget_width = 300;
    int board_layout_min_hpad = 50;

};

#endif // CONFIG_H
