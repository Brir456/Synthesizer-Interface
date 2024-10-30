// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 8.3.11
// Project name: Synthesizer

#include "ui.h"

// COMPONENT MacroElementComponent

lv_obj_t *ui_MacroElementComponent_create(lv_obj_t *comp_parent)
{

    lv_obj_t *cui_MacroElementComponent;
    cui_MacroElementComponent = lv_obj_create(comp_parent);
    lv_obj_remove_style_all(cui_MacroElementComponent);
    lv_obj_set_height(cui_MacroElementComponent, 55);
    lv_obj_set_width(cui_MacroElementComponent, lv_pct(100));
    lv_obj_set_align(cui_MacroElementComponent, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(cui_MacroElementComponent, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_radius(cui_MacroElementComponent, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cui_MacroElementComponent, lv_color_hex(0x3E495C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_MacroElementComponent, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(cui_MacroElementComponent, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(cui_MacroElementComponent, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(cui_MacroElementComponent, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(cui_MacroElementComponent, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *cui_Macro_Mod_Dropdown;
    cui_Macro_Mod_Dropdown = lv_dropdown_create(cui_MacroElementComponent);
    lv_dropdown_set_options(cui_Macro_Mod_Dropdown,
                            "-\nMacro 1\nMacro 2\nMacro 3\nMacro 4\nMacro 5\nMacro 6\nMacro 7\nMacro 8\nMacro 9\nMacro 10");
    lv_obj_set_width(cui_Macro_Mod_Dropdown, 105);
    lv_obj_set_height(cui_Macro_Mod_Dropdown, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(cui_Macro_Mod_Dropdown, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(cui_Macro_Mod_Dropdown, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_set_style_text_font(cui_Macro_Mod_Dropdown, &ui_font_Kallisto_Medium_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(cui_Macro_Mod_Dropdown, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cui_Macro_Mod_Dropdown, lv_color_hex(0x202532), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_Macro_Mod_Dropdown, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(cui_Macro_Mod_Dropdown, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(cui_Macro_Mod_Dropdown, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(cui_Macro_Mod_Dropdown, &lv_font_montserrat_14, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(cui_Macro_Mod_Dropdown, &ui_font_Kallisto_Medium_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *cui_Macro_Mod_Dropdown_List = lv_dropdown_get_list(cui_Macro_Mod_Dropdown);
    lv_obj_set_style_bg_color(cui_Macro_Mod_Dropdown_List, lv_color_hex(0x2EBAE4), LV_PART_SELECTED | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(cui_Macro_Mod_Dropdown_List, &ui_font_Kallisto_Medium_14, 0);
    lv_obj_set_style_opa(cui_Macro_Mod_Dropdown_List, 0, LV_PART_SCROLLBAR);
    lv_obj_set_style_bg_color(cui_Macro_Mod_Dropdown_List, lv_color_hex(0x131927), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(cui_Macro_Mod_Dropdown_List, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(cui_Macro_Mod_Dropdown_List, lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *cui_Macro_Dest_Dropdown;
    cui_Macro_Dest_Dropdown = lv_dropdown_create(cui_MacroElementComponent);
    lv_dropdown_set_options(cui_Macro_Dest_Dropdown,
                            "-\nSlide Time\nOsc 1 Level\nOsc 1 Fine\nOsc 2 Level\nOsc 2 Fine\nNoise Level\nEnv 1 AL\nEnv 1 DL\nEnv 1 SL\nEnv 1 RL\nEnv 1 A\nEnv 1 D\nEnv 1 S\nEnv 1 R\nEnv 2 AL\nEnv 2 DL\nEnv 2 SL\nEnv 2 RL\nEnv 2 A\nEnv 2 D\nEnv 2 S\nEnv 2 R\nLFO 1 Freq\nLFO 2 Freq\nPre Dist Drive\nPost Dist Drive\nFilter Cutoff\nFilter Res");
    lv_obj_set_width(cui_Macro_Dest_Dropdown, 150);
    lv_obj_set_height(cui_Macro_Dest_Dropdown, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(cui_Macro_Dest_Dropdown, 155);
    lv_obj_set_y(cui_Macro_Dest_Dropdown, 0);
    lv_obj_set_align(cui_Macro_Dest_Dropdown, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(cui_Macro_Dest_Dropdown, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_set_style_text_font(cui_Macro_Dest_Dropdown, &ui_font_Kallisto_Medium_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(cui_Macro_Dest_Dropdown, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cui_Macro_Dest_Dropdown, lv_color_hex(0x202532), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_Macro_Dest_Dropdown, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(cui_Macro_Dest_Dropdown, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(cui_Macro_Dest_Dropdown, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(cui_Macro_Dest_Dropdown, &lv_font_montserrat_14, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(cui_Macro_Dest_Dropdown, &ui_font_Kallisto_Medium_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *cui_Macro_Dest_Dropdown_List = lv_dropdown_get_list(cui_Macro_Dest_Dropdown);
    lv_obj_set_style_bg_color(cui_Macro_Dest_Dropdown_List, lv_color_hex(0x2EBAE4), LV_PART_SELECTED | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(cui_Macro_Dest_Dropdown_List, &ui_font_Kallisto_Medium_14, 0);
    lv_obj_set_style_opa(cui_Macro_Dest_Dropdown_List, 0, LV_PART_SCROLLBAR);
    lv_obj_set_style_bg_color(cui_Macro_Dest_Dropdown_List, lv_color_hex(0x131927), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(cui_Macro_Dest_Dropdown_List, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(cui_Macro_Dest_Dropdown_List, lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *cui_Amount_Slider;
    cui_Amount_Slider = lv_slider_create(cui_MacroElementComponent);
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

    lv_obj_t *cui_Single_Arrow_Label;
    cui_Single_Arrow_Label = lv_label_create(cui_MacroElementComponent);
    lv_obj_set_width(cui_Single_Arrow_Label, LV_SIZE_CONTENT);  /// 36
    lv_obj_set_height(cui_Single_Arrow_Label, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(cui_Single_Arrow_Label, 112);
    lv_obj_set_y(cui_Single_Arrow_Label, 0);
    lv_obj_set_align(cui_Single_Arrow_Label, LV_ALIGN_LEFT_MID);
    lv_label_set_text(cui_Single_Arrow_Label, "A");
    lv_obj_set_style_text_color(cui_Single_Arrow_Label, lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(cui_Single_Arrow_Label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(cui_Single_Arrow_Label, &ui_font_Arrows_36, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t **children = lv_mem_alloc(sizeof(lv_obj_t *) * _UI_COMP_MACROELEMENTCOMPONENT_NUM);
    children[UI_COMP_MACROELEMENTCOMPONENT_MACROELEMENTCOMPONENT] = cui_MacroElementComponent;
    children[UI_COMP_MACROELEMENTCOMPONENT_MACRO_MOD_DROPDOWN] = cui_Macro_Mod_Dropdown;
    children[UI_COMP_MACROELEMENTCOMPONENT_MACRO_DEST_DROPDOWN] = cui_Macro_Dest_Dropdown;
    children[UI_COMP_MACROELEMENTCOMPONENT_AMOUNT_SLIDER] = cui_Amount_Slider;
    children[UI_COMP_MACROELEMENTCOMPONENT_SINGLE_ARROW_LABEL] = cui_Single_Arrow_Label;
    lv_obj_add_event_cb(cui_MacroElementComponent, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(cui_MacroElementComponent, del_component_child_event_cb, LV_EVENT_DELETE, children);
    ui_comp_MacroElementComponent_create_hook(cui_MacroElementComponent);
    return cui_MacroElementComponent;
}