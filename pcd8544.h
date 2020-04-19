#define PCD8544_WIDTH 84
#define PCD8544_HEIGHT 48

#define PCD8544_EXTENDEDINSTRUCTION 0x01
#define PCD8544_ENTRYMODE 0x02
#define PCD8544_POWERDOWN 0x04

#define PCD8544_DISPLAYBLANK 0x00
#define PCD8544_DISPLAYALLON 0x01
#define PCD8544_DISPLAYNORMAL 0x04
#define PCD8544_DISPLAYINVERTED 0x05

#define PCD8544_DISPLAYCONTROL 0x08
#define PCD8544_FUNCTIONSET 0x20
#define PCD8544_SETYADDR 0x40
#define PCD8544_SETXADDR 0x80

#define PCD8544_SETTEMP 0x04
#define PCD8544_SETBIAS 0x10
#define PCD8544_SETVOP 0x80

void pcd8544_spi_write(uint8_t c);
void pcd8544_command(uint8_t c);
void pcd8544_data(uint8_t c);
void pcd8544_init(uint8_t contrast);
void pcd8544_set_contrast(uint8_t val);
void pcd8544_display();
void pcd8544_clear();
void pcd8544_set_pixel(int x, int y, int v);
