#define ROWS 2
#define COLS 2
#define UP 0
#define DOWN 1
#define MAX_KEY_COUNT 6

#define KEY_MOD_LCTRL 0x01
#define KEY_MOD_LSHIFT 0x02
#define KEY_MOD_LALT 0x04
#define KEY_MOD_LMETA 0x08
#define KEY_MOD_RCTRL 0x10
#define KEY_MOD_RSHIFT 0x20
#define KEY_MOD_RALT 0x40
#define KEY_MOD_RMETA 0x80
#define KEY_A 0x04          // Keyboard a and A
#define KEY_B 0x05          // Keyboard b and B
#define KEY_C 0x06          // Keyboard c and C
#define KEY_D 0x07          // Keyboard d and D
#define KEY_E 0x08          // Keyboard e and E
#define KEY_F 0x09          // Keyboard f and F
#define KEY_G 0x0a          // Keyboard g and G
#define KEY_H 0x0b          // Keyboard h and H
#define KEY_I 0x0c          // Keyboard i and I
#define KEY_J 0x0d          // Keyboard j and J
#define KEY_K 0x0e          // Keyboard k and K
#define KEY_L 0x0f          // Keyboard l and L
#define KEY_M 0x10          // Keyboard m and M
#define KEY_N 0x11          // Keyboard n and N
#define KEY_O 0x12          // Keyboard o and O
#define KEY_P 0x13          // Keyboard p and P
#define KEY_Q 0x14          // Keyboard q and Q
#define KEY_R 0x15          // Keyboard r and R
#define KEY_S 0x16          // Keyboard s and S
#define KEY_T 0x17          // Keyboard t and T
#define KEY_U 0x18          // Keyboard u and U
#define KEY_V 0x19          // Keyboard v and V
#define KEY_W 0x1a          // Keyboard w and W
#define KEY_X 0x1b          // Keyboard x and X
#define KEY_Y 0x1c          // Keyboard y and Y
#define KEY_Z 0x1d          // Keyboard z and Z
#define KEY_1 0x1e          // Keyboard 1 and !
#define KEY_2 0x1f          // Keyboard 2 and @
#define KEY_3 0x20          // Keyboard 3 and #
#define KEY_4 0x21          // Keyboard 4 and $
#define KEY_5 0x22          // Keyboard 5 and %
#define KEY_6 0x23          // Keyboard 6 and ^
#define KEY_7 0x24          // Keyboard 7 and &
#define KEY_8 0x25          // Keyboard 8 and *
#define KEY_9 0x26          // Keyboard 9 and (
#define KEY_0 0x27          // Keyboard 0 and )
#define KEY_ENTER 0x28      // Keyboard Return (ENTER)
#define KEY_ESC 0x29        // Keyboard ESCAPE
#define KEY_BACKSPACE 0x2a  // Keyboard DELETE (Backspace)
#define KEY_TAB 0x2b        // Keyboard Tab
#define KEY_SPACE 0x2c      // Keyboard Spacebar
#define KEY_MINUS 0x2d      // Keyboard - and _
#define KEY_EQUAL 0x2e      // Keyboard = and +
#define KEY_LEFTBRACE 0x2f  // Keyboard [ and {
#define KEY_RIGHTBRACE 0x30 // Keyboard ] and }
#define KEY_BACKSLASH 0x31  // Keyboard \ and |
#define KEY_HASHTILDE 0x32  // Keyboard Non-US # and ~
#define KEY_SEMICOLON 0x33  // Keyboard ; and :
#define KEY_APOSTROPHE 0x34 // Keyboard ' and "
#define KEY_GRAVE 0x35      // Keyboard ` and ~
#define KEY_COMMA 0x36      // Keyboard , and <
#define KEY_DOT 0x37        // Keyboard . and >
#define KEY_SLASH 0x38      // Keyboard / and ?
#define KEY_CAPSLOCK 0x39   // Keyboard Caps Lock
#define KEY_F1 0x3a         // Keyboard F1
#define KEY_F2 0x3b         // Keyboard F2
#define KEY_F3 0x3c         // Keyboard F3
#define KEY_F4 0x3d         // Keyboard F4
#define KEY_F5 0x3e         // Keyboard F5
#define KEY_F6 0x3f         // Keyboard F6
#define KEY_F7 0x40         // Keyboard F7
#define KEY_F8 0x41         // Keyboard F8
#define KEY_F9 0x42         // Keyboard F9
#define KEY_F10 0x43        // Keyboard F10
#define KEY_F11 0x44        // Keyboard F11
#define KEY_F12 0x45        // Keyboard F12

uint8_t buf[8]             = {0}; /* Keyboard report buffer */
bool state_buf[ROWS][COLS] = {UP};
// NOTE: Rows and cols are swapped here to make this array more readable.
uint8_t button_map[COLS][ROWS] = {{KEY_Q, KEY_W}, {KEY_A, KEY_S}};

void setup() {
    Serial.begin(9600);
    DDRB = 0;   // Set pins 8 to 13 as input
    DDRC = 255; // Set analog pins 0 to 5 as output
    DDRD = 0;   // Set pins 0 to 7 as input

    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    delay(200);
}

void erase_buffer() {
    for (int i = 0; i < 8; i++) { buf[i] = 0; }
}

void update_buffer() {
    uint8_t key_counter = 0;
    for (int row = 0; row < ROWS; row++) {
        PORTC = 1 << row; // Turn on one output at a time in sequence
        delay(5);
        for (int col = 0; col < COLS; col++) {
            bool reading = digitalRead(col + 2);
            if (reading) {
                state_buf[row][col] = DOWN;
            } else {
                state_buf[row][col] = UP;
            }
            if (state_buf[row][col] == DOWN) {
                // NOTE: Cols and rows are swapped here on purpose
                uint8_t key_code     = button_map[col][row];
                buf[2 + key_counter] = key_code;
                // Serial.write(61 + key_code);
                if (key_counter < MAX_KEY_COUNT) {
                    key_counter++;
                } else {
                    return;
                }
            }
        }
    }
}

void loop() {
    erase_buffer();
    update_buffer();
    Serial.write(buf, 8);
}
