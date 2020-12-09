

void TransmissorParidadePar (int quadro[]) {

}

void TransmissorParidadeImpar (int quadro[]) {

}

void TransmisorCRC (int quadro[]) {
	int i,j;
	unsigned int byte, crc, mask;

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

}

void TransmissorControleDeErro (int quadro[]){
	int tipoDeControleDeErro = 0;
	switch (tipoDeControleDeErro) {
		case 0:
			//codigo
			break;
		case 1:
			//codigo
			break;
		case 2:
			//codigo
			break;
	}
}

void camadaEnlaceTransm (int quadro[]){
	TransmissorControleDeErro(quadro);
}

void CamApTr(string mensagem){
	int quadro[]=mensagem;
	camadaEnlaceTransm(quadro);
}

void AplicacaoTransmissora(void){
	string mensagem;
	cout <<"Qual mensagem você quer enviar?"<< endl;
	cin >>mensagem;
	
	CamApTr(mensagem);
}

void main (void){
	AplicacaoTransmissora();
}