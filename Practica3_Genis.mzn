/*Practica3 Genis Martinez Sanchis*/

%La capa A es la primera fila y la B la segunda
array  [1..9, 1..2] of var 1..18:  capa;
include "alldifferent.mzn";
constraint alldifferent(capa);

%-A-
/*La capa E2, aplicada de forma A (es decir E2A) no debe aplicarse junto a una capa de tipo E1, E5, E9 (aplicadas de modo A), ni a una capa E4 aplicada de modo B. Es decir, E2A no debe aplicarse junto a una capa E1A, E5A, E9A o E4B*/
constraint abs(capa[1,1]-capa[2,1])>= 2 /\ abs(capa[5,1]-capa[2,1])>= 2 /\ abs(capa[9,1]-capa[2,1])>= 2 /\ abs(capa[4,2]-capa[2,1])>= 2;

%-B-
/*E3A no puede aplicarse junto a ninguna capa que sea aplicada también de modo A*/
constraint forall(i in 1..9 where i!=7) (abs(capa[i,1]-capa[7,1])>= 2);

%-C-
/*Entre  las    capas  A  y  B  del elemento E4  (E4A  y E4B)  debe  haber  al menos  4  capas  de  otros elementos  (aplicados  de cualquier modo A o B).*/
constraint abs(capa[4,1]-capa[4,2])>= 5;

%-D-
/*Entre las capas A y B del elemento E8  (E8A y E8B), debe haber como mucho una capa de otro elemento cualquiera, pero tampoco deben estar juntas. */
constraint abs(capa[8,1]-capa[8,2])= 2;

%-E-
/*Las capas A y B del elemento E5 deben aplicarse consecutivas */
constraint abs(capa[5,1]-capa[5,2])= 1;

%-F-
/*E4A debe aplicarse junto a (encima o debajo de), una capa del elemento E1 o del elemento E9. */
constraint abs(capa[1,1]-capa[4,1])= 1 \/ abs(capa[1,2]-capa[4,1])= 1 \/ abs(capa[9,1]-capa[4,1])= 1 \/ abs(capa[9,2]-capa[4,1])= 1;

%-G-
/*Las capas E6A y E7A no pueden aplicarse juntas*/
constraint abs(capa[6,1]-capa[7,1])>= 2;

%-H-
/*La capa E5A debe aplicarse junto a  (encima o debajo de) una capa E4B, E8B, E2A, E3A o E7A. Y la capa E5B no debe 
estar junto a E6A. */
constraint abs(capa[4,2]-capa[5,1])= 1 \/ abs(capa[8,2]-capa[5,1])= 1 \/ abs(capa[2,1]-capa[5,1])= 1 \/ abs(capa[3,1]-capa[5,1])= 1 \/ abs(capa[7,1]-capa[5,1])= 1;
constraint abs(capa[6,1]-capa[5,2])>= 2;

%-I-
/*Hay  una  precedencia  entre  las  aplicaciones  del  modo  A. Concretamente,  cada  capa  A  de  un  elemento  Ei  (EiA)  debe aplicarse en un nivel k, que debe ser posterior al nivel p donde se ha aplicado cualquier capa EjA, si i>j. Es decir, si i>j, la capa Ei A se aplica en un nivel (k) superior al nivel (p) de la capa EjA (es decir, k>p). */
constraint forall (i,j in 1..9 where j<i) (capa[j,1]-capa[i,1] < 0);

%-J-
/*Algo  similar  ocurre  con  las  aplicaciones  B,  pero  a  la  inversa  y  solo  hasta  el  séptimo  elemento.  Es  decir,  si  i>j,    i<7, entonces la capa EiB se aplica en un nivel (k) menor que el nivel (p) deEjB (es decir, k < p). */
constraint forall (i,j in 1..6 where j<i) (capa[j,2]-capa[i,2] > 0);

solve satisfy;
%solve minimize abs(capa[8,1]-capa[9,1]) + abs(capa[1,2]-capa[2,2]);
%solve minimize abs(capa[7,1]-capa[7,2]) - abs(capa[3,1]-capa[9,2]);

output["Capa: \t|1| \t|2| \t|3| \t|4| \t|5| \t|6| \t|7| \t|8| \t|9| \n"];

output["A: \t"];

output [show(capa[i,1]) ++ "\t"
   | i in 1..9];
   
output ["\n"];

output["B: \t"];  
         
output [show(capa[i,2]) ++ "\t"
   | i in 1..9];
