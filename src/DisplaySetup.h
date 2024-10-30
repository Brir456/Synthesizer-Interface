#ifndef DISPLAYSETUP_H
#define DISPLAYSETUP_H

#include <LovyanGFX.hpp>

// Example of using LovyanGFX with custom settings on ESP32

/// Create a class that inherits from LGFX_Device to perform custom settings.
class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ILI9488     _panel_instance;
  lgfx::Bus_SPI           _bus_instance;   // Instance of the SPI bus

public:
  LGFX(void);
};

#endif 