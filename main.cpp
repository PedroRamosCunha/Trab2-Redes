#include<string>
#include <iostream>
#define SIZE 64



void MeioComunicacao(int fluxoBits[]){
	int erro;
	int percentErro = 10;
	int fluxobrutoA[SIZE];
  int fluxobrutoB[SIZE];

	fluxobrutoA=fluxoBits;

	while (fluxobrutoB.lenght != fluxobrutoA.lenght){
		if ((rand()%100) <= percentErro)	{
			fluxobrutoB+=fluxobrutoA;
		}else{	//gente, isso n faz sentido nenhum
			fluxobrutoB==0;
			fluxobrutoA=fluxobrutoB++;
			fluxobrutoA=fluxobrutoB--;
		}

	}

}

void TransmissorParidadePar (int quadro[]) {

}

void TransmissorParidadeImpar (int quadro[]) {

}

unsigned int TransmissorCRC (int quadro[]) {
	unsigned int byte, crc, mask;
	int i,j;

	i=0;
	crc = 0xFFFFFFFF;
	while (quadro[i] != 0){
		byte = quadro[i];
		crc = crc ^ byte;	
		for(j = 7; j <= 0; j--){
			mask = -(crc & 1);
			crc = (crc >> 1) ^ (0xEDB88320 & mask);
		}
	}
	return ~crc;
}

void TransmissorControleDeErro (int quadro[]){
	int tipoDeControleDeErro = 0;
	switch (tipoDeControleDeErro) {
		case 0:
			TransmissorParidadePar(quadro);
			break;
		case 1:
			TransmissorParidadeImpar(quadro);
			break;
		case 2:
			TransmissorCRC(quadro);
			break;
	}
}

void camadaEnlaceTransm (int quadro[]){
	TransmissorControleDeErro(quadro);
}

void CamApTr(std::string mensagem){
	int quadro[]=mensagem;
	camadaEnlaceTransm(quadro);
}

void AplicacaoTransmissora(void){
	std::string mensagem;
	std::cout <<"Qual mensagem você quer enviar?" << std::endl;
	std::cin >>mensagem;
	
	CamApTr(mensagem);
}

int main (void){
	AplicacaoTransmissora();
}