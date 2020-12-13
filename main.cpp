#include<string>
#include <iostream>
#define SIZEQUADRO 288
#define TIPODECONTROLE 0


unsigned int resultadoParidade(char quadro[SIZEQUADRO / 8]) {
	unsigned int res = 0;
	int cont, i, j;
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
}

void camadaAplicRecep(char quadro[SIZEQUADRO / 8]) {	//Converte quadro em mensagem
	std::string mensagem = quadro;

	mensagem.resize((SIZEQUADRO - 32) / 8);//Remove os bits de verificação

	AplicRecep(mensagem);
}


int ReceptorParidadeImpar(char quadro[SIZEQUADRO / 8]) {	//Analisa se resposta paridade ímpar possui algum erro
	unsigned int resposta = ~resultadoParidade(quadro);
	unsigned int comparacao = 0;
	int len = SIZEQUADRO / 8;
	int i, j;

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
	unsigned int resposta = resultadoParidade(quadro);
	unsigned int comparacao = 0;
	int len = SIZEQUADRO / 8;
	int i, j;

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
	unsigned int byte, crc, mask;
	int i, j;

	i = 0;
	crc = 0xFFFFFFFF;
	while (quadro[i] != 0) {
		byte = quadro[i];
		crc = crc ^ byte;
		for (j = 7; j >= 0; j--) {
			mask = crc & 1;
			crc = (crc >> 1) ^ (0xEDB88320 & mask);
		}
		i++;
	}
	return ~crc;
}

int ReceptorCRC(char quadro[SIZEQUADRO / 8]) {
	int len = SIZEQUADRO / 8;
	unsigned int crc = 0;
	int i, j;

	for (i = 4; i > 0; i--) {
		for (j = 7; j >= 0; j--) {
			crc = crc << 1;
			if ((quadro[len - i] >> j) % 2 == 1 || (quadro[len - i] >> j) % 2 == -1) {
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

	std::cout << "32 bits escritos: " << res << std::endl;



	quadro[len - 4] = res >> 24;
	quadro[len - 3] = (res >> 16) % 256;
	quadro[len - 2] = (res >> 8) % 256;
	quadro[len - 1] = res % 256;

	std::cout << (res >> 24) << " " << (res >> 16) % 256 << " " << (res >> 6) % 256 << " " << (res % 256) << std::endl;

}

void camadaEnlaceTransm(char quadro[SIZEQUADRO / 8]) {	//Camada de enlace chama a função que gera o controle de erros
	TransmissorControleDeErro(quadro);
	MeioComunicacao(quadro);
}

void CamApTr(std::string mensagem) {
	int chars_available = (SIZEQUADRO - 32) / 8;
	int len = mensagem.length();
	char quadro[SIZEQUADRO / 8];
	int chars_left = len;
	int i, j;

	for (j = 0; j < (SIZEQUADRO / 8); j++) {
		quadro[j] = 0;
	}

	for (i = 0; chars_left > 0; i++) {
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
	std::cin >> mensagem;

	CamApTr(mensagem);
}

int main(void) {
	AplicacaoTransmissora();

	return 0;
}