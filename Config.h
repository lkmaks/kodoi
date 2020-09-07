#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

struct Config
{
#ifdef LOCAL
    QString black_stone = "/home/max/qt_projects/kodoi/b_succ.png";
    QString white_stone = "/home/max/qt_projects/kodoi/w_succ.png";
    QString lightwood_background = "/home/max/qt_projects/kodoi/lightwoodbackground.jpg";
#else
    QString black_stone = "b_succ.png";
    QString white_stone = "w_succ.png";
    QString lightwood_background = "lightwoodbackground.jpg";
#endif
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
