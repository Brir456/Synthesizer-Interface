SET(SOURCES ui_comp_macroelementcomponent.c
    ui_comp_matrixelementcomponent.c
    ui_comp.c
    ui_Screen1.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_img_analyzer_bg_transparent_100x50_png.c
    ui_img_env_1_png.c
    ui_img_env_2_png.c
    ui_img_adsr_bg_220x75_png.c
    ui_img_back_icon_png.c
    ui_img_forward_icon_png.c
    ui_img_saw_waveform_png.c
    ui_img_sine_waveform_png.c
    fonts/ui_font_Kallisto_Bold_10.c
    fonts/ui_font_Kallisto_Bold_16.c
    fonts/ui_font_Kallisto_Heavy_20.c
    fonts/ui_font_Kallisto_Medium_14.c
    fonts/ui_font_Kallisto_Medium_18.c
    fonts/ui_font_Kallisto_Medium_20.c
    fonts/ui_font_Arrows_36.c)

add_library(ui ${SOURCES})
