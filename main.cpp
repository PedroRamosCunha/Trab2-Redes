#include<string>
#include <iostream>
#define SIZEQUADRO 256
#define TIPODECONTROLE 0;

void camadaAplicRecep(int quadro[]){
	std::string mensagem=quadro[];

}

void camadaEnlacRecep(int quadro[]){
		camadaAplicRecep(quadro);
}

void ReceptorParidadeImpar(int quadro[]){
	int cont = 0;
	int resposta;

	for (int i=0; i<SIZEQUADRO; i++){
		if(quadro[i]==1){
			cont++;
		}
	}
	if((cont%2==0 && quadro[SIZEQUADRO]==1) || (cont%2==1 && quadro[SIZEQUADRO]==0)){	//Se mensagem foi recebida corretamente
		camadaEnlacRecep(quadro);
	}else{	//se ocorreu um erro na transmissão
		std::cout <<"Erro ao receber a mensagem" << std::endl;
	}
}

void ReceptorParidadePar(int quadro[]){
	int cont = 0;
	int resposta;

	for (int i=0; i<SIZEQUADRO; i++){
		if(quadro[i]==1){
			cont++;
		}
	}
	if((cont%2==0 && quadro[SIZEQUADRO]==0) || (cont%2==1 && quadro[SIZEQUADRO]==1)){	//Se mensagem foi recebida corretamente
		camadaEnlacRecep(quadro);
	}else{	//se ocorreu um erro na transmissão
		std::cout <<"Erro ao receber a mensagem" << std::endl;
	}
}

void ReceptorControleDeErro (int quadro[]){
	int tipoDeControleDeErro = TIPODECONTROLE;
	switch (tipoDeControleDeErro) {
		case 0:
			ReceptorParidadePar(quadro);
			break;
		case 1:
			ReceptorParidadeImpar(quadro);
			break;
		case 2:
			ReceptorCRC(quadro);
			break;
	}
}

void MeioComunicacao(int fluxoBits[]){
	int percentErro = 5;
	int i,j;
	int erro;

	for(i=0; i<(SIZEQUADRO/sizeof(int)); i++){
		erro=0;
		for(j=0; j<8; j++){
			if(rand()%100<percentErro){
				erro++;
			}
			erro = erro << 1;
		}
		fluxoBits[i] = erro ^ fluxoBits[i];
	}
}

void TransmissorParidadePar (int quadro[]) {
	int cont = 0;
	int resposta;

	for (int i=0; i<SIZEQUADRO; i++){
		if(quadro[i]==1){
			cont++;
		}
	}
	if(cont%2==0){	//Se for par
		quadro[SIZEQUADRO]=0;
	}else{	//se for ímpar
		quadro[SIZEQUADRO]=1;
	}
}

void TransmissorParidadeImpar (int quadro[]) {
	int cont = 0;
	int resposta;

	for (int i=0; i<SIZEQUADRO; i++){	//for para ver se quantidade de bits 1 é par ou ímpar
		if(quadro[i]==1){
			cont++;
		}
	}
	if(cont%2==0){	//Se for par
		quadro[SIZEQUADRO]=1;
	}else{	//se for ímpar
		quadro[SIZEQUADRO]=0;
	}
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
	int tipoDeControleDeErro = TIPODECONTROLE;
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
	int len = mensagem.length();
	int 
	
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