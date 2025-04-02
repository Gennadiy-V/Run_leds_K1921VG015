openocd -f ft232h.cfg  -f k1921vg015.cfg -c "program Run_leds.bin 0x80000000 verify reset exit"





pause