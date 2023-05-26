#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PORT 5656
#define MAX_MSG 100
#define PACOTE 1024

int enviarArquivo(char * nomeDoArquivo, int clienteSock);

int main(int argc, char const *argv[]) {

  int servidorSock = socket(AF_INET, SOCK_STREAM, 0);
  //Configuração do servidor
  struct sockaddr_in saddr, caddr;
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = htonl(INADDR_ANY);
  saddr.sin_port = htons(PORT);

  int clienteSock, clienteTamanho = sizeof(caddr);//Tamanho do cliente - descritore com as informações

  if (servidorSock == -1){
    printf("Erro ao criar o socket.\n");
    exit(1);
  }
  bind(servidorSock, (struct sockaddr *) &saddr, sizeof saddr); //Conecta o IP com a Porta
  listen(servidorSock, 5); //Limite de conecxão no servidor
  printf("Aguadando......\n");

  /* 
  while (1) {
    cliente = accept(servidor, (struct sockaddr *) &caddr,  &ctamanho); //Faz a conecxão com cliente
    recv(cliente, buffer, sizeof buffer, 0);
    printf("\nServer:\n\tMensagem Recebida: %s\n", buffer);
    close(cliente);
  }*/

  clienteSock = accept(servidorSock, (struct sockaddr *) &caddr,  &clienteTamanho);
  char nomeDoArquivo[100];

  memset(nomeDoArquivo, 0, sizeof(nomeDoArquivo));
  recv(clienteSock, nomeDoArquivo, sizeof(nomeDoArquivo) - 1, 0);
  printf("Nome do arquivo solicitado: %s\n", nomeDoArquivo);

  enviarArquivo(nomeDoArquivo, clienteSock);

  close(clienteSock);
  return 0;
}
int enviarArquivo(char * nomeDoArquivo, int clienteSock){
  FILE * arquivo;
  char nome[300];
  char buffer[PACOTE];
  ssize_t bytesLidos;
  printf("Arquivo: %ld", bytesLidos);
  ///home/renan/Documentos/projetos/servido_cliente/servidor
  sprintf(nome, "%s", nomeDoArquivo);

  arquivo = fopen(nome, "rb");
  if(arquivo == NULL){
    printf("s- Erro ao abrir arquivo\n");
    send(clienteSock, "s2 - Erro ao abrir arquivo\n", 22, 0);
    return 1;
  }
  do {
    send(clienteSock, buffer, bytesLidos, 0);
    printf("%ld\n", bytesLidos);
  }while((bytesLidos = fread(buffer, 1, PACOTE, arquivo)) > 0);

  printf("Arquivo enviado\n");
  fclose(arquivo);
}