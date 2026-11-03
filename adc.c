#include <xc.h>
#include "adc.h"

void adc_init(void)
{
    // Desliga o ADC e limpa suas config de controle
    AD1CON1 = 0;
    AD1CON2 = 0;
    AD1CON3 = 0;
    
    // Configura RB0 como entrada e habilita a função analógica AN0
    TRISBbits.TRISB0 = 1;
    AD1PCFGbits.PCFG0 = 0;
    
    // Seleciona AN0 como entrada positiva e VREF- como entrada negativa
    AD1CHS = 0;
    
    // O contador interno encerra a amostragem e inicia a conversão
    AD1CON1bits.SSRC = 0b111;
    
    // Define o clock do ADC e o tempo de amostragem
    AD1CON3bits.ADCS = 2;
    AD1CON3bits.SAMC = 31;
    
    // Habilita o módulo ADC
    AD1CON1bits.ADON = 1;
}

uint16_t adc_read(void)
{
    // Inicia a amostragem do sinal em AN0.
    AD1CON1bits.SAMP = 1;

    // Aguarda a conversão terminar.
    while (!AD1CON1bits.DONE);

    // Retorna o valor convertido.
    return ADC1BUF0;
}
