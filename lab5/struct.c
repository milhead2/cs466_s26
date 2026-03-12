typedef unsigned char uint8_t;

enum {CS_HIGH, CS_LOW, CLK_HIGH, CLK_LOW};
typedef struct {
    // Helper union to access nibbles individually
    union {
        struct {
            uint8_t cs_clk  : 4;  // Bits 0-3
            uint8_t mosi    : 4;  // Bits 4-7
        } nibbles;
        uint8_t encodedByte;
    };
} queueData;

//
// This ISR will fire for any change in CS or CLK
//
void myISRForCSCLK(void)  // Added return type and fixed syntax
{
    uint8_t cs, clk, mosi;
    queueData qd;
    static uint8_t priorCS = 0, priorCLK = 0;  // Added type, initialized
    
    // Get current states (assuming these functions exist)
    cs = getCS();
    clk = getCLK();
    mosi = getMOSI();

    if (cs != priorCS) {
        qd.nibbles.cs_clk = (cs == 0) ? CS_LOW : CS_HIGH;
        qd.nibbles.mosi = 0;  // Clear MOSI
        queueSend(qd);        // Assuming this takes the whole struct
        priorCS = cs;
    }
    else if (clk != priorCLK) {
        qd.nibbles.cs_clk = (clk == 0) ? CLK_LOW : CLK_HIGH;
        qd.nibbles.mosi = mosi;  // Include MOSI value when clock changes
        queueSend(qd);
        priorCLK = clk;
    }
    // No else needed - spurious interrupts can be ignored
}