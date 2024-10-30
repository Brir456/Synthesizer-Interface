// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 8.3.11
// Project name: Synthesizer

#include "ui.h"

// COMPONENT MatrixElementComponent

lv_obj_t *ui_MatrixElementComponent_create(lv_obj_t *comp_parent)
{

    lv_obj_t *cui_MatrixElementComponent;
    cui_MatrixElementComponent = lv_obj_create(comp_parent);
    lv_obj_remove_style_all(cui_MatrixElementComponent);
    lv_obj_set_height(cui_MatrixElementComponent, 55);
    lv_obj_set_width(cui_MatrixElementComponent, lv_pct(100));
    lv_obj_set_align(cui_MatrixElementComponent, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(cui_MatrixElementComponent, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_radius(cui_MatrixElementComponent, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cui_MatrixElementComponent, lv_color_hex(0x3E495C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_MatrixElementComponent, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(cui_MatrixElementComponent, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(cui_MatrixElementComponent, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(cui_MatrixElementComponent, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(cui_MatrixElementComponent, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *cui_Matrix_Mod_Dropdown;
    cui_Matrix_Mod_Dropdown = lv_dropdown_create(cui_MatrixElementComponent);
    lv_dropdown_set_options(cui_Matrix_Mod_Dropdown, "-\nENV 2\nLFO 1\nLFO 2");
    lv_obj_set_width(cui_Matrix_Mod_Dropdown, 105);
    lv_obj_set_height(cui_Matrix_Mod_Dropdown, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(cui_Matrix_Mod_Dropdown, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(cui_Matrix_Mod_Dropdown, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_set_style_text_font(cui_Matrix_Mod_Dropdown, &ui_font_Kallisto_Medium_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(cui_Matrix_Mod_Dropdown, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cui_Matrix_Mod_Dropdown, lv_color_hex(0x131927), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_Matrix_Mod_Dropdown, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(cui_Matrix_Mod_Dropdown, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(cui_Matrix_Mod_Dropdown, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(cui_Matrix_Mod_Dropdown, &ui_font_Kallisto_Medium_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(cui_Matrix_Mod_Dropdown, &lv_font_montserrat_14, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_t *cui_Matrix_Mod_Dropdown_List = lv_dropdown_get_list(cui_Matrix_Mod_Dropdown);
    lv_obj_set_style_bg_color(cui_Matrix_Mod_Dropdown_List, lv_color_hex(0x2EBAE4), LV_PART_SELECTED | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(cui_Matrix_Mod_Dropdown_List, &ui_font_Kallisto_Medium_14, 0);
    lv_obj_set_style_opa(cui_Matrix_Mod_Dropdown_List, 0, LV_PART_SCROLLBAR);
    lv_obj_set_style_bg_color(cui_Matrix_Mod_Dropdown_List, lv_color_hex(0x131927), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(cui_Matrix_Mod_Dropdown_List, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(cui_Matrix_Mod_Dropdown_List, lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *cui_Single_Arrow_Label;
    cui_Single_Arrow_Label = lv_label_create(cui_MatrixElementComponent);
    lv_obj_set_width(cui_Single_Arrow_Label, 36);
    lv_obj_set_height(cui_Single_Arrow_Label, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(cui_Single_Arrow_Label, 112);
    lv_obj_set_y(cui_Single_Arrow_Label, 3);
    lv_obj_set_style_radius(cui_Single_Arrow_Label, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(cui_Single_Arrow_Label, "A");
    lv_obj_set_style_text_color(cui_Single_Arrow_Label, lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(cui_Single_Arrow_Label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(cui_Single_Arrow_Label, &ui_font_Arrows_36, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *cui_Double_Arrow_Label;
    cui_Double_Arrow_Label = lv_label_create(cui_MatrixElementComponent);
    lv_obj_set_width(cui_Double_Arrow_Label, 36);
    lv_obj_set_height(cui_Double_Arrow_Label, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(cui_Double_Arrow_Label, 112);
    lv_obj_set_y(cui_Double_Arrow_Label, -3);
    lv_obj_set_style_radius(cui_Double_Arrow_Label, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_align(cui_Double_Arrow_Label, LV_ALIGN_BOTTOM_LEFT);
    lv_label_set_text(cui_Double_Arrow_Label, "B");
    lv_obj_set_style_text_color(cui_Double_Arrow_Label, lv_color_hex(0x131927), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(cui_Double_Arrow_Label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(cui_Double_Arrow_Label, &ui_font_Arrows_36, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *cui_Matrix_Dest_Dropdown;
    cui_Matrix_Dest_Dropdown = lv_dropdown_create(cui_MatrixElementComponent);
    lv_dropdown_set_options(cui_Matrix_Dest_Dropdown,
                            "-\nOSC 1 Level\nOSC 1 Fine\nOsc 2 Level\nOsc 2 Fine\nNoise Level\nPre Dist Drive\nPost Dist Drive\nFilter Cutoff\nFilter Res");
    lv_obj_set_width(cui_Matrix_Dest_Dropdown, 150);
    lv_obj_set_height(cui_Matrix_Dest_Dropdown, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(cui_Matrix_Dest_Dropdown, 155);
    lv_obj_set_y(cui_Matrix_Dest_Dropdown, 0);
    lv_obj_set_align(cui_Matrix_Dest_Dropdown, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(cui_Matrix_Dest_Dropdown, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_set_style_text_font(cui_Matrix_Dest_Dropdown, &ui_font_Kallisto_Medium_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(cui_Matrix_Dest_Dropdown, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cui_Matrix_Dest_Dropdown, lv_color_hex(0x131927), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_Matrix_Dest_Dropdown, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(cui_Matrix_Dest_Dropdown, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(cui_Matrix_Dest_Dropdown, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(cui_Matrix_Dest_Dropdown, &ui_font_Kallisto_Medium_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(cui_Matrix_Dest_Dropdown, &lv_font_montserrat_14, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_t *cui_Matrix_Dest_Dropdown_List = lv_dropdown_get_list(cui_Matrix_Dest_Dropdown);
    lv_obj_set_style_bg_color(cui_Matrix_Dest_Dropdown_List, lv_color_hex(0x2EBAE4), LV_PART_SELECTED | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(cui_Matrix_Dest_Dropdown_List, &ui_font_Kallisto_Medium_14, 0);
    lv_obj_set_style_opa(cui_Matrix_Dest_Dropdown_List, 0, LV_PART_SCROLLBAR);
    lv_obj_set_style_bg_color(cui_Matrix_Dest_Dropdown_List, lv_color_hex(0x131927), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(cui_Matrix_Dest_Dropdown_List, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(cui_Matrix_Dest_Dropdown_List, lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *cui_Amount_Slider;
    cui_Amount_Slider = lv_slider_create(cui_MatrixElementComponent);
    lv_slider_set_range(cui_Amount_Slider, -255, 255);
    lv_slider_set_mode(cui_Amount_Slider, LV_SLIDER_MODE_SYMMETRICAL);
    lv_slider_set_value(cui_Amount_Slider, 0, LV_ANIM_OFF);
    if (lv_slider_get_mode(cui_Amount_Slider) == LV_SLIDER_MODE_RANGE)
        lv_slider_set_left_value(cui_Amount_Slider, 0,
                                 LV_ANIM_OFF);
    lv_obj_set_width(cui_Amount_Slider, 135);
    lv_obj_set_height(cui_Amount_Slider, 10);
    lv_obj_set_align(cui_Amount_Slider, LV_ALIGN_RIGHT_MID);
    lv_obj_set_style_radius(cui_Amount_Slider, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cui_Amount_Slider, lv_color_hex(0x131927), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_Amount_Slider, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(cui_Amount_Slider, lv_color_hex(0x2EBAE4), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_Amount_Slider, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(cui_Amount_Slider, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_Amount_Slider, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(cui_Amount_Slider, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(cui_Amount_Slider, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(cui_Amount_Slider, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(cui_Amount_Slider, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

    lv_obj_t **children = lv_mem_alloc(sizeof(lv_obj_t *) * _UI_COMP_MATRIXELEMENTCOMPONENT_NUM);
    children[UI_COMP_MATRIXELEMENTCOMPONENT_MATRIXELEMENTCOMPONENT] = cui_MatrixElementComponent;
    children[UI_COMP_MATRIXELEMENTCOMPONENT_MATRIX_MOD_DROPDOWN] = cui_Matrix_Mod_Dropdown;
    children[UI_COMP_MATRIXELEMENTCOMPONENT_SINGLE_ARROW_LABEL] = cui_Single_Arrow_Label;
    children[UI_COMP_MATRIXELEMENTCOMPONENT_DOUBLE_ARROW_LABEL] = cui_Double_Arrow_Label;
    children[UI_COMP_MATRIXELEMENTCOMPONENT_MATRIX_DEST_DROPDOWN] = cui_Matrix_Dest_Dropdown;
    children[UI_COMP_MATRIXELEMENTCOMPONENT_AMOUNT_SLIDER] = cui_Amount_Slider;
    lv_obj_add_event_cb(cui_MatrixElementComponent, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(cui_MatrixElementComponent, del_component_child_event_cb, LV_EVENT_DELETE, children);
    ui_comp_MatrixElementComponent_create_hook(cui_MatrixElementComponent);
    return cui_MatrixElementComponent;
}