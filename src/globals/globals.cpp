/**
 * @file globals.cpp
 * @brief This file contains global variables, constants, and configurations used throughout the project.
 * @author Juan Alberto Serrano Redondo.
 */
#include "globals.h"
#include <libraries.h>
/** @brief Timing and Scheduling Constants */
const uint32_t LOOP_TICKS = 50 / portTICK_PERIOD_MS; ///< Loop period in ticks for RTOS task
const unsigned long min_bright_timer = 180000; ///< Minimum time before dimming the screen (in milliseconds)
const unsigned long turn_off_timer = 600000;   ///< Timer for turning off the display (in milliseconds)

/** @brief Rotary Encoder Pin Definitions */
const int DIR_PIN = 15; ///< Direction pin for rotary encoder
const int STEP_PIN = 2; ///< Step pin for rotary encoder
int DT = 16;            ///< Data pin for rotary encoder
int CLK = 17;           ///< Clock pin for rotary encoder

/** @brief Rotary Encoder State Variables */
int lastClk = HIGH; ///< Last clock state
int newClk = digitalRead(CLK); ///< Current clock state
int dtValue = digitalRead(DT); ///< Data pin value for rotary encoder
int counter = 0; ///< Encoder counter to track position
int angle = 0; ///< Current angle of the encoder
int aState; ///< Current state of the encoder
int aLastState; ///< Last state of the encoder
long position = 0; ///< Rotary encoder position
int lastAngle = 0; ///< Last measured angle

/** @brief Display Geometry Variables */
int centerX = 80; ///< X coordinate for center of display
int centerY = 74; ///< Y coordinate for center of display
int radius = 40; ///< Radius of UI elements

/** @brief Brightness Control Variables */
const int minBrightness = 200; ///< Minimum brightness level
const int maxBrightness = 230; ///< Maximum brightness level
const int numBrightnessLevels = 10; ///< Total number of brightness levels
int brightnessLevel = 5; ///< Current brightness level


/**
 * @brief MenuState enum values represent different menu screens
 */
MenuState currentMenu = INITIAL_SCREEN; ///< Current menu state
MenuState previousMenu = MAIN_MENU; ///< Previous menu state

/** @brief Menu Structure and Indexes */
int menuIndex = 0; ///< Index for current menu item
const int menuItems = 4; ///< Total number of main menu items
String menu[menuItems] = {"MAIN MENU","Settings","TCP server","Stepper motor"}; ///< Main menu labels
const int submenuItems = 4; ///< Total number of submenu items
String submenu[submenuItems] = {"SETTINGS","Brightness","Colors","Font type"}; ///< Submenu labels
const int colormenuItems = 5; ///< Number of items in color menu
String colormenu[colormenuItems] = {"COLORS","Background","Selection bar","Selection border","Text"}; ///< Color menu labels
const int ethernetmenuItems = 5;
String ethernetmenu[ethernetmenuItems] = {"IP","Port","Subnet","Status","Back"}; 
const int steppermenuItems = 3;
String steppermenu[steppermenuItems] = {"STEPPER","Automatic", "Manual"};
const int manualmenuItems = 3;
String manualmenu[manualmenuItems] = {"MANUAL","Forward","Backward"};
const int automenuItems = 1;
String automenu[automenuItems] = {"AUTO"};
/** @brief Ethernet Configuration */
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

char ipchar[16];
char subnetchar[16];
IPAddress subnet = Ethernet.subnetMask();
char ipadress = sprintf(ipchar, "%d.%d.%d.%d", ip[0],ip[1],ip[2],ip[3]);
char subnetadress = sprintf(subnetchar, "%d.%d.%d.%d", subnet[0],subnet[1],subnet[2],subnet[3]);
String ipip = ipchar;
String subnetsubnet = subnetchar;
String ethernetInfo[5] = {ipip, "80", subnetsubnet, ""}; ///< Ethernet settings: IP, Port, Subnet, Status


/** @brief Icons for Menus */
const unsigned short *images_main[] = {NULL, settings_icon, ethernet_icon, step_icon}; ///< Main menu icons
const unsigned short *images_settings[] = {NULL, brightness_icon, color_icon, font_icon}; ///< Settings menu icons
const unsigned short *images_stepper[] = {NULL, automatic_icon, manual_icon}; ///< Stepper motor menu icons

/** @brief Screen Control Variables */
bool screenOn = true; ///< Flag indicating if the screen is currently on
bool engineeringMode = false; ///< Flag for enabling engineering/debug mode
volatile int encoderState = 0; ///< Encoder state for handling inputs
RtcDateTime lastDisplayedTime; ///< Time last displayed on the screen

/** @brief Queues and Synchronization */
QueueHandle_t screenUpdateQueue; ///< Queue for handling screen update commands
xSemaphoreHandle gatekeeper = 0; ///< Semaphore for managing screen updates



/** @brief Color Arrays for Background, Selection Bar, Border, and Text */
const uint16_t backgroundpossibleColors[] = {MINT_GREEN, TFT_WHITE, LIGHT_BLUE, PALE_GRAY, PALE_YELLOW, PALE_PINK, BEIGE, LAVENDER, SKY_BLUE}; ///< Possible background colors
const uint16_t selectbarpossibleColors[] = {TFT_RED, TFT_BLUE, TFT_YELLOW, TFT_PINK, TFT_ORANGE, TFT_CYAN, TFT_SKYBLUE, TFT_VIOLET}; ///< Possible selection bar colors
const uint16_t borderpossibleColors[] = {TFT_BLACK, TFT_RED, TFT_GREENYELLOW, TFT_BLUE, TFT_YELLOW, TFT_PINK, TFT_ORANGE, TFT_CYAN, TFT_LIGHTGREY, TFT_SKYBLUE, TFT_VIOLET}; ///< Possible border colors
const uint16_t textpossibleColors[] = {TFT_BLACK, DARK_ORANGE, DARK_BLUE, DARK_GREEN}; ///< Possible text colors

/** @brief Color Count Variables */
const int backgroundcolors = 9; ///< Number of background colors
const int selectbarcolors = 8; ///< Number of selection bar colors
const int textcolors = 4; ///< Number of text colors
const int bordercolors = 11; ///< Number of border colors
const int selectnumcolors[] ={backgroundcolors,selectbarcolors,textcolors,bordercolors};

/** @brief Color Menu Arrays */
MenuState colormenuArray[] = {MenuState::SELECTION_BAR, MenuState::BACKGROUND, MenuState::TEXT, MenuState::SELECTION_BORDER}; ///< Color menu states
String background[backgroundcolors] = {"Mint","White","Light blue", "Pale gray", "Yellow","Pale pink","Beige", "Lavender","Skyblue"}; ///< Background color labels
String selectionbar[selectbarcolors] = {"Red", "Blue", "Yellow","Pink", "Orange","Cyan", "Skyblue","Violet"}; ///< Selection bar color labels
String border[bordercolors] = {"Black","Red","Green","Blue","Yellow","Pink","Orange","Cyan","Light grey","Skyblue","Violet"}; ///< Border color labels
String text[textcolors] = {"Black","Orange","Dark blue","Dark green"}; ///< Text color labels

/** @brief Selected Colors */
int selectedBackgroundColor = 1; ///< Selected background color index
int selectedTextColor = 0; ///< Selected text color index
int selectedBarColor = 5; ///< Selected selection bar color index
int selectedBorderColor = 0; ///< Selected border color index
int *selection[] = {NULL, &selectedBackgroundColor, &selectedBarColor, &selectedBorderColor, &selectedTextColor}; ///< Array for selected colors

/** @brief Font Configuration */
const GFXfont* possibleFont12[] = {NULL, sansserif12, mono12, arimo12, serif12}; ///< Array of possible fonts (12-point size)
const GFXfont* possibleFont9[] = {NULL, sansserif9, mono9, arimo9, serif9}; ///< Array of possible fonts (9-point size)
const int numFonts = sizeof(possibleFont12) / sizeof(possibleFont12[0]); ///< Number of font options
String fontType[numFonts] = {"TEXT FONT", "SansSerif", "Monospaced", "Arimo", "Serif"}; ///< Font type labels
const GFXfont* textType12 = possibleFont12[4]; ///< Selected 12-point font
const GFXfont* textType9 = possibleFont9[4]; ///< Selected 9-point font


/** @brief Hardware Initialization */
TFT_eSPI tft = TFT_eSPI();
ThreeWire myWire(21, 22, 4);
TickType_t previousTick = 0;
TickType_t previousTime = 0;
IPAddress ip(192, 168, 1, 200);
RtcDS1302<ThreeWire> Rtc(myWire);
BfButton btn(BfButton::STANDALONE_DIGITAL, 5, true, LOW);
EthernetServer server(80);
AccelStepper motor(AccelStepper::DRIVER, 2, 15);