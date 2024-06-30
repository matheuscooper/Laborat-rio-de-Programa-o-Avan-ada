#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//Matheus Soouza de Oliveira 22153438

int main(int argc, char const **argv){
    FILE* mpg_input = fopen(argv[1],"rb");
    FILE *mpg_saida_atual = fopen("video_parte_1.mpg","wb");     

    int max_tam_bytes = atoi(argv[2]) * 1024 * 1024;

    int tam_buffer = 0;
    int tam_arq = 0;
    
    unsigned char* buffer_sequence = (unsigned char*)malloc(max_tam_bytes * sizeof(char)); 
    unsigned char readed_code[4];
    int cont = 1;

    printf("Criando Arquivo Video_Parte_1.mpg\n");
    while(true){
        fread(readed_code,4,1,mpg_input); 
        if (memcmp(readed_code,"\x00\x00\x01\xB3",4)!=0 && !feof(mpg_input)){   
            
            buffer_sequence[tam_buffer] = readed_code[0];  
            tam_buffer++;
            fseek(mpg_input,-3,SEEK_CUR);   
        }
        else{
            if( tam_buffer + tam_arq > max_tam_bytes){ 
                fclose(mpg_saida_atual); 
                char new_string[20] = "";      
                sprintf(new_string,"%s%d%s","video_parte_",++cont,".mpg");   
                mpg_saida_atual = fopen(new_string,"wb");
                fwrite(buffer_sequence,sizeof(char),tam_buffer,mpg_saida_atual); 
                tam_arq = tam_buffer; 
                tam_buffer = 0;
                printf("Criando Arquivo %s\n",new_string);
            }
            else{  
                fwrite(buffer_sequence,sizeof(char),tam_buffer,mpg_saida_atual);   
                tam_buffer = 0;
            }

            if(feof(mpg_input)!=0){    
                fclose(mpg_saida_atual);
                break;     
            }

            
            memcpy(buffer_sequence, "\x00\x00\x01\xB3", 4);
            tam_buffer = 4;

        }
    }
    free(buffer_sequence);
    fclose(mpg_input);
}