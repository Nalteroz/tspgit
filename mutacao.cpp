/*
*  mutacao.cpp
*
*  Módulo responsável pelo mutação de um indivíduo
*  Autor: Geraldo José Ferreira Chagas Junior - gjr.doc@gmail.com
*
*  PPGI - NCE - UFRJ
*  Data Criação: 14/05/2017
*  Datas de Modificações:
*
*/

#include "mutacao.hpp"
#include <iostream>

//Apenas letura
TMapaGenes *TMutacao::getMapa() { return VP_Mapa; }
TArqLog *TMutacao::getArqLog()  { return VP_ArqSaida; }

TMutacao::TMutacao (TMapaGenes *mapa, TArqLog *arqSaida, int tipoMutacao)
{
   VP_Mapa = mapa;
   VP_ArqSaida = arqSaida;
   VP_mutacao = tipoMutacao;
}

int TMutacao::processa (TIndividuo *individuo)
{
   switch (VP_mutacao)
   {
      case 0:
      {
         return EM(individuo);
         break;
      }
      case 1:
      {
         return SM(individuo);
         break;
      }
      case 2:
      {
         return twoOPT(individuo);
         break;
      }
      case 3:
      {
         return twoOPT2(individuo);
         break;
      }
      case 4:
      {
         return SIM(individuo);
         break;
      }
      case 5:
      {
         return threeOPT(individuo);
         break;
      }
      case 6:
      {
         return threeOPT2(individuo);
         break;
      }
      case 7:
      {
         return DM(individuo);
         break;
      }
      case 8:
      {
         return IVM(individuo);
         break;
      }
      case 9:
      {
         return ISM(individuo);
         break;
      }
      case 10:
      {
         return GSM(individuo);
         break;
      }
      case 11:
      {
         return DBM(individuo);
         break;
      }
      case 12:
      {
         return DBM2(individuo);
         break;
      }
      case 13:
      {
         return SHMO(individuo);
         break;
      }
      case 14:
      {
         return TestMutation(individuo);
         break;
      }
      default:
      {
         break;
      }
   }

   return 0;
}

/**
 *
 * Exchange mutation (EM)
 *
 * Banzhaf (1990). The “Molecular” Traveling Salesman. Biological Cybernetics 64: 7–14.
 *
 **/
int TMutacao::EM(TIndividuo *individuo)
{
  double dist = individuo->get_distancia();
  int i = TUtils::rnd(1, individuo->get_qtdeGenes()-1);
  int j = TUtils::rnd(1, individuo->get_qtdeGenes()-1);

  individuo->troca_indice(i , j);

  return (dist == individuo->get_distancia())?0:1;
}

/**
 *
 * Scramble mutation (SM)
 *
 * Syswerda (1991). Schedule Optimization Using Genetic Algorithms. In Davis, L. (ed.)
 * Handbook of Genetic Algorithms, 332–349. New York: Van Nostrand Reinhold.
 *
 **/
int TMutacao::SM(TIndividuo *individuo)
{
  int i1, i2;

  double dist = individuo->get_distancia();

  do
  {
    i1 = TUtils::rnd(1, individuo->get_qtdeGenes()-1);
    i2 = TUtils::rnd(1, individuo->get_qtdeGenes()-1);
  }
  while(i1 == i2);

  individuo->embaralha(i1, i2);

  return (dist == individuo->get_distancia())?0:1;
}

/**
 *
 * 2-opt
 *
 * Croes, G. A. (1958). A Method for Solving Traveling-Salesman
 * Problems. Operations Research, 6(6), 791–812.
 *
 **/
int TMutacao::twoOPT(TIndividuo *individuo)
{
   //O 2-opt não pode ser realizado quando existem
   //menos de 4 vértices
   if (individuo->get_qtdeGenes()<4) return 0;

   double diff;
   double melhor;
   int Ci = -1, Cj = -1;
   melhor = individuo->get_distancia();

   for(int i = 1; i < individuo->get_qtdeGenes(); i++)
   {
      for(int j = i + 2; j < individuo->get_qtdeGenes() - 2; j++)
      {
         diff = individuo->get_dist_sub_reverso_indice(i, j);

         if(diff < melhor)
         {
				melhor = diff;
				Ci = i;
				Cj = j;
			}
		}
	}

   if(Ci != -1 && Cj != -1)
      individuo->inverte_sub_indice(Ci, Cj);
   else
      individuo->inverte_sub_indice(TUtils::rnd(1, individuo->get_qtdeGenes()-1), TUtils::rnd(1, individuo->get_qtdeGenes()-1));

   return (melhor == individuo->get_distancia())?0:1;
}

/**
 *
 * 2-opt(2)
 *
 * Utilizado em nosso primeiro artigo
 *
 **/
int TMutacao::twoOPT2(TIndividuo *individuo)
{
   if (individuo->get_extra()) return 0;

   //O 2-opt não pode ser realizado quando existem
   //menos de 4 vértices
   if (individuo->get_qtdeGenes()<4) return 0;

   double diff;
   double dist = individuo->get_distancia();

   for(int i = 1; i < individuo->get_qtdeGenes()-2; i++)
   {
      for(int j = i + 2; j < individuo->get_qtdeGenes(); j++)
      {
         diff = individuo->get_dist_sub_reverso_indice(i, j);

         if(individuo->get_distancia() > diff)
            individuo->inverte_sub_indice(i, j);
      }
   }

   individuo->set_extra((dist == individuo->get_distancia())?1:0);
   return individuo->get_extra()^1;
}

/**
 *
 * Simple Inversion mutation (SIM)
 *
 * Holland (1975). Adaptation in Natural and Artificial Systems. Ann Arbor:
 * University of Michigan Press.
 *
 * Grefenstette (1987). (Ed). Genetic Algorithms and Their Applications:
 * Proceedings of the Second International Conference. Hillsdale, New Jersey:
 * Lawrence Erlbaum.
 *
 **/
int TMutacao::SIM(TIndividuo *individuo)
{
   int begin, end;
   begin = TUtils::rnd(1, individuo->get_qtdeGenes() - 1);
   end = TUtils::rnd(1, individuo->get_qtdeGenes() - 1);

   //O método inverte_sub_indice realiza a inversão de uma subrota
   //entre duas posições passada como parâmetro.
   individuo->inverte_sub_indice(begin, end);
   return 1;
}

/**
 *
 * 3-opt
 *
 * Croes, G. A. (1958). A Method for Solving Traveling-Salesman
 * Problems. Operations Research, 6(6), 791–812.
 *
 **/
int TMutacao::threeOPT(TIndividuo *individuo)
{
   //O 3-opt não pode ser realizado quando existem
   //menos de 6 vértices
   if (individuo->get_qtdeGenes()<6) return 0;
   
   double gainActual, gainCandidato, diff;
   int Imas1, Jmas1, Kmas1, Ci = -1, Cj = -1, Ck = -1, op = -1;
   double min, valor = individuo->get_distancia();
   int n = individuo->get_qtdeGenes() - 1;
   min = valor;

   for(int i = 0; i < n - 4; i++)
   {
      for(int j = i + 2; j < n - 2; j++)
      {
         for(int K = 0; K < 10; K++)
         {
            int k = TUtils::rnd(j+2, n-1);

            Imas1 = i + 1;
            Jmas1 = j + 1;
            Kmas1 = k + 1;

            TGene *gi = individuo->get_por_indice(i);
            TGene *gj = individuo->get_por_indice(j);
            TGene *gk = individuo->get_por_indice(k);

            TGene *gImas1 = individuo->get_por_indice(Imas1);
            TGene *gJmas1 = individuo->get_por_indice(Jmas1);
            TGene *gKmas1 = individuo->get_por_indice(Kmas1);

            //operação 1
            gainActual = VP_Mapa->get_distancia(gi->ori, gImas1->dest) +
                         VP_Mapa->get_distancia(gj->ori, gJmas1->dest) +
                         VP_Mapa->get_distancia(gk->ori, gKmas1->dest);

            gainCandidato = VP_Mapa->get_distancia(gi->ori, gj->dest) +
                            VP_Mapa->get_distancia(gImas1->ori, gk->dest) +
                            VP_Mapa->get_distancia(gJmas1->ori, gKmas1->dest);

            diff = valor - gainActual + gainCandidato + 
				       this->calcValReverso(gImas1, gj) +
				       this->calcValReverso(gJmas1, gk);

            if(min > diff)
            {
               op = 0;
               Ci = i;
               Cj = j;
               Ck = k;
               min = diff;
            }

            //operação 2
            gainCandidato = VP_Mapa->get_distancia(gi->ori, gJmas1->dest) +
                            VP_Mapa->get_distancia(gk->ori, gImas1->dest) +
                            VP_Mapa->get_distancia(gj->ori, gKmas1->dest);

            diff = valor - gainActual + gainCandidato;

            if(min > diff)
            {
               op = 1;
               Ci = i;
               Cj = j;
               Ck = k;
               min = diff;
            }

            //operação 3
            gainCandidato = VP_Mapa->get_distancia(gi->ori, gk->dest) +
                            VP_Mapa->get_distancia(gJmas1->ori, gImas1->dest) +
                            VP_Mapa->get_distancia(gj->ori, gKmas1->dest);

            diff = valor - gainActual + gainCandidato +
                   this->calcValReverso(gJmas1, gk);

            if(min > diff)
            {
               op = 2;
               Ci = i;
               Cj = j;
               Ck = k;
               min = diff;
            }

            //operação 4
            gainCandidato = VP_Mapa->get_distancia(gi->ori, gJmas1->dest) +
                            VP_Mapa->get_distancia(gk->ori, gj->dest) +
                            VP_Mapa->get_distancia(gImas1->ori, gKmas1->dest);

            diff = valor - gainActual + gainCandidato +
				       this->calcValReverso(gImas1, gj);

            if(min > diff)
            {
               op = 3;
               Ci = i;
               Cj = j;
               Ck = k;
               min = diff;
            }
         }
      }
   }

   // realizando a troca
   if(Ci == -1 || Cj == -1)
   {
		Ci = TUtils::rnd(1, n-4);
      Cj = TUtils::rnd(Ci+2, n-2);
      Ck = TUtils::rnd(Cj+2, n);
      op = TUtils::rnd(0, 3);
   }
   
   switch(op)
   {
		case 0:
      {
			individuo->inverte_sub_indice(Ci+1, Cj);
         individuo->inverte_sub_indice(Cj+1, Ck);
			break;
		}
		case 1:
   	{
			individuo->troca_sub(Ci+1, Cj-Ci, Cj+1, Ck-Cj);
   	   break;
		}
		case 2:
		{
			individuo->inverte_sub_indice(Cj+1, Ck);
			individuo->troca_sub(Ci+1, Cj-Ci, Cj+1, Ck-Cj);						
   	   break;
		}
		case 3:
   	{
			individuo->inverte_sub_indice(Ci+1, Cj);
			individuo->troca_sub(Ci+1, Cj-Ci, Cj+1, Ck-Cj);
   	   break;
		}
		default:
			break;
	}
	
   return (valor == individuo->get_distancia())?0:1;
}

/**
 *
 * 3-opt(2)
 *
 * Utilizado em nossos primeiros testes
 *
 **/
int TMutacao::threeOPT2(TIndividuo *individuo)
{
	if (individuo->get_extra()) return 0;
   //O 3-opt não pode ser realizado quando existem
   //menos de 6 vértices
   if (individuo->get_qtdeGenes()<6) return 0;
   
   double gainActual, gainCandidato, diff;
   int Imas1, Jmas1, Kmas1, min, op = -1;
   double valor;
   double valorInicial = individuo->get_distancia();
   int n = individuo->get_qtdeGenes() - 1;
   
   for(int i = 0; i < n - 4; i++)
   {
      for(int j = i + 2; j < n - 2; j++)
      {
         for(int k = j+2; k < n-1; k++)
         {
				valor = individuo->get_distancia();
				min = valor;
				op = -1;
            Imas1 = i + 1;
            Jmas1 = j + 1;
            Kmas1 = k + 1;

            TGene *gi = individuo->get_por_indice(i);
            TGene *gj = individuo->get_por_indice(j);
            TGene *gk = individuo->get_por_indice(k);

            TGene *gImas1 = individuo->get_por_indice(Imas1);
            TGene *gJmas1 = individuo->get_por_indice(Jmas1);
            TGene *gKmas1 = individuo->get_por_indice(Kmas1);

            //operação 1
            gainActual = VP_Mapa->get_distancia(gi->ori, gImas1->dest) +
                         VP_Mapa->get_distancia(gj->ori, gJmas1->dest) +
                         VP_Mapa->get_distancia(gk->ori, gKmas1->dest);

            gainCandidato = VP_Mapa->get_distancia(gi->ori, gj->dest) +
                            VP_Mapa->get_distancia(gImas1->ori, gk->dest) +
                            VP_Mapa->get_distancia(gJmas1->ori, gKmas1->dest);


            diff = valor - gainActual + gainCandidato + 
			          this->calcValReverso(gImas1, gj) +
				       this->calcValReverso(gJmas1, gk);

            if(min > diff)
            {
               op = 0;
               min = diff;
            }

            //operação 2
            gainCandidato = VP_Mapa->get_distancia(gi->ori, gJmas1->dest) +
                            VP_Mapa->get_distancia(gk->ori, gImas1->dest) +
                            VP_Mapa->get_distancia(gj->ori, gKmas1->dest);

            diff = valor - gainActual + gainCandidato;

            if(min > diff)
            {
               op = 1;
               min = diff;
            }

            //operação 3
            gainCandidato = VP_Mapa->get_distancia(gi->ori, gk->dest) +
                            VP_Mapa->get_distancia(gJmas1->ori, gImas1->dest) +
                            VP_Mapa->get_distancia(gj->ori, gKmas1->dest);

            diff = valor - gainActual + gainCandidato +
                   this->calcValReverso(gJmas1, gk);
				   
            if(min > diff)
            {
               op = 2;
               min = diff;
            }

            //operação 4
            gainCandidato = VP_Mapa->get_distancia(gi->ori, gJmas1->dest) +
                            VP_Mapa->get_distancia(gk->ori, gj->dest) +
                            VP_Mapa->get_distancia(gImas1->ori, gKmas1->dest);

            diff = valor - gainActual + gainCandidato +
                   this->calcValReverso(gImas1, gj);

            if(min > diff)
            {
               op = 3;
               min = diff;
            }

            switch(op)
				{
					case 0:
					{
						individuo->inverte_sub_indice(i+1, j);
                  individuo->inverte_sub_indice(j+1, k);
						break;
					}
					case 1:
					{
						individuo->troca_sub(i+1, j-i, j+1, k-j);
						break;
					}
					case 2:
					{
						individuo->inverte_sub_indice(j+1, k);						
						individuo->troca_sub(i+1, j-i, j+1, k-j);
						break;
					}
					case 3:
					{
						individuo->inverte_sub_indice(i+1, j);						
						individuo->troca_sub(i+1, j-i, j+1, k-j);
						break;
					}
					default:
						break;
				}
			}
		}
	}
   
   individuo->set_extra((valorInicial == individuo->get_distancia())?1:0);
   return individuo->get_extra()^1;
}

/**
  * Mutação DM:
  *   Nessa mutação ele pega um início e um fim randomico, retira esse pedaço do vetor principal,
  *   depois pega uma posição randômica e reinplanta o pedaço.
**/
int TMutacao::DM(TIndividuo *individuo)
{
  int begin=0, end=0, insertpoint, diff;
  int lastindex = individuo->get_qtdeGenes() -1;
  
  begin = TUtils::rnd(1, lastindex -1);
  end = TUtils::rnd(begin+1, lastindex);
  diff = end-begin;
  insertpoint = TUtils::rnd(1, lastindex - diff);
  if(insertpoint > begin) insertpoint+= diff;
  //cout<<begin<<" "<<end<<" "<<diff<<" "<<insertpoint<<endl;
  individuo->troca_sub(insertpoint+1, 0, begin, (end-begin)+1);

  return 1;
}

/**
  * IVM:
  *   Nessa mutação ele pega um início e um fim randomico, retira esse pedaço do vetor principal,
  *   depois pega uma posição randômica e reinplanta o pedaço em ordem invertida.
**/
int TMutacao::IVM(TIndividuo *individuo)
{
  int begin=0, end=0, insertpoint, diff;
  int lastindex = individuo->get_qtdeGenes() -1;
  
  begin = TUtils::rnd(1, lastindex -1);
  end = TUtils::rnd(begin+1, lastindex);
  diff = end-begin;
  insertpoint = TUtils::rnd(1, lastindex - diff);
  if(insertpoint > begin) insertpoint+= diff;
  //cout<<begin<<" "<<end<<" "<<diff<<" "<<insertpoint<<endl;
  individuo->troca_sub(insertpoint+1, 0, begin, -((end-begin)+1));

  return 1;
}

/**
  * ISM:
  *   Tira um gene randômico do indivíduo e coloca em uma posição randômica.
**/
int TMutacao::ISM(TIndividuo *individuo)
{
  int i=0, j=0, lastindex = individuo->get_qtdeGenes();
  i = TUtils::rnd(1, lastindex - 2);
  j = TUtils::rnd(i+1, lastindex - 1);
  cout<<i<<" "<<j<<endl;
  individuo->troca_sub(i, 1, j+1, 0);

  return 1;
}
/**
  * GSM:
  *   Durante um número k<=quantidade de genes, pega-se indices de genes i,j randômicos dentro do
  *   individuo, testa se a troca do genes gera um caminho mais rápido, se for o caso, troca os genes
  *   e quebra o loop.
**/
int TMutacao::GSM(TIndividuo *individuo)
{
  int i=0, j=0, lastindex = individuo->get_qtdeGenes()-1;
  double originaldist=0, changeddist=0;

  for(int k=0; k <= lastindex; k++)
  {
    
    i = TUtils::rnd(1, lastindex - 1);
    j = TUtils::rnd(i+1, lastindex);
    originaldist = individuo->get_distancia();
    individuo->troca_indice(i,j);
    changeddist = individuo->get_distancia();
    //cout<<"i: "<<i<<";j: "<<j<<"; OD: "<<originaldist<<"; CD: "<<changeddist<<endl;
    if(originaldist > changeddist)
    {
      cout<<"loopbreak."<<endl;
      //cout<<individuo->toString()<<endl;
      break;
    }
    else
    {
      individuo->troca_indice(i,j);
    }
  }

  return 1;
}
/**
  *DBM:
  *  O flag no contreras estava inicializado false, mas isso n tem sentido pq o loop fica inútil.
  *  
**/

int TMutacao::DBM(TIndividuo *individuo)
{
  int i, j, k, l, lastindex =  individuo->get_qtdeGenes()-1;
  
  i = TUtils::rnd(1, lastindex - 6);
  j = i+2;
  k = TUtils::rnd(j+2, lastindex - 2);
  l = k+2;
  cout<<"i: "<<i<<"; j: "<<j<<"; k: "<<k<<"; l: "<<l<<endl;
  individuo->troca_indice((i+1), (k+1));
  individuo->troca_indice(j, l);

  return 1;
}

/**
  *DBM2:
  *  Essa me confundiu um pouco, mas depois de ler o código percebi que o que ele faz é pegar quatro
  *  indices em sequencia e depois realizar uma troca de subrotas.
  *  
**/

int TMutacao::DBM2(TIndividuo *individuo)
{
  int i, j, k, l, lastindex = individuo->get_qtdeGenes()-1;

  i = TUtils::rnd(1, lastindex - 6);
  j = TUtils::rnd(i+1, lastindex - 4);
  k = TUtils::rnd(j+1, lastindex - 2);
  l = TUtils::rnd(k+1, lastindex);
  cout<<"i: "<<i<<"; j: "<<j<<"; k: "<<k<<"; l:"<<l<<endl;
  individuo->troca_sub(i+1, (j-i), k+1, (l-k));

  return 1;
}
/**
  *SHMO:
  *  Esse é um método guloso. Dentro desse O(n^2) aí, ele realiza três testes de troca.
  *  Os testes estão descritos abaixo. Não achei modos de simplificar esse n^2.
  *  
**/
int TMutacao::SHMO(TIndividuo *individuo)
{
  int  lastindex = individuo->get_qtdeGenes()-1;
  double value, new_value;

  for(int i=1;i<lastindex-2; i++)
  {
    for(int j=i+2; j<lastindex;j++)
    {
      //Primeira troca: Testa se a inversão entre i+1 e j+1 vale a pena.
      value = individuo->get_distancia();
      new_value = individuo->get_dist_sub_reverso_indice(i+1, j+1);
      if(value>new_value)
      {
        individuo->inverte_sub_indice(i+1, j+1);
      }
      //Segunda troca: Testa se tirando j e colocar antes do i vale a pena.
      value = individuo->get_distancia();
      individuo->troca_sub(i, 0, j, 1);
      new_value = individuo->get_distancia();
      if(new_value > value) individuo->troca_sub(i, 1, j, 0);
      //Terceira troca: Testa se trocar i pelo j vale a pena.
      value = individuo->get_distancia();
      individuo->troca_indice(i,j);
      new_value = individuo->get_distancia();
      if(new_value > value) individuo->troca_indice(i,j);
    }
  }
  return 1;
}
int TMutacao::TestMutation(TIndividuo *individuo)
{
  TGene *p;
  p = individuo->get_por_indice(2);
  individuo->troca_sub(2, 2, 4, 2);
  cout<<p->i<<endl;

  return 1;
}
/**
  *NJ:
  *  Esse é um método guloso também. 
  *  
**/
void TMutacao::Tipo3(TIndividuo *opcao, TGene *c, TGene *cLinha)
{
  TGene *PathIni, *PathEnd, *R, *S, *StopPoint, *InsertPoint, *rProx;
  double ChangedValue, MaxValue = -1 * infinito;


  if(c->i < cLinha->i)
  {
    PathIni = cLinha->prox;
    PathEnd = c->ant;
  }
  else
  {
    PathIni = c->prox;
    PathEnd = cLinha->ant;
  }

  for(TGene *GenR = c; GenR != cLinha->prox; GenR = GenR->prox)
  {
    for(TGene *GenS = PathIni; GenS != PathEnd->prox; GenS = GenS->prox)
    {
      opcao->troca_indice(GenR->i, GenS->i);
      ChangedValue = opcao->get_distancia();
      if(ChangedValue > MaxValue)
      {
        R = GenR;
        S = GenS;
        MaxValue = ChangedValue;
      }
    }
  }
  StopPoint = PathEnd->prox->prox;
  InsertPoint = rProx = R->prox;
  for(TGene *NextGen = PathIni->prox, *GenS = NextGen->ant;
     NextGen != StopPoint; NextGen = NextGen->prox, GenS = NextGen->ant)
  {
    opcao->troca_sub(InsertPoint->i, 0, GenS->i, 1);
    InsertPoint = GenS->prox;
  }
  opcao->troca_sub(S->prox->i, 0, rProx->i, 1);
}

int TMutacao::NJ(TIndividuo *individuo, TPopulacao *populacao)
{
  float probabilidade = 0.5;
  TGene *c, *cLinha;
  TIndividuo *indTemp, **opcoes;
  TMapaGenes *mapaGenes = individuo->getMapa();

  int nMaxVizinhos = 3, nMaxInteracoes = 5, QtdGenes = individuo->get_qtdeGenes(), rndi=-1;
  
  opcoes = (TIndividuo **) malloc(4 * sizeof(TIndividuo*));
  for(int interacao=0; interacao<nMaxInteracoes; interacao++)
  {
    //Opções de indivíduos a ser escolhida
    opcoes[0] = individuo->clona();
    opcoes[1] = individuo->clona();
    opcoes[2] = individuo->clona();
    opcoes[3] = individuo->clona();

    //seleciona o gene c.
    c = individuo->get_por_indice(TUtils::rnd(1, QtdGenes-1));

    //seleciona o gene cLinha
    if(TUtils::flip(probabilidade))
    {
      //pega o proximo gene de um indivíduo randomico na populaçao, sendo c != cLinha.
      do
      {
        rndi = TUtils::rnd(0, populacao->get_qtdeIndividuo() -1);
        indTemp = populacao->get_individuo(rndi);
        cLinha = indTemp->get_por_id(c->id)->prox;
      }while (c->id == cLinha->id);

    }
    else
    {
      //Pega no individuo o id do gene que tem a menor distancia possivel com c dentro do mapa.
      vector< pair<int, double> > distancias;
      for(int i=1; i<QtdGenes; i++)
      {
        if(i!=c->id)
        {
          pair<int, double> dist(i, mapaGenes->get_distancia(i, c->id));
          distancias.push_back(dist);
        }
      }
      sort(distancias.begin(), distancias.end(), TUtils::compare_pair_second<int, double, std::less<double>>());
      rndi = TUtils::rnd(0, nMaxVizinhos-1);
      cLinha = individuo->get_por_id(distancias[rndi].first);
    }
    //Se são visinhos, pula a interação;
    if(c->prox == cLinha || c->ant == cLinha) continue;
    else
    {
      //Opçao 1: o c é fixo.
      if(c->i < cLinha->i) opcoes[0]->inverte_sub_indice(c->prox->i, cLinha->i);
      else opcoes[0]->inverte_sub_indice(cLinha->i, c->ant->i);
      //Opção 2: o cLinha é fixo.
      if(c->i < cLinha->i) opcoes[1]->inverte_sub_indice(c->i, cLinha->ant->i);
      else opcoes[1]->inverte_sub_indice(cLinha->prox->i, c->i);
      Tipo3(opcoes[2], c, cLinha);
    }
  }


  return 1;
}

/*************************
*  Métodos auxiliares
*************************/
double TMutacao::calcValReverso (TGene *g1, TGene *g2)
{
	double resultado = 0;
	for (TGene *aux = g1; aux->id != g2->id; aux = aux->prox)
	{
		resultado -= VP_Mapa->get_distancia(aux->ori, aux->prox->dest);
		resultado += VP_Mapa->get_distancia(aux->prox->ori, aux->dest);
	}
	
	return resultado;
}