/*
* Centro Universitário de João Pessoa, 2018.
* Projeto de programação: TETRIS USANDO A LINGUAGEM C E A BIBLIOTECA ALLEGRO.
*
* Arthur Vasconcellos <contatoarthursilveira@gmail.com>
* 	github.com/arthursvpb
* 
* Lucca Barbosa <luccab73@gmail.com>
* 	github.com/LuccaBM73
*/

/***
 *    _________ _______ _________ _______ _________ _______ 
 *    \__   __/(  ____ \\__   __/(  ____ )\__   __/(  ____ \
 *       ) (   | (    \/   ) (   | (    )|   ) (   | (    \/
 *       | |   | (__       | |   | (____)|   | |   | (_____ 
 *       | |   |  __)      | |   |     __)   | |   (_____  )
 *       | |   | (         | |   | (\ (      | |         ) |
 *       | |   | (____/\   | |   | ) \ \_____) (___/\____) |
 *       )_(   (_______/   )_(   |/   \__/\_______/\_______)
 *                                                          
 ***/

/* CÓDIGO PRINCIPAL */

#include <allegro.h> // Biblioteca principal para o funcionamento do jogo.
#include "ranking.h" // Biblioteca para salvar o RANKING em arquivo txt.
#define BUFFERSIZE 128 // BUFFERSIZE para o input do nome do jogdor.

// FUNÇÕES
int texto();
void sair();
int  timer();
void girar();
int  confere();
void control();
void criaForma();
void apagaLinha();
void menu(BITMAP *_title1, BITMAP *_title2);	

// OBJETOS
struct Partes{int x, y;} parte_a[4], parte_b[4], parte_aux;
	
// FECHA JANELA (X)
int sai = 0;
	
// TAMANHOS CONSTANTES
const int COL = 10, WIDTH = 660;
const int LIN = 20, HEIGHT = 600;
const int WDT = 25, NPART = 4;

// VARIÁVEIS	
int i, j;
int cor = 0;
int cont = 0;
int conf = 0;
int giro = 0;
int gover = 0;
int passo = 0;
int lines = 0;
int nTimer = 6;
int pos_x = 0;
int giroKey = 0;
int fundoX = 140;
int fundoY = 106;
int keyRanking = 0;
	
// NOVA PEÇA
int newPart = 0; 
int nextPartCor = 1;
int nextPartX[4] = {0};
int nextPartY[4] = {0};
	
// POSIÇÃO MUDA A FORMA (0 - 6)	
int nForma = 0; 
int grade[20][10] = {0};
	
// PEÇAS
int forma[7][4] =
		{1,3,5,7,   // I (0)
		 2,4,5,7,   // N (1)
		 3,5,4,6,   // N ESPELHADO (2)
		 3,5,4,7,   // T (3)
		 2,3,5,7,   // L ESPELHADO (4)
		 3,5,7,6,   // L (5)
		 2,3,4,5};  // QUADRADO (6)
	
// BITMAPS
BITMAP *buffer, *img, *title1, *title2, *gover1, *gover2;
		
// FUNÇÃO PRINCIPAL 
int main()
{
	allegro_init();// Inicia o allegro
	install_keyboard();// Funções para teclado
	install_timer();// Relógio
	set_color_depth(32);// Profundidade de cores
	set_close_button_callback(sair);// Botão de sair
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, WIDTH, HEIGHT, 0, 0);// Resolução da tela
	set_window_title("TETRIS EM C - ARTHUR E LUCCA"); // Titulo do programa
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL); // Função para som
	
	// VARIÁVEIS LOCAIS
	buffer = create_bitmap(WIDTH, HEIGHT);
	img = load_bitmap("imagem.bmp", NULL);
	title1 = load_bitmap("title1.bmp", NULL);
	title2 = load_bitmap("title2.bmp", NULL);
	gover1 = load_bitmap("gover1.bmp", NULL);
	gover2 = load_bitmap("gover2.bmp", NULL);
	
	
	// DECLARA A TOCA O SOM
	MIDI *midi = load_midi("midi.mid"); // Background
	play_midi(midi, 1);
	
	// IGNORAR PRIMEIRO QUADRADO (BUG)
	for(i = 0; i < NPART; i++) parte_a[i].x = -LIN; 
	
	// CHAMA O MENU PRINCIPAL
	menu(title1, title2);
	
	// Cria elementos aleatórios sem repetir
	while (key[KEY_ENTER] || key[KEY_SPACE]) rand () % 1;
	
	// GERADOR DE FORMAS
	criaForma();
	
	// LOOP ONDE SE ENCONTRA O JOGO
	while (!(sai || key[KEY_ESC]))
	{
		
		// FUNÇÕES PARA O JOGO
		control();
		girar();
		timer();
		apagaLinha();
		clear(buffer);
		
		// BACKGROUND
		masked_blit(img, buffer, 0, WDT, 0, 0, WIDTH, HEIGHT);
		
		// LINHAS (OBS: USAR A VARIÁVEL LINES PARA CRIAR ARQUIVO COM RANKING)
		textprintf_ex(buffer, font, 430, 200, 0, -1, "  %i", lines);
		
		// RECORTAR E COLAR PEÇA ATUAL NO CHÃO
		for(i = 0; i < LIN; i++){
			for(j = 0; j < COL; j++){
				if(grade[i][j]){
					masked_blit(img, buffer, grade[i][j]*WDT, 0, j*WDT + fundoX, i*WDT + fundoY, WDT, WDT);
				}
			}
		}
		
		// DESENHA PEÇAS
		for(i = 0; i< NPART; i++)
		{
			masked_blit(img, buffer, cor*WDT, 0, parte_a[i].x*WDT + fundoX, parte_a[i].y*WDT + fundoY, WDT, WDT); // primeira peça
			masked_blit(img, buffer, nextPartCor*WDT, 0, nextPartX[i]*WDT + 446, nextPartY[i]*WDT + 264, WDT, WDT); // próxima peça
		}
		draw_sprite(screen, buffer, 0, 0);
		rest(100);
		pos_x = 0; giro = 0;
		
		// GAME OVER
		if (gover){
			rest(1000);
			// Escreve o ranking no arquivo txt
			menu(gover1, gover2);
			
			texto(); // Input para o nome do jogador
					
			salva_ranking(lines); // Pontuação final
			
			gover = 0;
			for (i = 0; i < LIN; i++){
				for (j = 0; j < COL; j++){
					grade[i][j] = 0;
					// 
					lines = 0;
				}
			}
		}
	}
	
	// FINALIZAÇÃO DE BITMAP (RECURSO DO ALLEGRO)
	destroy_bitmap(buffer);
	destroy_bitmap(img);
	destroy_bitmap(title1);
	destroy_bitmap(title2);
	destroy_bitmap(gover1);
	destroy_bitmap(gover2);
	destroy_midi(midi);
	
	return 0;
}

END_OF_MAIN();

// A biblioteca allegro não tem a função "scanf" do C, portanto tive que pesquisar uma função que atue de forma manual para obtenção do texto digitado pelo usuário.
int texto() {

  char    edittext[BUFFERSIZE];
  int     caret  = 0;

  do
  {
     if(keypressed())
     {
        int  newkey   = readkey();
        char ASCII    = newkey & 0xff;
        char scancode = newkey >> 8;
        
        /* Uma peça foi pressionada, adicione-a à string. */
        if(ASCII >= 32 && ASCII <= 126)
        {
           if(caret < BUFFERSIZE - 1)
           {
              edittext[caret] = ASCII;
              caret++;
              edittext[caret] = '\0';
           }
        }
        else if(scancode == KEY_BACKSPACE)
        {
           if (caret > 0) caret--;
           edittext[caret] = '\0';
        }
     }
     
     /*DESENHOS*/
     clear(buffer);
     textout(buffer, font, "DIGITE SEU NOME PARA SALVAR NO RANKING: ", 0, 10, makecol(255, 255, 255));
     textout(buffer, font, edittext, 0, 50, makecol(255, 255, 255));
     vline(buffer, caret * 8, 8, 18, makecol(255, 255, 255));
     blit(buffer, screen, 0, 0, 150, 350, 320, 240);
  }
  
  while(!key[KEY_UP]); // Confirmação do nome do usuário
  
  // Escreve o nome do usuário no arquivo TXT quando confirmado.
  FILE *f = fopen("ranking.txt", "a");
	if (f == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}
	
	fprintf(f,"%s: ", edittext); 
	
	fclose(f);
  
  return 0;
}

// FUNÇÃO MENU PRINCIPAL
void menu(BITMAP *_title1, BITMAP *_title2){
	static int keyMenu = 0;
	while (!(sai || key[KEY_ENTER] || key[KEY_SPACE])){
			if(key[KEY_ESC]) sai = 1;
			
			if(keyMenu++ > 20) keyMenu = 0;
			if(keyMenu > 10){
			
			if (key[KEY_1]) {system("ranking.txt");} // Chama o ranking
			if (key[KEY_2]) {system("creditos.txt");} // Chama os créditos
				
			if (_title1) masked_blit(_title1, buffer, 0, 0, 0, 0, WIDTH, HEIGHT);
			}else if(_title2) masked_blit(_title2, buffer, 0, 0, 0, 0, WIDTH, HEIGHT);	
			
			draw_sprite(screen, buffer, 0, 0);
			
			rest(80);
	}		
}

// FUNÇÃO APAGAR A LINHA
void apagaLinha(){
	conf = LIN - 1; // lendo de trás para frente
	for (i = conf; i > 0; i--){
		cont = 0;
		for(j = 0; j < COL; j++){
			if(grade[i][j]) cont++;
			grade[conf][j] = grade [i][j];
		}
		if(cont < COL) conf--;
		else{
			lines++;
		}
	}
}

// FUNÇÃO GIRAR
void girar(){
	if(giro && cor != 7){
		parte_aux = parte_a[1]; // Centro
		for(i = 0; i < NPART; i++){
			// Guarda posição invertida
			int	x = parte_a[i].y - parte_aux.y;
			int y = parte_a[i].x - parte_aux.x;
			parte_a[i].x = parte_aux.x - x;
			parte_a[i].y = parte_aux.y + y;
		}
		if (!confere()){
			for(i = 0; i < NPART; i++) parte_a[i] = parte_b[i];
		}
	}
}

// FUNÇÃO CONTROLES DO TECLADO
void control()
{
	if(key[KEY_UP] && !giroKey && cor != 7){giro = 1; giroKey = 1;}
	if(key[KEY_LEFT]) {pos_x = -1;}
	else if(key[KEY_DOWN]) passo = nTimer + 1;
	else if(key[KEY_RIGHT]) {pos_x = 1;}
	else if(!key[KEY_UP]) giroKey = 0;
	
	for (i = 0; i < NPART; i++)
	{
		parte_b[i] = parte_a[i]; // Copia posição atual
		parte_a[i].x += pos_x; // Recebe a posição
	}
	
	// Se movimento inválido desfaz (Retorna 0)
	if(!confere())
	{
		for(i = 0; i < NPART; i++) parte_a[i] = parte_b[i];
	}
}

// FUNÇÃO CONFERIR COLISÃO (PAREDES E COLUNAS)
int confere(){
	for (i = 0; i < NPART; i++){
		if(parte_a[i].x < 0 || parte_a[i].x >= COL) return 0; // não pode ser movido
		else if(parte_a[i].y >= LIN || grade[parte_a[i].y][parte_a[i].x]){
			return 0; // não pode ser movido
		}
	}
	return 1;
}

// FUNÇÃO TEMPORIZADOR
int timer()
{
	if(passo++ > nTimer)
	{
		for (i = 0; i < NPART; i++)
		{
			parte_b[i] = parte_a[i];
			parte_a[i].y += 1; // Desce a peça (y ganha 1)
		}
		criaForma();
		passo = 0;
	}
}

// FUNÇÃO CRIAÇÃO DE FORMAS
void criaForma(){
	
	if(!confere()){
		for (i = 0; i < NPART; i++) grade[parte_b[i].y][parte_b[i].x] = cor;
		for (i = 0; i < COL; i++) if(grade[1][i]) gover = 1;
		nForma = rand() % 7;
		
		if(newPart){
			cor = nextPartCor;
			for (i = 0; i < NPART; i++){
				parte_a[i].x = nextPartX[i];
				parte_a[i].y = nextPartY[i];	
			}
		}
		for (i = 0; i < NPART; i++){
			nextPartX[i] = forma[nForma][i] % 2;
			nextPartY[i] = forma[nForma][i] / 2;	
		}
		newPart = 1;
	}
}

// FUNÇÃO SAIR
void sair(){sai = 1;}

END_OF_FUNCTION(sair);
