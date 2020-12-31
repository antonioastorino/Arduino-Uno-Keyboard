#define ROWS 2
#define COLS 1
#define UP 0
#define DOWN 1

uint8_t buf[8]       = {0}; /* Keyboard report buffer */
bool state_buf[2][1] = {UP};

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
    buf[2] = 0;
    for (int row = 0; row < ROWS; row++) {
        PORTC = 1 << row; // Turn on one output at a time in sequence
        for (int col = 0; col < COLS; col++) {
            bool reading = digitalRead(col + 2);
            delay(100);
            if (state_buf[row][col] == DOWN) {
                // Serial.write("Key: ");
                // Serial.write(65 + row);
                // Serial.write(" DOWN ");
                buf[2] = 4 + row;
            } else {
                // Serial.write("Key: ");
                // Serial.write(65 + row);
                // Serial.write(" UP ");
            }

            if (reading) {
                state_buf[row][col] = DOWN;
            } else {
                state_buf[row][col] = UP;
            }
        }
    }
    Serial.write(buf, 8);
    // Serial.write("\n");
}

void releaseKey() {
    buf[0] = 0;
    buf[2] = 0;
    Serial.write(buf, 8); // Release key
}
