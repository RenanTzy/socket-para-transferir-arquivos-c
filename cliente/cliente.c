#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PORT 5656
#define PACOTE 512


char *frasePalavra();
void receberArquivo(char * nomeDoArquivo, int clienteSock);

int main(int argc, char const *argv[]) {

  int clienteSock = socket(AF_INET, SOCK_STREAM, 0);

  //Configuração para conecxão no servidor
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //IP do servidor
  server_addr.sin_port = htons(PORT); //Porta do servidor
 
  while(1){
  connect(clienteSock, (struct sockaddr *) &server_addr, sizeof server_addr);
  char nomeDoArquivo[100];
  printf("Nome do arquivo para ser recebido: ");
  scanf("%s", nomeDoArquivo);
  if(nomeDoArquivo == "Sair" || nomeDoArquivo == "sair"){
  	close(clienteSock);
  }
  send(clienteSock, nomeDoArquivo, strlen(nomeDoArquivo), 0);
  receberArquivo(nomeDoArquivo, clienteSock);
  }
  return 0;
}
void receberArquivo(char * nomeDoArquivo, int clienteSock){
  FILE * arquivo;
  char buffer[PACOTE];
  ssize_t bytesLidos;

  arquivo = fopen(nomeDoArquivo, "wb");
  if(arquivo == NULL){
    printf("C - Erro ao criar o arquivo\n");
    exit(1);
  }

  do{
    fwrite(buffer, 1, bytesLidos, arquivo);
  }while((bytesLidos = recv(clienteSock, buffer, PACOTE, 0))>0);

  fclose(arquivo);
}

char *frasePalavra(){
  char * nome = (char *)malloc(4 * sizeof(char));
  char caracter;
  int pos = 0;
  if (nome == NULL){
      printf("Erro  ao alocar o vetor");
      return 1;
  }
  do{
      caracter = getchar();
      nome[pos++] = caracter;
  }while( caracter != '\n');

  nome[pos - 1] = '\0';

  return nome;
}
