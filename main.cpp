#include<string>
#include <iostream>
#define SIZEQUADRO 288
#define TIPODECONTROLE 0

void AplicRecep(std::string mensagem){	//Printa a mensagem recebida
	std::cout <<"A mensagem recebida foi: "<< mensagem << std::endl;
}

void camadaAplicRecep(char quadro[]){	//Converte quadro em mensagem
	std::string mensagem=quadro;

	AplicRecep(mensagem);
}

void camadaEnlacRecep(char quadro[]){ //Recebe o quadro sem o controle de erro
		camadaAplicRecep(quadro);
}

void ReceptorParidadeImpar(char quadro[]){	//Analisa se resposta paridade ímpar possui algum erro
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

void ReceptorParidadePar(char quadro[]){	//Analisa se resposta paridade par possui algum erro
	int i, j;
	int k=0;
	int cont = 0;
	unsigned int resposta;

	for(i=0; i<256*8; i+=4){
		cont=0;
		for(j=i;j<i+4;j++){
			if(quadro[i]==1){
				cont++;
			}
		}
		if(cont%2==0){	//Se for par add 1
			resposta++;
		}
		if(i!=255){
			resposta=(resposta<<1);
		}
	}
	
}

void ReceptorCRC(int quadro[]){
  char CRC[31] = GeradorCRC(quadro[SIZEQUADRO]);
  cont = 0;
  for(int i=0, i<32, i++)
  {
  if(CRC[i]!=quadro[SIZEQUADRO-32+i])
  {cont++}
  }
  if(cont>0) //Caso tenha algum erro cont>0
  {std::cout <<"Erro ao receber a mensagem" << std::endl;}

}

void ReceptorControleDeErro (char quadro[]){	//Recebe o quadro com controle de erro que transmissor enviou
	int tipoDeControleDeErro = TIPODECONTROLE;
	switch (tipoDeControleDeErro) {
		case 0:	//Controle de erro bit de paridade par
			ReceptorParidadePar(quadro);
			break;
		case 1:	//Controle de erro bit de paridade ímpar
			ReceptorParidadeImpar(quadro);
			break;
		case 2:	//Controle de erro CRC
			ReceptorCRC(quadro);
			break;
	}
}

void MeioComunicacao(char fluxoBits[]){
	int chance_erro = 1;
	int i,j;
	int erro;

	for(i=0; i<(SIZEQUADRO/sizeof(int)); i++){
		erro=0;
		for(j=0; j<8; j++){
			if(rand()%1000<chance_erro){//75% chance de não ter nenhum erro
				erro++;
			}
			erro = erro << 1;
		}
		fluxoBits[i] = erro ^ fluxoBits[i];
	}
}

unsigned int TransmissorParidadePar (char quadro[]) {	//Cálculo para paridade par
	int i, j;
	int k=0;
	int cont = 0;
	unsigned int resposta;

	for(i=0; i<256*8; i+=4){
		cont=0;
		for(j=i;j<i+4;j++){
			if(quadro[i]==1){
				cont++;
			}
		}
		if(cont%2==0){	//Se for par add 1
			resposta++;
		}
		if(i!=255){
			resposta=(resposta<<1);
		}
	}
	return resposta;
}

unsigned int TransmissorParidadeImpar (char quadro[]) {	//Cálculo para paridade ímpar
	int i, j;
	int k=0;
	int cont = 0;
	unsigned int resposta;

	for(i=0; i<256*8; i+=4){
		cont=0;
		for(j=i;j<i+4;j++){
			if(quadro[i]==1){
				cont++;
			}
		}
		if(cont%2==1){	//Se for ímpar add 1
			resposta++;
		}
		if(i!=255){
			resposta=(resposta<<1);
		}
	}
	return resposta;
}

unsigned int TransmissorCRC (char quadro[]) {
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
		i++;
	}
	return ~crc;
}

void TransmissorControleDeErro (char quadro[]){
	int tipoDeControleDeErro = TIPODECONTROLE;
	int len = SIZEQUADRO/8;
	unsigned int res;
	switch (tipoDeControleDeErro) {		//Vê qual o controle de erro feito
		case 0:	//Controle de erro de bit de paridade par
			res = TransmissorParidadePar(quadro);
			break;
		case 1:	//Controle de erro de bit de paridade ímpar
			res = TransmissorParidadeImpar(quadro);
			break;
		case 2:	//Controle de erro CRC
			res = TransmissorCRC(quadro);
			break;
	}

	quadro[len-4] += res >> 8*3;
	quadro[len-3] += res >> 8*2;
	quadro[len-2] += res >> 8;
	quadro[len-1] += res;
}

void camadaEnlaceTransm (char quadro[]){	//Camada de enlace chama a função que gera o controle de erros
	TransmissorControleDeErro(quadro);
}

void CamApTr(std::string mensagem){
	int chars_available = (SIZEQUADRO-32)/8;
	int len = mensagem.length();
	char quadro[SIZEQUADRO/8];
	int chars_left = len;
	int i,j;

	
	for(i=0; chars_left>0; i++){
		for(j=0; j<chars_available && j<chars_left; j++){
			quadro[j] = mensagem[(i*chars_available)+j];
		}
		
		camadaEnlaceTransm(quadro);

		chars_left-=chars_available;
		for(j=0; j<(SIZEQUADRO/8); j++){
			quadro[j]=0;
		}
	}
}

void AplicacaoTransmissora(void){
	std::string mensagem;
	std::cout <<"Qual mensagem você quer enviar?" << std::endl;
	std::cin >>mensagem;
	
	CamApTr(mensagem);
}

int main (void){
	AplicacaoTransmissora();

	return 0;
}