#define PIN_VOLUME_UP 5
#define PIN_VOLUME_DOWN 6
#define COL_1 7
#define ROWS 2
#define COLS 1
#define UP 0
#define DOWN 1
#define REP 2
#define COUNT_BEFORE_REPEAT 3

uint8_t buf[8]          = {0}; /* Keyboard report buffer */
uint8_t state_buf[2][1] = {UP};

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
        for (int col = 0; col < COLS; col++) {
            bool reading = digitalRead(col + 2);
            delay(500);
            if (state_buf[row][col] == DOWN) {
                Serial.write("Row: ");
                Serial.write(65 + row);
                Serial.write(" PRESSED ");
            } else if (state_buf[row][col] == REP) {
                Serial.write("Row: ");
                Serial.write(65 + row);
                Serial.write(" REPEAT ");
            } else if (state_buf[row][col] == UP) {
                Serial.write("Row: ");
                Serial.write(65 + row);
                Serial.write(" UP ");
            } else {
                Serial.write("Row: ");
                Serial.write(65 + row);
                Serial.write(" HELD ");
            }

            if (reading) {
                if (state_buf[row][col] == UP) {
                    // Push down only the just pushed button
                    state_buf[row][col] = DOWN;
                } else if (state_buf[row][col] == DOWN) {
                    state_buf[row][col] = REP + COUNT_BEFORE_REPEAT;
                } else if (state_buf[row][col] > REP) {
                    state_buf[row][col]--;
                }
            } else {
                state_buf[row][col] = UP;
            }
            Serial.write(" - Repeat count: ");
            Serial.write(48 - REP + state_buf[row][col]);
            Serial.write(" - ");
        }
    }
    Serial.write("\n");
}

void releaseKey() {
    buf[0] = 0;
    buf[2] = 0;
    Serial.write(buf, 8); // Release key
}
