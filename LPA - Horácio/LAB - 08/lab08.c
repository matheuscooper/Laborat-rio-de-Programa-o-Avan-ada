#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Matheus Souza de Oliveira
//Matrícula: 22153438

int main(int argc, char *argv[]) {

    FILE *mpg_file = fopen(argv[1], "rb");
    if (!mpg_file) {
        fprintf(stderr, "Erro ao abrir o arquivo\n");
        return 1;
    }
    unsigned char vetChar[4];
    while(fread(vetChar, sizeof(unsigned char), 4, mpg_file)==4){
        if(vetChar[0] == 0x00 && vetChar[1] == 0x00 && vetChar[2] == 0x01){
            unsigned char codHex = vetChar[3];
            printf("--> Código %.2x --", codHex);
            if( codHex == 0xBA){
                printf("Pack\n");
            } 
            else if( codHex == 0xBB){
                printf("System\n");
            }
            else if( codHex == 0xB8){
                printf("Group of Pictures\n");
            }
            
            else if( codHex >= 0x01 && codHex <= 0xAF){
                printf("Slice\n");
            }
            else if( codHex >= 0xC0 && codHex <= 0xDF){
                printf("Packet Video\n");
            }
            else if( codHex >= 0xE0 && codHex <= 0xEF){
                printf("Packet Audio\n");
            }
            else if(codHex == 0xB3){
                unsigned char byte1 = fgetc(mpg_file);
                unsigned char byte2 = fgetc(mpg_file);
                unsigned char byte3 = fgetc(mpg_file);
                unsigned char byte4 = fgetc(mpg_file);
                unsigned int largura = byte1 * 16 + (byte2 >> 4);
                unsigned int altura = ( byte2 & 0x0F) * 256 + byte3;
                unsigned int frame_rate = byte4 & 0x0f;

                if( frame_rate == 1){
                    printf("Sequence Header -- Width = %d, Height = %d -- Frame rate =  23.976fps\n", largura, altura);
                }
                else if( frame_rate == 2){
                    printf("Sequence Header -- Width = %d, Height = %d -- Frame rate =  24.000fps\n", largura, altura);
                }
                else if( frame_rate == 3){
                     printf("Sequence Header -- Width = %d, Height = %d -- Frame rate = 25.000fps\n", largura, altura);
                }
                else if( frame_rate == 4){
                     printf("Sequence Header -- Width = %d, Height = %d -- Frame rate = 29.970fps\n", largura, altura);
                }
                else if( frame_rate == 5){
                     printf("Sequence Header -- Width = %d, Height = %d -- Frame rate = 30.000fps\n", largura, altura);
                }
                else if( frame_rate == 6){
                     printf("Sequence Header -- Width = %d, Height = %d -- Frame rate = 50.000fps\n", largura, altura);
                }
                else if( frame_rate == 7){
                     printf("Sequence Header -- Width = %d, Height = %d -- Frame rate = 59.940fps\n", largura, altura);
                }
                else if( frame_rate == 8){
                     printf("Sequence Header -- Width = %d, Height = %d -- Frame rate = 60.000fps\n", largura, altura);
                }
             }
            else if( codHex == 0x00){
                unsigned char byte1 = fgetc(mpg_file);
                unsigned char byte2 = fgetc(mpg_file);
                unsigned int tipo = (byte2 >> 3) & 0x07;
                if( tipo == 1){
                    printf(" Picture -- Tipo = I\n");
                }
                else if( tipo == 2){
                    printf(" Picture -- Tipo = P\n");
                }
                else if( tipo == 3){
                    printf(" Picture -- Tipo = B\n");
                }
            }
            else {
                printf("--> Código:%.2x -- Tipo de stream não implementado\n", codHex); 
                }
        }    
    }
}
