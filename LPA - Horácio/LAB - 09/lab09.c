#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const **argv){
    FILE* mpg_input = fopen(argv[1],"rb"); // Abre o arquivo de entrada
    FILE *mpg_saida_atual = fopen("video_parte_1.mpg","wb");     //Abre o primeiro arquivo de saída

    int max_tam_bytes = atoi(argv[2]) * 1024 * 1024;

    int tam_buffer = 0;
    int tam_arq = 0;
    
    unsigned char* buffer_sequence = (unsigned char*)malloc(max_tam_bytes * sizeof(char));  //Aloca memória para um buffer (tamanho passado no argv[2] convertido para bytes). 
    unsigned char readed_code[4];
    int cont = 1;

    printf("Criando arquivo video_parte_1.mpg\n");
    while(true){
        fread(readed_code,4,1,mpg_input); //Lê 4 bytes
        if (memcmp(readed_code,"\x00\x00\x01\xB3",4)!=0 && !feof(mpg_input)){     //Se não for código de Sequence
            
            buffer_sequence[tam_buffer] = readed_code[0];   //Salve o primeiro byte lido no buffer
            tam_buffer++;
            fseek(mpg_input,-3,SEEK_CUR);   //Retorna três bytes
        }
        else{
            if( tam_buffer + tam_arq > max_tam_bytes){ // Se o tamanho do arquivo de saída atual mais o tamanho do buffer
                fclose(mpg_saida_atual); // Fecha o arquivo de saída atual       // for maior que o tamanho máximo
                char new_string[20] = "";      
                sprintf(new_string,"%s%d%s","video_parte_",++cont,".mpg");      //Crie uma string para ter o nome do novo arquivo de saída
                mpg_saida_atual = fopen(new_string,"wb"); //Abra o novo arquivo de saída
                fwrite(buffer_sequence,sizeof(char),tam_buffer,mpg_saida_atual); //Escreva todo o buffer nele 
                tam_arq = tam_buffer; //Sete o tamanho do arquivo atual para o tamanho do buffer recém-salvo
                tam_buffer = 0;
                printf("Criando arquivo %s\n",new_string);
            }
            else{   // Se não (o tamanho ainda é menor)
                fwrite(buffer_sequence,sizeof(char),tam_buffer,mpg_saida_atual);   // Salve o buffer lido no arquivo já aberto (fwrite)
                tam_arq += tam_buffer;
                tam_buffer = 0;
            }

            if(feof(mpg_input)!=0){    //Se for o final do arquivo de entrada
                fclose(mpg_saida_atual);
                break;      //Saia do loop
            }

            //Se chegou aqui, então nós lemos os quatro bytes do código Sequence e não retornamos os três bytes.
            memcpy(buffer_sequence, "\x00\x00\x01\xB3", 4);
            tam_buffer = 4;

        }
    }
    free(buffer_sequence);
    fclose(mpg_input);
}