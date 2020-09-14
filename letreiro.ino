/*
Letreiro UsinaInfo - Impressão e rolagem das palavras da direta para a esquerda

Projeto programando uma Matriz de LED's 8x8 com 16 pinos do Arduino
Criado por Flávio Babos (https://flaviobabos.com.br/arduino/)

*/

// Inclua a biblioteca que está no artigo
// Ela será usada para executar uma rotina de interrupção em uma frequência especificada constantemente.
// Além disso, ela irá atualizar a exibição do display de forma ininterrupta e sem a interferência do loop.
#include <FrequencyTimer2.h>

//Definir as matrizes correspondentes a cada palavra e ao espaço (intervalo)
#define intervalo { \
{0, 0, 0, 0, 0, 0, 0, 0}, \
{0, 0, 0, 0, 0, 0, 0, 0}, \
{0, 0, 0, 0, 0, 0, 0, 0}, \
{0, 0, 0, 0, 0, 0, 0, 0}, \
{0, 0, 0, 0, 0, 0, 0, 0}, \
{0, 0, 0, 0, 0, 0, 0, 0}, \
{0, 0, 0, 0, 0, 0, 0, 0}, \
{0, 0, 0, 0, 0, 0, 0, 0} \
}

#define U { \
{0, 1, 0, 0, 0, 0, 1, 0}, \
{0, 1, 0, 0, 0, 0, 1, 0}, \
{0, 1, 0, 0, 0, 0, 1, 0}, \
{0, 1, 0, 0, 0, 0, 1, 0}, \
{0, 1, 0, 0, 0, 0, 1, 0}, \
{0, 1, 0, 0, 0, 0, 1, 0}, \
{0, 1, 0, 0, 0, 0, 1, 0}, \
{0, 0, 1, 1, 1, 1, 0, 0} \
}

#define S { \
{0, 1, 1, 1, 1, 1, 1, 0}, \
{0, 1, 0, 0, 0, 0, 0, 0}, \
{0, 1, 0, 0, 0, 0, 0, 0}, \
{0, 1, 1, 1, 1, 1, 1, 0}, \
{0, 0, 0, 0, 0, 0, 1, 0}, \
{0, 0, 0, 0, 0, 0, 1, 0}, \
{0, 0, 0, 0, 0, 0, 1, 0}, \
{0, 1, 1, 1, 1, 1, 1, 0} \
}

#define I { \
{0, 0, 1, 1, 1, 1, 0, 0}, \
{0, 0, 0, 1, 1, 0, 0, 0}, \
{0, 0, 0, 1, 1, 0, 0, 0}, \
{0, 0, 0, 1, 1, 0, 0, 0}, \
{0, 0, 0, 1, 1, 0, 0, 0}, \
{0, 0, 0, 1, 1, 0, 0, 0}, \
{0, 0, 0, 1, 1, 0, 0, 0}, \
{0, 0, 1, 1, 1, 1, 0, 0} \
}

#define N { \
{0, 1, 1, 0, 0, 1, 0, 0}, \
{0, 1, 1, 0, 0, 1, 0, 0}, \
{0, 1, 1, 1, 0, 1, 0, 0}, \
{0, 1, 1, 1, 1, 1, 0, 0}, \
{0, 1, 1, 0, 1, 1, 0, 0}, \
{0, 1, 1, 0, 0, 1, 0, 0}, \
{0, 1, 1, 0, 0, 1, 0, 0}, \
{0, 1, 1, 0, 0, 1, 0, 0} \
}

#define A { \
{0, 0, 0, 1, 1, 0, 0, 0}, \
{0, 0, 1, 0, 0, 1, 0, 0}, \
{0, 0, 1, 0, 0, 1, 0, 0}, \
{0, 0, 1, 0, 0, 1, 0, 0}, \
{0, 0, 1, 1, 1, 1,0 , 0}, \
{0, 0, 1, 0, 0, 1, 0, 0}, \
{0, 0, 1, 0, 0, 1, 0, 0}, \
{0, 0, 1, 0, 0, 1, 0, 0} \
}

#define F { \
{0, 1, 1, 1, 1, 1, 0, 0}, \
{0, 1, 0, 0, 0, 0, 0, 0}, \
{0, 1, 0, 0, 0, 0, 0, 0}, \
{0, 1, 1, 1, 0, 0, 0, 0}, \
{0, 1, 0, 0, 0, 0, 0, 0}, \
{0, 1, 0, 0, 0, 0, 0, 0}, \
{0, 1, 0, 0, 0, 0, 0, 0}, \
{0, 1, 0, 0, 0, 0, 0, 0} \
}

#define O { \
{0, 0, 1, 1, 1, 1, 0, 0}, \
{0, 1, 0, 0, 0, 0, 1, 0}, \
{0, 1, 0, 0, 0, 0, 1, 0}, \
{0, 1, 0, 0, 0, 0, 1, 0}, \
{0, 1, 0, 0, 0, 0, 1, 0}, \
{0, 1, 0, 0, 0, 0, 1, 0}, \
{0, 1, 0, 0, 0, 0, 1, 0}, \
{0, 0, 1, 1, 1, 1, 0, 0} \
}

byte col = 0;

// variável byte para a matriz de leds 8 linhas por 8 colunas
byte leds[8][8];

// vetor que relaciona as portas do Arduino às saídas do display
// 14 - 19 é a conversão em portas digitais das portas analógicas A0 - A5
int pinos[17] = { 0, 5, 4, 3, 2, 14, 15, 16, 17, 13, 12, 11, 10, 9, 8, 7, 6 };

// vetor de 8 inteiros que são as portas digitais que representam as colunas
int colunas[8] = {9, 3, 2, 12, 15, 11, 7, 6};

// vetor de 8 inteiros que são as portas digitais que representam as linhas
int linhas[8] = {13, 8, 17, 10, 5, 16, 4, 14};

// variável de leitura que representa a quantidade de termos
const int termos = 11;

// imprimindo no painel
byte padroes[termos][8][8] = { U, S, I, N, A, intervalo, I, N, F, O, intervalo};

int padrao = 0;

void setup() {

// definindo a porta do potenciômetro
pinMode(A4, INPUT);

// loop for para percorrer os pinos e identifica-los como OUTPUT
for (int i = 1; i <= 16; i++) {
pinMode(pinos[i], OUTPUT);
}

// definindo rotina de interrupção a ser chamada
FrequencyTimer2::setOnOverflow(display);

setpadrao(padrao);
}

void loop() {

padrao = ++padrao % termos;

// executa o movimento dos caracteres e controla sua velocidade
slidepadrao(padrao, analogRead(A4));
}


void setpadrao(int padrao) {

// percorre as linhas 'i' e as colunas 'j' uma por uma
for (int i = 0; i < 8; i++) {
for (int j = 0; j < 8; j++) {
leds[i][j] = padroes[padrao][i][j];
}
}
}

void slidepadrao(int padrao, const int del) {

// permite movimentar os caracteres da direita para a esquerda
for (int l = 0; l < 8; l++) {
for (int i = 0; i < 8; i++) {
for (int j = 0; j < 8; j++) {

// incremento de colunas representadas aqui por 'i'
// isso permite o movimento horizontal dos caracteres
leds[j][i] = leds[j][i + 1];
}
}

for (int j = 0; j < 8; j++) {
leds[j][7] = padroes[padrao][j][0 + l];
}

delay(del);
}
}

// função ininterrupta da biblioteca
void display() {

// apaga toda a coluna anterior
digitalWrite(colunas[col], HIGH);
col++;
if (col == 8) {
col = 0;
}
for (int linha = 0; linha < 8; linha++) {
if (leds[col][7 - linha] == 1) {

// liga este led
digitalWrite(linhas[linha], HIGH);
}
else {

// apaga este led
digitalWrite(linhas[linha], LOW);
}
}

// liga a coluna inteira de uma só vez (para tempos de iluminação iguais)
digitalWrite(colunas[col], LOW);
delay(analogRead(A4));
}
