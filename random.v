`timescale 1ns / 1ps

module random(
    input clk100,
    input clk75,
    input clk50,
    input clk25,
    input clk10,
    input button,
    input comSwitch,
    output reg[4:0] leds = 0);
    
    always @(posedge clk10) begin
        if (button & comSwitch) begin
            leds[0] <= !leds[0];
        end
    end
    
    always @(posedge clk25) begin
        if (button & comSwitch)
            leds[1] <= !leds[1];
    end
    
    always @(posedge clk50) begin
        if (button & comSwitch)
            leds[2] <= !leds[2];
    end
    
    always @(posedge clk75) begin
        if (button & comSwitch)
            leds[3] <= !leds[3];
    end
    
    always @(posedge clk100) begin
        if (button & comSwitch)
            leds[4] <= !leds[4];
    end
endmodule
