#define ROWS 2
#define COLS 1
#define UP 0
#define DOWN 1
#define MAX_KEY_COUNT 6

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

void erase_buffer() {
    for (int i = 0; i < 8; i ++) {
        buf[i] = 0;
    }
}

void loop() {
    erase_buffer();

    uint8_t key_counter = 0;
    for (int row = 0; row < ROWS && key_counter < MAX_KEY_COUNT; row++) {
        PORTC = 1 << row; // Turn on one output at a time in sequence
        for (int col = 0; col < COLS && key_counter < MAX_KEY_COUNT; col++) {
            bool reading = digitalRead(col + 2);
            delay(100);
            if (reading) {
                state_buf[row][col] = DOWN;
            } else {
                state_buf[row][col] = UP;
            }
            if (state_buf[row][col] == DOWN) {
                // Serial.write("Key: ");
                // Serial.write(65 + row);
                // Serial.write(" DOWN ");
                buf[2 + key_counter] = 4 + row;
                key_counter++;
            } else {
                // Serial.write("Key: ");
                // Serial.write(65 + row);
                // Serial.write(" UP ");
            }
        }
    }
    Serial.write(buf, 8);
    // Serial.write("- Key counter");
    // Serial.write(48 + key_counter);
    // Serial.write("\n");
}
