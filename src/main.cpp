/////////////////////////////////////////////////////////////////
/*
  ESP32 | LVGL8 | Ep 0. GFX Setup (ft. LovyanGFX)
  Video Tutorial: https://youtu.be/IPCvQ4o_WP8
  Created by Eric N. (ThatProject)
*/
/////////////////////////////////////////////////////////////////

#include <stdarg.h>
#include <algorithm>
#include <vector>
#include <lvgl.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <DisplaySetup.h>
#include <ui.h>
#include <ui_helpers.h>
#include <driver/adc.h>
#include <arduinoFFT.h>
#include <ESP32Encoder.h>
#include <Button.h>
#include "esp_task_wdt.h"

static IRAM_ATTR void enc_cb(void *arg)
{
  ESP32Encoder *enc = (ESP32Encoder *)arg;
}

extern bool loopTaskWDTEnabled;
extern TaskHandle_t loopTaskHandle;

Button encoderButton(40);
ESP32Encoder encoder(true, enc_cb);

Button prevMenuButton(46);
Button nextMenuButton(48);
Button tenXMultiplierButton(47);
Button hunXMultiplierButton(21);

//------------------SCREEN SETUP--------------------------
LGFX tft;

static const uint32_t screenWidth = 480;
static const uint32_t screenHeight = 320;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

void screenSetup()
{
  tft.begin();
  tft.setRotation(3);
  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);

  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  lv_indev_drv_register(&indev_drv);
}

//------------------------UI Stuff--------------------------------------------

int audioValues[45] = {
    4,    //  0: Portamento Time
    0,    //  1: Osc 1 Table
    255,  //  2: Osc 1 Level
    0,    //  3: Osc 1 Semi
    0,    //  4: Osc 1 Fine
    0,    //  5: Osc 2 Table
    0,    //  6: Osc 2 Level
    0,    //  7: Osc 2 Semi
    0,    //  8: Osc 2 Fine
    4,    //  9: Noise Table
    0,    // 10: Noise Level
    1,    // 11: Env 1 State
    255,  // 12: Env 1 AL
    255,  // 13: Env 1 DL
    100,  // 14: Env 1 SL
    0,    // 15: Env 1 RL
    20,   // 16: Env 1 A
    500,  // 17: Env 1 D
    5000, // 18: Env 1 S
    50,   // 19: Env 1 R
    1,    // 20: Env 2 State
    255,  // 21: Env 2 AL
    255,  // 22: Env 2 DL
    0,    // 23: Env 2 SL
    0,    // 24: Env 2 RL
    5,    // 25: Env 2 A
    40,   // 26: Env 2 D
    200,  // 27: Env 2 S
    50,   // 28: Env 2 R
    1,    // 29: LFO 1 Table
    1,    // 30: LFO 1 State
    255,  // 31: LFO 1 Freq
    1,    // 32: LFO 2 Table
    1,    // 33: LFO 2 State
    1,    // 34: LFO 2 Freq
    0,    // 35: Pre Dist State
    0,    // 36: Pre Dist Drive
    0,    // 37: Pre Dist Mode
    0,    // 38: Post Dist State
    0,    // 39: Post Dist Drive
    0,    // 40: Post Dist Mode
    0,    // 41: Filter Type
    255,  // 42: Filter Cutoff
    5,    // 43: Filter Resonance
    0     // 44: Filter State
};

String audioValuesName[45] = {
    "SLIDETIME",
    "OSC1_TABLE",
    "OSC1_LEVEL",
    "OSC1_SEMI",
    "OSC1_FINE",
    "OSC2_TABLE",
    "OSC2_LEVEL",
    "OSC2_SEMI",
    "OSC2_FINE",
    "NOISE_TABLE",
    "NOISE_LEVEL",
    "ENV1_STATE",
    "ENV1_AL",
    "ENV1_DL",
    "ENV1_SL",
    "ENV1_RL",
    "ENV1_A",
    "ENV1_D",
    "ENV1_S",
    "ENV1_R",
    "ENV2_STATE",
    "ENV2_AL",
    "ENV2_DL",
    "ENV2_SL",
    "ENV2_RL",
    "ENV2_A",
    "ENV2_D",
    "ENV2_S",
    "ENV2_R",
    "LFO1_TABLE",
    "LFO1_STATE",
    "LFO1_FREQ",
    "LFO2_TABLE",
    "LFO2_STATE",
    "LFO2_FREQ",
    "PREDISTSTATE",
    "PREDISTAMOUNT",
    "PREDISTMODE",
    "POSTDISTSTATE",
    "POSTDISTAMOUNT",
    "POSTDISTMODE",
    "FILTERTYPE",
    "FILTERCUTOFF",
    "FILTERRESONANCE",
    "FILTERSTATE" //
};

bool editValue = false;
int lastEncoderCount = 0;

int incrementMultiplier = 1;
bool isEditing = false;

byte currentMainTab = 0;
byte currentFXPage = 0;
byte currentMatrixPage = 0;
byte currentMacroPage = 0;
byte currentOsc = 0;
byte currentLFO = 0;
byte currentEnv = 0;

void sendUARTMsg(String msg)
{
  Serial.println(msg);
  Serial1.println(msg);
}
/**
 * @brief Updates the background color of a group of buttons or labels,
 *        highlighting the selected one based on the provided index.
 * @param selectedLabelNdx The index of the label that should be highlighted (selected).
 *                         It must be within the range of 0 to `numLabels - 1`.
 * @param numLabels The total number of labels. This must be greater than 0.
 * @param colorOn The background color to apply to the selected label. This should be
 *                provided in a hexadecimal format
 * @param colorOff The background color to apply to all non-selected labels. This should
 *                 also be in a hexadecimal format.
 * @param ... A variadic list of `lv_obj_t *` pointers, representing the labels or buttons.
 *            The number of objects provided should match `numLabels`.
 */
void updateSelectedButtonColor(int selectedLabelNdx, int numLabels, int colorOn, int colorOff, ...)
{
  // Check if the number of labels is valid
  if (numLabels <= 0 || selectedLabelNdx < 0 || selectedLabelNdx >= numLabels)
  {
    return;
  }

  va_list args;
  va_start(args, colorOff);

  for (int i = 0; i < numLabels; i++)
  {
    lv_obj_t *currentLabel = va_arg(args, lv_obj_t *);
    lv_obj_set_style_bg_color(currentLabel, lv_color_hex(colorOff), LV_PART_MAIN | LV_STATE_DEFAULT);

    if (i == selectedLabelNdx)
    {
      lv_obj_set_style_bg_color(currentLabel, lv_color_hex(colorOn), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
  }
  va_end(args);
}

/**
 * @brief Toggles the color of a button between different states.
 *
 * @param obj A pointer to a LVGL objecct.
 * @param numStates The number of button states.
 * @param currentState The index of the current state (which color to use).
 * @param ... Variadic arguments specifying the colors for each state.
 *            Each argument should be an integer (hexadecimal color) corresponding
 *            to the background color of each button state. There should be `numStates` colors.
 */
void toggleSingleButtonColor(lv_obj_t *obj, int numStates, int currentState, ...)
{
  if (numStates < 1 || currentState < 0 || currentState > numStates - 1)
  {
    return;
  }
  va_list args;
  va_start(args, currentState);
  for (int i = 0; i < numStates; i++)
  {
    int color = va_arg(args, int);
    if (i == currentState)
    {
      lv_obj_set_style_bg_color(obj, lv_color_hex(color), 0);
    }
  }
}

/**
 * @brief Updates the text of multiple labels with corresponding integer values and a specified unit.
 *
 * @param numLabels The number of labels to be updated. Must be greater than zero.
 * @param unit The unit string to append to each label's numerical value.
 * @param ... Pairs of label objects (of type `lv_obj_t*`) and their corresponding integer values.
 *            The first argument of each pair should be a label object followed by an integer.
 */
void updateLabelValue(int numLabels, String unit, ...)
{
  // Check if the number of labels is valid
  if (numLabels <= 0)
  {
    return;
  }

  va_list args;
  va_start(args, unit);

  for (int i = 0; i < numLabels; i++)
  {
    lv_obj_t *currentLabel = va_arg(args, lv_obj_t *);
    int number = va_arg(args, int);
    char str[10];
    dtostrf(number, 1, 0, str);
    String outputStr = str + unit;
    lv_label_set_text(currentLabel, outputStr.c_str());
  }
  va_end(args);
}

/**
 * @brief Updates the text of multiple labels with corresponding float values and a specified unit.
 *
 * @param numLabels The number of labels to be updated. Must be greater than zero.
 * @param numDigits The number of digits to display after the decimal point.
 * @param unit The unit string to append to each label's numerical value.
 * @param ... Pairs of label objects (of type `lv_obj_t*`) and their corresponding integer values.
 *            The first argument of each pair should be a label object followed by an integer.
 */
void updateLabelValueFloat(int numLabels, int numDigits, String unit, ...)
{
  // Check if the number of labels is valid
  if (numLabels <= 0)
  {
    return;
  }

  va_list args;
  va_start(args, unit);

  for (int i = 0; i < numLabels; i++)
  {
    lv_obj_t *currentLabel = va_arg(args, lv_obj_t *);
    double number = va_arg(args, double);
    char str[10];
    dtostrf(number, 1, numDigits, str);
    String outputStr = str + unit;
    lv_label_set_text(currentLabel, outputStr.c_str());
  }
  va_end(args);
}

/**
 * @brief Updates the values of multiple sliders.
 *
 * @param numSliders The number of sliders to be updated. Must be greater than zero.
 * @param ... Pairs of slider objects (of type `lv_obj_t*`) and their corresponding integer values.
 *            The first argument of each pair should be a slider object followed by an integer value.
 */
void updateSliderValue(int numSliders, ...)
{
  if (numSliders <= 0)
  {
    return;
  }
  va_list args;
  va_start(args, numSliders);
  for (int i = 0; i < numSliders; i++)
  {
    lv_obj_t *currentSlider = va_arg(args, lv_obj_t *);
    int number = va_arg(args, int);
    lv_slider_set_value(currentSlider, number, LV_ANIM_OFF);
  }
}

/**
 * @brief Changes the value of an integer, ensuring it stays within specified bounds.
 *
 * This function modifies the integer value pointed to by `obj` by adding a calculated change,
 * which is the product of `change` and `multiplicator`. The resulting value is clamped to
 * be within the range defined by `minVal` and `maxVal`.
 *
 * @param obj A pointer to the integer value to be modified.
 * @param minVal The minimum allowable value for the integer.
 * @param maxVal The maximum allowable value for the integer.
 * @param change The amount to change the integer by (before applying the multiplicator).
 * @param multiplicator The factor by which to multiply the change.
 */
void changeValue(int *obj, int minVal, int maxVal, int change, int multiplicator)
{
  *obj += change * multiplicator;
  if (*obj < minVal)
  {
    *obj = minVal;
  }
  if (*obj > maxVal)
  {
    *obj = maxVal;
  }
}

/**
 * @brief Updates the background image of a waveform display object based on the selected waveform type.
 *
 * @param obj A pointer to the LVGL object that displays the waveform.
 *            This object should be initialized and created prior to calling this function.
 * @param ndx The index of the selected waveform.
 *            Valid indices are:
 *            - 0: Sawtooth waveform
 *            - 1: Sine waveform
 *            - 2: Square waveform
 *            - 3: Triangle waveform
 *            - 4: Noise waveform
 */
void setWaveform(lv_obj_t *obj, byte ndx)
{
  switch (ndx)
  {
  case 0:
    lv_obj_set_style_bg_img_src(obj, &ui_img_saw_waveform_png, LV_PART_MAIN | LV_STATE_DEFAULT);
    break;
  case 1:
    lv_obj_set_style_bg_img_src(obj, &ui_img_sine_waveform_png, LV_PART_MAIN | LV_STATE_DEFAULT);
    break;
  case 2:
    lv_obj_set_style_bg_img_src(obj, &ui_img_square_waveform_png, LV_PART_MAIN | LV_STATE_DEFAULT);
    break;
  case 3:
    lv_obj_set_style_bg_img_src(obj, &ui_img_triangle_waveform_png, LV_PART_MAIN | LV_STATE_DEFAULT);
    break;
  case 4:
    lv_obj_set_style_bg_img_src(obj, &ui_img_noise_waveform_png, LV_PART_MAIN | LV_STATE_DEFAULT);
    break;
  }
}

void updateOscSettings()
{
  setWaveform(ui_OSC_Waveform, audioValues[1 + 4 * currentOsc]);
  updateSelectedButtonColor(currentOsc, 3, 0x2EBAE4, 0x3E495C, ui_OSC_1_Label_Container, ui_OSC_2_Label_Container, ui_Noise_Label_Container);
  if (currentOsc < 2)
  {
    updateLabelValue(3, "", ui_Osc_Level_Val, audioValues[2 + 4 * currentOsc], ui_Osc_Semi_Val, audioValues[3 + 4 * currentOsc], ui_Osc_Fine_Val, audioValues[4 + 4 * currentOsc]);
  }
  else
  {
    updateLabelValue(1, "", ui_Osc_Level_Val, audioValues[2 + 4 * currentOsc]);
    lv_label_set_text(ui_Osc_Semi_Val, "-");
    lv_label_set_text(ui_Osc_Fine_Val, "-");
  }
}

void updateLFOSettings()
{
  setWaveform(ui_LFO_Waveform, audioValues[29 + 3 * currentLFO]);
  updateSelectedButtonColor(currentLFO, 2, 0x2EBAE4, 0x3E495C, ui_LFO_1_Label_Container, ui_LFO_2_Label_Container);

  static int lfoStateColor;
  switch (audioValues[30 + currentLFO * 3])
  {
  case 0:
    lfoStateColor = 0x131927;
    break;
  case 1:
    lfoStateColor = 0x2EBAE4;
    break;
  }
  updateSelectedButtonColor(1 - audioValues[30 + currentLFO * 3], 2, lfoStateColor, 0x3E495C, ui_LFO_ON_Container, ui_LFO_OFF_Container);
  updateLabelValueFloat(1, 1, " Hz", ui_LFO_Freq_Val, audioValues[31 + currentLFO * 3] / 10.0);
}

void updateEnvGraph()
{
  static float scaleEnvX = 69.0f / 1000.0f;
  static float scaleEnvY = 69.0f / 255.0f;
  static lv_point_t line_points[] = {{4, 71}, {70, 70}, {120, 10}, {180, 60}, {240, 10}};

  line_points[1].x = line_points[0].x + audioValues[16 + 9 * currentEnv] * scaleEnvX;
  line_points[1].y = 71 - audioValues[12 + 9 * currentEnv] * scaleEnvY;

  line_points[2].x = line_points[1].x + audioValues[17 + 9 * currentEnv] * scaleEnvX;
  line_points[2].y = 71 - audioValues[13 + 9 * currentEnv] * scaleEnvY;

  line_points[3].x = line_points[2].x + audioValues[18 + 9 * currentEnv] * scaleEnvX;
  line_points[3].y = 71 - audioValues[14 + 9 * currentEnv] * scaleEnvY;

  line_points[4].x = line_points[3].x + audioValues[19 + 9 * currentEnv] * scaleEnvX;
  line_points[4].y = 71 - audioValues[15 + 9 * currentEnv] * scaleEnvY;

  lv_line_set_points(ui_ENV_Line, line_points, 5);
  lv_obj_set_style_line_rounded(ui_ENV_Line, true, 0);
  switch (audioValues[11 + 9 * currentEnv])
  {
  case 1:
    lv_obj_set_style_line_color(ui_ENV_Line, lv_color_hex(0x2EBAE4), 0);
    break;
  default:
    lv_obj_set_style_line_color(ui_ENV_Line, lv_color_hex(0x131927), 0);
    break;
  }
}

void updateEnvSettings()
{
  updateSelectedButtonColor(currentEnv, 2, 0x2EBAE4, 0x3E495C, ui_ENV_1_Tab_Label, ui_ENV_2_Tab_Label);
  static int envStateColor;
  switch (audioValues[11 + currentEnv * 9])
  {
  case 0:
    envStateColor = 0x131927;
    break;
  case 1:
    envStateColor = 0x2EBAE4;
    break;
  }
  updateSelectedButtonColor(1 - audioValues[11 + currentEnv * 9], 2, envStateColor, 0x3E495C, ui_ENV_ON_Container, ui_ENV_OFF_Container);
  updateLabelValue(4, "", ui_Env_1_Attack_Time, audioValues[16 + 9 * currentEnv], ui_Env_1_Decay_Time, audioValues[17 + 9 * currentEnv], ui_Env_1_Sustain_Time, audioValues[18 + 9 * currentEnv], ui_Env_1_Release_Time, audioValues[19 + 9 * currentEnv]);
  updateLabelValue(4, "", ui_Env_1_Attack_Level, audioValues[12 + 9 * currentEnv], ui_Env_1_Decay_Level, audioValues[13 + 9 * currentEnv], ui_Env_1_Sustain_Level, audioValues[14 + 9 * currentEnv], ui_Env_1_Release_Level, audioValues[15 + 9 * currentEnv]);
  updateEnvGraph();
}

void updateOscTab()
{
  setWaveform(ui_LFO_Waveform, audioValues[28 + 3 * currentLFO]);
  updateOscSettings();
  updateLFOSettings();
  updateEnvSettings();
}

void focusEncoderOscTab()
{
  static lv_obj_t *oscTabObjects[27] = {ui_OSC_1_Label_Container, ui_OSC_2_Label_Container, ui_Noise_Label_Container, ui_OSC_Prev_Waveform, ui_OSC_Next_Waveform, ui_Osc_Level_Val, ui_Osc_Semi_Val, ui_Osc_Fine_Val, ui_LFO_1_Label_Container, ui_LFO_2_Label_Container, ui_LFO_Prev_Waveform, ui_LFO_Next_Waveform, ui_LFO_ON_Container, ui_LFO_OFF_Container, ui_LFO_Freq_Val, ui_ENV_1_Tab_Label, ui_ENV_2_Tab_Label, ui_ENV_ON_Container, ui_ENV_OFF_Container, ui_Env_1_Attack_Time, ui_Env_1_Attack_Level, ui_Env_1_Decay_Time, ui_Env_1_Decay_Level, ui_Env_1_Sustain_Time, ui_Env_1_Sustain_Level, ui_Env_1_Release_Time, ui_Env_1_Release_Level};
  int oscTabObjValNdx[27] = {-1, -1, -1, -1, -1, 2 + 4 * currentOsc, 3 + 4 * currentOsc, 4 + 4 * currentOsc, -1, -1, -1, -1, -1, -1, 31 + 3 * currentLFO, -1, -1, -1, -1, 16 + 9 * currentEnv, 12 + 9 * currentEnv, 17 + 9 * currentEnv, 13 + 9 * currentEnv, 18 + 9 * currentEnv, 14 + 9 * currentEnv, 19 + 9 * currentEnv, 15 + 9 * currentEnv};
  static int focusedOscTabObjNdx = 0;

  int encoderNow = encoder.getCount();

  if (encoderNow != lastEncoderCount)
  {
    int incrementVal = 0;
    if (encoderNow > lastEncoderCount)
    {
      incrementVal = 1;
    }
    if (encoderNow < lastEncoderCount)
    {
      incrementVal = -1;
    }
    if (!isEditing)
    {

      focusedOscTabObjNdx = (focusedOscTabObjNdx + 27 + incrementVal) % 27;
      lv_obj_set_style_outline_color(oscTabObjects[focusedOscTabObjNdx], lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_outline_opa(oscTabObjects[focusedOscTabObjNdx], 255, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_outline_width(oscTabObjects[focusedOscTabObjNdx], 2, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_outline_pad(oscTabObjects[focusedOscTabObjNdx], 2, LV_PART_MAIN | LV_STATE_DEFAULT);
      switch (focusedOscTabObjNdx)
      {
      case 17:
      case 18:
        if (currentEnv == 0)
        {
          lv_obj_set_style_outline_color(oscTabObjects[focusedOscTabObjNdx], lv_color_hex(0x2A303C), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        break;
      case 3:
      case 4:
      case 6:
      case 7:
        if (currentOsc == 2)
        {
          lv_obj_set_style_outline_color(oscTabObjects[focusedOscTabObjNdx], lv_color_hex(0x2A303C), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        break;
      }
      lv_obj_set_style_outline_opa(oscTabObjects[(focusedOscTabObjNdx + 27 - incrementVal) % 27], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    if (isEditing)
    {
      if (oscTabObjValNdx[focusedOscTabObjNdx] != -1)
      {
        switch (focusedOscTabObjNdx)
        {
        case 6:
          if (currentOsc == 2)
          {
            break;
          }
          else
          {
            changeValue(&audioValues[oscTabObjValNdx[focusedOscTabObjNdx]], -48, 48, incrementVal, incrementMultiplier);
          }
          break;

        case 7:
          if (currentOsc == 2)
          {
            break;
          }
          else
          {
            changeValue(&audioValues[oscTabObjValNdx[focusedOscTabObjNdx]], -255, 255, incrementVal, incrementMultiplier);
          }
          break;

        case 19:
        case 21:
        case 23:
        case 25:
          changeValue(&audioValues[oscTabObjValNdx[focusedOscTabObjNdx]], 0, 9999, incrementVal, incrementMultiplier);
          break;

        default:
          changeValue(&audioValues[oscTabObjValNdx[focusedOscTabObjNdx]], 0, 255, incrementVal, incrementMultiplier);
          break;
        }
      }
    }
    updateOscTab();
  }

  if (encoderButton.pressed())
  {
    String msg;
    switch (focusedOscTabObjNdx)
    {
    case 0:
      currentOsc = 0;
      break;
    case 1:
      currentOsc = 1;
      break;
    case 2:
      currentOsc = 2;
      break;
    case 3:
      if (currentOsc != 2)
      {
        audioValues[1 + 4 * currentOsc] = (audioValues[1 + 4 * currentOsc] + 4) % 5;
        msg = "<" + audioValuesName[1 + 4 * currentOsc] + ":" + audioValues[1 + 4 * currentOsc] + ">";
        sendUARTMsg(msg);
      }
      break;
    case 4:
      if (currentOsc != 2)
      {
        audioValues[1 + 4 * currentOsc] = (audioValues[1 + 4 * currentOsc] + 1) % 5;
        msg = "<" + audioValuesName[1 + 4 * currentOsc] + ":" + audioValues[1 + 4 * currentOsc] + ">";
        sendUARTMsg(msg);
      }
      break;
    case 8:
      currentLFO = 0;
      break;
    case 9:
      currentLFO = 1;
      break;
    case 10:
      audioValues[29 + 3 * currentLFO] = (audioValues[29 + 3 * currentLFO] + 3) % 4;
      msg = "<" + audioValuesName[29 + 3 * currentLFO] + ":" + audioValues[29 + 3 * currentLFO] + ">";
      sendUARTMsg(msg);
      break;
    case 11:
      audioValues[29 + 3 * currentLFO] = (audioValues[29 + 3 * currentLFO] + 1) % 4;
      msg = "<" + audioValuesName[29 + 3 * currentLFO] + ":" + audioValues[29 + 3 * currentLFO] + ">";
      sendUARTMsg(msg);
      break;
    case 12:
      audioValues[30 + 3 * currentLFO] = 1;
      msg = "<" + audioValuesName[30 + 3 * currentLFO] + ":" + audioValues[30 + 3 * currentLFO] + ">";
      sendUARTMsg(msg);
      break;
    case 13:
      audioValues[30 + 3 * currentLFO] = 0;
      msg = "<" + audioValuesName[30 + 3 * currentLFO] + ":" + audioValues[30 + 3 * currentLFO] + ">";
      sendUARTMsg(msg);
      break;
    case 15:
      currentEnv = 0;
      break;
    case 16:
      currentEnv = 1;
      break;
    case 17:
      audioValues[11 + 9 * currentEnv] = 1;
      msg = "<" + audioValuesName[11 + 9 * currentEnv] + ":" + audioValues[11 + 9 * currentEnv] + ">";
      sendUARTMsg(msg);
      break;
    case 18:
      audioValues[11 + 9 * currentEnv] = 0;
      audioValues[11] = 1;
      msg = "<" + audioValuesName[11 + 9 * currentEnv] + ":" + audioValues[11 + 9 * currentEnv] + ">";
      sendUARTMsg(msg);
      break;

    default:
      isEditing = !isEditing;
      if (isEditing)
      {
        lv_obj_set_style_text_color(oscTabObjects[focusedOscTabObjNdx], lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);
        if (currentOsc == 2)
        {
          if (focusedOscTabObjNdx == 6 || focusedOscTabObjNdx == 7)
          {
            isEditing = false;
          }
        }
      }
      if (!isEditing)
      {
        lv_obj_set_style_text_color(oscTabObjects[focusedOscTabObjNdx], lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        msg = msg = "<" + audioValuesName[oscTabObjValNdx[focusedOscTabObjNdx]] + ":" + audioValues[oscTabObjValNdx[focusedOscTabObjNdx]] + ">";
        sendUARTMsg(msg);
      }

      break;
    }
    updateOscTab();
  }
  lastEncoderCount = encoderNow;
}

void updateFXTab()
{
  switch (currentFXPage)
  {
  case 0:
    updateLabelValue(1, " ms", ui_Portamento_Time_Val, audioValues[0]);
    toggleSingleButtonColor(ui_Pre_Dist_Title_Container, 2, audioValues[35], 0x131927, 0x2EBAE4);
    updateSelectedButtonColor(audioValues[37], 2, 0x2EBAE4, 0x131927, ui_Pre_Dist_Clip_Label_Container, ui_Pre_Dist_Foldback_Label_Container);
    updateSliderValue(1, ui_Pre_Drive_Slider, audioValues[36]);
    toggleSingleButtonColor(ui_Filter_Title_Container, 2, audioValues[44], 0x131927, 0x2EBAE4);
    updateSelectedButtonColor(audioValues[41], 4, 0x2EBAE4, 0x131927, ui_LP_Label_Container, ui_HP_Label_Container, ui_BP_Label_Container, ui_Notch_Label_Container);
    updateSliderValue(2, ui_Cutoff_Slider, audioValues[42], ui_Res_Slider, audioValues[43]);
    break;
  case 1:
    toggleSingleButtonColor(ui_Post_Dist_Title_Container, 2, audioValues[38], 0x131927, 0x2EBAE4);
    updateSelectedButtonColor(audioValues[40], 2, 0x2EBAE4, 0x131927, ui_Post_Dist_Clip_Label_Container, ui_Post_Dist_Foldback_Label_Container);
    updateSliderValue(1, ui_Post_Drive_Slider, audioValues[39]);
    break;
  }
}

void focusEncoderFXTab()
{
  static lv_obj_t *FXTabObjects[2][14] = {{ui_Portamento_Time_Val, ui_Pre_Dist_Title_Container, ui_Pre_Dist_Clip_Label_Container, ui_Pre_Dist_Foldback_Label_Container, ui_Pre_Drive_Slider, ui_Filter_Title_Container, ui_LP_Label_Container, ui_HP_Label_Container, ui_BP_Label_Container, ui_Notch_Label_Container, ui_Cutoff_Slider, ui_Res_Slider, ui_FX_Page_1_Led, ui_FX_Page_2_Led},
                                          {ui_Post_Dist_Title_Container, ui_Post_Dist_Clip_Label_Container, ui_Post_Dist_Foldback_Label_Container, ui_Post_Drive_Slider, ui_FX_Page_1_Led, ui_FX_Page_2_Led}};
  int FXTabObjValNdx[2][14] = {{0, 35, 37, 37, 36, 44, 41, 41, 41, 41, 42, 43, -1, -1},
                               {38, 40, 40, 39, -1, -1}};
  int FXTabObjCount[2] = {14, 6};
  int encoderNow = encoder.getCount();
  static int focusedFXTabObjNdx = 0;

  if (encoderNow != lastEncoderCount)
  {
    int incrementVal = 0;
    if (encoderNow > lastEncoderCount)
    {
      incrementVal = 1;
    }
    if (encoderNow < lastEncoderCount)
    {
      incrementVal = -1;
    }
    if (!isEditing)
    {
      focusedFXTabObjNdx = (focusedFXTabObjNdx + FXTabObjCount[currentFXPage] + incrementVal) % FXTabObjCount[currentFXPage];
      lv_obj_set_style_outline_color(FXTabObjects[currentFXPage][focusedFXTabObjNdx], lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_outline_opa(FXTabObjects[currentFXPage][focusedFXTabObjNdx], 255, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_outline_width(FXTabObjects[currentFXPage][focusedFXTabObjNdx], 2, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_outline_pad(FXTabObjects[currentFXPage][focusedFXTabObjNdx], 2, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_outline_opa(FXTabObjects[currentFXPage][(focusedFXTabObjNdx + FXTabObjCount[currentFXPage] - incrementVal) % FXTabObjCount[currentFXPage]], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
    if (isEditing)
    {
      if (FXTabObjValNdx[currentFXPage][focusedFXTabObjNdx] != -1)
      {
        if (currentFXPage == 0)
        {

          switch (focusedFXTabObjNdx)
          {
          case 0:
          case 4:
          case 10:
          case 11:
            changeValue(&audioValues[FXTabObjValNdx[currentFXPage][focusedFXTabObjNdx]], 0, 255, incrementVal, incrementMultiplier);
            break;

          default:
            break;
          }
        }
        if (currentFXPage == 1)
        {
          switch (focusedFXTabObjNdx)
          {
          case 3:
            changeValue(&audioValues[FXTabObjValNdx[currentFXPage][focusedFXTabObjNdx]], 0, 255, incrementVal, incrementMultiplier);
            break;

          default:
            break;
          }
        }
      }
    }

    updateFXTab();
  }

  if (encoderButton.pressed())
  {
    String msg;

    if (currentFXPage == 0)
    {
      switch (focusedFXTabObjNdx)
      {
      case 1:
        audioValues[FXTabObjValNdx[currentFXPage][focusedFXTabObjNdx]]++;
        audioValues[FXTabObjValNdx[currentFXPage][focusedFXTabObjNdx]] %= 2;
        break;
      case 2:
      case 6:
        audioValues[FXTabObjValNdx[currentFXPage][focusedFXTabObjNdx]] = 0;
        break;
      case 3:
      case 7:
        audioValues[FXTabObjValNdx[currentFXPage][focusedFXTabObjNdx]] = 1;
        break;
      case 5:
        audioValues[FXTabObjValNdx[currentFXPage][focusedFXTabObjNdx]]++;
        audioValues[FXTabObjValNdx[currentFXPage][focusedFXTabObjNdx]] %= 2;
        break;
      case 8:
        audioValues[FXTabObjValNdx[currentFXPage][focusedFXTabObjNdx]] = 2;
        break;
      case 9:
        audioValues[FXTabObjValNdx[currentFXPage][focusedFXTabObjNdx]] = 3;
        break;
      case 12:
        break;
      case 13:
        focusedFXTabObjNdx = FXTabObjCount[(currentFXPage + 1) % 2] - 1;
        currentFXPage = 1;
        updateSelectedButtonColor(currentFXPage, 2, 0x2EBAE4, 0x3E495C, ui_FX_Page_1_Led, ui_FX_Page_2_Led);
        lv_tabview_set_act(ui_FX_Tabs, currentFXPage, LV_ANIM_OFF);

        break;
      default:
        isEditing = !isEditing;
        if (isEditing)
        {
          lv_obj_set_style_text_color(FXTabObjects[currentFXPage][focusedFXTabObjNdx], lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        if (!isEditing)
        {
          lv_obj_set_style_text_color(FXTabObjects[currentFXPage][focusedFXTabObjNdx], lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
      }
    }

    if (currentFXPage == 1)
    {
      switch (focusedFXTabObjNdx)
      {
      case 0:
        audioValues[FXTabObjValNdx[currentFXPage][focusedFXTabObjNdx]]++;
        audioValues[FXTabObjValNdx[currentFXPage][focusedFXTabObjNdx]] %= 2;
        break;
      case 1:
        audioValues[FXTabObjValNdx[currentFXPage][focusedFXTabObjNdx]] = 0;
        break;
      case 2:
        audioValues[FXTabObjValNdx[currentFXPage][focusedFXTabObjNdx]] = 1;
        break;
      case 4:
        focusedFXTabObjNdx = FXTabObjCount[(currentFXPage + 1) % 2] - 2;
        currentFXPage = 0;
        updateSelectedButtonColor(currentFXPage, 2, 0x2EBAE4, 0x3E495C, ui_FX_Page_1_Led, ui_FX_Page_2_Led);
        lv_tabview_set_act(ui_FX_Tabs, currentFXPage, LV_ANIM_OFF);
        break;
      case 5:
        break;
      default:
        isEditing = !isEditing;
        if (isEditing)
        {
          lv_obj_set_style_text_color(FXTabObjects[currentFXPage][focusedFXTabObjNdx], lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        if (!isEditing)
        {
          lv_obj_set_style_text_color(FXTabObjects[currentFXPage][focusedFXTabObjNdx], lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
      }
    }
    if (!isEditing)
    {
      if (FXTabObjValNdx[currentFXPage][focusedFXTabObjNdx] != -1)
      {
        msg = msg = "<" + audioValuesName[FXTabObjValNdx[currentFXPage][focusedFXTabObjNdx]] + ":" + audioValues[FXTabObjValNdx[currentFXPage][focusedFXTabObjNdx]] + ">";
        sendUARTMsg(msg);
      }
    }

    updateFXTab();
  }
  lastEncoderCount = encoderNow;
}

/**
 * @brief Generates a formatted message string based on the provided parameters.
 *
 * This function constructs a message string that encodes information about modulators,
 * their settings and their destinations.
 *
 * @param msgType The type of message to generate. Valid message types are:
 *                - -3: Reset Variable index message
 *                - -2: Reset Amount message
 *                - -1: Reset Modulation type message
 *                - 0: Variable index message with destination index
 *                - 1: Amount message with specified amount
 *                - 2: Modulation type message with specified mod type
 *
 * @param modNameNdx The index representing the modulator's name. Valid indices are:
 *                   - 1: Envelope (ENV)
 *                   - 2: Low-Frequency Oscillator 1 (LFO1)
 *                   - 3: Low-Frequency Oscillator 2 (LFO2)
 *                   - 0: Exit (no modulator)
 *
 * @param modType The type of modulation.
 *                  - 0: Modulation only in one direction
 *                  - 1: Modulation in both directions
 *
 * @param destNdx The index representing the destination.
 *
 * @param amount A numeric value representing the amount of the modulation
 *
 * @param assignNdx An index used for assigning the settings to a "slot" of the modulator.
 */
String generateMatrixMsg(int msgType, int modNameNdx, int modType, int destNdx, int amount, int assignNdx)
{
  String modulatorName;
  String msgTypeName;
  String outputMsg = "";
  switch (modNameNdx)
  {
  case 0:
    break;
  case 1:
    modulatorName = "ENV";
    break;
  case 2:
    modulatorName = "LFO1";
    break;
  case 3:
    modulatorName = "LFO2";
    break;
  }

  switch (destNdx)
  {
  case 0:
    break;
  }
  switch (msgType)
  {
  case -3:
    msgTypeName = "VARNDX_";
    outputMsg = "<" + modulatorName + msgTypeName + String(assignNdx) + ":" + "-1" + ">";
    break;
  case -2:
    msgTypeName = "AMOUNT_";
    outputMsg = "<" + modulatorName + msgTypeName + String(assignNdx) + ":" + "0" + ">";
    break;
  case -1:
    msgTypeName = "MODTYPE_";
    outputMsg = "<" + modulatorName + msgTypeName + String(assignNdx) + ":" + "0" + ">";
    break;
  case 0:
    msgTypeName = "VARNDX_";
    outputMsg = "<" + modulatorName + msgTypeName + String(assignNdx) + ":" + String(destNdx) + ">";
    break;
  case 1:
    msgTypeName = "AMOUNT_";
    outputMsg = "<" + modulatorName + msgTypeName + String(assignNdx) + ":" + String(amount) + ">";
    break;
  case 2:
    msgTypeName = "MODTYPE_";
    outputMsg = "<" + modulatorName + msgTypeName + String(assignNdx) + ":" + String(modType) + ">";
    break;
  }
  return outputMsg;
}

int findSmallestMissingIndex(int arr[20][5], int checkNum)
{
  std::vector<int> bValues;

  for (int i = 0; i < 20; i++)
  {
    if (arr[i][0] == checkNum)
    {
      bValues.push_back(arr[i][4]);
    }
  }

  std::sort(bValues.begin(), bValues.end());

  int smallestMissing = 0;
  for (int num : bValues)
  {
    if (num == smallestMissing)
    {
      smallestMissing++;
    }
  }

  return smallestMissing;
}

void focusMatrixTab()
{
  static lv_obj_t *matrixTabObjects[5][9] = {{ui_MatrixElementComponent, ui_MatrixElementComponent1, ui_MatrixElementComponent2, ui_MatrixElementComponent3, ui_Matrix_Page_1_Led, ui_Matrix_Page_2_Led, ui_Matrix_Page_3_Led, ui_Matrix_Page_4_Led, ui_Matrix_Page_5_Led},
                                             {ui_MatrixElementComponent4, ui_MatrixElementComponent5, ui_MatrixElementComponent6, ui_MatrixElementComponent7, ui_Matrix_Page_1_Led, ui_Matrix_Page_2_Led, ui_Matrix_Page_3_Led, ui_Matrix_Page_4_Led, ui_Matrix_Page_5_Led},
                                             {ui_MatrixElementComponent8, ui_MatrixElementComponent9, ui_MatrixElementComponent10, ui_MatrixElementComponent11, ui_Matrix_Page_1_Led, ui_Matrix_Page_2_Led, ui_Matrix_Page_3_Led, ui_Matrix_Page_4_Led, ui_Matrix_Page_5_Led},
                                             {ui_MatrixElementComponent12, ui_MatrixElementComponent13, ui_MatrixElementComponent14, ui_MatrixElementComponent15, ui_Matrix_Page_1_Led, ui_Matrix_Page_2_Led, ui_Matrix_Page_3_Led, ui_Matrix_Page_4_Led, ui_Matrix_Page_5_Led},
                                             {ui_MatrixElementComponent16, ui_MatrixElementComponent17, ui_MatrixElementComponent18, ui_MatrixElementComponent19, ui_Matrix_Page_1_Led, ui_Matrix_Page_2_Led, ui_Matrix_Page_3_Led, ui_Matrix_Page_4_Led, ui_Matrix_Page_5_Led}};
  int encoderNow = encoder.getCount();
  static int focusedMatrixTabCompNdx = 0;
  static int focusedMatrixCompChildNdx = 0;
  static int dropdownSelectNdx = 0;

  static int matrixArrNdx[20][5] = {
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0}}; // {a, b, c, d, e}; a = modNameNdx, b = modType, c = dest Index, d = amount, e = assigned Index

  if (encoderNow != lastEncoderCount)
  {
    int incrementVal = 0;
    if (encoderNow > lastEncoderCount)
    {
      incrementVal = 1;
    }
    if (encoderNow < lastEncoderCount)
    {
      incrementVal = -1;
    }
    if (!isEditing)
    {
      if (focusedMatrixTabCompNdx < 4)
      {
        focusedMatrixCompChildNdx += incrementVal;
        if (focusedMatrixCompChildNdx > 4)
        {
          lv_obj_set_style_outline_opa(ui_comp_get_child(matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx], focusedMatrixCompChildNdx), 0, LV_PART_MAIN | LV_STATE_DEFAULT);
          focusedMatrixCompChildNdx = 0;
          focusedMatrixTabCompNdx++;
        }
        if (focusedMatrixCompChildNdx < 0)
        {
          lv_obj_set_style_outline_opa(ui_comp_get_child(matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx], 2 + focusedMatrixCompChildNdx), 0, LV_PART_MAIN | LV_STATE_DEFAULT);
          focusedMatrixCompChildNdx = 4;
          focusedMatrixTabCompNdx--;
          if (focusedMatrixTabCompNdx < 0)
          {
            focusedMatrixTabCompNdx = 8;
          }
        }
      }
      else
      {
        focusedMatrixTabCompNdx += incrementVal;
        if (focusedMatrixTabCompNdx > 8)
        {
          lv_obj_set_style_outline_opa(matrixTabObjects[currentMatrixPage][8], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
          focusedMatrixTabCompNdx = 0;
          focusedMatrixCompChildNdx = 0;
        }
        else if (focusedMatrixTabCompNdx < 4)
        {
          focusedMatrixCompChildNdx = 4;
          lv_obj_set_style_outline_opa(matrixTabObjects[currentMatrixPage][4], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
      }

      if (focusedMatrixTabCompNdx < 4)
      {
        lv_obj_t *focusedObject = ui_comp_get_child(matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx], 1 + focusedMatrixCompChildNdx);
        lv_obj_set_style_outline_color(focusedObject, lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_outline_opa(focusedObject, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_outline_width(focusedObject, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_outline_pad(focusedObject, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_outline_opa(ui_comp_get_child(matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx], 1 + (focusedMatrixCompChildNdx + 5 - incrementVal) % 5), 0, LV_PART_MAIN | LV_STATE_DEFAULT);
      }
      else
      {
        lv_obj_t *focusedObject = matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx];

        lv_obj_set_style_outline_color(focusedObject, lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_outline_opa(focusedObject, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_outline_width(focusedObject, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_outline_pad(focusedObject, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        switch (focusedMatrixTabCompNdx)
        {
        case 8:
          lv_obj_set_style_outline_opa(matrixTabObjects[currentMatrixPage][7], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
          break;
        default:
          lv_obj_set_style_outline_opa(matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx - incrementVal], 0, LV_PART_MAIN | LV_STATE_DEFAULT); // couldn't be applied to the last page, weird bug crashed esp32
        }
      }
    }
    if (isEditing)
    {
      switch (focusedMatrixCompChildNdx)
      {
      case 0:
        dropdownSelectNdx = (dropdownSelectNdx + incrementVal);
        if (dropdownSelectNdx < 0 || dropdownSelectNdx > 3)
        {
          dropdownSelectNdx = (dropdownSelectNdx < 3) ? 0 : 3;
        }

        lv_dropdown_set_selected(ui_comp_get_child(matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx], 1 + focusedMatrixCompChildNdx), dropdownSelectNdx);

        break;
      case 3:
        dropdownSelectNdx = (dropdownSelectNdx + incrementVal);
        if (dropdownSelectNdx < 0 || dropdownSelectNdx > 9)
        {
          dropdownSelectNdx = (dropdownSelectNdx < 9) ? 0 : 9;
        }
        lv_dropdown_set_selected(ui_comp_get_child(matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx], 1 + focusedMatrixCompChildNdx), dropdownSelectNdx);

        break;
      case 4:
        lv_slider_set_value(ui_comp_get_child(matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx], 1 + focusedMatrixCompChildNdx), lv_slider_get_value(ui_comp_get_child(matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx], 1 + focusedMatrixCompChildNdx)) + incrementVal * incrementMultiplier, LV_ANIM_OFF);
        break;
      }
    }
  }

  if (encoderButton.pressed())
  {
    int smallestIndex;

    if (!isEditing)
    {
      if (focusedMatrixTabCompNdx < 4)
      {
        switch (focusedMatrixCompChildNdx)
        {
        case 1:
          lv_obj_set_style_text_color(ui_comp_get_child(matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx], 1 + focusedMatrixCompChildNdx), lv_color_hex(0x2EBAE4), 0);
          lv_obj_set_style_text_color(ui_comp_get_child(matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx], 2 + focusedMatrixCompChildNdx), lv_color_hex(0x131927), 0);
          matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][1] = 0;
          sendUARTMsg(generateMatrixMsg(2, matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][0], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][1], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][2], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][3], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][4]));

          break;
        case 2:
          lv_obj_set_style_text_color(ui_comp_get_child(matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx], 1 + focusedMatrixCompChildNdx), lv_color_hex(0x2EBAE4), 0);
          lv_obj_set_style_text_color(ui_comp_get_child(matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx], focusedMatrixCompChildNdx), lv_color_hex(0x131927), 0);
          matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][1] = 1;
          sendUARTMsg(generateMatrixMsg(2, matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][0], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][1], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][2], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][3], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][4]));

          break;

        case 0:
        case 3:
          lv_dropdown_open(ui_comp_get_child(matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx], 1 + focusedMatrixCompChildNdx));
          isEditing = true;
          break;

        case 4:
          isEditing = true;
          break;
        }
      }
      else
      {
        switch (focusedMatrixTabCompNdx)
        {
        case 4:
          lv_obj_set_style_bg_color(matrixTabObjects[0][4 + currentMatrixPage], lv_color_hex(0x3E495C), LV_PART_MAIN | LV_STATE_DEFAULT);
          currentMatrixPage = 0;
          lv_tabview_set_act(ui_Matrix_Pages, currentMatrixPage, LV_ANIM_OFF);
          lv_obj_set_style_bg_color(matrixTabObjects[0][4 + currentMatrixPage], lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);
          break;
        case 5:
          lv_obj_set_style_bg_color(matrixTabObjects[0][4 + currentMatrixPage], lv_color_hex(0x3E495C), LV_PART_MAIN | LV_STATE_DEFAULT);
          currentMatrixPage = 1;
          lv_tabview_set_act(ui_Matrix_Pages, currentMatrixPage, LV_ANIM_OFF);
          lv_obj_set_style_bg_color(matrixTabObjects[0][4 + currentMatrixPage], lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);
          break;
        case 6:
          lv_obj_set_style_bg_color(matrixTabObjects[0][4 + currentMatrixPage], lv_color_hex(0x3E495C), LV_PART_MAIN | LV_STATE_DEFAULT);
          currentMatrixPage = 2;
          lv_tabview_set_act(ui_Matrix_Pages, currentMatrixPage, LV_ANIM_OFF);
          lv_obj_set_style_bg_color(matrixTabObjects[0][4 + currentMatrixPage], lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);
          break;
        case 7:
          lv_obj_set_style_bg_color(matrixTabObjects[0][4 + currentMatrixPage], lv_color_hex(0x3E495C), LV_PART_MAIN | LV_STATE_DEFAULT);
          currentMatrixPage = 3;
          lv_tabview_set_act(ui_Matrix_Pages, currentMatrixPage, LV_ANIM_OFF);
          lv_obj_set_style_bg_color(matrixTabObjects[0][4 + currentMatrixPage], lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);
          break;
        case 8:
          lv_obj_set_style_bg_color(matrixTabObjects[0][4 + currentMatrixPage], lv_color_hex(0x3E495C), LV_PART_MAIN | LV_STATE_DEFAULT);
          currentMatrixPage = 4;
          lv_tabview_set_act(ui_Matrix_Pages, currentMatrixPage, LV_ANIM_OFF);
          lv_obj_set_style_bg_color(matrixTabObjects[0][4 + currentMatrixPage], lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);
          break;
        }
      }
    }
    else
    {
      switch (focusedMatrixCompChildNdx)
      {
      case 0:
        lv_dropdown_set_selected(ui_comp_get_child(matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx], 1 + focusedMatrixCompChildNdx), dropdownSelectNdx);

        switch (dropdownSelectNdx)
        {
        case 0:
          lv_obj_set_style_bg_color(ui_comp_get_child(matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx], 1 + focusedMatrixCompChildNdx), lv_color_hex(0x131927), LV_PART_MAIN | LV_STATE_DEFAULT);
          sendUARTMsg(generateMatrixMsg(-3, matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][0], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][1], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][2], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][3], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][4]));
          sendUARTMsg(generateMatrixMsg(-2, matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][0], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][1], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][2], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][3], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][4]));
          sendUARTMsg(generateMatrixMsg(-1, matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][0], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][1], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][2], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][3], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][4]));

          matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][0] = dropdownSelectNdx;
          break;
        default:
          sendUARTMsg(generateMatrixMsg(-3, matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][0], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][1], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][2], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][3], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][4]));
          sendUARTMsg(generateMatrixMsg(-2, matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][0], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][1], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][2], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][3], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][4]));
          sendUARTMsg(generateMatrixMsg(-1, matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][0], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][1], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][2], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][3], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][4]));
          smallestIndex = findSmallestMissingIndex(matrixArrNdx, dropdownSelectNdx);
          matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][0] = dropdownSelectNdx;
          matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][4] = smallestIndex;
          sendUARTMsg(generateMatrixMsg(0, matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][0], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][1], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][2], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][3], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][4]));
          sendUARTMsg(generateMatrixMsg(1, matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][0], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][1], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][2], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][3], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][4]));
          sendUARTMsg(generateMatrixMsg(2, matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][0], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][1], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][2], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][3], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][4]));

          lv_obj_set_style_bg_color(ui_comp_get_child(matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx], 1 + focusedMatrixCompChildNdx), lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);
          break;
        }
        isEditing = false;
        lv_dropdown_close(ui_comp_get_child(matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx], 1 + focusedMatrixCompChildNdx));
        break;
      case 3:
        lv_dropdown_set_selected(ui_comp_get_child(matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx], 1 + focusedMatrixCompChildNdx), dropdownSelectNdx);
        switch (dropdownSelectNdx)
        {
        case 0:
          sendUARTMsg(generateMatrixMsg(-3, matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][0], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][1], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][2], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][3], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][4]));
          matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][2] = dropdownSelectNdx;

          lv_obj_set_style_bg_color(ui_comp_get_child(matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx], 1 + focusedMatrixCompChildNdx), lv_color_hex(0x131927), LV_PART_MAIN | LV_STATE_DEFAULT);
          break;
        default:
          matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][2] = dropdownSelectNdx - 1;
          Serial.println("dropdownSelectNdx: " + String(dropdownSelectNdx));
          sendUARTMsg(generateMatrixMsg(0, matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][0], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][1], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][2], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][3], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][4]));
          lv_obj_set_style_bg_color(ui_comp_get_child(matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx], 1 + focusedMatrixCompChildNdx), lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);
          break;
        }
        lv_dropdown_close(ui_comp_get_child(matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx], 1 + focusedMatrixCompChildNdx));
        isEditing = false;
        break;
      case 4:
        int sliderValue = lv_slider_get_value(ui_comp_get_child(matrixTabObjects[currentMatrixPage][focusedMatrixTabCompNdx], 1 + focusedMatrixCompChildNdx));
        matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][3] = sliderValue;
        sendUARTMsg(generateMatrixMsg(1, matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][0], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][1], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][2], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][3], matrixArrNdx[currentMatrixPage * 4 + focusedMatrixTabCompNdx][4]));

        isEditing = false;
        break;
      }
    }
  }

  lastEncoderCount = encoderNow;
}

#define NUM_MACROS 10
const byte numDestElements = 28;
const byte macroPins[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // all adc1 pins
int oldMacroValues[10];
int macroValues[10];
int macroConnections[20][3] = {
    {-1, -1, 0},
    {-1, -1, 0},
    {-1, -1, 0},
    {-1, -1, 0},
    {-1, -1, 0},
    {-1, -1, 0},
    {-1, -1, 0},
    {-1, -1, 0},
    {-1, -1, 0},
    {-1, -1, 0},
    {-1, -1, 0},
    {-1, -1, 0},
    {-1, -1, 0},
    {-1, -1, 0},
    {-1, -1, 0},
    {-1, -1, 0},
    {-1, -1, 0},
    {-1, -1, 0},
    {-1, -1, 0},
    {-1, -1, 0}};
const byte macroDestNameNdx[numDestElements] = {0, 2, 4, 6, 8, 10, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 24, 25, 26, 27, 28, 31, 34, 36, 39, 42, 43};

void checkMacros()
{
  for (int i = 0; i < NUM_MACROS; i++)
  {
    macroValues[i] = 255 - (analogRead(macroPins[i]) >> 4);
    if (abs(oldMacroValues[i] - macroValues[i]) >= 2) // checks if new value at a threshold of 2
    {
      for (int j = 0; j < 20; j++) // checks each assign panel of the ui
      {
        if (macroConnections[j][0] == i) // checks if current knob is assigned
        {
          if (macroConnections[j][0] >= 0 && macroConnections[j][1] >= 0) // checks if knob has destination
          {
            String destName = audioValuesName[macroDestNameNdx[macroConnections[j][1]]]; // name of variable
            int value = (macroValues[i] * macroConnections[j][2]) >> 8;                  // adjusting send value using the set amount
            String msg = "<" + destName + ":" + String(value) + ">";                     // generate msg string
            sendUARTMsg(msg);                                                            // send msg
            audioValues[macroDestNameNdx[macroConnections[j][1]]] = value;               // update audioValues (for ui)
          }
        }
      }
    }
    oldMacroValues[i] = macroValues[i];
  }
}

void focusMacroTab()
{
  static lv_obj_t *macroTabObjects[5][9] = {{ui_MacroElement, ui_MacroElement1, ui_MacroElement2, ui_MacroElement3, ui_Knob_Page_1_Led, ui_Knob_Page_2_Led, ui_Knob_Page_3_Led, ui_Knob_Page_4_Led, ui_Knob_Page_5_Led},
                                            {ui_MacroElement4, ui_MacroElement5, ui_MacroElement6, ui_MacroElement7, ui_Knob_Page_1_Led, ui_Knob_Page_2_Led, ui_Knob_Page_3_Led, ui_Knob_Page_4_Led, ui_Knob_Page_5_Led},
                                            {ui_MacroElement8, ui_MacroElement9, ui_MacroElement10, ui_MacroElement11, ui_Knob_Page_1_Led, ui_Knob_Page_2_Led, ui_Knob_Page_3_Led, ui_Knob_Page_4_Led, ui_Knob_Page_5_Led},
                                            {ui_MacroElement12, ui_MacroElement13, ui_MacroElement14, ui_MacroElement15, ui_Knob_Page_1_Led, ui_Knob_Page_2_Led, ui_Knob_Page_3_Led, ui_Knob_Page_4_Led, ui_Knob_Page_5_Led},
                                            {ui_MacroElement16, ui_MacroElement17, ui_MacroElement18, ui_MacroElement19, ui_Knob_Page_1_Led, ui_Knob_Page_2_Led, ui_Knob_Page_3_Led, ui_Knob_Page_4_Led, ui_Knob_Page_5_Led}};
  int encoderNow = encoder.getCount();
  static int focusedMacroTabCompNdx = 0;
  static int focusedMacroCompChildNdx = 0;
  static int dropdownSelectNdx = 0;

  if (encoderNow != lastEncoderCount)
  {
    int incrementVal = 0;
    if (encoderNow > lastEncoderCount)
    {
      incrementVal = 1;
    }
    if (encoderNow < lastEncoderCount)
    {
      incrementVal = -1;
    }
    if (!isEditing)
    {
      if (focusedMacroTabCompNdx < 4)
      {
        focusedMacroCompChildNdx += incrementVal;
        if (focusedMacroCompChildNdx > 2)
        {
          lv_obj_set_style_outline_opa(ui_comp_get_child(macroTabObjects[currentMacroPage][focusedMacroTabCompNdx], focusedMacroCompChildNdx), 0, LV_PART_MAIN | LV_STATE_DEFAULT);
          focusedMacroCompChildNdx = 0;
          focusedMacroTabCompNdx++;
        }
        if (focusedMacroCompChildNdx < 0)
        {
          lv_obj_set_style_outline_opa(ui_comp_get_child(macroTabObjects[currentMacroPage][focusedMacroTabCompNdx], 2 + focusedMacroCompChildNdx), 0, LV_PART_MAIN | LV_STATE_DEFAULT);
          focusedMacroCompChildNdx = 2;
          focusedMacroTabCompNdx--;
          if (focusedMacroTabCompNdx < 0)
          {
            focusedMacroTabCompNdx = 8;
          }
        }
      }
      else
      {
        focusedMacroTabCompNdx += incrementVal;
        if (focusedMacroTabCompNdx > 8)
        {
          lv_obj_set_style_outline_opa(macroTabObjects[currentMacroPage][8], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
          focusedMacroTabCompNdx = 0;
          focusedMacroCompChildNdx = 0;
        }
        else if (focusedMacroTabCompNdx < 4)
        {
          focusedMacroCompChildNdx = 2;
          lv_obj_set_style_outline_opa(macroTabObjects[currentMacroPage][4], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
      }

      if (focusedMacroTabCompNdx < 4)
      {
        lv_obj_t *focusedObject = ui_comp_get_child(macroTabObjects[currentMacroPage][focusedMacroTabCompNdx], 1 + focusedMacroCompChildNdx);
        lv_obj_set_style_outline_color(focusedObject, lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_outline_opa(focusedObject, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_outline_width(focusedObject, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_outline_pad(focusedObject, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_outline_opa(ui_comp_get_child(macroTabObjects[currentMacroPage][focusedMacroTabCompNdx], 1 + (focusedMacroCompChildNdx + 3 - incrementVal) % 3), 0, LV_PART_MAIN | LV_STATE_DEFAULT);
      }
      else
      {
        lv_obj_t *focusedObject = macroTabObjects[currentMatrixPage][focusedMacroTabCompNdx];
        lv_obj_set_style_outline_color(focusedObject, lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_outline_opa(focusedObject, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_outline_width(focusedObject, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_outline_pad(focusedObject, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        switch (focusedMacroTabCompNdx)
        {
        case 8:
          lv_obj_set_style_outline_opa(macroTabObjects[currentMacroPage][7], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
          break;
        default:
          lv_obj_set_style_outline_opa(macroTabObjects[currentMacroPage][focusedMacroTabCompNdx - incrementVal], 0, LV_PART_MAIN | LV_STATE_DEFAULT); // couldn't be applied to the last page, weird bug crashed esp32
        }
      }
    }
    if (isEditing)
    {
      lv_obj_t *focusedObject = ui_comp_get_child(macroTabObjects[currentMacroPage][focusedMacroTabCompNdx], 1 + focusedMacroCompChildNdx);
      switch (focusedMacroCompChildNdx)
      {
      case 0:
        dropdownSelectNdx = (dropdownSelectNdx + incrementVal);
        if (dropdownSelectNdx < 0 || dropdownSelectNdx > 10)
        {
          dropdownSelectNdx = (dropdownSelectNdx < 10) ? 0 : 10;
        }

        lv_dropdown_set_selected(focusedObject, dropdownSelectNdx);

        break;
      case 1:
        dropdownSelectNdx = (dropdownSelectNdx + incrementVal);
        if (dropdownSelectNdx < 0 || dropdownSelectNdx > 28)
        {
          dropdownSelectNdx = (dropdownSelectNdx < 28) ? 0 : 28;
        }
        lv_dropdown_set_selected(focusedObject, dropdownSelectNdx);

        break;
      case 2:
        lv_slider_set_value(focusedObject, lv_slider_get_value(focusedObject) + incrementVal * incrementMultiplier, LV_ANIM_OFF);
        break;
      }
    }
  }

  if (encoderButton.pressed())
  {
    if (focusedMacroTabCompNdx < 4)
    {
      if (!isEditing)
      {
        isEditing = true;
        switch (focusedMacroCompChildNdx)
        {
        case 0:
        case 1:
          lv_dropdown_open(ui_comp_get_child(macroTabObjects[currentMacroPage][focusedMacroTabCompNdx], 1 + focusedMacroCompChildNdx));
          break;
        default:
          break;
        }
      }
      else
      {
        isEditing = false;
        switch (focusedMacroCompChildNdx)
        {
        case 0:
          macroConnections[focusedMacroTabCompNdx + 4 * currentMacroPage][0] = lv_dropdown_get_selected(ui_comp_get_child(macroTabObjects[currentMacroPage][focusedMacroTabCompNdx], 1 + focusedMacroCompChildNdx)) - 1;
          lv_dropdown_close(ui_comp_get_child(macroTabObjects[currentMacroPage][focusedMacroTabCompNdx], 1 + focusedMacroCompChildNdx));
          // Serial.println("Selected: " + String(macroConnections[focusedMacroTabCompNdx + 4 * currentMacroPage][0]));
          break;
        case 1:
          macroConnections[focusedMacroTabCompNdx + 4 * currentMacroPage][1] = lv_dropdown_get_selected(ui_comp_get_child(macroTabObjects[currentMacroPage][focusedMacroTabCompNdx], 1 + focusedMacroCompChildNdx)) - 1;
          lv_dropdown_close(ui_comp_get_child(macroTabObjects[currentMacroPage][focusedMacroTabCompNdx], 1 + focusedMacroCompChildNdx));
          // Serial.println("Selected: " + String(macroConnections[focusedMacroTabCompNdx + 4 * currentMacroPage][1]));
          break;
        case 2:
          macroConnections[focusedMacroTabCompNdx + 4 * currentMacroPage][2] = lv_slider_get_value(ui_comp_get_child(macroTabObjects[currentMacroPage][focusedMacroTabCompNdx], 1 + focusedMacroCompChildNdx));
          // Serial.println("Selected: " + String(macroConnections[focusedMacroTabCompNdx + 4 * currentMacroPage][2]));
          break;
        }
      }
    }
    else
    {
      switch (focusedMacroTabCompNdx)
      {
      case 4:
        lv_obj_set_style_bg_color(macroTabObjects[0][4 + currentMacroPage], lv_color_hex(0x3E495C), LV_PART_MAIN | LV_STATE_DEFAULT);
        currentMacroPage = 0;
        lv_tabview_set_act(ui_Macro_Pages, currentMacroPage, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(macroTabObjects[0][4 + currentMacroPage], lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);
        break;
      case 5:
        lv_obj_set_style_bg_color(macroTabObjects[0][4 + currentMacroPage], lv_color_hex(0x3E495C), LV_PART_MAIN | LV_STATE_DEFAULT);
        currentMacroPage = 1;
        lv_tabview_set_act(ui_Macro_Pages, currentMacroPage, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(macroTabObjects[0][4 + currentMacroPage], lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);
        break;
      case 6:
        lv_obj_set_style_bg_color(macroTabObjects[0][4 + currentMacroPage], lv_color_hex(0x3E495C), LV_PART_MAIN | LV_STATE_DEFAULT);
        currentMacroPage = 2;
        lv_tabview_set_act(ui_Macro_Pages, currentMacroPage, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(macroTabObjects[0][4 + currentMacroPage], lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);
        break;
      case 7:
        lv_obj_set_style_bg_color(macroTabObjects[0][4 + currentMacroPage], lv_color_hex(0x3E495C), LV_PART_MAIN | LV_STATE_DEFAULT);
        currentMacroPage = 3;
        lv_tabview_set_act(ui_Macro_Pages, currentMacroPage, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(macroTabObjects[0][4 + currentMacroPage], lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);
        break;
      case 8:
        lv_obj_set_style_bg_color(macroTabObjects[0][4 + currentMacroPage], lv_color_hex(0x3E495C), LV_PART_MAIN | LV_STATE_DEFAULT);
        currentMacroPage = 4;
        lv_tabview_set_act(ui_Macro_Pages, currentMacroPage, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(macroTabObjects[0][4 + currentMacroPage], lv_color_hex(0x2EBAE4), LV_PART_MAIN | LV_STATE_DEFAULT);
        break;
      }
    }
  }

  lastEncoderCount = encoderNow;
}
//----------------------------------------------------------------------------

void checkButtons()
{
  if (nextMenuButton.pressed())
  {
    currentMainTab = (currentMainTab + 1) % 4;
    lv_tabview_set_act(ui_Main_Tabs, currentMainTab, LV_ANIM_OFF);
    updateSelectedButtonColor(currentMainTab, 4, 0x2EBAE4, 0x3E495C, ui_OSC, ui_SFX1, ui_Matrix, ui_Macro);
    isEditing = false;
  }
  if (prevMenuButton.pressed())
  {
    currentMainTab = (currentMainTab + 3) % 4;
    lv_tabview_set_act(ui_Main_Tabs, currentMainTab, LV_ANIM_OFF);
    updateSelectedButtonColor(currentMainTab, 4, 0x2EBAE4, 0x3E495C, ui_OSC, ui_SFX1, ui_Matrix, ui_Macro);
    isEditing = false;
  }
  if (tenXMultiplierButton.pressed())
  {
    incrementMultiplier = incrementMultiplier * 10;
  }
  if (hunXMultiplierButton.pressed())
  {
    incrementMultiplier = incrementMultiplier * 100;
  }
  if (tenXMultiplierButton.released())
  {
    incrementMultiplier = incrementMultiplier / 10;
  }
  if (hunXMultiplierButton.released())
  {
    incrementMultiplier = incrementMultiplier / 100;
  }
}

hw_timer_t *analyzerUpdateTimer = NULL;
hw_timer_t *updateUITimer = NULL;
volatile bool AUT_expired = false;
volatile bool UI_expired = false;
unsigned int sampling_period_us;

void IRAM_ATTR analyzerUpdateTimerInterrupt()
{
  AUT_expired = true;
}

void IRAM_ATTR updateUITimerInterrupt()
{
  UI_expired = true;
}

#define SAMPLES 1024        // Must be a power of 2
#define SAMPLING_FREQ 40000 // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
#define AMPLITUDE 60000     // Depending on your audio source level, you may need to alter this value. Can be used as a 'sensitivity' control.
#define NUM_BANDS 10        // To change this, you will need to change the bunch of if statements describing the mapping from bins to bands
#define NOISE 0             // Used as a crude noise filter, values below this are ignored

int count = 0;
int analogValue;

// Sampling and FFT stuff
int oldBarHeights[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int bandValues[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// float coefficient[] = {16.32, 19.03, 12.12, 10.53, 7.45, 5.43, 4.07, 2.96, 2.14, 1}; //for a flat analyzer slope with white noise
float coefficient[] = {24.69, 27.22, 16.46, 13.6, 9.18, 6.39, 4.59, 3.2, 2.22, 1}; // for a -4.5db/Oct analyzer slope with white noise
// float coefficient[] ={1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int currentScreen = 0;

double vReal[SAMPLES];
int vRealR[SAMPLES];
int vRealL[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime;
ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, SAMPLES, SAMPLING_FREQ);

void updateAnalyzer()
{

  for (int i = 0; i < NUM_BANDS; i++)
  {
    bandValues[i] = 0;
  }

  // Sample the audio pin
  for (int i = 0; i < SAMPLES; i++)
  {
    newTime = micros();
    esp_err_t r = adc2_get_raw(ADC2_CHANNEL_2, ADC_WIDTH_12Bit, &vRealR[i]);
    esp_err_t l = adc2_get_raw(ADC2_CHANNEL_3, ADC_WIDTH_12Bit, &vRealL[i]);

    vReal[i] = (vRealR[i] + vRealL[i]) / 2;
    vImag[i] = 0;
    while ((micros() - newTime) < sampling_period_us)
    { /* chill */
    }
  }

  // Compute FFT
  FFT.dcRemoval();
  FFT.windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD); // Weigh data
  FFT.compute(vReal, vImag, SAMPLES, FFT_FORWARD);                 // Compute FFT
  FFT.complexToMagnitude(vReal, vImag, SAMPLES);                   // Compute magnitudes

  // Analyse FFT results
  for (int i = 2; i < (SAMPLES / 2); i++)
  { // Don't use sample 0 and only first SAMPLES/2 are usable. Each array element represents a frequency bin and its value the amplitude.
    if (vReal[i] > NOISE)
    { // Add a crude noise filter
      if (i <= 3)
        bandValues[0] += (int)vReal[i];

      if (i > 3 && i <= 5)
        bandValues[1] += (int)vReal[i];

      if (i > 5 && i <= 9)
        bandValues[2] += (int)vReal[i];

      if (i > 9 && i <= 15)
        bandValues[3] += (int)vReal[i];

      if (i > 15 && i <= 26)
        bandValues[4] += (int)vReal[i];

      if (i > 26 && i <= 45)
        bandValues[5] += (int)vReal[i];

      if (i > 45 && i <= 79)
        bandValues[6] += (int)vReal[i];

      if (i > 79 && i <= 138)
        bandValues[7] += (int)vReal[i];

      if (i > 138 && i <= 242)
        bandValues[8] += (int)vReal[i];

      if (i > 242)
        bandValues[9] += ((int)vReal[i]);
    }
  }

  static lv_obj_t *panels[10] = {ui_Panel1, ui_Panel2, ui_Panel3, ui_Panel4, ui_Panel5, ui_Panel6, ui_Panel7, ui_Panel8, ui_Panel9, ui_Panel10};
  // Process the FFT data into bar heights
  for (byte band = 0; band < NUM_BANDS; band++)
  {
    int barHeight = bandValues[band] * coefficient[band] / AMPLITUDE;
    if (barHeight > 100)
      barHeight = 100;
    barHeight = 100 - barHeight;
    barHeight = (oldBarHeights[band] + barHeight) / 2;
    oldBarHeights[band] = barHeight;
    lv_obj_set_height(panels[band], lv_pct(barHeight));
  }
}

void setup()
{

  loopTaskWDTEnabled = true;
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, 15, 16);

  //------------Encoder---------------

  ESP32Encoder::useInternalWeakPullResistors = puType::up;
  encoder.attachSingleEdge(41, 42);
  encoder.clearCount();
  encoder.setFilter(1023);

  //------------Buttons---------------

  encoderButton.begin();
  nextMenuButton.begin();
  prevMenuButton.begin();
  tenXMultiplierButton.begin();
  hunXMultiplierButton.begin();

  //------------Other stuff-----------

  screenSetup();
  ui_init();

  analyzerUpdateTimer = timerBegin(0, 80, true);                                  // Timer 0, clock divider 80
  timerAttachInterrupt(analyzerUpdateTimer, &analyzerUpdateTimerInterrupt, true); // Attach the interrupt handling function
  timerAlarmWrite(analyzerUpdateTimer, 30000, true);
  timerAlarmEnable(analyzerUpdateTimer); // Enable the alarm

  updateUITimer = timerBegin(1, 80, true);                            // Timer 0, clock divider 80
  timerAttachInterrupt(updateUITimer, &updateUITimerInterrupt, true); // Attach the interrupt handling function
  timerAlarmWrite(updateUITimer, 600000, true);
  timerAlarmEnable(updateUITimer); // Enable the alarm

  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQ));
  updateOscTab();
  updateFXTab();
  focusMatrixTab();
  focusMacroTab();
  lv_tabview_set_act(ui_Main_Tabs, currentMainTab, LV_ANIM_OFF);
  updateEnvGraph();
  updateSelectedButtonColor(currentMainTab, 4, 0x2EBAE4, 0x3E495C, ui_OSC, ui_SFX1, ui_Matrix, ui_Macro);
  delay(1000);
  Serial.println("Setup done");
}

uint8_t pageCount = 0;
void loop()
{
  if (AUT_expired)
  {
    updateAnalyzer();
    checkMacros();
    AUT_expired = false;
  }

  if (UI_expired)
  {
    switch (currentMainTab)
    {
    case 0:
      updateOscTab();
      break;
    case 1:
      focusEncoderFXTab();
      updateFXTab();

      break;
    default:
      break;
    }
    UI_expired = false;
  }
  checkButtons();
  switch (currentMainTab)
  {
  case 0:
    focusEncoderOscTab();
    break;
  case 1:
    focusEncoderFXTab();
    break;
  case 2:
    focusMatrixTab();
    break;
  case 3:
    focusMacroTab();
    break;
  }
  lv_timer_handler();
}
