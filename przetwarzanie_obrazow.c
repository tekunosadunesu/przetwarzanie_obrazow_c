#include<stdio.h>
#include<string.h>
#include<stdlib.h>//biblioteka potrzebna do wartosci bezwglednej (abs)
#include<math.h>

#define DL_LINII 1024
#define MAX 1024

void menu(){
  printf("\n\n\tCo chcesz wykonac? [wpisz cyfre stojaca przy opjci]\n");
  printf("\n0) Wczytaj obraz\n");
  printf("\n1) Negatyw\n");
  printf("2) Progowanie\n");
  printf("3) Polprogowanie bieli\n");
  printf("4) Polprogowanie czerni\n");
  printf("5) Korekcja gamma\n");
  printf("6) Zmiana poziomow\n");
  printf("7) Konturowanie\n");
  printf("8) Rozmywanie w poziomie\n");
  printf("9) Rozmywanie w pionie\n");
  printf("10) Rozciaganie histogramu\n");
  printf("\n77) Wyswietl podglad\n");
  printf("88) Zapisz obraz\n");
  printf("99) Wyswietl zapisany obraz\n");
  printf("100) Zakoncz dzialanie programu\n");
}

void wybrana_opcja(int o){
  
  switch(o){
  case 89:
    printf("\nAktywne efekty to: ");
    break;
  case 1:
    printf("negatyw, ");
    break;
  case 2:
    printf("progowanie, ");
    break;
  case 3:
    printf("polprogowanie bieli, ");
    break;
  case 4:
    printf("polprogowanie czerni, ");
    break;
  case 5:
    printf("korekcja gamma, ");
    break;
  case 6:
    printf("zmiana poziomow, ");
    break;
  case 7:
    printf("konturowanie, ");
    break;
  case 8:
    printf("rozmywanie w poziomie, ");
    break;
  case 9:
    printf("rozmywanie w pionie, ");
    break;
  case 10:
    printf("rozciaganie histogramu, ");
    break;
  }
}

  

int wczytaj(FILE*plik,int *w, int *k, double *n,int obraz[][MAX]){
  if(plik == NULL){
    fprintf(stderr, "\n!!!BLAD!!!\n\n");//nie udalo sie otworzyc pliku
    return -1;
  }
  char bufor[DL_LINII];
  fgets(bufor,DL_LINII, plik);
  printf("\nPodstawowe informacje o pliku:\n");
  printf("%s", bufor); //numer magiczny
  

  //czytanie numeru magicznego
  if(bufor[0] !='P' || bufor[1] != '2'){
    fprintf(stderr, "\n!!!BLAD!!!\n\n");//blad numeru maicznego
    return -2;
  }
  
  //sprawdzanie czy jest komentarz
  char kom = fgetc(plik);
  if (kom == '#'){
    fgets(bufor, DL_LINII, plik);
    printf("Komentarz do zdjecia: %s\n", bufor);
  }
  else{//jesli nie ma komenatzra trzeba zwrocic znak do pliku
    ungetc(kom, plik);
    printf("Nie ma komenarza\n");
  }
  
  //odczyt wymiarow
  if(fscanf(plik,"%d%d%lf", w, k, n )!= 3){//odczytywanie szerkosci, wyskosci, odc szarosci
    fprintf(stderr, "\n!!!BLAD!!!\n\n");
    return -3;
  }

  printf("Liczba wierszy: %d\nLiczba kolumn: %d\nSkala odcieni szarosci: %.0f\n", *w,*k,*n);


  //wczytywanie wartosci pikslei  
  int y, x;
  for(x=0; x< *k; x++){
    for(y=0; y< *w; y++){
      if(fscanf(plik, "%d", &(obraz[x][y])) != 1){
	fprintf(stderr, "Blad wczytania pikseli (%d, %d)", x, y);
	return -4;
      }
    }
  }

  return ((*w)*(*k));
}

void zapisz_obraz(FILE* plik, int obraz[MAX][MAX], int w, int k, int n){
  
  // przy zapisie do pliku postepujemy podobnie. Zaczynamy od wpisania do pliku liczby magicznej P2:
  fprintf(plik, "P2\n");
  
  // nastepnie dodajemy rozmiary obrazka:
  fprintf(plik, "%d %d\n", w, k);

  //kolejno piszemy do pliku liczbe odcieni szarosci:
  fprintf(plik, "%d\n", n);

  //i dalej lecimy z cala tablica pikseli:
  for(int i=0; i<k; i++){
    for(int j=0; j<w; j++){
      fprintf(plik, "%d ", obraz[i][j]);
    }
  }
  printf("\nZapisywanie obrazu zakończono \n");
}

void wyswietl(char *nazwa) {
  char pol[DL_LINII]; //bufor do stworzenia skladni polecenia
  strcpy(pol,"display "); //kolejne fragmenty polecenia
  strcat(pol,nazwa);
  strcat(pol," &"); // finalnie: display nazwa &
  printf("%s\n",pol); // kontrolnie wyswietlamy w terminalu
  system(pol); // i wykonujemy w systemie
}


/* Operacja nakładania negatywu tworzy obraz wyjściowy, w którym nowa wartość
   każdego z pikseli zastepowana jest różnica maksymalnej wartości szarości i aktualna wartościa piksela */

void negatyw(int obraz[MAX][MAX], int w, int k,int n){
  for(int i=0; i<k; i++){
    for(int j=0; j<w; j++){
      obraz[i][j]= n- obraz[i][j];
    }
  }
}


/* Operacja progowania, czyli klasyfikacji piksela do jednej z dwóch grup (białej lub
   czarnej). Jesli wartosc piksela jest wieksza od progu przypisujemy maks wartosc, jesli mniejsze 0 */

void progowanie(int obraz[MAX][MAX], int w, int k, int n, int prog_procentowy){
  long double prog= prog_procentowy/100.00 *n; //zmiana procentow na wartosc
  for(int i=0; i<k; i++){
    for(int j=0; j<w; j++){
	
      if(obraz[i][j]>prog){
	obraz[i][j] =n ;
      }
      else{
	obraz[i][j]=0;
      }
    }
  }
}


/*Operacja  polprogowania bieli polega na zastapieniu pikseli wiekszych od progu
  maksymalna wartoscia, a mniejsze pozostaja bez zmian */
 
void polprogowanie_bieli(int obraz[MAX][MAX], int w, int k, int n, int prog_procentowy){
  long double prog= prog_procentowy/100.00 *n;
  for(int i=0; i<k; i++){
    for(int j=0; j<w; j++){
      if(obraz[i][j]>prog){
	obraz[i][j] = n ;
      }
      else{
	obraz[i][j]= obraz[i][j];
      }
    }
  }
}



/* Polprogowanie czerni polega na zastapieniu pikseli mniejszych niz prog kolorem czarnym,
   natomiast reszta pozostaje bez zmian */

void polprogowanie_czerni(int obraz[MAX][MAX], int w, int k, int n, int prog_procentowy){
  long double prog= prog_procentowy/100.00 *n;
  for(int i=0; i<k; i++){
    for(int j=0; j<w; j++){
      if(obraz[i][j]>prog){
	obraz[i][j] =obraz[i][j] ;
      }
      else{
	obraz[i][j]=0;
      }
    }
  }
}

/*  Korekcja gamma przeskalowuje poziomy jasności pikseli na obrazie. Pozwala na
    przyciemnienie lub rozjaśnienie obrazu (w zależności od przyjetego gamma) */

void korekcja_gamma(int obraz[MAX][MAX], int w, int k,double n, double gamma) {
  for (int i = 0; i < k; i++) {
    for (int j = 0; j < w; j++) {
      obraz[i][j] = pow(obraz[i][j] / n, 1.0 / gamma) * n; //skala odcieni szarosci (n) musi byc typu double, poniewaz jest wykonywane potegowanie przyu uzyciu gammy, ktora tez jest typu double
    }
  }
}



void zmiana_poziomow(int obraz[MAX][MAX], int w, int k,double n, double p_prog_c, double p_prog_b){
  double prog_b= p_prog_b/100 *n;
  double prog_c= p_prog_c/100 *n;
  for(int i=0; i<k; i++){
    for(int j=0; j<w; j++){
      if(obraz[i][j]<prog_c){
	obraz[i][j]= 0;
      }
      else if(prog_c < obraz[i][j] && obraz[i][j] < prog_b){
	obraz[i][j]= (obraz[i][j]-prog_c)*n /(prog_b - prog_c);
      }
      
      else if (obraz[i][j]> prog_b){
	obraz[i][j]= n;
      }
      
    }
  }
}

/* Operacja polegajaca  na wyostrzeniu obrysów obiektu z wykorzystaniem metody
   gradientowej. Piksele o jasności podobnej do jasności otoczenia zostaja rozjaśniane,
   a te, które różnia sie od otoczenia, przyciemniane. W przedstawianej poniżej operacji
   konturowania, wartość piksela uzależniana jest od jego wartości aktualnej, wartości
   piksela po prawej stronie oraz wartości piksela poniżej. */ 

void konturowanie(int obraz[MAX][MAX], int w, int k, int n){
  for(int i=0; i<k; i++){
    for(int j=0; j<w; j++){
      obraz[i][j]=abs(obraz[i+1][j]-obraz[i][j])+ abs(obraz[i][j+1]-obraz[i][j]);
    }
  }
}

/* Rozmywanie w poziomie polega  na zmniejszeniu ostrości obrazu w kierunku poziomym.
   Operacja wymaga uśrednienia wartości pikseli znajdujacych sie po lewej oraz po prawej stronie
   analizowanego piksela (parametr promienia rozmywania). */

void rozmywanie_pion(int obraz[MAX][MAX], int w, int k, int r){
  for(int i=0; i<k; i++){
    for(int j=0; j<w; j++){
      int suma =0;
      for(int p=-r; p<=r; p++){
	if(i+p >= 0 && i+p < k){//warunek zeby promien nie wychodzil poza obszar obrazu
	  suma += obraz[i+p][j];//analogicznie mozna zapisac suma=suma+(wzor)
	}
      }
      obraz[i][j] = suma/(2*r+1);//dzielenie otrzymanej sumy i podstawienie pod piksele
    }
  }
  } 

/*Operacja polegajaca na zmniejszeniu ostrości obrazu w kierunku pionowym. Operacja
  wymaga uśrednienia wartości pikseli znajdujacych sie nad oraz pod analizowanym pikselem. */

void rozmywanie_poziom(int obraz[MAX][MAX], int w, int k, int r){
  for(int i=0; i<k; i++){
    for(int j=0; j<w; j++){
      int suma =0;
      for(int p=-r; p<=r; p++){
	if(i+p >= 0 && i+p < k){
	  suma += obraz[i][j+p];
	}
      }
      obraz[i][j] = suma/(2*r+1);
    }
  }
} 

/*Operacja wykonywana wówczas, gdy jasności pikseli w obranie nie odwzorowuja
  całego dopuszczalnego zakresu szarości obrazu. */

void rozciaganie_histogramu(int obraz[MAX][MAX], int w, int k, int n){
  
  int s_min= obraz[0][0];
  int s_max= obraz[0][0];

  for(int x=0; x<k; x++){
    for(int y=0; y<w; y++){
      if(obraz[x][y]< s_min){//szukanie najmniejszej wartosci
	s_min= obraz[x][y];
      }
      if(obraz[x][y] > s_max){//szukanie najwiekszej wartosci
	s_max=obraz[x][y];
      }
    }
  }
  for(int i=0; i<k; i++){
    for(int j=0; j<w; j++){
      obraz[i][j]= (obraz[i][j]-s_min)*n/(s_max-s_min);
    }
  }
}




int main(){
  int obraz[MAX][MAX];
  int w, k, r;
  double n;
  FILE* plik;
  FILE* plik_tmp;
  char nazwa[MAX];
  char nazwa_plik[MAX];
  int o, o1, o2, o3, o4, o5, o6, o7, o8, o9, o10;
  int opcja, opcja0, opcja1;
  int prog, polprog_b, polprog_c,prog_b,prog_c;
  double gamma;
  opcja0=1;
  opcja1=1;
  
        
  menu(0);
  scanf("%d", &opcja);
  
  while(opcja!=100){
    
    switch(opcja){

    case 0:
      printf("Wybrano wczytywanie pliku\n");
      printf("\nPliki ktore mozesz otworzyc\n");
      int z;
      z=system("ls *.pgm"); //"wpisuje" w terminal polecenie
      printf("\n");
      
      printf("Podaj nazwe pliku, ktory chcesz otworzyc [z uwzglednieniem rozszerzenia]:\n");
      scanf("%s",nazwa);
      plik=fopen(nazwa, "r");
      double liczba= wczytaj(plik, &w, &k, &n, obraz);
      if(liczba<0){
      }
      printf("Lacznie wczytano %.0f pikseli", liczba);
      fclose(plik);

      	plik_tmp=fopen("tmp.pgm", "w");
	zapisz_obraz(plik_tmp, obraz, w, k, n);
	fclose(plik_tmp);
      o=89;
      o1=0;
      o2=0;
      o3=0;
      o4=0;
      o5=0;
      o6=0;
      o7=0;
      o8=0;
      o9=0;
      o10=0;
      opcja0=0;
      break;
	
	
    case 1:
      if(opcja0 != 0){
	printf("\n!!! Nie wczytano obrazu !!!\n");//funkcja zapobiegajaca korzystaniu z operacji bez wczytania obrazu
      }
      else{
	printf("Wybrano negatyw");
	negatyw(obraz, w, k, n);
	o=89;
	o1=1;
        
	plik_tmp=fopen("tmp.pgm", "w");//tymczasowy plik pozwalajacy na wyswietlenie obrazu bez jego zapisywania
	zapisz_obraz(plik_tmp, obraz, w, k, n);
	fclose(plik_tmp);
      }
    
      break;

    case 2:
      if(opcja0 != 0){
	printf("\n!!! Nie wczytano obrazu !!!\n");
      }
      else{
	printf("Wybrano progowanie\n");
	printf("Wybierz prog: ");
	scanf("%d", &prog);
	if(prog <=0 || prog >100){
	  printf("Niepoprawna wartosc\nPodaj prog: ");
	  scanf("%d", &prog);
	}
	else{
	  progowanie(obraz, w, k, n, prog);
	}
	o=89;
	o2=2;
        
	plik_tmp=fopen("tmp.pgm", "w");
	zapisz_obraz(plik_tmp, obraz, w, k, n);
	fclose(plik_tmp);
      }
      break;

    case 3:
      if(opcja0 != 0){
	printf("\n!!! Nie wczytano obrazu !!!\n");
      }
      else{
	printf("Wybrano polprogowanie bieli\n");
	printf("Wybierz prog (w procentach): ");
	       
	scanf("%d", &prog);
	if(prog <=0 || prog >100){
	  printf("Niepoprawna wartosc\nPodaj prog: ");
	  scanf("%d", &prog);
	}
	else{
	  polprogowanie_bieli(obraz, w, k, n, prog);
	}
	o3=3;
	o=89;
	
	plik_tmp=fopen("tmp.pgm", "w");
	zapisz_obraz(plik_tmp, obraz, w, k, n);
	fclose(plik_tmp);
      }
      break;

    case 4:
      if(opcja0 != 0){
	printf("\n!!! Nie wczytano obrazu !!!\n");
      }
      else{
	printf("Wybrano polprogowanie czerni\n");
	printf("Podaj prog (w procentach): ");
	scanf("%d", &prog);
	if(prog <=0 || prog >100){
	  printf("Niepoprawna wartosc\nPodaj prog: ");
	  scanf("%d", &prog);
	}
	else{
	  polprogowanie_czerni(obraz, w, k, n, prog);
	}
	o4=4;
	o=89;

	plik_tmp=fopen("tmp.pgm", "w");
	zapisz_obraz(plik_tmp, obraz, w, k, n);
	fclose(plik_tmp);
      }
      break;

    case 5:
      if(opcja0 != 0){
	printf("\n!!! Nie wczytano obrazu !!!\n");
      }
      else{
	printf("Wybrano gamma\n");
	printf("wybierz wspolczynnik gamma:");
	scanf("%lf", &gamma);
	if(gamma<=0){
	  printf("Niepoprawna wartosc\nPodaj wspolczynnik gamma: ");
	  scanf("%lf", &gamma);
	}
	else{
	  korekcja_gamma(obraz, w, k,n, gamma);
	}
	o5=5;
	o=89;
      
	plik_tmp=fopen("tmp.pgm", "w");
	zapisz_obraz(plik_tmp, obraz, w, k, n);
	fclose(plik_tmp);
      }
      break;

    case 6:
      if(opcja0 != 0){
	printf("\n!!! Nie wczytano obrazu !!!\n");
      }
      else{
	printf("Wybrano zmiane poziomow\n");
	printf("Podaj prog czerni (w procentach): ");
	scanf("%d", &prog_c);
	if(prog_c<=0 || prog_c>100){
	  printf("Niepoprawna wartosc\nPodaj prog czerni(w procentach): ");
	  scanf("%d", &prog_c);
	}
	else{
	  printf("Podaj prog bieli (w procentach): ");
	  scanf("%d", &prog_b);
	  if(prog_c<=0 || prog_c>100){
	    printf("Niepoprawna wartosc\nPodaj prog bieli(w procentach): ");
	    scanf("%d", &prog_b);
	  }
	  else{
	    zmiana_poziomow(obraz, w, k, n, prog_c, prog_b);
	
	    plik_tmp=fopen("tmp.pgm", "w");
	    zapisz_obraz(plik_tmp, obraz, w, k, n);
	    fclose(plik_tmp);
	  }
	}
	o6=6;
	o=89;
      }
      break;

    case 7:
      if(opcja0 != 0){
	printf("\n!!! Nie wczytano obrazu !!!\n");
      }
      else{
	printf("Wybrano konturowanie\n");
	konturowanie(obraz, w, k, n);
	o7=7;
	o=89;

        
	plik_tmp=fopen("tmp.pgm", "w");
	zapisz_obraz(plik_tmp, obraz, w, k, n);
	fclose(plik_tmp);
      }
      break;
	
    case 8:
      if(opcja0 != 0){
	printf("\n!!! Nie wczytano obrazu !!!\n");
      }
      else{
	printf("Wybrano rozmywanie obrazu w poziomie\n");
	printf("Podaj promien rozmywania: ");
	scanf("%d", &r);
		if(r<=0){
	  printf("Niepoprawna wartosc\nPodaj promien rozmywania: ");
	  scanf("%d", &r);
	}
	else{
	 rozmywanie_poziom(obraz, w, k, r);//dodac ogranicznik ze r nie moze byc ujemne
	}
	o8=8;
	o=89;
	
	
	plik_tmp=fopen("tmp.pgm", "w");
	zapisz_obraz(plik_tmp, obraz, w, k, n);
	fclose(plik_tmp);
      }
      break;

    case 9:
      if(opcja0 != 0){
	printf("\n!!! Nie wczytano obrazu !!!\n");
      }
      else{
	printf("Wybrano rozmywanie obrazu w pionie\n");
	printf("Podaj promien rozmywania: ");
	scanf("%d", &r);
	if(r<=0){
	  printf("Niepoprawna wartosc\nPodaj promien rozmywania: ");
	  scanf("%d", &r);
	}
	else{
	 rozmywanie_pion(obraz, w, k, r);
	}
	o9=9;
	o=89;
        
	plik_tmp=fopen("tmp.pgm", "w");
	zapisz_obraz(plik_tmp, obraz, w, k, n);
	fclose(plik_tmp);
      }
      break;

    case 10:
      if(opcja0 != 0){
	printf("\n!!! Nie wczytano obrazu !!!\n");
      }
      else{
	printf("Wybrano rozciaganie histogramu\n:");
	rozciaganie_histogramu(obraz, w, k, n);
	o10=10;
	o=89;


	plik_tmp=fopen("tmp.pgm", "w");
	zapisz_obraz(plik_tmp, obraz, w, k, n);
	fclose(plik_tmp);
      }
      break;

    case 77:
      if(opcja0 != 0){
	printf("\n!!! Nie wczytano obrazu !!!\n");
      }
      else{
	wyswietl("tmp.pgm");
      }
      break;

    case 88:
      if(opcja0 != 0){
	printf("\n!!! Nie wczytano obrazu !!!\n");
      }
      else{
	printf("Podaj nazwe pod jaka chcesz zapisac obraz [z uwzglednieniem rozszerzenia]\n");
	scanf("%s", nazwa_plik);
	
	FILE* nowy_plik;
	nowy_plik = fopen(nazwa_plik , "w");

	if(nowy_plik == NULL){
	  fprintf(stderr, "Blad podczas otwierania pliku\n");
	  return -5;
	}

	zapisz_obraz(nowy_plik, obraz, w, k, n);
	fclose(nowy_plik);
      }
      opcja1=0;
      break;
	

    case 99:
      if(opcja0 != 0){
	printf("\n!!! Nie wczytano obrazu !!!\n");
      }
      else{
	if(opcja1 != 0){
	  printf("\n!!! Nie zapisano obrazu !!!\n");
	}
	else{
	  wyswietl(nazwa_plik);
	}
      }
      break;
	
	
    default:
      printf("Nie ma takiej opcji\n");
      break;
    }
      
    wybrana_opcja(o);  // pokazuje zastosowane efekty
    wybrana_opcja(o1);
    wybrana_opcja(o2);
    wybrana_opcja(o3);
    wybrana_opcja(o4);
    wybrana_opcja(o5);
    wybrana_opcja(o6);
    wybrana_opcja(o7);
    wybrana_opcja(o8);
    wybrana_opcja(o9);
    wybrana_opcja(o10);
    menu(0);
    scanf("%d", &opcja);

  }

  remove("tmp.pgm");
  
  return 0;
}  

