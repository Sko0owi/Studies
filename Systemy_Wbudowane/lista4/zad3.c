#include <avr/io.h>
#include <util/delay.h>

#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>

#define BAUD 9600   //baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1) //zgodnie ze wzorem



#define R_REG OCR1B
#define G_REG OCR1A
#define B_REG OCR2A



typedef struct RgbColor
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} RgbColor;

typedef struct HsvColor
{
    uint8_t hue;
    uint8_t saturation;
    uint8_t value;
} HsvColor;

RgbColor HsvToRgb(HsvColor hsv)
{
    RgbColor rgb;
    unsigned char region, remainder, p, q, t;
    
    if (hsv.saturation == 0)
    {
        rgb.red = hsv.value;
        rgb.green = hsv.value;
        rgb.blue = hsv.value;
        return rgb;
    }
    
    region = hsv.hue / 43;
    remainder = (hsv.hue - (region * 43)) * 6; 
    
    p = (hsv.value * (255 - hsv.saturation)) >> 8;
    q = (hsv.value * (255 - ((hsv.saturation * remainder) >> 8))) >> 8;
    t = (hsv.value * (255 - ((hsv.saturation * (255 - remainder)) >> 8))) >> 8;
    
    switch (region)
    {
        case 0:
            rgb.red = hsv.value; rgb.green = t; rgb.blue = p;
            break;
        case 1:
            rgb.red = q; rgb.green = hsv.value; rgb.blue = p;
            break;
        case 2:
            rgb.red = p; rgb.green = hsv.value; rgb.blue = t;
            break;
        case 3:
            rgb.red = p; rgb.green = q; rgb.blue = hsv.value;
            break;
        case 4:
            rgb.red = t; rgb.green = p; rgb.blue = hsv.value;
            break;
        default:
            rgb.red = hsv.value; rgb.green = p; rgb.blue = q;
            break;
    }
    
    return rgb;
}



// inicjalizacja UART
void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // wyczyść rejestr UCSR0A
  UCSR0A = 0;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

// transmisja jednego znaku
int uart_transmit(char data, FILE *stream)
{
  // czekaj aż transmiter gotowy
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0)));
  return UDR0;
}

FILE uart_file;


//inicjalizowanie ADC
void adc_init(){

    ADMUX = _BV(REFS0);

    DIDR0 = _BV(ADC0D);

    ADCSRA = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2)   ;
    ADCSRA |= _BV(ADEN);
}



void my_delay_us(uint16_t value){
    while(value){
        _delay_us(1);
        value--;
    }
}

void timer_init()
{

        // setup 
    
    // t1
    ICR1 = 10624;
    TCCR1A = _BV(COM1A1) | _BV(COM1A0) | _BV(WGM10);
    TCCR1B = _BV(WGM12) | _BV(CS00);

    // t2
    TCCR1A |= _BV(COM1B1) | _BV(COM1B0);

    // t3
    TCCR2A |= _BV(COM2A0) | _BV(COM2A1) | _BV(WGM21) | _BV(WGM20);
    TCCR2B |= _BV(CS20);


    // set ddr for the pins
    DDRB |= _BV(PB1) | _BV(PB2) | _BV(PB3);

}

void myDelay(int ms){
    while(ms--)
        _delay_ms(1);
}




int main(){
    // UCSR0B |= _BV(RXEN0) | _BV(TXEN0);

    uart_init();
    fdev_setup_stream(&uart_file,uart_transmit,uart_receive,_FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;
    adc_init();
    timer_init();

    RgbColor rgb_color;
    HsvColor hsv_color;

    hsv_color.hue = 140;
    hsv_color.saturation = 160;
    hsv_color.value = 0;

    bool breathe_in = 1;

    while(1)
    {
        
        if(breathe_in)
            hsv_color.value += 1;
        else
            hsv_color.value -= 1;

        if(hsv_color.value >= 130) breathe_in = false;
        else if(hsv_color.value <= 1)
        {
            breathe_in = true;
            hsv_color.hue = rand()%240;
            hsv_color.saturation = 200 + rand()%50;
        }  

        
        
        rgb_color = HsvToRgb(hsv_color);

        R_REG = rgb_color.red;
        G_REG = rgb_color.green;
        B_REG = rgb_color.blue;

        uint8_t delay = 0;

        if(hsv_color.value < 60){
            delay = 10 - (hsv_color.value / 6);
            delay *= delay;
        }

        myDelay(10 + delay);     
    }
}