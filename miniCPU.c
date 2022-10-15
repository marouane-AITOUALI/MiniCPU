#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>



#define NBITS 8  // attention, votre programme doit pouvoir être adapté à d'autres tailles juste en modifiant la valeur de cette constante
// en ayant toujours NBITS < 32

/////////////////////////////////////////////////////////
// définition de types
/////////////////////////////////////////////////////////

typedef struct {
  int* accu;
  int* flags; // indicateurs ZF CF OF NF
} ALU;

typedef struct {
  ALU alu;
  int* R0;
  int* R1;
  int* R2;
} CPU;

/////////////////////////////////////////////////////////
// fonctions d'initialisation
/////////////////////////////////////////////////////////

/*
 * allocation d'un mot entier de NBITS bits initialisé à 0
 */
int* word() {
  int* tab;
  int i;
  tab=(int*)malloc(NBITS*sizeof(int));
  for(i=0;i<NBITS;i++) tab[i]=0;
  // poids faible : tab[0]
  // poids fort : tab[NBITS-1]
  return tab;
}

/*
 * Initialisation du mot (mot de NBITS bits, codant un entier en Cà2) avec une valeur entière.
 */
void setValue(int* word,int n) {
  int n_low = n & 0x7FFFFFFF;
  // revient à mettre à 0 le bit de poids fort en 32 bits
  // on peut alors travailler sur la partie positive du codage de n
  // remarque : si n est bien codable en Ca2 sur NBITS, et si n est négatif, on récupère quand même le codage de n sur NBITS en Ca2 en prenant les NBITS de poids faible de n_low
  
  // à compléter
  for (int a = 0; a < NBITS; ++a){
    word[a] = 0;
  }
  int i = 0;
  while(n_low != 0){
    word[i] = n_low % 2;
    n_low /= 2;
    i +=1;  
  }

  int indice = 0;
  if (n < 0){
    for (int i = 0; i < NBITS; i++){
      if (word[i] == 1){
        indice = i;
      }
    }
    for (int j = indice + 1; j < NBITS; ++j){
      if(word[j] == 1){
        word[j] = 0;
      }else if(word[j] == 0){
        word[j] = 1;
      }
    }
  }
  
  
}

/*
 * instanciation d'un mot de NBITS bits initialisé avec la valeur n
 */
int* initWord(int n) {
  int* tab=word();
  setValue(tab,n) ;
  return tab;
}
    
/*
 * Initialisation du mot (mot de NBITS bits) par recopie des bits du mot en paramètre.
 */
void copyValue(int* word,int* src) {
    // à compléter
    for (int i =0; i < NBITS; ++i){
      word[i] = src[i];
    }
}

/*
 * instanciation d'un mot de NBITS bits initialisé par recopie d'un mot
 */
int* copyWord(int* src) {
  int* tab=word();
  copyValue(tab,src) ;
  return tab;
}

/*
 * initialise l'ALU
 */
ALU initALU() {
  ALU res;
  res.accu=word();
  res.flags=(int*)malloc(4*sizeof(int));
  return res;
}

/*
 * initialise le CPU
 */
CPU initCPU() {
  CPU res;
  res.alu=initALU();
  res.R0=(int*)malloc(NBITS*sizeof(int));
  res.R1=(int*)malloc(NBITS*sizeof(int));
  res.R2=(int*)malloc(NBITS*sizeof(int));
  return res;
}

/////////////////////////////////////////////////////////
// fonctions de lecture
/////////////////////////////////////////////////////////

/*
 * Retourne la valeur entière signée représentée par le mot (complément à 2).
 */    
int intValue(int* word) {
    int res = 0;
    int p = 1;
    for(int i =0; i < NBITS - 1; ++i){
      res += word[i] * p;
       p *=2;
    }
    res += word[NBITS - 1] * (-p);
    return res;
}

/*
 * Retourne une chaîne de caractères décrivant les NBITS bits
 */
char* toString(int* word) {
  int i,j=0;
  char* s=(char*)malloc((2+NBITS)*sizeof(char));
  for (i=NBITS-1;i>=0;i--) {
    if (word[i]==1) s[j]='1';
    else s[j]='0';
    j++;
  }
  s[j]=0;
  return s;
}

/*
 * Retourne l'écriture des indicateurs associés à l'ALU.
 */
char* flagsToString(ALU alu) {
  char *string=(char*)malloc(10*sizeof(char));
  sprintf(string,"z%dc%do%dn%d",alu.flags[0],alu.flags[1],alu.flags[2],alu.flags[3]);
  return string;
}

/*
 * affiche à l'écran le contenu d'une ALU
 */
void printing(ALU alu) {
    // à compléter
    printf("ALU.acc =%s\nALU.flags =%s\n", toString(alu.accu), flagsToString(alu));
}

/////////////////////////////////////////////////////////
// fonctions de manipulations élémentaires
/////////////////////////////////////////////////////////

/*
 * Mise à la valeur b du bit spécifié dans le mot
 */
void set(int* word,int bitIndex,int b) {
  if ((bitIndex > NBITS-1) || (bitIndex < 0)) 
    printf("valeur d'index incorrecte\n");
  word[bitIndex] = b ;
}

/*
 * Retourne la valeur du bit spécifié dans le mot
 */
int get(int* word,int bitIndex) {
  if ((bitIndex > NBITS-1) || (bitIndex < 0)) 
    printf("valeur d'index incorrecte\n");
  return word[bitIndex] ;
}

/*
 * Positionne l'indicateur ZF en fonction de l'état de l'accumulateur
 */
void setZ(ALU alu) {
  // à compléter
  int i = 0;
  while (i < NBITS && alu.accu[i] == 0){
    i += 1;
  }
  if (i == NBITS){
    set(alu.flags, 0, 1);
  }else{
    set(alu.flags, 0, 0);
  }

  // alu.flags[0] = (intValue(alu.accu) == 0)
}

/////////////////////////////////////////////////////////
// opérateurs de base de l'ALU
// IMPORTANT : les indicateurs doivent être mis à jour
/////////////////////////////////////////////////////////

/*
 * Stocke le paramètre dans le registre accumulateur
 */
void pass(ALU alu,int* B) {
  // à compléter
  copyValue(alu.accu, B);
  setZ(alu);
  alu.flags[1] = 0; // pas de carry
  alu.flags[2] = 0; // ni overflow
  alu.flags[3] = alu.accu[NBITS - 1];
}

/*
 * Effectue un NAND (NON-ET) entre le contenu de l'accumulateur et le paramètre.
 */
void nand(ALU alu,int* B) {
  // à compléter
  for (int i =0; i < NBITS; i++){
    set(alu.accu, i, !(get(alu.accu, i) && get(B, i)));
  }
  setZ(alu);
  alu.flags[1] = 0; // pas de carry
  alu.flags[2] = 0; // ni overflow
  alu.flags[3] = alu.accu[NBITS - 1];
}

/*
 * Décale le contenu de l'accumulateur de 1 bit vers la droite
 */
void shift(ALU alu) {
  int tab[NBITS];
  copyValue(tab, alu.accu);
  for (int i = NBITS - 2; i >= 0; i--) // bit poid faible indice 0
  {
    set(alu.accu, i, tab[i+1]); // le decalage a droite est equivalent au decalage a gauche du tableau
  }
  set(alu.accu, NBITS - 1, 0);  // La derniere case(poid fort) on rajoute ensuite un 0
  setZ(alu);
  alu.flags[1] = 0; // pas de carry
  alu.flags[2] = 0; // ni overflow
  alu.flags[3] = alu.accu[NBITS - 1];
}

/*
 * module Full Adder : a+b+c_in = s + 2 c_out
 * retourne un tableau contenant s et c_out
 */
int* fullAdder(int a,int b,int c_in) {
  int* res=(int*)malloc(2*sizeof(int));
  // à compléter
  res[0] = (a + b + c_in) % 2; // elimine 2*c_out ms la valeur de s fixe car((0 ou 1) % 2 == 0 ou 1)
  res[1] = (a+b+c_in - res[0]) / 2; // Module FA
  return res;
}

/*
 * Additionne le paramètre au contenu de l'accumulateur (addition entière Cà2).
 * Les indicateurs sont positionnés conformément au résultat de l'opération.
 */
void add(ALU alu,int* B) {
  // à compléter
  int *res = (int*)malloc(2*sizeof(int));
  int carry = 0;
  for (int i =0; i < NBITS; i++){
    res = fullAdder(get(alu.accu, i), get(B, i), carry);
    set(alu.accu, i, res[0]);
    carry = res[1]; 
  }
  setZ(alu);
  alu.flags[1] = carry;
  alu.flags[2] = (carry && alu.accu[NBITS - 1]); // on a un overflow si on a une carry et un bit à 1
  alu.flags[3] = alu.accu[NBITS - 1];

}

////////////////////////////////////////////////////////////////////
// Opérations logiques :
////////////////////////////////////////////////////////////////////

/*
 * Négation.
 */
void not(CPU cpu){
  // à compléter
  nand(cpu.alu, cpu.alu.accu);
}

/*
 * Et.
 */
void and(CPU cpu,int* B) {
  // à compléter
  nand(cpu.alu, B);
  not(cpu); // ET == !!ET
}


/*
 * Ou.
 */
void or(CPU cpu,int* B) {
  for (int i =0; i < NBITS; ++i){
    set(cpu.alu.accu, i, get(cpu.alu.accu, i) || get(B, i));
  }
}

/*
 * Xor.
 */
void xor(CPU cpu,int* B) {
  for (int i =0; i < NBITS; ++i){
    set(cpu.alu.accu, i, get(cpu.alu.accu, i) ^ get(B, i));
  }
}

/*
 * Décale le receveur de |n| positions.
 * Le décalage s'effectue vers la gauche si n>0 vers la droite dans le cas contraire.
 * C'est un décalage logique (pas de report du bit de signe dans les positions 
 * libérées en cas de décalage à droite).
 * L'indicateur CF est positionné avec le dernier bit "perdu".
 */
void logicalShift(CPU cpu,int n) {
  // à compléter
  int i = n;
  while (i != 0){
    if (n <= 0){
      shift(cpu.alu);
      ++i;
    }else{
      add(cpu.alu, cpu.alu.accu);
      --i;
    }
  }
}

/////////////////////////////////////////////////////////
// Opérations arithmétiques entières
/////////////////////////////////////////////////////////

/*
 * Opposé.
 */
void opp(CPU cpu) {
  // à compléter
  for (int i = 0; i < NBITS; i++){
    set(cpu.alu.accu, i, !get(cpu.alu.accu, i));
  }
}

/*
 * Soustraction.
 */
void sub(CPU cpu,int* B) {
  // à compléter
  int v = intValue(B);
  int *tmp = (int*)malloc(sizeof(int));
  setValue(tmp, -v);
  add(cpu.alu, tmp); 
}

/*
 * Multiplication.
 */
void mul(CPU cpu,int* B) {
  // à compléter
  int v = intValue(B); 
  if (v < 0){
    opp(cpu);   // si le B est negatif on fait l'opposé du contenu de cpu
    v = -v;
  }
  for (int i =0; i < v; i++){
    add(cpu.alu, cpu.alu.accu); // ensuite on additionne B fois la valeur du cpu avec lui meme
  }
}

/////////////////////////////////////////////////////////
// Programme de test
/////////////////////////////////////////////////////////

int main(int argc,char *argv[]) {
  
  /*
    Ce programme est fourni à titre d'exemple pour permettre de tester simplement vos fonctions.
    Il vous est bien entendu possible de le modifier/compléter, ou encore d'écrire vos propres fonctions de test.
   */
  
  int* operand;
  ALU alu;
  CPU cpu;
  
  int chosenInt,integer ;
  int go_on = 1 ;
  
  char* menu =     
    "              Programme de test\n\n0  Quitter\n1  setValue(operande,int)\n2  pass(alu,operande)\n3  printing(alu)\n4  afficher toString(operande)\n5  afficher intValue(operande)\n6  afficher intValue(accu)\n7  accu=nand(accu,operande)\n8  accu=add(accu,operande)\n9  accu=sub(accu,operande)\n10 accu=and(accu,operande)\n11 accu=or(accu,operande)\n12 accu=xor(accu,operande)\n13 accu=not(accu)\n14 accu=opp(accu)\n15 accu=shift(accu)\n16 accu=logicalShift(accu,int)\n17 accu=mul(accu,operande)\n\n" ;
  
  char* invite = "--> Quel est votre choix  ? " ;
  
  printf("%s",menu) ; 

  operand=word();
  cpu=initCPU();
  alu=cpu.alu;
  
  while (go_on==1) {
    printf("%s",invite);
    scanf("%d",&chosenInt);
    switch (chosenInt) {
    case 0 : 
      go_on = 0 ;
      break ;
    case 1 :
      printf("Entrez un nombre :"); 
      scanf("%d",&integer);
      setValue(operand,integer);
      break ;
    case 2 : 
      pass(alu,operand);
      break ;
    case 3 : 
      printing(alu);
      break ;
    case 4 : 
      printf("%s\n",toString(operand));
      break ;
    case 5 : 
      printf("intValue(operand)=%d\n",intValue(operand));
      break ;
    case 6 : 
      printf("intValue(accu)=%d\n",intValue(alu.accu));
      break ;
    case 7 : 
      nand(alu,operand);
      printf("apres nand(), accu = ");
      printing(alu);
      break ;
    case 8 : 
      add(alu,operand);
      printf("apres add(), accu = ");
      printing(alu);
      break ;
    case 9 : 
      sub(cpu,operand);
      printf("apres sub(), A = ");
      printing(alu);
      break ;
    case 10 : 
      and(cpu,operand);
      printf("apres and(), A = ");
      printing(alu);
      break ;
    case 11 : 
      or(cpu,operand);
      printf("apres or(), A = ");
      printing(alu);
      break ;
    case 12 : 
      xor(cpu,operand);
      printf("apres xor(), A = ");
      printing(alu);
      break ;
    case 13 : 
      not(cpu);
      printf("apres not(), A = ");
      printing(alu);
      break ;
    case 14 : 
      opp(cpu);
      printf("apres opp(), A = ");
      printing(alu);
      break ;
    case 15 : 
      shift(alu);
      printf("apres shift(), A = ");
      printing(alu);
      break ;
    case 16 : 
      printf("Entrez un entier :") ;
      scanf("%d",&integer);
      logicalShift(cpu,integer);
      printf("apres logicalshift(%d), A = ",integer);
      printing(alu);
      break ;
    case 17 : 
      mul(cpu,operand);
      printf("apres mul(), A = ");
      printing(alu);
      break ;
    default : 
      printf("Choix inexistant !!!!\n");
      printf("%s",menu);
    }
  }
  printf("Au revoir et a bientot\n");
  return 0;
}


  
