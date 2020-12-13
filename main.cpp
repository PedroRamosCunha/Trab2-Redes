#include<string>
#include <iostream>
#include <time.h>
#define SIZEQUADRO 288
#define TIPODECONTROLE 0


unsigned int resultadoParidade(char quadro[SIZEQUADRO / 8]) {
	unsigned int res = 0;
	int cont, i, j;
	
	//Calcula 1 bit de verificacao para cada char, total de 32 bits de paridade
	for (i = 0;i < (SIZEQUADRO - 32) / 8;i++) {
		cont = 0;
		for (j = 0; j < 8;j++) {
			if (((quadro[i] >> j) % 2) == 1) {
				cont++;
			}
		}
		if (cont % 2 == 0) {	//Se for par add 1
			res++;
		}
		if (i != 31) {
			res = (res << 1);
		}
	}
	return res;
}


void AplicRecep(std::string mensagem) {	//Printa a mensagem recebida
	std::cout << "A mensagem recebida foi:" << std::endl;

	for (int i = 0; i < (SIZEQUADRO-32)/8; i++){
		std::cout << mensagem[i];
	}
	std::cout << "\n";
}

void camadaAplicRecep(char quadro[SIZEQUADRO / 8]) {	//Converte quadro em mensagem
	std::string mensagem = quadro;

	mensagem.resize((SIZEQUADRO - 32) / 8);//Remove os bits de verificação

	AplicRecep(mensagem);
}


int ReceptorParidadeImpar(char quadro[SIZEQUADRO / 8]) {	//Analisa se resposta paridade ímpar possui algum erro
	//Faz o calculo dos bits de paridade par e inverte o resultado
	unsigned int resposta = ~resultadoParidade(quadro);
	unsigned int comparacao = 0;
	int len = SIZEQUADRO / 8;
	int i, j;

	//Copia os bits de verificação em comparacao
	for (i = 4; i > 0; i--) {
		for (j = 7; j >= 0; j--) {
			comparacao = comparacao << 1;
			if ((quadro[len - i] >> j) % 2 == 1 || (quadro[len - i] >> j) % 2 == -1) {
				comparacao++;
			}
		}
	}

	if (resposta != comparacao) {	//Se é diferente, então a mensagem veio com erro
		std::cout << "Mensagem recebida não está de acordo com a mensagem enviada!" << std::endl;
		return -1;	//Retorno que indica que houve erro
	}
	return 0;	//retorno que indica que mensagem foi recebida sem erros
}

int ReceptorParidadePar(char quadro[SIZEQUADRO / 8]) {	//Analisa se resposta paridade par possui algum erro
	//Faz o calculo dos bits de paridade par
	unsigned int resposta = resultadoParidade(quadro);
	unsigned int comparacao = 0;
	int len = SIZEQUADRO / 8;
	int i, j;

	//Copia os bits de verificação em comparacao
	for (i = 4; i > 0; i--) {
		for (j = 7; j >= 0; j--) {
			comparacao = comparacao << 1;
			if ((quadro[len - i] >> j) % 2 == 1 || (quadro[len - i] >> j) % 2 == -1) {
				comparacao++;
			}
		}
	}

	if (resposta != comparacao) {	//Se é diferente, então a mensagem veio com erro
		std::cout << "Mensagem recebida não está de acordo com a mensagem enviada!" << std::endl;
		return -1;	//Retorno que indica que houve erro
	}
	return 0;	//retorno que indica que mensagem foi recebida sem erros
}

unsigned int CalcCRC(char quadro[SIZEQUADRO / 8]) {
	unsigned int res, poly_crc;
	int i, j, pos;

	//Quadro -> Dividendo 256 bits msg + 32 bits 0
	
	poly_crc = 0x82608EDB;//Polinomio de 33 bits sem o ultimo bit 1, para caber em unsigned int
	res = 0;//Resto
	
	//Copia os primeiros quatro bytes para o res
	for (i = 0; i < 4; i++) {
		for (j = 7; j >= 0; j--) {
			res = res << 1;
			if ((quadro[i] >> j) % 2 != 0) {
				res++;
			}
		}
	}

	//Faz a divisão de polinomio
	for(pos = 255; pos >= 0; pos--){
		if(res >> 31 == 1){
			res = res ^ poly_crc;
		}
		res = res << 1;
		
		//pos > 32 ignora os bits de verificacao

		//Como o ultimo bit do polinomio eh 1,
		//o resultado do xor sera 1 apenas se o bit na mensagem for 0
		if(pos > 32 && (quadro[(SIZEQUADRO - pos)/8] >> ((SIZEQUADRO - pos)%8))%2 == 0){
			res++;
		}
	}

	return res;
}

int ReceptorCRC(char quadro[SIZEQUADRO / 8]) {
	int len = SIZEQUADRO / 8;
	unsigned int crc = 0;
	int i, j;

	//Copia os bits de verificação em CRC
	for (i = 4; i > 0; i--) {
		for (j = 7; j >= 0; j--) {
			crc = crc << 1;
			if ((quadro[len - i] >> j) % 2 != 0) {
				crc++;
			}
		}
	}

	if (crc == CalcCRC(quadro)) {//Sem erro
		return 0;
	}
	else {//Com erro
		std::cout << "Mensagem recebida não está de acordo com a mensagem enviada!" << std::endl;
		return -1;
	}
}

void ReceptorControleDeErro(char quadro[SIZEQUADRO / 8]) {	//Recebe o quadro com controle de erro que transmissor enviou
	int tipoDeControleDeErro = TIPODECONTROLE;
	int res;
	switch (tipoDeControleDeErro) {
	case 0:	//Controle de erro bit de paridade par
		res = ReceptorParidadePar(quadro);
		break;
	case 1:	//Controle de erro bit de paridade ímpar
		res = ReceptorParidadeImpar(quadro);
		break;
	case 2:	//Controle de erro CRC
		res = ReceptorCRC(quadro);
		break;
	}
}

void camadaEnlacRecep(char quadro[SIZEQUADRO / 8]) {
	ReceptorControleDeErro(quadro);	//Chama a função que analisa se houve um erro

	camadaAplicRecep(quadro);	//Chama a camada de aplicação da receptora
}

void MeioComunicacao(char fluxoBits[]) {
	int chance_erro = 2;
	int i, j;
	char erro;

	for (i = 0; i < (SIZEQUADRO / 8); i++) {
		erro = 0;
		for (j = 0; j < 8; j++) {
			if ((rand() % 1000) < chance_erro) {//60% chance de não ter nenhum erro
				std::cout << "Erro no bit " << 8*i+j << "\n";
				erro++;
			}
			erro = erro << 1;
		}
		fluxoBits[i] = erro ^ fluxoBits[i];
	}

	camadaEnlacRecep(fluxoBits);
}

unsigned int TransmissorParidadePar(char quadro[SIZEQUADRO / 8]) {	//Cálculo para paridade par
	unsigned int resposta = resultadoParidade(quadro);
	return resposta;
}

unsigned int TransmissorParidadeImpar(char quadro[SIZEQUADRO / 8]) {	//Cálculo para paridade ímpar
	unsigned int resposta = resultadoParidade(quadro);
	return ~resposta;
}

unsigned int TransmissorCRC(char quadro[SIZEQUADRO / 8]) {
	return CalcCRC(quadro);
}

void TransmissorControleDeErro(char quadro[SIZEQUADRO / 8]) {
	int tipoDeControleDeErro = TIPODECONTROLE;
	int len = SIZEQUADRO / 8;
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

	quadro[len - 4] = res >> 24;
	quadro[len - 3] = (res >> 16) % 256;
	quadro[len - 2] = (res >> 8) % 256;
	quadro[len - 1] = res % 256;
}

void camadaEnlaceTransm(char quadro[SIZEQUADRO / 8]) {	//Camada de enlace chama a função que gera o controle de erros
	TransmissorControleDeErro(quadro);
	MeioComunicacao(quadro);
}

void CamadaAplicacaoTransm(std::string mensagem) {
	int chars_available = (SIZEQUADRO - 32) / 8;
	int len = mensagem.length();
	char quadro[SIZEQUADRO / 8];
	int chars_left = len;
	int i, j;

	for (j = 0; j < (SIZEQUADRO / 8); j++) {	//Seta todos os bits do quadro como 0
		quadro[j] = 0;
	}

	for (i = 0; chars_left > 0; i++) {	//Passar a strint mensagem para quadro
	//Caso a mensagem tenha mais de 32 char, ela é dividida em várias e mandada em pacotes de 32 char
		for (j = 0; j < chars_available && j < chars_left; j++) {
			quadro[j] = mensagem[(i * chars_available) + j];
		}

		camadaEnlaceTransm(quadro);

		chars_left -= chars_available;
		for (j = 0; j < (SIZEQUADRO / 8); j++) {
			quadro[j] = 0;
		}
	}
}

void AplicacaoTransmissora() {
	std::string mensagem;
	std::cout << "Qual mensagem você quer enviar?" << std::endl;
	std::getline(std::cin, mensagem);	//Forma de receber a mensagem com espaços
	std::cout<<"\n";
	CamadaAplicacaoTransm(mensagem);
}

int main(void) {
  srand (time(NULL));
	AplicacaoTransmissora();

	return 0;
}