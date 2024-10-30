#include "DisplaySetup.h"

LGFX::LGFX(void)
{
  {                                    // Perform bus control settings
    auto cfg = _bus_instance.config(); // Get the structure for bus settings

    cfg.spi_host = SPI2_HOST; // Select the SPI to use  ESP32-S2, C3: SPI2_HOST or SPI3_HOST / ESP32: VSPI_HOST or HSPI_HOST
    // Note: With the upgrade of ESP-IDF, the description of VSPI_HOST and HSPI_HOST is deprecated. If an error occurs, use SPI2_HOST or SPI3_HOST instead.
    cfg.spi_mode = 0;                  // Set SPI communication mode (0 ~ 3)
    cfg.freq_write = 40000000;         // SPI clock for transmission (up to 80MHz, rounded to a value divisible by 80MHz)
    cfg.freq_read = 16000000;          // SPI clock for reception
    cfg.spi_3wire = true;              // Set to true if receiving via MOSI pin
    cfg.use_lock = true;               // Set to true if using transaction lock
    cfg.dma_channel = SPI_DMA_CH_AUTO; // Set the DMA channel to use (0=No DMA / 1=1ch / 2=2ch / SPI_DMA_CH_AUTO=Auto)
    // Note: With the upgrade of ESP-IDF, the DMA channel SPI_DMA_CH_AUTO (automatic setting) is recommended. The specification of 1ch, 2ch is deprecated.
    cfg.pin_sclk = 12; // Set the pin number for SPI SCLK
    cfg.pin_mosi = 11; // Set the pin number for SPI MOSI
    cfg.pin_miso = -1; // Set the pin number for SPI MISO (-1 = disable)
    cfg.pin_dc = 17;   // Set the pin number for SPI D/C  (-1 = disable)
    // If sharing the SPI bus with an SD card, always set MISO without omission.

    _bus_instance.config(cfg);              // Reflect the settings in the bus
    _panel_instance.setBus(&_bus_instance); // Set the bus to the panel
  }

  {                                      // Perform display panel control settings
    auto cfg = _panel_instance.config(); // Get the structure for display panel settings

    cfg.pin_cs = 38;   // Pin number where CS is connected   (-1 = disable)
    cfg.pin_rst = 39;  // Pin number where RST is connected  (-1 = disable)
    cfg.pin_busy = -1; // Pin number where BUSY is connected (-1 = disable)

    // Note: The following settings have general initial values for each panel, so try commenting out unknown items.

    cfg.panel_width = 320;    // Actual displayable width
    cfg.panel_height = 480;   // Actual displayable height
    cfg.offset_x = 0;         // Offset amount in the X direction of the panel
    cfg.offset_y = 0;         // Offset amount in the Y direction of the panel
    cfg.offset_rotation = 2;  // Offset value of the rotation direction 0~7 (4~7 are inverted upside down)
    cfg.dummy_read_pixel = 8; // Number of dummy read bits before pixel reading
    cfg.dummy_read_bits = 1;  // Number of dummy read bits before reading non-pixel data
    cfg.readable = true;      // Set to true if data reading is possible
    cfg.invert = false;       // Set to true if the panel brightness is inverted
    cfg.rgb_order = false;    // Set to true if the red and blue of the panel are swapped
    cfg.dlen_16bit = false;   // Set to true if the panel sends data in 16-bit units with 16-bit parallel or SPI
    cfg.bus_shared = true;    // Set to true if sharing the bus with an SD card (bus control is performed with drawJpgFile etc.)

    _panel_instance.config(cfg);
  }

  setPanel(&_panel_instance); // Set the panel to use
}
