/**
 * @file globals.h
 * @brief Global variable and constant definitions for the project.
 *
 * This header file contains the global definitions for UI colors, menu states, 
 * rotary encoder configurations, display settings, and other system-wide constants and variables. 
 * It also declares external variables and enums that are used across different modules 
 * of the program, ensuring centralized configuration and ease of modification.
 *
 * The file includes:
 * - Custom color definitions for user interface (UI) elements.
 * - Pin assignments for hardware components (brightness control, rotary encoder).
 * - Timing constants and menu states (main menu, settings, motor control).
 * - Rotary encoder state tracking and UI geometry settings.
 * - Brightness control and menu structure.
 * - Ethernet configuration parameters and TCP server settings.
 * - Screen control and synchronization using FreeRTOS tasks and queues.
 * - Enum definitions for menus and screen updates.
 * 
 * @author Juan Alberto Serrano Redondo.
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <libraries.h>


// Graphics and Pin Definitions
#define LOAD_GFXFF
#define BRIGHTNESS_PIN 25 ///< Pin for brightness control

// Custom Color Definitions
#define LIGHT_GRAY 0xC618  ///< Light gray color for UI elements
#define LIGHT_BLUE 0xE7DF  ///< Light blue color for UI elements
#define PALE_GRAY 0xE71C   ///< Pale gray color for UI elements
#define MINT_GREEN 0x97F6  ///< Mint green color for UI elements
#define PALE_YELLOW 0xFFFB ///< Pale yellow color for UI elements
#define PALE_PINK 0xFDBC   ///< Pale pink color for UI elements
#define BEIGE 0xFF7B       ///< Beige color for UI elements
#define LAVENDER 0xE5FF    ///< Lavender color for UI elements
#define SKY_BLUE 0x9FFF    ///< Sky blue color for UI elements
#define DARK_ORANGE 0xDBC9 ///< Dark orange color for UI elements
#define DARK_BLUE 0x00B1   ///< Dark blue color for UI elements
#define DARK_GREEN 0x22A2  ///< Dark green color for UI elements

// Timing and Scheduling Constants
extern const uint32_t LOOP_TICKS; ///< Loop period in ticks for RTOS task
extern const unsigned long min_bright_timer; ///< Minimum time before dimming the screen (in milliseconds)
extern const unsigned long turn_off_timer;   ///< Timer for turning off the display (in milliseconds)

// Rotary Encoder Pin Definitions
extern const int DIR_PIN; ///< Direction pin for rotary encoder
extern const int STEP_PIN; ///< Step pin for rotary encoder
extern int DT;            ///< Data pin for rotary encoder
extern int CLK;           ///< Clock pin for rotary encoder

// Rotary Encoder State Variables
extern int lastClk; ///< Last clock state
extern int newClk; ///< Current clock state
extern int dtValue; ///< Data pin value for rotary encoder
extern int counter; ///< Encoder counter to track position
extern int angle; ///< Current angle of the encoder
extern int aState; ///< Current state of the encoder
extern int aLastState; ///< Last state of the encoder
extern long position; ///< Rotary encoder position
extern int lastAngle; ///< Last measured angle

// Display Geometry Variables
extern int centerX; ///< X coordinate for center of display
extern int centerY; ///< Y coordinate for center of display
extern int radius; ///< Radius of UI elements

// Brightness Control Variables
extern const int minBrightness; ///< Minimum brightness level
extern const int maxBrightness; ///< Maximum brightness level
extern const int numBrightnessLevels; ///< Total number of brightness levels
extern int brightnessLevel; ///< Current brightness level

// Enum for Menu States
/**
 * @enum MenuState
 * @brief Defines various states of the menu interface.
 */
enum MenuState {
  MAIN_MENU,        ///< Main menu
  SETTINGS,         ///< Settings menu
  BRIGHTNESS,       ///< Brightness control menu
  COLOR,            ///< Color configuration menu
  TEXT_TYPE,        ///< Text font selection
  BACKGROUND,       ///< Background color selection
  SELECTION_BAR,    ///< Selection bar color
  SELECTION_BORDER, ///< Selection border color
  TEXT,             ///< Text color configuration
  SERVIDOR_TCP,     ///< TCP server configuration
  STEPPER_MOTOR,    ///< Stepper motor control
  MANUAL,           ///< Manual stepper control
  AUTO,             ///< Automatic stepper control
  INITIAL_SCREEN    ///< Initial splash screen
};

extern MenuState currentMenu; ///< Current menu state
extern MenuState previousMenu; ///< Previous menu state

// Menu Structure and Indexes
extern int menuIndex; ///< Index for current menu item
extern const int menuItems; ///< Total number of main menu items
extern String menu[]; ///< Main menu labels
extern const int submenuItems; ///< Total number of submenu items
extern String submenu[]; ///< Submenu labels
extern const int colormenuItems; ///< Number of items in color menu
extern String colormenu[]; ///< Color menu labels
extern const int ethernetmenuItems;
extern String ethernetmenu[]; 
extern const int steppermenuItems;
extern String steppermenu[];
extern const int manualmenuItems;
extern String manualmenu[];
extern const int automenuItems;
extern String automenu[];
// Ethernet Configuration
extern byte mac[];
extern IPAddress ip;
extern EthernetServer server;
extern char ipchar[16];
extern char subnetchar[16];
extern IPAddress subnet;
extern char ipadress;
extern char subnetadress;
extern String ipip;
extern String subnetsubnet;
extern String ethernetInfo[5]; ///< Ethernet settings: IP, Port, Subnet, Status


// Icons for Menus
extern const unsigned short *images_main[]; ///< Main menu icons
extern const unsigned short *images_settings[]; ///< Settings menu icons
extern const unsigned short *images_stepper[]; ///< Stepper motor menu icons

// Screen Control Variables
extern bool screenOn; ///< Flag indicating if the screen is currently on
extern bool engineeringMode; ///< Flag for enabling engineering/debug mode
extern volatile int encoderState; ///< Encoder state for handling inputs
extern RtcDateTime lastDisplayedTime; ///< Time last displayed on the screen

// Queues and Synchronization
extern QueueHandle_t screenUpdateQueue; ///< Queue for handling screen update commands
extern xSemaphoreHandle gatekeeper; ///< Semaphore for managing screen updates

/**
 * @enum ScreenUpdateCommand
 * @brief Enum for screen update commands.
 */
enum ScreenUpdateCommand { 
  UPDATE_MAIN_MENU,            ///< Command to update the main menu
  UPDATE_CONFIG_BRIGHTNESS,    ///< Command to update brightness configuration
  UPDATE_CONFIG_BACKGROUNDCOLOR, ///< Command to update background color
  UPDATE_CONFIG_ASPECT         ///< Command to update UI aspects
};

// Color Arrays for Background, Selection Bar, Border, and Text
extern const uint16_t backgroundpossibleColors[]; ///< Possible background colors
extern const uint16_t selectbarpossibleColors[]; ///< Possible selection bar colors
extern const uint16_t borderpossibleColors[]; ///< Possible border colors
extern const uint16_t textpossibleColors[]; ///< Possible text colors

// Color Count Variables
extern const int backgroundcolors; ///< Number of background colors
extern const int selectbarcolors; ///< Number of selection bar colors
extern const int textcolors; ///< Number of text colors
extern const int bordercolors; ///< Number of border colors
extern const int selectnumcolors[];

// Color Menu Arrays
extern MenuState colormenuArray[]; ///< Color menu states
extern String background[]; ///< Background color labels
extern String selectionbar[]; ///< Selection bar color labels
extern String border[]; ///< Border color labels
extern String text[]; ///< Text color labels

// Selected Colors
extern int selectedBackgroundColor; ///< Selected background color index
extern int selectedTextColor; ///< Selected text color index
extern int selectedBarColor; ///< Selected selection bar color index
extern int selectedBorderColor; ///< Selected border color index
extern int *selection[]; ///< Array for selected colors

// Font Configuration
extern const GFXfont* possibleFont12[]; ///< Array of possible fonts (12-point size)
extern const GFXfont* possibleFont9[]; ///< Array of possible fonts (9-point size)
extern const int numFonts; ///< Number of font options
extern String fontType[]; ///< Font type labels
extern const GFXfont* textType12; ///< Selected 12-point font
extern const GFXfont* textType9; ///< Selected 9-point font

extern BfButton btn;
extern ThreeWire myWire;
extern RtcDS1302<ThreeWire> Rtc;
extern AccelStepper motor;
extern TFT_eSPI tft;
extern TickType_t previousTick;
extern TickType_t previousTime;


#endif // GLOBALS_H
