#define PIN_VOLUME_UP 5
#define PIN_VOLUME_DOWN 6
#define COL_1 7
#define ROWS 2
#define COLS 1
#define UP 0
#define DOWN 1
#define REPEAT 2
#define COUNT_BEFORE_REPEAT 3

uint8_t buf[8]          = {0}; /* Keyboard report buffer */
uint8_t state_buf[2][1] = {UP};
uint8_t repeat_count    = COUNT_BEFORE_REPEAT;

void setup() {
    Serial.begin(9600);
    DDRD = 0;   // Set pins 0 to 7 as input
    DDRB = 0;   // Set pins 8 to 13 as input
    DDRC = 255; // Set analog pins 0 to 5 as output

    PORTC = 0; // Initialize output port to LOW
    PORTD = 0;
    PORTB = 0;
    delay(200);
}

void loop() {
    for (int row = 0; row < ROWS; row++) {
        PORTC = 1 << row; // Turn on one output at a time in sequence
        for (int col = 2; col < 2 + COLS; col++) {
            bool reading          = digitalRead(col);
            uint8_t* button_state = &state_buf[row][col];

            if (*button_state == DOWN && repeat_count == COUNT_BEFORE_REPEAT) {
                Serial.write("PRESSED\n");
            } else if (*button_state == REPEAT) {
                Serial.write("REPEAT\n");
            } else {
                Serial.write("UP\n");
            }

            if (reading) {
                if (repeat_count == 0) {
                    *button_state = REPEAT;
                } else {
                    *button_state = DOWN;
                    repeat_count--;
                }
            } else {
                *button_state = UP;
                repeat_count  = COUNT_BEFORE_REPEAT;
            }
            delay(500);
        }
    }
}

void releaseKey() {
    buf[0] = 0;
    buf[2] = 0;
    Serial.write(buf, 8); // Release key
}
